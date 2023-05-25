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
    //FILE *fptr;
    // Open file for writing
    //fptr = fopen("output.txt", "w");

    // Redirect stdout to the file
    //freopen("output.txt", "w", stdout);

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
    int ret_rx;

    struct iio_device *main_dev = iio_context_find_device(ctx, "ad9361-phy");
    const void  *filename = "filter.txt";
    int ret = iio_device_attr_write_raw(main_dev, "filter_fir_config", filename, strlen(filename) + 1);
    if (ret < 0) {
        printf("Error loading filter configuration: %s\n", strerror(-ret));
        iio_context_destroy(ctx);
        return -1;
    }
    printf("Filter configuration loaded successfully\n");


    dev_rx = iio_context_find_device(ctx, "cf-ad9361-lpc"); //dispositivo para recibir

// /** @brief Set the value of the given device-specific attribute
//  * @param dev A pointer to an iio_device structure
//  * @param attr A NULL-terminated string corresponding to the name of the
//  * attribute
//  * @param src A pointer to the data to be written
//  * @param len The number of bytes that should be written
//  * @return On success, the number of bytes written
//  * @return On error, a negative errno code is returned */
// __api __check_ret ssize_t iio_device_attr_write_raw(const struct iio_device *dev,
//		const char *attr, const void *src, size_t len);
    int num_channels;
    int num_attrs = iio_device_get_attrs_count(main_dev);
        //printf("Device----------------: %s\n",name);
        printf("%d device-specific attributes found\n", num_attrs);
        num_channels = iio_device_get_channels_count(main_dev);//ANALISIS DE CANAL
        printf("Number of channels for this device : %d\n", num_channels);
        printf("Device attributes\n\n");
        //ANALISIS DEL DISPOSITIVO
        for (int j = 0; j < num_attrs; j++) 
        {
                
                const char *attr = iio_device_get_attr(main_dev, j);
                char *value = calloc(1, 1024); // Allocate memory for attribute value
                iio_device_attr_read(main_dev, attr, value, 1024);
                printf("\tattr %2d: %-32s value: %s\n", j, attr, value);
                free(value); // Free allocated memory
        }

    // Load the filter configuration file
   // const char *filename = "RX 3 GAIN -6 DEC 4 TX 3 GAIN 0 INT 4 \n-15,-15\n -27,-27 -23,-23 -6,-6 17,17 33,33 31,31 9,9 -23,-23 -47,-47 -45,-45 -13,-13 34,34 69,69 67,67 21,21 -49,-49 -102,-102 -99,-99 -32,-32 69,69 146,146 143,143 48,48 -96,-96 -204,-204 -200,-200 -69,-69 129,129 278,278 275,275 97,97 -170,-170 -372,-372 -371,-371 -135,-135 222,222 494,494497,497187,187-288,-288-654,-654-665,-665-258,-258376,376875,875902,902363,363-500,-500-1201,-1201-1265,-1265-530,-530699,6991748,17481906,1906845,845-1089,-1089-2922,-2922-3424,-3424-1697,-16972326,23267714,771412821,1282115921,1592115921,1592112821,128217714,77142326,2326-1697,-1697-3424,-3424-2922,-2922-1089,-1089845,8451906,19061748,1748699,699-530,-530-1265,-1265-1201,-1201-500,-500363,363902,902875,875376,376-258,-258-665,-665-654,-654-288,-288187,187497,497494,494222,222-135,-135-371,-371-372,-372-170,-17097,97275,275278,278129,129-69,-69-200,-200-204,-204-96,-9648,48143,143146,14669,69-32,-32-99,-99-102,-102-49,-4921,2167,6769,6934,34-13,-13-45,-45-47,-47-23,-239,931,3133,3317,17-6,-6-23,-23-27,-27-15,-15";
    // FILE *archivo;
 	
 	// char caracteres[100];
 	
 	// archivo = fopen("filter.txt","r");
 	
 	// if (archivo == NULL)
    // {
    //     return 0;
    // }
 	// else
    // {
 	//     printf("\nEl contenido del archivo de prueba es \n\n");
 	//     while (feof(archivo) == 0)
 	//     {
 	//         fgets(caracteres,100,archivo);
 	//         printf("%s",caracteres);
 	//     }
        
    //     //system("PAUSE");
    // }
    
    

    
    // fclose(archivo);
    //configuracion de la entrada
    /// configurando la salida
    struct iio_channel *chnn_device_output = iio_device_find_channel(main_dev, "voltage0", true);//output 
    
    // ret_rx = iio_channel_attr_write(chnn_device_output, "sampling_frequency", "2500000");
    // if (ret_rx < 0) {
    //    perror("Error setting sampling_frequency rate TX : "); 
    //     return ret_rx;
    // }
    //else printf("Set correct sampling_frequency TX  \n");

    ret_rx = iio_channel_attr_write(chnn_device_output, "rf_bandwidth", "16000000");
    if (ret_rx < 0) {
       perror("Error setting rf_bandwidth rate TX: "); 
        return ret_rx;
    }


    //iio_channel_enable(chnn_device_input);
    struct iio_channel *chnn_device_input = iio_device_find_channel(main_dev, "voltage0", false);//input 
    iio_channel_attr_write(chnn_device_input, "rf_port_select", "A");


    
    ret_rx = iio_channel_attr_write(chnn_device_input, "rf_bandwidth", "16000000");//3999999
    if (ret_rx < 0) {
       perror("Error setting rf_bandwidth rate RX: "); 
        return ret_rx;
    }
    else printf("Set correct rf_bandwidth RX \n");
    printf("====================================\n");   
    const char *name_channel = iio_channel_get_name(chnn_device_input);
    printf("Channel name----------: %s\n", name_channel);
    int attr_count = iio_channel_get_attrs_count(chnn_device_input);
           
            printf("Channel has %d attributes:\n", attr_count);
            // print attribute names and values
            for (int i = 0; i < attr_count; i++) {
                const char *attr = iio_channel_get_attr(chnn_device_input, i);
                char val[256];
                iio_channel_attr_read(chnn_device_input, attr, val, sizeof(val));
                printf("  %s   = %s\n", attr,val);
            }
    printf("====================================\n");   
    //long long val = 10000000; //700000;
    long long val = 700000;
    ret_rx = iio_channel_attr_write_longlong(chnn_device_input, "sampling_frequency",val);//3999999
    if (ret_rx < 0) {
        perror("Error setting sampling_frequency rate aca RX: "); 
        return ret_rx;
     }
    else printf("Set correct sampling_frequency RX \n");
    
    
    //else printf("Set correct rf_bandwidth TX  \n");

    ret_rx = iio_channel_attr_write(chnn_device_output, "hardwaregain", "-40");//3999999
    if (ret_rx < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_rx;
    }
    //else printf("Set correct hardwaregain TX  \n");

    double rx_hardwaregain_get;
    double rx_hardwaregain = 10;

    iio_channel_attr_write_double(chnn_device_input, "hardwaregain", rx_hardwaregain);
    ret_rx = iio_channel_attr_read_double(chnn_device_input, "hardwaregain", &rx_hardwaregain_get);
    
    if (ret_rx < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_rx;
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
    ret_rx = iio_channel_attr_write(chnn_device_input, "gain_control_mode", "slow_attack");//# Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_rx < 0) {
       perror("Error setting gain_control_mode rate RX: "); 
        return ret_rx;
    }
    //else printf("Set correct gain_control_mode RX  \n");
    struct iio_channel *chnn_altvoltage1_output = iio_device_find_channel(main_dev, "altvoltage1", true);//output 
    struct iio_channel *chnn_altvoltage0_output = iio_device_find_channel(main_dev, "altvoltage0", true);//output 
    ret_rx = iio_channel_attr_write(chnn_altvoltage1_output, "frequency", "710000000");//# Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_rx < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_rx;
    }
    //else printf("Set correct frecuency RX  \n");

    ret_rx = iio_channel_attr_write(chnn_altvoltage0_output, "frequency", "710000000");//#
    if (ret_rx < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_rx;
    }
    //else printf("Set correct frecuency RX  \n");
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //RECEPTOR
    
   
    rx0_i = iio_device_find_channel(dev_rx, "voltage0", false);//(input)
    rx0_q = iio_device_find_channel(dev_rx, "voltage1", false);//(input)
    

    iio_channel_enable(rx0_i);
    iio_channel_enable(rx0_q);


    size_t BufferSize     = 1024;
    rxbuf = iio_device_create_buffer(dev_rx, BufferSize, false);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
     if (!rxbuf) {
         perror("Could not create TX buffer");
          iio_context_destroy(ctx);
     }

    size_t samples_count = 1024 ;
    //Funcion a recibir
	complex_rx *rxSignal = (complex_rx *) malloc(samples_count * sizeof(complex_rx));
	if (rxSignal == NULL) {
	    printf("Error allocating memory\n");
	    return 1;
	}
    //Recibir
    //printf("Recibir...\n");
    // Refill RX buffer
    //ssize_t nbytes_tx;
	char *p_dat, *p_end;
	ptrdiff_t p_inc;
    int increm = 0;


    iio_buffer_refill(rxbuf);
    increm = 0;
    p_inc = iio_buffer_step(rxbuf);
	p_end = iio_buffer_end(rxbuf);
    
    for (p_dat = (char *)iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc) {
        
        const int16_t i = ((int16_t*)p_dat)[0]; // Real (I)
		const int16_t q = ((int16_t*)p_dat)[1]; // Imag (Q)
        rxSignal[increm].re = (double) (i / pow(2, 14));
        rxSignal[increm].im = (double) (q / pow(2, 14));
        increm = increm + 1;
	}
    
    // for (int i = 0; i < 20; i++) {
    //     printf("rxSignal[%d] = (%f, %f) \n", i, (rxSignal[i].re ), (rxSignal[i].im));
    // } 
    int n_Sample=1024;
    printf("[");
    for (int i = 0; i < n_Sample; i++) {
       
        if(i < 99){
            if(rxSignal[i].im>=0){
                printf("%f +%fj,", rxSignal[i].re , rxSignal[i].im);   
            }
            else printf(" %f %fj,", rxSignal[i].re , rxSignal[i].im);
        }
        else {
            if(rxSignal[i].im>=0)
            {
                printf("%f +%fj", rxSignal[i].re , rxSignal[i].im);   
            }
            else printf(" %f %fj", rxSignal[i].re , rxSignal[i].im);
        }
        
    }
    printf("]\n");
    //while (!stop);
    
    free(rxSignal);
    
    
    iio_channel_disable(rx0_i);
    iio_channel_disable(rx0_q);
    iio_buffer_destroy(rxbuf);

    iio_context_destroy(ctx);
    //fclose(fptr);
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
    printf("txSignal:\n");
    //for (int i = 0; i < 11; i++) {
    //    printf("txSignal[%d] = (%f, %f)\n", i, (signal[i].re ), (signal[i].im));
    //}
}
