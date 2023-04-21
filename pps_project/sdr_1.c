#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
//#include <errno>
#define IIO_CHANNEL_TX 0x1
#define SAMPLE_COUNT 1024 // Para el buffer

bool stop;

typedef struct {
    int16_t re;
    int16_t im;
} complex_t;

void complexExp(complex_t *signal, int N, double Fc, double Fs);

// Callback function to print the attribute name, value, and length
int print_channel_attrs(struct iio_channel *chn, const char *attr, const char *val, size_t len, void *data)
{
    printf("Attribute: %s, Value: %s, Length: %zu\n", attr, val, len);
    printf("Channel %s \n", iio_channel_get_name(chn));
    // int *count = (int *)data;
    // (*count)++;
    // printf("Channel attribute #%d\n", *count);
    return 0;
}
// Callback function that reads the value of an attribute
int print_device_attrs(struct iio_device *dev, const char *attr, const char *value, size_t len, void *d) {
   
   printf("%s: %s\n", attr, value);
    // int *count = (int *)d;
    // (*count)++;
    //printf("Channel attribute #%d\n", *count);
   // printf("Channel %s \n", iio_device_get_name(dev));
    //printf("len %zu \n", len);
    return 0;
}

void all_attr_channel(struct iio_context *ctx,struct iio_channel *chn, const char *channel_rx)
{
     // Read all the attributes of the channel and print them using the callback function
    printf("\nIMPRIMIENDO TODOS LOS ATRIBUTOS PARA EL CANAL : %s \n\n",channel_rx);
    ssize_t ret = iio_channel_attr_read_all(chn, &print_channel_attrs, NULL);
    if (ret < 0) {
        printf("Failed to read channel attributes\n");
        iio_context_destroy(ctx);
        return ;
    }
}

void all_attr_device(struct iio_device *dev)
{
    printf("\nIMPRIMIENDO TODOS LOS ATRIBUTOS PARA EL DISPOSITIVO : %s \n\n",iio_device_get_name(dev));
     // Read all the attributes of the channel and print them using the callback function
    iio_device_attr_read_all(dev, print_device_attrs, NULL);
    
}

int transmit(struct iio_context *ctx,complex_t* txSignal);

void handle_sig(int sig)
{
	printf("Waiting for process to finish... Got signal %d\n", sig);
	stop = true;
}

