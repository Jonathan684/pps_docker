#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
//#include <time.h>
//#include <errno>
#define IIO_CHANNEL_TX 0x1
#define SAMPLE_COUNT 1024 // Para el buffer

bool stop;

typedef struct {
    int16_t re;
    int16_t im;
} complex_t;

void handle_sig(int sig)
{
	printf("Waiting for process to finish... Got signal %d\n", sig);
	stop = true;
}
/*
    Para transmitir verificar:
    - loopback desactivado
    - Frecuencias
    - 
*/
void complexExp(complex_t *signal, int N, double Fc, double Fs);
int main(){
    stop = false;
    signal(SIGINT, handle_sig);
    struct iio_context *ctx = NULL;
    ctx = iio_create_default_context();
    if (!ctx) {
        printf("Unable to create IIO context\n");
        return -1;
    }

    struct iio_device *dev_ad9361;

    struct iio_device *dev;
    struct iio_channel *tx0_i, *tx0_q;
    struct iio_buffer *txbuf;

    /* Set sample rate */


    dev_ad9361 = iio_context_find_device(ctx, "ad9361-phy"); //dispositivo para enviar
    struct iio_channel *chnn_device0 = iio_device_find_channel(dev_ad9361, "voltage0", false);//input 
    int ret = iio_channel_attr_write(chnn_device0, "sampling_frequency", "4999999");//3999999
    if (ret < 0) {
       perror("Error setting TX I sample rate: "); 
        return ret;
    }
    else printf("correct \n");
    dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");   //device to send
    tx0_i = iio_device_find_channel(dev, "voltage0", true);         //device - channel - True (output)
    tx0_q = iio_device_find_channel(dev, "voltage1", true);         //device - channel - True (output)
    
    iio_channel_enable(tx0_i);  //enable channel
    iio_channel_enable(tx0_q);  //enable channel

    txbuf = iio_device_create_buffer(dev, 4096, true);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
    if (!txbuf) 
    {
        perror("Could not create TX buffer");
        iio_context_destroy(ctx);
    }
   
    /*
        Cargar datos a la funcion
    */
     /*----------------------------------------------------------------------------------*/   
    size_t samples_count = 1024;
	complex_t *txSignal = (complex_t *) malloc(samples_count * sizeof(complex_t));
	if (txSignal == NULL) {
	    printf("Error allocating memory\n");
	    return 1;
	}

	double samplingRate = 16.0e6;
	double freq = samplingRate/16.0;
	complexExp(txSignal, 1024, freq, samplingRate);
	
    /*----------------------------------------------------------------------------------*/
    //iio_buffer_set_data(txbuf,&txSignal[0]);
    //txbuf = 999999;
    unsigned char *p_dat, *p_end;
    // Get a pointer to the buffer data
    p_dat = iio_buffer_start(txbuf);
    p_end = p_dat + iio_buffer_step(txbuf);

    // Write data to the buffer
    while (p_dat < p_end) {
        *p_dat++ = 0x00;
        *p_dat++ = 0x80;
    }
    printf("number bytes write : %d\n",iio_buffer_push(txbuf));
    printf("Transmitiendo ... buffer ciclico \n");
    sleep(10);

    printf("== Finish Sdr_1 ==\n");
    iio_buffer_destroy(txbuf);
    iio_context_destroy(ctx);
    return 0;
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