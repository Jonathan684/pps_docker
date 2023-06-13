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
/* Estructuras de configuracion */
struct iio_context *ctx = NULL;
struct iio_device *main_dev;

/* tx */
struct iio_device *dev_tx;
struct iio_channel *tx0_i, *tx0_q;
struct iio_buffer *txbuf;
struct iio_channel *chnn_device_output; 
struct iio_channel *chnn_altvoltage1_output;

/* rx */
//struct iio_device *main_dev;
struct iio_device *dev_rx;
struct iio_channel *rx0_i, *rx0_q;
struct iio_buffer *rxbuf;
struct iio_channel *chnn_device_input ;
struct iio_channel *chnn_altvoltage0_output;

/*------ */
FILE *fptr;/*archivo de salida*/
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

int creando_contexto();
int configuracion_tx();
int configuracion_rx();
void complexExp(complex_t *signal, int N, double Fc, double Fs);
void generatePulse(double *signal, int N);
int num_lines();
int read_config_tx(char *config[], char *values[]);
int read_config_rx(char *config[], char *values[]);
//void applyRaisedCosineFilter(double*, int, double, int, double*);
void open_out(){
    // Open file for writing
    fptr = fopen("output_pulse.txt", "w");
    if (fptr == NULL) {
        fclose(fptr); // Close the file when done
    } 
    // Redirect stdout to the file
    freopen("output_pulse.txt", "w", stdout);
}
int main(){
    //printf("configurando \n");
    open_out();
    stop = false;
    signal(SIGINT, handle_sig);
    //printf("configurando cx\n");
    creando_contexto();
    //printf("configurando tx\n");
    configuracion_tx();
    //printf("configurando rx\n");
    configuracion_rx();
    double samplingRate = 16000000.0;
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
    double signal[N];
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
    /*------------*/
    //int nSamples     = 4096 ;
    samples_count = nSamples ;

    //printf("Transmitiendo ... buffer ciclico \n");
    nbytes_tx = iio_buffer_push(txbuf);// solo es valida para buffer de salida.
    if (nbytes_tx < 0) { 
        printf("Error pushing buf %d\n", (int) nbytes_tx); 
    //   shutdown(); 
    }
     // WRITE: Get pointers to TX buf and write IQ to TX buf port 0
	 p_inc = iio_buffer_step(txbuf);
	 p_end = iio_buffer_end(txbuf);
    //Funcion a recibir
    
    double signal_i[nSamples];
    double signal_q[nSamples];
	complex_rx *rxSignal = (complex_rx *) malloc(samples_count * sizeof(complex_rx));
	if (rxSignal == NULL) {
	    printf("Error allocating memory\n");
	    return 1;
	}

    /* Endiando datos*/
    int flag = 1;

    for (p_dat = (char *)iio_buffer_first(txbuf, tx0_i); p_dat < p_end; p_dat += p_inc) {
        
        ((int16_t*)p_dat)[0] = signal[increm];
        ((int16_t*)p_dat)[1] =(int16_t)0;// (int16_t)(1*pow(2, 14));//signal[increm];
         
         //((int16_t*)p_dat)[1] = (int16_t)0;
          increm = increm + 1;
          if(increm == 4095){
              increm = 0;
          }
     }
    
     


    //for (int i = 0; i < nSamples; i++) {
    //     //printf("shaped_signal[%d] = %f\n", i, signal[i]);
    //     signal_i[i] = 0;
    //     signal_q[i] = 0;
    //     printf("shaped_signal[%d] = %f\n", i,signal_i[i]);
    //     printf("shaped_signal[%d] = %f\n", i,signal_q[i]);
    // }

	//char *p_dat, *p_end;
	//ptrdiff_t p_inc;
    //int increm = 0,
    sleep(1);
    iio_buffer_refill(rxbuf);
    increm = 0;
    p_inc = iio_buffer_step(rxbuf);
	p_end = iio_buffer_end(rxbuf);
    
    for (p_dat = (char *)iio_buffer_first(rxbuf, rx0_i); p_dat < p_end; p_dat += p_inc) {        
        signal_q[increm] = (double) ((((int16_t*)p_dat)[0]) / pow(2, 14)); //i real
        signal_i[increm] = (double) ((((int16_t*)p_dat)[1])/ pow(2, 14)); //q ima
        increm = increm + 1;
        if(increm == (nSamples - 1) ){
            increm = 0;
            flag =  flag + 1;
        }    
	}
 
    // for (int i = 0; i < n_Sample; i++) {
    //     //printf("shaped_signal[%d] = %f\n", i, signal[i]);
    //     printf("pulse_i[%d] = %f\n", i,signal_i[i]);
    //     printf("pulse_q[%d] = %f\n", i,signal_q[i]);
    // }

    printf("[");
     for (int i = 0; i < nSamples; i++) {
        
         if(i < (nSamples-1)){
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
    
    //finalizamos cambiando la frecuencia
    int ret_input = iio_channel_attr_write(chnn_altvoltage0_output,  "frequency", "950000000"); //frequency 710000000 # RX LO frequency 710 Mhz
    if (ret_input < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_input;
    }
    
    

    //while (!stop);
    //sleep(10);                                                               //710000000
    int ret_output = iio_channel_attr_write(chnn_altvoltage1_output, "frequency", "710000000");//# Frecuencia de la portadora 710000000
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
    free(rxSignal);
    
    iio_buffer_destroy(txbuf);
    iio_buffer_destroy(rxbuf);

    //printf("== Finish Sdr_1 ==\n");
    iio_channel_disable(rx0_i);
    iio_channel_disable(rx0_q);
    iio_channel_disable(tx0_i);
    iio_channel_disable(tx0_q);
    iio_context_destroy(ctx);
    fclose(fptr);
    //printf("Fin\n");
    return 0;
}
int creando_contexto(){
    ctx = iio_create_default_context();
    if (!ctx) {
        printf("Unable to create IIO context\n");
        return -1;
    }
    return 0;
}

int configuracion_tx(){
    //printf("Transmision de un pulso\n");
    int num_linea = num_lines();
    char *config[num_linea];
    char *values[num_linea];

    read_config_tx(config,values);
    
    // Imprimir los arreglos separados
    // for (int i = 0; i < num_linea; i++) {
    //     printf("Attr : %s, Value: %s\n", config[i], values[i]);
    // }

    
    
    
    
    //printf("\t  TRANSMITTER\n");
    
    
    int ret_output;
    main_dev = iio_context_find_device(ctx, "ad9361-phy");
    
    /// configurando la salida
    
    chnn_device_output = iio_device_find_channel(main_dev, "voltage0", true);//output 
    
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
   
    chnn_altvoltage1_output = iio_device_find_channel(main_dev, "altvoltage1", true);//output 
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
    return 0;
}

int configuracion_rx(){
    //printf("configurando rx \n");
    int num_linea = num_lines();
    char *config[num_linea];
    char *values[num_linea];

    read_config_rx(config,values);
    
    // Imprimir los arreglos separados
    // for (int i = 0; i < num_linea; i++) {
    //     printf("Attr rx: %s, Value: %s\n", config[i], values[i]);
    // }
    
    int ret_input;
    main_dev = iio_context_find_device(ctx, "ad9361-phy");
    //configuracion de la entrada
    
    chnn_device_input = iio_device_find_channel(main_dev, "voltage0", false);//input 
    int ret = iio_channel_attr_write(chnn_device_input, config[0], values[0]); //sampling_frequency /*RX Baseband rate 5000000=5 MSPS  */
    if (ret < 0) {
       perror("Error setting sampling_frequency rate RX: "); 
        return ret;
    }
    //else printf("Set correct sampling_frequency RX \n");
    
    ret_input = iio_channel_attr_write(chnn_device_input,  config[1], values[1]);//rf_bandwidth 16000000
    if (ret_input < 0) {
       perror("Error setting rf_bandwidth rate RX: "); 
        return ret_input;
    }
    //else printf("Set correct rf_bandwidth RX \n");
    
    ret_input = iio_channel_attr_write(chnn_device_input,  config[2], values[2]);//gain_control_mode manual  Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_input < 0) {
       perror("Error setting gain_control_mode rate RX: "); 
        return ret_input;
    }
    //else printf("Set correct gain_control_mode RX  \n");
   
    ret_input = iio_channel_attr_write(chnn_device_input,  config[3], values[3]);//Ganancia hardwaregain max 73 # Gain applied to RX path. Only applicable when gain_control_mode is set to 'manual'
    if (ret_input < 0) {
       perror("Error setting hardwaregain rate RX: "); 
        return ret_input;
    }
    //else printf("Set correct hardwaregain RX  \n");
    

    chnn_altvoltage0_output = iio_device_find_channel(main_dev, "altvoltage0", true);//RX LO
    //printf("aca falla \n");}

    // ret_input = iio_channel_attr_write(chnn_altvoltage0_output,  config[4], values[4]); //frequency 710000000 # RX LO frequency 710 Mhz
    // if (ret_input < 0) { 
    //    perror("Error setting frecuency rate RX: "); 
    //     return ret_input;
    // }
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
    
    return 0;
}

int read_config_tx(char *config[], char *values[]) {
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

int read_config_rx(char *config[], char *values[]) {
    FILE *archivo;
    // Abre el archivo en modo de lectura ("r")
    archivo = fopen("receive_config.txt", "r");
    // Comprueba si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        exit(EXIT_FAILURE);

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

void applyRaisedCosineFilter(double* pulse_signal, int signal_length, double alpha, int oversampling_factor, double* shaped_signal) {
    int num_taps = FILTER_LENGTH;
    double Ts = 1.0 / oversampling_factor;
    double t;
    double h[FILTER_LENGTH];
    
    for (int n = -num_taps/2; n <= num_taps/2; n++) {
        t = n * Ts;
        if (fabs(t / Ts) == 1 / (2 * alpha)) {
            h[n + num_taps/2] = (alpha / (2 * M_PI * Ts)) * sin(M_PI * t / Ts) / (M_PI * t / Ts);
        } else {
            h[n + num_taps/2] = (sin(M_PI * t / Ts) / (M_PI * t / Ts)) * cos(2 * M_PI * alpha * t / Ts) / (1 - pow(2 * alpha * t / Ts, 2));
        }
    }
    
    for (int i = 0; i < signal_length; i++) {
        shaped_signal[i] = 0.0;
        for (int j = 0; j < num_taps; j++) {
            if (i - j >= 0) {
                shaped_signal[i] += pulse_signal[i - j] * h[j];
            }
        }
    }
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