int main(){

    signal(SIGINT, handle_sig);
    struct iio_context *ctx = NULL;
    ctx = iio_create_default_context();
    if (!ctx) {
        printf("Unable to create IIO context\n");
        return -1;
    }

    struct iio_device *dev = NULL;
    dev = iio_context_find_device(ctx, "ad9361-phy");
    if (!dev) {
        printf("Unable to find Pluto device\n");
        iio_context_destroy(ctx);
        return -1;
    }
    char buffer[50];

    // PASOS 
    // 1 .- LOOPBACK SDR 192.168.1.36
    int i = iio_device_get_debug_attrs_count(dev);
    const char * attr = iio_device_find_debug_attr(dev, "loopback");

    iio_device_debug_attr_read(dev,"loopback",buffer,sizeof(buffer));
    iio_device_debug_attr_write(dev,"loopback","1");//0-1-2

    printf("attribute count ---------->%d\n",i);
    printf("attr name %s\n",attr);
    printf("value %s\n",buffer);
    iio_device_debug_attr_read(dev,"loopback",buffer,sizeof(buffer));
    printf("value :%s\n",buffer);

    const char *chn_str_rx = "voltage0";
    const char *chn_str_tx = "voltage0";
    // Get the "in_voltage" attribute of the device
    //struct iio_attribute *attr = iio_device_find_attr(dev, "sampling_frequency");
    struct iio_channel *chn_rx = iio_device_find_channel(dev, chn_str_rx, false);//input() * @param output True if the searched channel is output, False otherwise
    struct iio_channel *chn_tx = iio_device_find_channel(dev, chn_str_tx, true);    //output() mismo nombre para el canal


    all_attr_channel(ctx,chn_rx, chn_str_rx);
    // Read the attribute value as a double
    double value;
    int ret = iio_channel_attr_read_double(chn_rx, "sampling_frequency", &value);

    if (ret < 0) {
       perror("Error reading attribute:");
       return 1;
    }

    // Print the value
    printf("Attribute value: %f\n", value);

    // 2.- SET Frecuencia de muestreo
    //ssize_t iio_device_attr_write(const struct iio_device *dev,const char *attr, const char *src);
    //iio_device_attr_write(dev,"sampling_frequency","800000");solo para debugg
    iio_channel_attr_write(chn_rx,"sampling_frequency","800000");

    iio_channel_attr_read_double(chn_rx, "sampling_frequency", &value);
    printf("Attribute value 2: %f\n", value);

    // 3.- Ancho de banda en RF

    double value_RxRfBw;
    double value_TxRfBw;

    const char *RxRfBw = "16000000"; // Bandwidth of front-end analog filter of TX path [Hz]
    const char *TxRfBw = RxRfBw;

    //printf("--> %s \n",iio_channel_find_attr(chn_rx,"rf_bandwidth"));

    iio_channel_attr_write(chn_rx,"rf_bandwidth",RxRfBw);
    iio_channel_attr_write(chn_tx,"rf_bandwidth",TxRfBw);

    iio_channel_attr_read_double(chn_rx, "rf_bandwidth", &value_RxRfBw);
    iio_channel_attr_read_double(chn_tx, "rf_bandwidth", &value_TxRfBw);

    printf("RxRfBw :%f and TxRfBw :%f\n",value_RxRfBw,value_TxRfBw);

    //4. Ganancia del Hardware
    // Get the TX hardware gain on channel 0 and print it to the console
    //RX
    double rx_hardwaregain_get;
    double rx_hardwaregain = 70;

    iio_channel_attr_write_double(chn_rx, "hardwaregain", rx_hardwaregain);
    iio_channel_attr_read_double(chn_rx, "hardwaregain", &rx_hardwaregain_get);
    //TX
    double tx_hardwaregain_get;
    double tx_hardwaregain = -40.0;

    iio_channel_attr_write_double(chn_tx, "hardwaregain", tx_hardwaregain);
    iio_channel_attr_read_double(chn_tx, "hardwaregain", &tx_hardwaregain_get);

    printf("TX hardware gain on channel 0: %f dB\n", tx_hardwaregain_get);
    printf("RX hardware gain on channel 0: %f dB\n", rx_hardwaregain_get);

    //const char *rx_GainControlModes = "slow_attack";
    char rx_GainControlModes_get[100];

    iio_channel_attr_write(chn_rx, "gain_control_mode", "slow_attack");
    iio_channel_attr_read(chn_rx, "gain_control_mode", rx_GainControlModes_get,sizeof(rx_GainControlModes_get));

    printf("RX gain_control_mode on channel 0: %s dB\n", rx_GainControlModes_get);

    // 5.- Frecuencia de portadora
    //https://github.com/analogdevicesinc/pyadi-iio/blob/master/adi/ad936x.py

    long long rx_frequency = 910e6;
    // long long rx_frequency_get;
    // long long tx_frequency_get;

    const char *channel_rx = "altvoltage0";
    struct iio_channel *chn_rx_lo = iio_device_find_channel(dev,channel_rx, true);//input() * @param output True if the searched channel is output, False otherwise
    iio_channel_attr_write_longlong(chn_rx_lo, "frequency", rx_frequency);
    //struct iio_channel *chn_tx_lo = iio_device_find_channel(dev, "altvoltage1", true);    //output() mismo nombre para el canal

    // Read the values of all attributes
    all_attr_channel(ctx,chn_rx_lo, channel_rx);

    const char *channel_tx = "altvoltage1";
    long long tx_frequency = rx_frequency;
    struct iio_channel *chn_tx_lo = iio_device_find_channel(dev,channel_tx, true);//input() * @param output True if the searched channel is output, False otherwise
    iio_channel_attr_write_longlong(chn_tx_lo, "frequency", tx_frequency);
    //struct iio_channel *chn_tx_lo = iio_device_find_channel(dev, "altvoltage1", true);    //output() mismo nombre para el canal

    // Read the values of all attributes
    all_attr_channel(ctx,chn_tx_lo, channel_tx);

    // 6.- Creando buffer
    //__api __check_ret struct iio_buffer * iio_device_create_buffer(const struct iio_device *dev,size_t samples_count, bool cyclic);
    size_t samples_count = 1024;
    // bool cyclic_tx = true;

    all_attr_device(dev);
    // Get the TX buffer

    // bool cyclic_rx = false;
    // struct iio_buffer * rxbuf = iio_device_create_buffer(dev,samples_count,cyclic_rx);

    //struct iio_buffer *txbuf = iio_device_create_buffer(dev, 1024, false);
    // bool cyclic_tx = true;
    // struct iio_buffer * buffer_tx = iio_device_create_buffer(dev,samples_count,cyclic_tx);

    // Read the values of all attributes
    all_attr_device(dev);

    // 7.-Transmitiendo y Recibiendo

    // Generate some data to push
    //complex_t txSignal[1024];
    // Allocate memory for the data to be pushed
    complex_t *txSignal = (complex_t *) malloc(samples_count * sizeof(complex_t));
    if (txSignal == NULL) {
        printf("Error allocating memory\n");
        return 1;
    }

    double samplingRate = 16.0e6;
    double freq = samplingRate/16.0;
    complexExp(txSignal, 1024, freq, samplingRate);

    iio_channel_enable(chn_tx); //ENABLE CHANNEL
    iio_channel_enable(chn_rx); //ENABLE CHANNEL
    printf("continue \n");
    transmit(ctx,txSignal);

    free(txSignal);
    iio_channel_disable(chn_rx);
    iio_channel_disable(chn_tx);
    iio_channel_disable(chn_rx_lo);
    iio_context_destroy(ctx);
    return 0;
}

