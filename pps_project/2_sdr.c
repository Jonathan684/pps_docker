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
    double re;
    double im;
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

    struct iio_device *dev_rx;
    struct iio_device *dev_main;
    struct iio_channel *rx0_i, *rx0_q;
    struct iio_buffer *rxbuf;

    /* Set sample rate */


    dev_main = iio_context_find_device(ctx, "ad9361-phy"); //dispositivo para enviar
    //configuracion de la entrada
    struct iio_channel *chnn_device_input = iio_device_find_channel(dev_main, "voltage0", false);//input 
    int ret = iio_channel_attr_write(chnn_device_input, "sampling_frequency", "700000");//3999999
    
    if (ret < 0) {
       perror("Error setting sampling_frequency rate RX: "); 
        return ret;
    }
    else printf("Set correct sampling_frequency RX \n");
    
    int ret2 = iio_channel_attr_write(chnn_device_input, "rf_bandwidth", "16000000");//3999999
    if (ret2 < 0) {
       perror("Error setting rf_bandwidth rate RX: "); 
        return ret2;
    }
    else printf("Set correct rf_bandwidth RX \n");
   
    /// configurando la salida
    struct iio_channel *chnn_device_output = iio_device_find_channel(dev_main, "voltage0", true);//output 
    int ret_output;
    ret_output = iio_channel_attr_write(chnn_device_output, "sampling_frequency", "700000");//3999999
    if (ret_output < 0) {
       perror("Error setting sampling_frequency rate TX : "); 
        return ret_output;
    }
    else printf("Set correct sampling_frequency TX  \n");
    ret_output = iio_channel_attr_write(chnn_device_output, "rf_bandwidth", "16000000");//3999999
    if (ret_output < 0) {
       perror("Error setting rf_bandwidth rate TX: "); 
        return ret_output;
    }
    else printf("Set correct rf_bandwidth TX  \n");
//////////////////////////////////////////////////////////////////////////////////////
// sdr.tx_hardwaregain_chan0   = TxAtten # Attenuation applied to TX path, valid range is -90 to 0 dB [dB]  
// -40
     
    ret_output = iio_channel_attr_write(chnn_device_output, "hardwaregain", "-40");//3999999
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
    else printf("Set correct hardwaregain TX  \n");
/////////////////////////////////////////////////////////////////////////////////////////////////////
        int attr_count = iio_channel_get_attrs_count(chnn_device_input);
           
            printf("Channel has %d attributes:\n", attr_count);
            // print attribute names and values
            for (int i = 0; i < attr_count; i++) {
                const char *attr = iio_channel_get_attr(chnn_device_input, i);
                char val[256];
                iio_channel_attr_read(chnn_device_input, attr, val, sizeof(val));
                printf("  %s   = %s\n", attr,val);
            }
    printf("---------------------------\n");    


    double rx_hardwaregain_get;
    double rx_hardwaregain = 10;

    iio_channel_attr_write_double(chnn_device_input, "hardwaregain", rx_hardwaregain);
    iio_channel_attr_read_double(chnn_device_input, "hardwaregain", &rx_hardwaregain_get);
    
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
    else printf("Set correct hardwaregain TX  \n");
    printf("Nuevo hardwaregain : %f \n",rx_hardwaregain_get);
