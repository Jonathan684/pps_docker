#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
//#include <errno>
//#define IIO_CHANNEL_TX 0x1
//#define SAMPLE_COUNT 1024 // Para el buffer

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
*/
void complexExp(complex_t *signal, int N, double Fc, double Fs);
void generatePulse(double *signal, int N, double pulseWidth, double Fs);
void applyRaisedCosineFilter(double*, int, double, int, double*);
int read_config(char **);

int main(){
    int num_lines = 7;
    char *lines[num_lines];
    read_config(lines);
    char *config[num_lines];
    char *values[num_lines];

     for (int i = 0; i < num_lines; i++) {
        //printf("%s\n", lines[i]);  // Imprime la línea leída
        char *key, *value;
        key = strtok(lines[i], " ");
        value = strtok(NULL, " ");

        if (key != NULL && value != NULL) {
            //printf("%s %s \n", key,value);
            config[i] = key;
            values[i] = value;
        } else {
            printf("Error al separar la línea.\n");
        }
    }

    // Imprimir los arreglos separados
    for (int i = 0; i < num_lines; i++) {
        printf("Key: %s, Value: %s\n", config[i], values[i]);
    }

    printf("key 0 values 0 ->%s<- ->%s<-\n",config[6], values[6]);
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
    struct iio_channel *tx0_i, *tx0_q;
    struct iio_buffer *txbuf;
    double samplingRate = 16000000.0;
    int ret_output;
    //int ret_input;
    main_dev = iio_context_find_device(ctx, "ad9361-phy");
    //configuracion de la entrada
    //struct iio_channel *chnn_device_input = iio_device_find_channel(main_dev, "voltage0", false);//input 
    
    //ret_input = iio_channel_attr_write(chnn_device_input, config[0],  values[0]);//3999999
    
    
    
    // if (ret_input < 0) {
    //    perror("Error setting sampling_frequency rate RX: "); 
    //     return ret_input;
    // }
    //else printf("Set correct sampling_frequency RX \n");
    // ret_input = iio_channel_attr_write(chnn_device_input, "rf_bandwidth", "16000000");//3999999
    // if (ret_input < 0) {
    //    perror("Error setting rf_bandwidth rate RX: "); 
    //     return ret_input;
    // }
    //else printf("Set correct rf_bandwidth RX \n");
    /* configurando la salida*/
    struct iio_channel *chnn_device_output = iio_device_find_channel(main_dev, "voltage0", true);//output 
    
    ret_output = iio_channel_attr_write(chnn_device_output, config[0],values[0]);
    if (ret_output < 0) {
       perror("Error setting sampling_frequency rate TX : "); 
        return ret_output;
    }
    //else printf("Set correct sampling_frequency TX  \n");
    ret_output = iio_channel_attr_write(chnn_device_output, config[1],values[1]);
    if (ret_output < 0) {
       perror("Error setting rf_bandwidth rate TX: "); 
        return ret_output;
    }
    //else printf("Set correct rf_bandwidth TX  \n");

    ret_output = iio_channel_attr_write(chnn_device_output, config[4],values[4]);//
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
    //else printf("Set correct hardwaregain TX  \n");

    struct iio_channel *chnn_altvoltage1_output = iio_device_find_channel(main_dev, "altvoltage1", true);//output 
    struct iio_channel *chnn_altvoltage0_output = iio_device_find_channel(main_dev, "altvoltage0", true);//output 
    ret_output = iio_channel_attr_write(chnn_altvoltage1_output, config[6], values[6]);//frequency 710000000  # Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_output < 0) { 
       perror("Error setting frecuency rate RX: [Transmisor]"); 
        return ret_output;
    }
    //else printf("Set correct frecuency RX  \n");

    ret_output = iio_channel_attr_write(chnn_altvoltage0_output, config[6], values[6]);//frequency 710000000              # Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_output < 0) { 
       perror("Error setting frecuency rate RX: [Transmisor]"); 
        return ret_output;
    }
    //else printf("Set correct frecuency RX  \n");

    ///////////////////////
    ///// TRANSMISOR  /////
    ///////////////////////
    dev_tx = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc"); //dispositivo para enviar
    tx0_i = iio_device_find_channel(dev_tx, "voltage0", true);
    tx0_q = iio_device_find_channel(dev_tx, "voltage1", true);
    iio_channel_enable(tx0_i);
    iio_channel_enable(tx0_q);
    //size_t TxBufferSize     = 1048574;
    size_t TxBufferSize     = 4096 * 171;//tamaño maximo permitido 409600
    txbuf = iio_device_create_buffer(dev_tx, TxBufferSize, true);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
    if (!txbuf) {
    perror("Could not create TX buffer");
        iio_context_destroy(ctx);
    }
    //Funcion a transmitir
    size_t samples_count = 4096 ;//1048575;
     //double pulse_signal[SIGNAL_LENGTH];  // Your pulse signal
	complex_t *txSignal = (complex_t *) malloc(samples_count * sizeof(complex_t));
	if (txSignal == NULL) {
	    printf("Error allocating memory\n");
	    return 1;
	}
    
    double freq = samplingRate/16.0;
    int nSamples     = 4096;
    int N = nSamples;//1048575;//1024;
    //double pulseWidth = 0.5;
    complexExp(txSignal, N, freq, samplingRate);
    //void generatePulse(double *signal, int N, double pulseWidth, double Fs)


    ssize_t nbytes_tx;
	char *p_dat, *p_end;
	ptrdiff_t p_inc;
    int increm = 0;
   
    //printf("Transmitiendo ... buffer ciclico \n");
    nbytes_tx = iio_buffer_push(txbuf);// solo es valida para buffer de salida.
    if (nbytes_tx < 0) { 
        printf("Error pushing buf %d\n", (int) nbytes_tx); 
     //   shutdown(); 
    }
    // WRITE: Get pointers to TX buf and write IQ to TX buf port 0
	p_inc = iio_buffer_step(txbuf);
	p_end = iio_buffer_end(txbuf);
   
    for (p_dat = (char *)iio_buffer_first(txbuf, tx0_i); p_dat < p_end; p_dat += p_inc) {
        ((int16_t*)p_dat)[0] =  (int16_t)txSignal[increm].re; // Real (I)
        ((int16_t*)p_dat)[1] = (int16_t)txSignal[increm].im; // Imag (Q)
        increm = increm + 1;
        if(increm == 4095){
            increm = 0;
        }
    }
    
    //while (!stop);
    sleep(10);

    free(txSignal);
    // Liberar la memoria asignada
    for (int i = 0; i < num_lines; i++) {
        free(lines[i]);
    } 

    //printf("== Finish Sdr_1 ==\n");
    iio_channel_disable(tx0_i);
    iio_channel_disable(tx0_q);
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
        signal[n].re = cos(angle)* pow(2, 14);
        signal[n].im = sin(angle)* pow(2, 14);
    }
    //printf("txSignal:\n");
    //for (int i = 0; i < 11; i++) {
    //    printf("txSignal[%d] = (%f, %f)\n", i, (signal[i].re ), (signal[i].im));
    //}
}



