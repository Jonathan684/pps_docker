#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <iio.h>

/* helper macros */
#define MHZ(x) ((long long)(x*1000000.0 + .5))
#define GHZ(x) ((long long)(x*1000000000.0 + .5))

/* RX is input, TX is output */
enum iodev { RX, TX }; //0 1
bool stop;

void handle_sig(int sig)
{
	printf("Waiting for process to finish... Got signal %d\n", sig);
	stop = true;
}

/* common RX and TX streaming params */
struct stream_cfg {
	long long bw_hz; // Analog banwidth in Hz
	long long fs_hz; // Baseband sample rate in Hz
	long long lo_hz; // Local oscillator frequency in Hz
	const char* rfport; // Port name
};
/* static scratch mem for strings */
char tmpstr[64];

// 1.- Configuraciones basicas

/* IIO structs required for streaming */
struct iio_context *ctx   = NULL;
struct iio_channel *rx0_i = NULL;
struct iio_channel *rx0_q = NULL;
struct iio_channel *tx0_i = NULL;
struct iio_channel *tx0_q = NULL;
struct iio_buffer  *rxbuf = NULL;
struct iio_buffer  *txbuf = NULL;


bool get_ad9361_stream_ch(enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn);
void shutdown();

int main(){

// Listen to ctrl+c and IIO_ENSURE
signal(SIGINT, handle_sig);



// Streaming devices
struct iio_device *tx =NULL;
struct iio_device *rx =NULL;
// RX and TX sample counters
size_t nrx = 0;
size_t ntx = 0;

// Stream configurations
struct stream_cfg rxcfg;
struct stream_cfg txcfg;

// RX stream config
rxcfg.bw_hz = MHZ(2);   // 2 MHz rf bandwidth
rxcfg.fs_hz = MHZ(2.5);   // 2.5 MS/s rx sample rate
rxcfg.lo_hz = GHZ(2.5); // 2.5 GHz rf frequency
rxcfg.rfport = "A_BALANCED"; // port A (select for rf freq.)
// TX stream config
txcfg.bw_hz = MHZ(1.5); // 1.5 MHz rf bandwidth
txcfg.fs_hz = MHZ(2.5);   // 2.5 MS/s tx sample rate
txcfg.lo_hz = GHZ(2.5); // 2.5 GHz rf frequency
txcfg.rfport = "A"; // port A (select for rf freq.)

// 2.- obtener contexto
printf("* Acquiring IIO context\n");
ctx = iio_create_default_context();

// 3.- dispositivos transmisor y receptor
printf("* Acquiring AD9361 streaming devices\n");
tx = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");
rx = iio_context_find_device(ctx, "cf-ad9361-lpc");

// 4.- configurando AD9361/63
printf("* Configuring AD9361 for streaming\n");

struct iio_device *dev =  iio_context_find_device(ctx, "ad9361-phy");
const struct iio_channel *chn_AD9361 = NULL;
//                                   voltage0:     (input)
printf("--> -4\n");
chn_AD9361 = iio_device_find_channel(dev,"voltage0",false);

printf("* Acquiring AD9361 lo channel RX \n");
printf("--> -3\n");
iio_channel_attr_write(chn_AD9361, "rf_port_select", rxcfg.rfport);
printf("--> -2\n");
iio_channel_attr_write_longlong(chn_AD9361, "rf_bandwidth", rxcfg.bw_hz);
printf("--> -1\n");
iio_channel_attr_write_longlong(chn_AD9361, "sampling_frequency", rxcfg.fs_hz);
printf("--> 0\n");
//bool output;
// Configure LO channel altvoltage0: RX_LO (output)    false
chn_AD9361 = iio_device_find_channel(dev,"altvoltage0",false);
printf("--> 1\n");
iio_channel_attr_write_longlong(chn_AD9361, "frequency", rxcfg.lo_hz);
printf("--> 2\n");
//                                   voltage0:     (output)
chn_AD9361 = iio_device_find_channel(dev,"voltage0",true);

printf("* Acquiring AD9361 lo channel TX\n");
iio_channel_attr_write(chn_AD9361, "rf_port_select", txcfg.rfport);
iio_channel_attr_write_longlong(chn_AD9361, "rf_bandwidth", txcfg.bw_hz);
iio_channel_attr_write_longlong(chn_AD9361, "sampling_frequency", txcfg.fs_hz);

// Configure LO channel //altvoltage1: TX_LO (output)  true
chn_AD9361 = iio_device_find_channel(dev,"altvoltage1",true);
iio_channel_attr_write_longlong(chn_AD9361, "frequency", txcfg.lo_hz);

// 5.- Inicialización de canales de transmisión AD9361 IIO
printf("* Initializing AD9361 IIO streaming channels\n");

//get_ad9361_stream_ch(enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn)

get_ad9361_stream_ch(RX, rx, 0, &rx0_i);
get_ad9361_stream_ch(RX, rx, 1, &rx0_q);
get_ad9361_stream_ch(TX, tx, 0, &tx0_i);
get_ad9361_stream_ch(TX, tx, 1, &tx0_q);

// 6.- Habilitación de canales de transmisión IIO
printf("* Enabling IIO streaming channels\n");
iio_channel_enable(rx0_i);
iio_channel_enable(rx0_q);
iio_channel_enable(tx0_i);
iio_channel_enable(tx0_q);

// 7.- Habilitación de canales de transmisión IIO
printf("* Creating non-cyclic IIO buffers with 1 MiS\n");
//Creando buffer de recepcion
rxbuf = iio_device_create_buffer(rx, 1024*1024, false);
if (!rxbuf) {
	perror("Could not create RX buffer");
	shutdown();
}
//Creando buffer de transmision
txbuf = iio_device_create_buffer(tx, 1024*1024, false);
if (!txbuf) {
	perror("Could not create TX buffer");
	shutdown();
}

// 8.-Iniciando transmisión IO (presione CTRL+C para cancelar)
printf("* Starting IO streaming (press CTRL+C to cancel)\n");
while (!stop)
{
    ssize_t nbytes_rx, nbytes_tx;
	char *p_dat, *p_end;
	ptrdiff_t p_inc;
	// Schedule TX buffer - Programar el búfer de TX
	nbytes_tx = iio_buffer_push(txbuf);
	if (nbytes_tx < 0) { printf("Error pushing buf %d\n", (int) nbytes_tx); 
	shutdown();
	 }
	// Refill RX buffer
	nbytes_rx = iio_buffer_refill(rxbuf);
	if (nbytes_rx < 0) 
	{ printf("Error refilling buf %d\n",(int) nbytes_rx); 
		shutdown();
	 }
	// READ: Get pointers to RX buf and read IQ from RX buf port 0
	p_inc = iio_buffer_step(rxbuf);
	p_end = iio_buffer_end(rxbuf);
	for (p_dat = (char *)iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc) {
		// Example: swap I and Q
		const int16_t i = ((int16_t*)p_dat)[0]; // Real (I)
		const int16_t q = ((int16_t*)p_dat)[1]; // Imag (Q)
		((int16_t*)p_dat)[0] = q;
		((int16_t*)p_dat)[1] = i;
	}
	// WRITE: Get pointers to TX buf and write IQ to TX buf port 0
	p_inc = iio_buffer_step(txbuf);
	p_end = iio_buffer_end(txbuf);
	for (p_dat = (char *)iio_buffer_first(txbuf, tx0_i); p_dat < p_end; p_dat += p_inc) {
		// Example: fill with zeros
		// 12-bit sample needs to be MSB aligned so shift by 4
		// https://wiki.analog.com/resources/eval/user-guides/ad-fmcomms2-ebz/software/basic_iq_datafiles#binary_format
		((int16_t*)p_dat)[0] = 0 << 4; // Real (I)
		((int16_t*)p_dat)[1] = 0 << 4; // Imag (Q)
	}
	// Sample counter increment and status output
	nrx += nbytes_rx / iio_device_get_sample_size(rx);
	ntx += nbytes_tx / iio_device_get_sample_size(tx);
	printf("\tRX %8.2f MSmp, TX %8.2f MSmp\n", nrx/1e6, ntx/1e6);
}
shutdown();
return 0;
}
bool get_ad9361_stream_ch(enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn)
{
    char tmpstr[64];
    snprintf(tmpstr, sizeof(tmpstr), "%s%d", "voltage", chid);
	*chn = iio_device_find_channel(dev,tmpstr , d == TX);
	if (!*chn){
		snprintf(tmpstr, sizeof(tmpstr), "%s%d", "altvoltage", chid);
        *chn = iio_device_find_channel(dev,tmpstr, d == TX);
    }
    return *chn != NULL;
}

void shutdown()
{
	printf("* Destroying buffers\n");
	if (rxbuf) { iio_buffer_destroy(rxbuf); }
	if (txbuf) { iio_buffer_destroy(txbuf); }

	printf("* Disabling streaming channels\n");
	if (rx0_i) { iio_channel_disable(rx0_i); }
	if (rx0_q) { iio_channel_disable(rx0_q); }
	if (tx0_i) { iio_channel_disable(tx0_i); }
	if (tx0_q) { iio_channel_disable(tx0_q); }

	printf("* Destroying context\n");
	if (ctx) { iio_context_destroy(ctx); }
	exit(0);
}