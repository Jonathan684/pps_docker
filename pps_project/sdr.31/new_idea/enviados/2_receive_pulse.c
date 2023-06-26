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
int num_lines();
int read_config(char *config[], char *values[]);

int main(){
    
    int num_linea = num_lines();
    char *config[num_linea];
    char *values[num_linea];

    read_config(config,values);
    
    // Imprimir los arreglos separados
    // for (int i = 0; i < num_linea; i++) {
    //     printf("Attr rx: %s, Value: %s\n", config[i], values[i]);
    // }


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
    int ret_input;
    
    main_dev = iio_context_find_device(ctx, "ad9361-phy");
    //configuracion de la entrada
    
    //iio_channel_enable(chnn_device_input);
    struct iio_channel *chnn_device_input = iio_device_find_channel(main_dev, "voltage0", false);//input 
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
    //printf("config %s %s \n",config[2], values[2]);
    
    ret_input = iio_channel_attr_write(chnn_device_input,  config[2], values[2]);//gain_control_mode manual  Receive path AGC Options: slow_attack, fast_attack, manual
    if (ret_input < 0) {
       perror("Error setting gain_control_mode rate RX: "); 
        return ret_input;
    }
    //else printf("Set correct gain_control_mode RX  \n");

    if(!strcmp(values[2],"manual")){
        ret_input = iio_channel_attr_write(chnn_device_input,  config[3], values[3]);//Ganancia hardwaregain max 73 # Gain applied to RX path. Only applicable when gain_control_mode is set to 'manual'
        if (ret_input < 0) {
            perror("Error setting hardwaregain rate RX: "); 
            return ret_input;
        }
        //else printf("Set correct hardwaregain RX  \n");
    }

    struct iio_channel *chnn_altvoltage0_output = iio_device_find_channel(main_dev, "altvoltage0", true);//RX LO
    
    ret_input = iio_channel_attr_write(chnn_altvoltage0_output,  config[4], values[4]); //frequency 710000000 # RX LO frequency 710 Mhz
    if (ret_input < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_input;
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
    double signal_i[nSamples];
    double signal_q[nSamples];
    //for (int i = 0; i < nSamples; i++) {
    //     //printf("shaped_signal[%d] = %f\n", i, signal[i]);
    //     signal_i[i] = 0;
    //     signal_q[i] = 0;
    //     printf("shaped_signal[%d] = %f\n", i,signal_i[i]);
    //     printf("shaped_signal[%d] = %f\n", i,signal_q[i]);
    // }

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
    ret_input = iio_channel_attr_write(chnn_altvoltage0_output,  "frequency", "950000000"); //frequency 710000000 # RX LO frequency 710 Mhz
    if (ret_input < 0) { 
       perror("Error setting frecuency rate RX: "); 
        return ret_input;
    }
    
    iio_channel_disable(rx0_i);
    iio_channel_disable(rx0_q);
    iio_buffer_destroy(rxbuf);
    iio_context_destroy(ctx);
 
    fclose(fptr);
    //freopen("/dev/tty", "w", stdout);
    return 0;
}

int read_config(char *config[], char *values[]) {
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
    archivo = fopen("receive_config.txt", "r");
    // Comprueba si el archivo se abrió correctamente
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return -1;
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
