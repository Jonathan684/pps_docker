#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iio.h>



#define NOMBRE_ARCHIVO "filter_jupyter.txt"
#define LONGITUD_MAXIMA_CADENA 1000

int main(void)
{
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "r"); // Modo lectura
    //char buffer[LONGITUD_MAXIMA_CADENA];         // Aquí vamos a ir almacenando cada línea
    char buffer[4096];
    ssize_t read_bytes;
    struct iio_context *ctx;
    struct iio_device *main_dev;

     ctx = iio_create_default_context();
     if (!ctx) {
         printf("Failed to create context\n");
         return -1;
     }

     main_dev = iio_context_find_device(ctx, "ad9361-phy");
     if (!main_dev) {
         printf("Failed to find device\n");
         iio_context_destroy(ctx);
         return -1;
     }
    while ((read_bytes = fread(buffer, sizeof(char), sizeof(buffer), archivo)) > 0) 
    {
        if (iio_device_attr_write_raw(main_dev, "filter_fir_config", buffer, read_bytes) < 0) {
            printf("Failed to write filter configuration to device\n");
            fclose(archivo);
            iio_context_destroy(ctx);
            return -1;
        }
        //printf(" %s\n",buffer);
    }//983040000 245760000 245760000 122880000 61440000 30720000
    int ret_rx;
    struct iio_channel *chnn_out_input = iio_device_find_channel(main_dev, "out", false);//input 
    ret_rx = iio_channel_attr_write(chnn_out_input, "voltage_filter_fir_en", "1");//3999999
    if (ret_rx < 0) {
       perror("Error setting voltage_filter_fir_en out: "); 
        return ret_rx;
    }

    struct iio_channel *chnn_device_input = iio_device_find_channel(main_dev, "voltage0", false);//input 
    
    ret_rx = iio_channel_attr_write(chnn_device_input, "rf_bandwidth", "16000000");//16000000
    if (ret_rx < 0) {
       perror("Error setting rf_bandwidth rate RX: "); 
        return ret_rx;
    }
    ret_rx = iio_channel_attr_write(chnn_device_input, "sampling_frequency", "700000");//3999999
    if (ret_rx < 0) {
       perror("Error setting sampling_frequency rate RX: "); 
        return ret_rx;
    }
    //else printf("Set correct sampling_frequency RX \n");
    

   

    fclose(archivo);
    return 0;
}
