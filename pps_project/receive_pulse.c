#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

bool stop;

typedef struct {
    double re;
    double im;
} complex_t;
typedef struct {
    double re;
    double im;
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
    FILE *fptr;
    // Open file for writing
    fptr = fopen("output_pulse.txt", "w");

    // Redirect stdout to the file
    freopen("output_pulse.txt", "w", stdout);

    // Write to stdout (will be redirected to file)
    //printf("Hello, world!\n");

    stop = false;

    signal(SIGINT, handle_sig);
    struct iio_context *ctx = NULL;
    ctx = iio_create_default_context();
    if (!ctx) {
        printf("Unable to create IIO context\n");
        return -1;
    }

    
    struct iio_device *main_dev;
    struct iio_device *dev_rx;

    struct iio_channel *rx0_i, *rx0_q;

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
    //else printf("Set correct sampling_frequency RX \n");
    

    int ret2 = iio_channel_attr_write(chnn_device_input, "rf_bandwidth", "16000000");//3999999
    if (ret2 < 0) {
       perror("Error setting rf_bandwidth rate RX: "); 
        return ret2;
    }
    //else printf("Set correct rf_bandwidth RX \n");
    
    /// configurando la salida
    struct iio_channel *chnn_device_output = iio_device_find_channel(main_dev, "voltage0", true);//output 
    int ret_output;
    ret_output = iio_channel_attr_write(chnn_device_output, "sampling_frequency", "700000");
    if (ret_output < 0) {
       perror("Error setting sampling_frequency rate TX : "); 
        return ret_output;
    }
    //else printf("Set correct sampling_frequency TX  \n");
    ret_output = iio_channel_attr_write(chnn_device_output, "rf_bandwidth", "16000000");
    if (ret_output < 0) {
       perror("Error setting rf_bandwidth rate TX: "); 
        return ret_output;
    }
    //else printf("Set correct rf_bandwidth TX  \n");

    ret_output = iio_channel_attr_write(chnn_device_output, "hardwaregain", "-40");//3999999
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
    //else printf("Set correct hardwaregain TX  \n");

    // printf("========================================\n"); 
    
    // int attr_count = iio_channel_get_attrs_count(chnn_device_input);
           
    //         printf("Channel has %d attributes:\n", attr_count);
    //         // print attribute names and values
    //         for (int i = 0; i < attr_count; i++) {
    //             const char *attr = iio_channel_get_attr(chnn_device_input, i);
    //             char val[256];
    //             iio_channel_attr_read(chnn_device_input, attr, val, sizeof(val));
    //             printf("  %s   = %s\n", attr,val);
    //         }
    // printf("========================================\n"); 
     


    double rx_hardwaregain_get;
    double rx_hardwaregain = 10;

    iio_channel_attr_write_double(chnn_device_input, "hardwaregain", rx_hardwaregain);
    iio_channel_attr_read_double(chnn_device_input, "hardwaregain", &rx_hardwaregain_get);
    
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
    //else printf("Set correct hardwaregain RX  \n");
    
    //printf("Nuevo hardwaregain : %f \n",rx_hardwaregain_get);
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
    //else printf("Set correct gain_control_mode RX  \n");
    struct iio_channel *chnn_altvoltage1_output = iio_device_find_channel(main_dev, "altvoltage1", true);//output 
    struct iio_channel *chnn_altvoltage0_output = iio_device_find_channel(main_dev, "altvoltage0", true);//output 
    ret_output = iio_channel_attr_write(chnn_altvoltage1_output, "frequency", "710000000");//# Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_output < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_output;
    }
    //else printf("Set correct frecuency RX  \n");

    ret_output = iio_channel_attr_write(chnn_altvoltage0_output, "frequency", "710000000");//#
    if (ret_output < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_output;
    }
    //else printf("Set correct frecuency RX  \n");
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //RECEPTOR
    
    dev_rx = iio_context_find_device(ctx, "cf-ad9361-lpc"); //dispositivo para enviar
    rx0_i = iio_device_find_channel(dev_rx, "voltage0", false);//(input)
    rx0_q = iio_device_find_channel(dev_rx, "voltage1", false);//(input)
    

    iio_channel_enable(rx0_i);
    iio_channel_enable(rx0_q);


    //size_t BufferSize     = 1048500;
    size_t BufferSize     = 4096 * 171;//tamaño maximo permitido 409600
    
    rxbuf = iio_device_create_buffer(dev_rx, BufferSize, false);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
     if (!rxbuf) {
         perror("Could not create rxbuf buffer");
          iio_context_destroy(ctx);
     }

    int nSamples     = 4096 ;
    size_t samples_count = nSamples ;

    //Funcion a recibir
	complex_rx *rxSignal = (complex_rx *) malloc(samples_count * sizeof(complex_rx));
	if (rxSignal == NULL) {
	    printf("Error allocating memory\n");
	    return 1;
	}


    double signal_i[nSamples];
    double signal_q[nSamples];
    for (int i = 0; i < nSamples; i++) {
    //     //printf("shaped_signal[%d] = %f\n", i, signal[i]);
    //     signal_i[i] = 0;
    //     signal_q[i] = 0;
    //     printf("shaped_signal[%d] = %f\n", i,signal_i[i]);
    //     printf("shaped_signal[%d] = %f\n", i,signal_q[i]);
     }

	char *p_dat, *p_end;
	ptrdiff_t p_inc;
    int increm = 0,flag = 1;
    iio_buffer_refill(rxbuf);
    increm = 0;
    p_inc = iio_buffer_step(rxbuf);
	p_end = iio_buffer_end(rxbuf);
    
    for (p_dat = (char *)iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc) {        
        signal_q[increm] = (double) (((int16_t*)p_dat)[0] / pow(2, 14)); //i real
        signal_i[increm] = (double) (((int16_t*)p_dat)[1] / pow(2, 14)); //q ima
        increm = increm + 1;
        if(increm == (nSamples - 1) ){
            increm = 0;
            flag =  flag + 1;
        }    
	}
    int n_Sample=nSamples;
    // for (int i = 0; i < n_Sample; i++) {
    //     //printf("shaped_signal[%d] = %f\n", i, signal[i]);
    //     printf("pulse_i[%d] = %f\n", i,signal_i[i]);
    //     printf("pulse_q[%d] = %f\n", i,signal_q[i]);
    // }

    printf("[");
     for (int i = 0; i < n_Sample; i++) {
        
         if(i < (n_Sample-1)){
             if(signal_i[i]>=0){
                 printf("%f +%fj,", signal_q[i] , signal_i[i]);   
          }
             else printf(" %f %fj,", signal_q[i] , signal_i[i]);
         }
         else {
             if(signal_i[i]>=0)
             {
                 printf("%f +%fj", signal_q[i] , signal_i[i]);   
             }
             else printf(" %f %fj", signal_q[i] , signal_i[i]);
         }
     }
     printf("]\n");


    free(rxSignal);
    
    
    iio_channel_disable(rx0_i);
    iio_channel_disable(rx0_q);
    iio_buffer_destroy(rxbuf);

    iio_context_destroy(ctx);
    fclose(fptr);
    //freopen("/dev/tty", "w", stdout);
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

  





    //printf("txSignal:\n");
    //for (int i = 0; i < 11; i++) {
    //    printf("txSignal[%d] = (%f, %f)\n", i, (signal[i].re ), (signal[i].im));
    //}
}