int read_config(char **lines) {
    FILE *archivo;
    //char linea[100];

    // Abre el archivo en modo de lectura ("r")
    archivo = fopen("transmition_config.txt", "r");

    // Comprueba si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo 2.\n");
        return 1;
    }

    // Lee el archivo línea por línea
    //int count = 0;
    
    //char buffer[100]
    // while (fgets(linea, sizeof(linea), archivo) != NULL) {
    //     printf("%s", linea);  // Imprime la línea leída
    //     if(count !=0 ){
    //         buffer[count] linea;  
    //     }
    //     count++;
    // }
    // Lee el archivo línea por línea y guarda cada línea en el arreglo
    int num_lines = 8;
    char linea[100];
    //char *lines[num_lines];
    
    for (int i = 0; i < num_lines; i++) {
        if (fgets(linea, sizeof(linea), archivo) != NULL) {
            // Elimina el carácter de nueva línea si existe
            size_t linea_length = strlen(linea);
            if (linea[linea_length - 1] == '\n') {
                linea[linea_length - 1] = '\0';
                linea_length--;
            }
            // Copia la línea al arreglo
            lines[i] = malloc(linea_length + 1);
           
            strcpy(lines[i], linea);
        } else {
            printf("El archivo no tiene suficientes líneas.\n");
            break;
        }
        
    }

    // Cierra el archivo
    fclose(archivo);
    // Separar y mostrar las líneas
   

    // // Liberar la memoria asignada
    // for (int i = 0; i < num_lines; i++) {
    //     free(lines[i]);
    // }
    
    return 0;
}