//////////////////////////////////////////////////////////////////////////////////////////////////////    
    /*
    ret_output = iio_channel_attr_write(chnn_device_input, "hardwaregain", "70");// # Gain applied to RX path. Only applicable when gain_control_mode is set to 'manual'
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate RX: "); 
        return ret_output;
    }
    else printf("Set correct hardwaregain RX  \n");
    */
    ret_output = iio_channel_attr_write(chnn_device_input, "gain_control_mode", "slow_attack");//# Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_output < 0) {
       perror("Error setting gain_control_mode rate RX: "); 
        return ret_output;
    }
    else printf("Set correct gain_control_mode RX  \n");

    

   //Frecuencia de la portadora
   //710000000
   //# cat out_altvoltage1_TX_LO_frequency
   //2400000000
    struct iio_channel *chnn_altvoltage1_output = iio_device_find_channel(dev_main, "altvoltage1", true);//output 
    struct iio_channel *chnn_altvoltage0_output = iio_device_find_channel(dev_main, "altvoltage0", true);//output 
    
    ret_output = iio_channel_attr_write(chnn_altvoltage1_output, "frequency", "710000000");//# Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_output < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_output;
    }
    else printf("Set correct frecuency RX  \n");

    
    ret_output = iio_channel_attr_write(chnn_altvoltage0_output, "frequency", "710000000");//# Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_output < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_output;
    }
    else printf("Set correct frecuency RX  \n");
    /////////////////////////////////////////////////////////////////////////////////////////////////
    iio_channel_enable(chnn_device_input);
    iio_channel_enable(chnn_device_output);

    iio_channel_enable(chnn_altvoltage1_output);
    iio_channel_enable(chnn_altvoltage0_output);
    printf("Posible error aqui 0\n");
    dev_rx = iio_context_find_device(ctx, "cf-ad9361-lpc"); //dispositivo para enviar
    
    printf("Posible error aqui 1\n");
    rx0_i = iio_device_find_channel(dev_rx, "voltage0", false);//(input)
    printf("Posible error aqui 2\n");
    rx0_q = iio_device_find_channel(dev_rx, "voltage1", false);//(input)
    printf("Posible error aqui 3\n");

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
    iio_channel_enable(rx0_i);
    iio_channel_enable(rx0_q);

    rxbuf = iio_device_create_buffer(dev_rx, 1024, false);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
    //txbuf = iio_device_create_buffer(dev, 1024, true);//Paso :0 Fin :-1225359360  Paso :-1225359360 Fin :-1225359360
    //txbuf = iio_device_create_buffer(dev, 512, true);   //Paso :0 Fin :-1225533440  Paso :-1225533440 Fin :-1225533440
    if (!rxbuf) {
        perror("Could not create TX buffer");
        //shutdown();
        iio_context_destroy(ctx);
 
    }
    
    
    //int i = 0;// 16717.000000, 0.000000    a: 12284 b:12286 -16388 b:-16386
    //int flag = 0; // 10 datos se enviaran 
    

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

	// Refill RX buffer
    printf("Recibiendo ...\n");
    ssize_t nbytes_rx;
	char *p_dat, *p_end;
	ptrdiff_t p_inc;
    int increm = 0;

	nbytes_rx = iio_buffer_refill(rxbuf); // solo en valida para buffer de entrada.
	//if (nbytes_rx < 0) { printf("Error refilling buf %d\n",(int) nbytes_rx); shutdown(); }
	    // READ: Get pointers to RX buf and read IQ from RX buf port 0
	p_inc = iio_buffer_step(rxbuf);
	p_end = iio_buffer_end(rxbuf);

	for (p_dat = (char *)iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc) {
        txSignal[increm].re = ((int16_t*)p_dat)[0];
        txSignal[increm].im = ((int16_t*)p_dat)[1];
        increm = increm +1;
	}
    printf("txSignal:\n");
    for (int i = 0; i < 11; i++) {
        printf("txSignal[%d] = (%f, %f)\n", i, (txSignal[i].re ), (txSignal[i].im));
    }
    
    
    //sleep(30);
    free(txSignal);
    printf("== Finish Sdr_1 ==\n");
    iio_buffer_destroy(rxbuf);
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
        //printf("angle :%f\n",angle);
    //    printf("angle cos %d: %f\n",n,cos(angle));
    //    printf("angle sin %d: %f\n",n,sin(angle));
        signal[n].re = cos(angle)* pow(2, 14);
        signal[n].im = sin(angle)* pow(2, 14);
    //    printf("angle cos : %f\n",signal[n].re);
    //    printf("angle sin : %f\n",signal[n].im);
    }
    printf("txSignal:\n");
    //for (int i = 0; i < 11; i++) {
    //    printf("txSignal[%d] = (%f, %f)\n", i, (signal[i].re ), (signal[i].im));
    //}
}