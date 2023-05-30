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
typedef struct {
    int16_t re;
    int16_t im;
} complex_rx;

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

    
    struct iio_device *main_dev;

    struct iio_device *dev_tx;
    struct iio_device *dev_rx;
    
    struct iio_channel *tx0_i, *tx0_q;
    struct iio_channel *rx0_i, *rx0_q;

    struct iio_buffer *txbuf;
    struct iio_buffer *rxbuf;

    main_dev = iio_context_find_device(ctx, "ad9361-phy");
    //configuracion de la entrada
    
    //iio_channel_enable(chnn_device_input);
    struct iio_channel *chnn_device_input = iio_device_find_channel(main_dev, "voltage0", false);//input 
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
    struct iio_channel *chnn_device_output = iio_device_find_channel(main_dev, "voltage0", true);//output 
    int ret_output;
    ret_output = iio_channel_attr_write(chnn_device_output, "sampling_frequency", "700000");
    if (ret_output < 0) {
       perror("Error setting sampling_frequency rate TX : "); 
        return ret_output;
    }
    else printf("Set correct sampling_frequency TX  \n");
    ret_output = iio_channel_attr_write(chnn_device_output, "rf_bandwidth", "16000000");
    if (ret_output < 0) {
       perror("Error setting rf_bandwidth rate TX: "); 
        return ret_output;
    }
    else printf("Set correct rf_bandwidth TX  \n");

    ret_output = iio_channel_attr_write(chnn_device_output, "hardwaregain", "-40");//3999999
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
    else printf("Set correct hardwaregain TX  \n");

    printf("========================================\n"); 
    
    int attr_count = iio_channel_get_attrs_count(chnn_device_input);
           
            printf("Channel has %d attributes:\n", attr_count);
            // print attribute names and values
            for (int i = 0; i < attr_count; i++) {
                const char *attr = iio_channel_get_attr(chnn_device_input, i);
                char val[256];
                iio_channel_attr_read(chnn_device_input, attr, val, sizeof(val));
                printf("  %s   = %s\n", attr,val);
            }
    printf("========================================\n"); 
     


    double rx_hardwaregain_get;
    double rx_hardwaregain = 10;

    iio_channel_attr_write_double(chnn_device_input, "hardwaregain", rx_hardwaregain);
    iio_channel_attr_read_double(chnn_device_input, "hardwaregain", &rx_hardwaregain_get);
    
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
    else printf("Set correct hardwaregain RX  \n");
    printf("Nuevo hardwaregain : %f \n",rx_hardwaregain_get);
    int attr_count2 = iio_channel_get_attrs_count(chnn_device_output);
           
            printf("Channel has %d attributes:\n", attr_count2);
            // print attribute names and values
            for (int i = 0; i < attr_count2; i++) {
                const char *attr = iio_channel_get_attr(chnn_device_output, i);
                char val[256];
                iio_channel_attr_read(chnn_device_output, attr, val, sizeof(val));
                printf("  %s   = %s\n", attr,val);
            }
    printf("========================================\n");    
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
    struct iio_channel *chnn_altvoltage1_output = iio_device_find_channel(main_dev, "altvoltage1", true);//output 
    struct iio_channel *chnn_altvoltage0_output = iio_device_find_channel(main_dev, "altvoltage0", true);//output 
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
    //RECEPTOR
    
    dev_rx = iio_context_find_device(ctx, "cf-ad9361-lpc"); //dispositivo para enviar
    rx0_i = iio_device_find_channel(dev_rx, "voltage0", false);//(input)
    rx0_q = iio_device_find_channel(dev_rx, "voltage1", false);//(input)
    

    //TRANSMISOR
    dev_tx = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc"); //dispositivo para enviar
    tx0_i = iio_device_find_channel(dev_tx, "voltage0", true);
    tx0_q = iio_device_find_channel(dev_tx, "voltage1", true);

    iio_channel_enable(rx0_i);
    iio_channel_enable(rx0_q);
    iio_channel_enable(tx0_i);
    iio_channel_enable(tx0_q);

    size_t BufferSize     = 1024;
    rxbuf = iio_device_create_buffer(dev_rx, BufferSize, false);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
     if (!rxbuf) {
         perror("Could not create TX buffer");
          iio_context_destroy(ctx);
     }

    txbuf = iio_device_create_buffer(dev_tx, BufferSize, true);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
    if (!txbuf) {
    perror("Could not create TX buffer");
        iio_context_destroy(ctx);
    }
    
    //Funcion a transmitir
    size_t samples_count = 1024 ;//1048575;
	complex_t *txSignal = (complex_t *) malloc(samples_count * sizeof(complex_t));
	if (txSignal == NULL) {
	    printf("Error allocating memory\n");
	    return 1;
	}
    //Funcion a recibir
	complex_rx *rxSignal = (complex_rx *) malloc(samples_count * sizeof(complex_rx));
	if (rxSignal == NULL) {
	    printf("Error allocating memory\n");
	    return 1;
	}
   
    
    