int transmit(struct iio_context *ctx,complex_t * txSignal)
{
    struct iio_device *dev;
    struct iio_channel *tx0_i, *tx0_q;
    struct iio_buffer *txbuf;

    dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");

    tx0_i = iio_device_find_channel(dev, "voltage0", true);
    tx0_q = iio_device_find_channel(dev, "voltage1", true);

    iio_channel_enable(tx0_i);
    iio_channel_enable(tx0_q);

    txbuf = iio_device_create_buffer(dev, 4096, true);
    if (!txbuf) {
        perror("Could not create TX buffer");
        //shutdown();
        iio_context_destroy(ctx);
 
    }
    
    printf("txSignal:\n");
    for (int i = 0; i < 1024; i++) {
        printf("txSignal[%d] = (%f, %f)\n", i, (txSignal[i].re * pow(2, 14)), (txSignal[i].im* pow(2, 14)));
        if(i == 10){
            break;
        }
    }
    //iio_buffer_set_data(txbuf, txSignal);
     printf("Transmitiendo \n");
     printf("Primer dato txSignal[%d] = (%f, %f)\n", 0, (txSignal[0].re * pow(2, 14)), (txSignal[0].im* pow(2, 14)));
     int i = 0;// 16717.000000, 0.000000    a: 12284 b:12286 -16388 b:-16386
    while (!stop) {
        void *p_dat, *p_end, *t_dat;
        ptrdiff_t p_inc;

        iio_buffer_refill(txbuf);

        p_inc = iio_buffer_step(txbuf);// Cuanto se incrementa?
        p_end = iio_buffer_end(txbuf); //

        for (p_dat = iio_buffer_first(txbuf, tx0_i), t_dat = iio_buffer_first(txbuf, tx0_q);
             p_dat < p_end; p_dat += p_inc, t_dat += p_inc) {
            int16_t *buf_i = p_dat;
            int16_t *buf_q = t_dat;
            // for( ; ; )
            
            /* Construct your signal here */
            buf_i = (int16_t)txSignal[i].im; //incremento en la señal
            buf_q = (int16_t)txSignal[i].re;
        }

        iio_buffer_push(txbuf);
    }
    printf("== Finish Sdr_1 ==\n");
    iio_buffer_destroy(txbuf);
}

void complexExp(complex_t *signal, int N, double Fc, double Fs) {
    // N : Number of samples to transmit at once
    // Fc: Carrier frequency in Hz
    // Fs: Sampling frequency in Hz
    if (Fs < 2*Fc) {
        printf("Error: Fs must be at least 2 time Fc\n");
        exit(EXIT_FAILURE);
    }
    double Tsamp  = 1/Fs;
    for (int n = 0; n < N; n++) {
        double angle = 2.0*M_PI*Fc*n*Tsamp;
        signal[n].re = cos(angle);
        signal[n].im = sin(angle);
    }
}

