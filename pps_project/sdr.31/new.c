#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
//#include <errno>
#define IIO_CHANNEL_TX 0x1
#define SAMPLE_COUNT 1024 // Para el buffer
#define FILTER_LENGTH 4096
#define SIGNAL_LENGTH 4096
//#define FILTER_LENGTH 101


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
void generatePulse(double *signal, int N);
int num_lines();
int read_config(char *config[], char *values[]);
//void applyRaisedCosineFilter(double*, int, double, int, double*);

int main(){
    //printf("Transmision de un pulso\n");
    int num_linea = num_lines();
    char *config[num_linea];
    char *values[num_linea];

    read_config(config,values);
    
    // Imprimir los arreglos separados
    // for (int i = 0; i < num_linea; i++) {
    //     printf("Attr : %s, Value: %s\n", config[i], values[i]);
    // }

    stop = false;
    signal(SIGINT, handle_sig);
    struct iio_context *ctx = NULL;
    ctx = iio_create_default_context();
    if (!ctx) {
        printf("Unable to create IIO context\n");
        return -1;
    }
    
    //printf("\t  TRANSMITTER\n");
    struct iio_device *main_dev;
    struct iio_device *dev_tx;
    struct iio_channel *tx0_i, *tx0_q;
    struct iio_buffer *txbuf;
    double samplingRate = 16000000.0;
    int ret_output;
    main_dev = iio_context_find_device(ctx, "ad9361-phy");
    
    /// configurando la salida
    struct iio_channel *chnn_device_output = iio_device_find_channel(main_dev, "voltage0", true);//output 
    
    ret_output = iio_channel_attr_write(chnn_device_output, config[0], values[0]);//sampling_frequency 700000
    if (ret_output < 0) {
       perror("Error setting sampling_frequency rate TX : "); 
        return ret_output;
    }

    ret_output = iio_channel_attr_write(chnn_device_output,config[1], values[1]);
    if (ret_output < 0) {
       perror("Error setting rf_bandwidth rate TX: "); 
        return ret_output;
    }
    //else printf("Set correct rf_bandwidth TX  \n");

    ret_output = iio_channel_attr_write(chnn_device_output,config[2], values[2]);//hardwaregain -10
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
   
    struct iio_channel *chnn_altvoltage1_output = iio_device_find_channel(main_dev, "altvoltage1", true);//output 
    ret_output = iio_channel_attr_write(chnn_altvoltage1_output, config[3], values[3]);//# Frecuencia de la portadora 710000000
    if (ret_output < 0) { 
       perror("Error setting frecuency rate TX: [Transmisor] frecuencia portadora"); 
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
    size_t TxBufferSize     = 1048575 ;//4096 * 171;//tamaño maximo permitido 409600
    txbuf = iio_device_create_buffer(dev_tx, TxBufferSize, true);//Paso :0 Fin :-1225617408  Paso :-1225617408 Fin :-1225617408
    if (!txbuf) {
    perror("Could not create TX buffer");
        iio_context_destroy(ctx);
    }
    //Funcion a transmitir
    size_t samples_count = 1048575;// 4096 ;//1048575;
     //double pulse_signal[SIGNAL_LENGTH];  // Your pulse signal
	complex_t *txSignal = (complex_t *) malloc(samples_count * sizeof(complex_t));
	if (txSignal == NULL) {
	    printf("Error allocating memory\n");
	    return 1;
	}
    
    double freq = samplingRate/16.0;
    
    int nSamples     = 4096;
    int N = nSamples;//1048575;//1024;
  
    complexExp(txSignal, N, freq, samplingRate);
    //void generatePulse(double *signal, int N, double pulseWidth, double Fs)
    double signal[N];
    //double pulseWidth = 0.1; // 0.1 seconds
    //double Fs = 1000; // Sampling frequency of 1000 Hz
    generatePulse(signal, N);

    // for (int i = 0; i < 10; i++) {
    //     printf("shaped_signal[%d] = %f\n", i, signal[i]);
    // }
    // // //     signal_i[i] = 0;
    // //     signal_q[i] = 0;
    //     printf("signal_pulse[%d] = %f\n", i,signal[i]);
    // //     printf("shaped_signal[%d] = %f\n", i,signal_q[i]);
    

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
         //((int16_t*)p_dat)[0] =  (int16_t)txSignal[increm].re; // Real (I)
         //((int16_t*)p_dat)[1] = (int16_t)txSignal[increm].im; // Imag (Q)
        
         ((int16_t*)p_dat)[0] = signal[increm];
         ((int16_t*)p_dat)[1] =(int16_t)0;// (int16_t)(1*pow(2, 14));//signal[increm];
         
         //((int16_t*)p_dat)[1] = (int16_t)0;
          increm = increm + 1;
          if(increm == 4095){
              increm = 0;
          }
     }
    
    while (!stop);
    //sleep(10);                                                               //710000000
    ret_output = iio_channel_attr_write(chnn_altvoltage1_output, "frequency", "710000000");//# Frecuencia de la portadora 710000000
    if (ret_output < 0) { 
       perror("Error setting frecuency rate TX: [Transmisor] frecuencia portadora"); 
        return ret_output;
    }
    ret_output = iio_channel_attr_write(chnn_device_output,"hardwaregain", "-89");//hardwaregain -10
    if (ret_output < 0) {
       perror("Error setting hardwaregain rate TX: "); 
        return ret_output;
    }
    memset(signal, 0x00, 4096);
    free(txSignal);
    iio_buffer_destroy(txbuf);
    //printf("== Finish Sdr_1 ==\n");
    iio_channel_disable(tx0_i);
    iio_channel_disable(tx0_q);
    iio_context_destroy(ctx);
    //printf("Fin\n");
    return 0;
}