////////////////////////////////////////////////////////////////

    double samplingRate = 16.0e6;
    double freq = samplingRate/16.0;
    int N = 1024;//1048575;//1024;
    complexExp(txSignal, N, freq, samplingRate);

    ssize_t nbytes_tx;
	char *p_dat, *p_end;
	ptrdiff_t p_inc;
    int increm = 0;
   
    printf("Transmitiendo ... buffer ciclico \n");
    nbytes_tx = iio_buffer_push(txbuf);// solo es valida para buffer de salida.
    if (nbytes_tx < 0) { 
        printf("Error pushing buf %d\n", (int) nbytes_tx); 
    }
    // WRITE: Get pointers to TX buf and write IQ to TX buf port 0
	p_inc = iio_buffer_step(txbuf);
	p_end = iio_buffer_end(txbuf);
    printf(" -- \n");
    for (p_dat = (char *)iio_buffer_first(txbuf, tx0_i); p_dat < p_end; p_dat += p_inc) {
        ((int16_t*)p_dat)[0] =  (int16_t)txSignal[increm].re; // Real (I)
        ((int16_t*)p_dat)[1] = (int16_t)txSignal[increm].im; // Imag (Q)
        increm = increm + 1;
    }
    sleep(2);
    //Recibir

    
    printf("Recibir...\n");
    // Refill RX buffer
    
    iio_buffer_refill(rxbuf);
    increm = 0;
    p_inc = iio_buffer_step(rxbuf);
	p_end = iio_buffer_end(rxbuf);
    printf("aqui\n");
    
    for (p_dat = (char *)iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc) {
        
        const int16_t i = ((int16_t*)p_dat)[0]; // Real (I)
		const int16_t q = ((int16_t*)p_dat)[1]; // Imag (Q)
        rxSignal[increm].re = i;
        rxSignal[increm].im = q;
        increm = increm + 1;
	}
    
    // for (int i = 0; i < 20; i++) {
    //     printf("rxSignal[%d] = (%f, %f) \n", i, (rxSignal[i].re ), (rxSignal[i].im));
    // } 
    printf("[");
    for (int i = 0; i < 100; i++) {
        if(rxSignal[i].im>=0){
         printf("%d +%dj,", rxSignal[i].re , rxSignal[i].im);   
        }
        else printf(" %d %dj,", rxSignal[i].re , rxSignal[i].im);
    }
    printf("]\n");
    while (!stop);
    
    free(txSignal);
    free(rxSignal);
    
    
    iio_channel_disable(tx0_i);
    iio_channel_disable(tx0_q);
    iio_channel_disable(rx0_i);
    iio_channel_disable(rx0_q);

    iio_buffer_destroy(txbuf);
    iio_buffer_destroy(rxbuf);

    iio_context_destroy(ctx);
    
    printf("== Finish Sdr_1 ==\n");

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