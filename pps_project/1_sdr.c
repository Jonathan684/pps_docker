#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <time.h>
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

    struct iio_device *dev;
    struct iio_device *dev2;
    struct iio_channel *tx0_i, *tx0_q;
    struct iio_buffer *txbuf;

    /* Set sample rate */


    dev2 = iio_context_find_device(ctx, "ad9361-phy"); //dispositivo para enviar
    struct iio_channel *chnn_device0 = iio_device_find_channel(dev2, "voltage0", false);//input 
    int ret = iio_channel_attr_write(chnn_device0, "sampling_frequency", "4999999");//3999999
    if (ret < 0) {
       perror("Error setting TX I sample rate: "); 
        return ret;
    }
    else printf("correct \n");
    dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc"); //dispositivo para enviar
    tx0_i = iio_device_find_channel(dev, "voltage0", true);
    tx0_q = iio_device_find_channel(dev, "voltage1", true);

    /* Set sample rate */
    // printf("Set frecuencia ... \n");
    // int ret = iio_channel_attr_write(tx0_i, "sampling_frequency", "2000000");
    // if (ret < 0) {
    //     perror("Error setting TX I sample rate: ");
    //     return ret;
    // }
    // ret = iio_channel_attr_write(tx0_q, "sampling_frequency", "2000000");
    // if (ret < 0) {
    //     perror("Error setting TX Q sample rate: ");
    //     return ret;
    // }
    iio_channel_enable(tx0_i);
    iio_channel_enable(tx0_q);

    txbuf = iio_device_create_buffer(dev, 4096, true);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
    //txbuf = iio_device_create_buffer(dev, 1024, true);//Paso :0 Fin :-1225359360  Paso :-1225359360 Fin :-1225359360
    //txbuf = iio_device_create_buffer(dev, 512, true);   //Paso :0 Fin :-1225533440  Paso :-1225533440 Fin :-1225533440
    if (!txbuf) {
        perror("Could not create TX buffer");
        //shutdown();
        iio_context_destroy(ctx);
 
    }
    
    
    //int i = 0;// 16717.000000, 0.000000    a: 12284 b:12286 -16388 b:-16386
    int flag = 0; // 10 datos se enviaran 
    

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
    iio_buffer_set_data(txbuf,&txSignal[0]);
    
    iio_buffer_push(txbuf);
    printf("Transmitiendo ... buffer ciclico \n");
    sleep(10);

    // while(!stop)
    // {
    //     sleep(10);
    //    if(flag == 1000)
    //    {
    //     stop = true;
    //     }
    //     flag ++; 
    // }
    
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

// if (ret < 0) {
    //     printf("error setting buffer data: %s\n", strerror(-ret));
    //     return ret;
    // }
    //txbuf = txSignal[0];
	//void *data;
	//int flag = 0;
	//iio_buffer_set_data(txbuf,(void *) );




// while (!stop) { // ctrl + K + C
    //     void *p_dat, *p_end, *t_dat;
    //     ptrdiff_t p_inc;

    //     //iio_buffer_refill(txbuf); No es valida para buffer de salida solo de entrada 

    //     p_inc = iio_buffer_step(txbuf);// Cuanto se incrementa? 
    //     // if (p_inc < 0) {
    //     //    perror("iio_buffer_step");
    //     //    }
    //     //else printf("Correct \n");
    //     p_end = iio_buffer_end(txbuf); //
    //     //printf("\tPaso :%d Fin :%d\n",(int)p_dat,p_end);
    //     for (p_dat = iio_buffer_first(txbuf, tx0_i), t_dat = iio_buffer_first(txbuf, tx0_q);
    //          p_dat < p_end; p_dat += p_inc, t_dat += p_inc) {
    //         int16_t *buf_i = p_dat;
    //         int16_t *buf_q = t_dat;
    //         // for( ; ; )
    //         /* Construct your signal here */
    //         //buf_i = (int16_t *)16000; //incremento en la señal Paso :0 Fin :-1230217216  Paso :-1230217216 Fin :-1230217216
    //         buf_i = (int16_t *)4095; //incremento en la señal Paso :0 Fin :-1230217216  Paso :-1230217216 Fin :-1230217216
    //         buf_q = (int16_t *)4095; //DAC 12 bit 2^12
    //     }

    //     iio_buffer_push(txbuf);

    //     if(flag == 1000){
    //         stop = true;
    //     }
    //     flag ++;
    // }