int read_config(char *config[], char *values[]) {
    FILE *archivo;
    // Abre el archivo en modo de lectura ("r")
    archivo = fopen("transmition_config.txt", "r");
    // Comprueba si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo 2.\n");
        return 1;
    }
    int num_linea = num_lines();
    //printf("num_linea : %d\n",num_linea);
    char *lines[num_linea];

    char linea[100];
    // Lee el archivo línea por línea y guarda cada línea en el arreglo
    for (int i = 0; i < num_linea; i++) {
        if (fgets(linea, sizeof(linea), archivo) != NULL) {
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

    for (int i = 0; i < num_linea; i++) {
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
    rewind(archivo); // Vuelve al inicio del archivo
    fclose(archivo);
    return 0;
}

int num_lines(){
     FILE *archivo;
    // Abre el archivo en modo de lectura ("r")
    archivo = fopen("transmition_config.txt", "r");
    // Comprueba si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo 2.\n");
        return 1;
    }
    // Lee el archivo línea por línea
    char linea[256];
    int num_lines = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
         num_lines++;
    }
    rewind(archivo); // Vuelve al inicio del archivo
    return num_lines;
}

void generatePulse(double *signal, int N) {
    // N: Number of samples
    // pulseWidth: Width of the pulse in seconds
    // Fs: Sampling frequency in Hz
    //double Tsamp = 1/Fs;
    //int pulseSamples = pulseWidth * Fs;
    //int pulseWidth = 7;//: Width of the pulse in seconds
    
    int Longitud_del_pulso = 8;
    int PRI = 16;
    int count = 0;

    for (int n = 0; n < N; n++) {
        //if(n<10)printf("f:%d\n",f);
        // if ( (count >= 0) && (f < pulseWidth)  ) {
        //     signal[n] = 1 *pow(2, 14) ;
        //     //if(n<10)printf("f entro 2:%d\n",f);
        // }
        // else if ( (count >= 4 ) && (f < (pulseWidth * 2) ) ) {
            
        //     signal[n] = 0.0;//40000;//1.0 ;//* pow(2, 14);
        //     //if(n<10)printf("f entro 3:%d\n",f);
            
        // }
        // count ++;

        if ( (count >= 0) && (count < Longitud_del_pulso)  ) {
            signal[n] = 1 *pow(2, 14) ;
            //if(n<10)printf("f entro 2:%d\n",f);
        }
        else if ( (count >= Longitud_del_pulso ) && (count < PRI ) ) {
            
            signal[n] = 0.0;//40000;//1.0 ;//* pow(2, 14);
            //if(n<10)printf("f entro 3:%d\n",f);
            
        }
        count ++;
        if (count == PRI) count = 0;
        //if (count == PRI) count = 0;

         //if(n<10)printf("f3:%d\n",f);
    }
}