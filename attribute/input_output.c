#include <stdio.h>
#include <iio.h>
#include <string.h>
#include <stdbool.h>

int main() {
    
    FILE *fptr;

    // Open file for writing
    fptr = fopen("output.txt", "w");

    // Redirect stdout to the file
    freopen("output.txt", "w", stdout);

    // Write to stdout (will be redirected to file)
    printf("Hello, world!\n");

    


    struct iio_context *ctx;/* Create a default IIO context */
    struct iio_device  *dev;
    struct iio_channel *channel;
    //struct iio_channel *chn;/*Find the RF port select channel*/
    //int ret;
    int num_devices; 
    int num_channels;
    //bool input_output;
    //double rate, bandwidth, freq;
    const char *name;

    
    ctx = iio_create_default_context();
    if (!ctx) {
        perror("Could not create IIO context");
        return -1;
    }
    num_devices = iio_context_get_devices_count(ctx);
    
    printf("======================\n");
    printf("Found %d IIO devices :\n", num_devices);
    printf("======================\n");
    
    // Loop through all available devices and print their names
    for (int i = 0; i < num_devices; i++) 
    {
        dev = iio_context_get_device(ctx, i);
        if (!dev) {
            perror("Could not find PlutoSDR device");
            iio_context_destroy(ctx);
            return -1;
        }
        name = iio_device_get_name(dev);
        if (strstr(name, "input"))
        {
            printf("Device is an input device.\n");
        }
        
        
        
        /** @brief Enumerate the device-specific attributes of the given device
        * @param dev A pointer to an iio_device structure
        * @return The number of device-specific attributes found */
        
        int num_attrs = iio_device_get_attrs_count(dev);
        printf("Device----------------: %s\n",name);
        printf("%d device-specific attributes found\n", num_attrs);
        num_channels = iio_device_get_channels_count(dev);//ANALISIS DE CANAL
        printf("Number of channels for this device : %d\n", num_channels);
        printf("Device attributes\n\n");
        //ANALISIS DEL DISPOSITIVO
        for (int j = 0; j < num_attrs; j++) 
        {
                
                const char *attr = iio_device_get_attr(dev, j);
                char *value = calloc(1, 1024); // Allocate memory for attribute value
                iio_device_attr_read(dev, attr, value, 1024);
                printf("\tattr %2d: %-32s value: %s\n", j, attr, value);
                free(value); // Free allocated memory
        }

        for (int i = 0; i < num_channels; i++) 
        {
            channel = iio_device_get_channel(dev, i);
            const char *name_channel = iio_channel_get_name(channel);
            //input_output = iio_channel_is_output(channel);

            int attr_count = iio_channel_get_attrs_count(channel);
            printf("Channel name----------: %s\n", name_channel);
            printf("Channel has %d attributes:\n", attr_count);
            // print attribute names and values
            for (int i = 0; i < attr_count; i++) {
                const char *attr = iio_channel_get_attr(channel, i);
                char val[256];
                iio_channel_attr_read(channel, attr, val, sizeof(val));
                printf("  %s   = %s\n", attr,val);
            }

            printf("---------------------------\n");
            //iio_channel_attr_read_double(channel, "sampling_frequency", &rate);
            // iio_channel_attr_read_double(channel, "rf_bandwidth", &bandwidth);
            // iio_channel_attr_read_double(channel, "frequency", &freq);
            

            /*if (iio_channel_attr_read_double(channel, "value", &value_1) < 0) {
                printf("Error reading frequency value.\n");
                return -1;
            }*/

            /*  Parametro generales */
             
            // printf("Channel input/output--: %d\n",i);
            // printf("Sampling_frequency----: %f\n",rate);
            // printf("RF bandwidth..........: %fHz\n", bandwidth);
            // printf("Frequency.............: %fHz\n", freq);
           
            //if(input_output == true)printf("input output----------: output\n\n");
            //else printf("input output----------: input\n\n");
            
        }
        
        printf("=========================\n");
    }
    
    
    /* Cleanup */
    iio_channel_disable(channel);
    //iio_device_destroy(dev);
    iio_context_destroy(ctx);
    // Close file and restore stdout
    fclose(fptr);
    freopen("/dev/tty", "w", stdout);
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////            
            // Find the RF port select channel
            //chn = iio_device_find_channel(dev, name_channel, input_output);
           
             // Print the new value of the channel attribute
            // char buf[32];
            // ret = iio_channel_attr_read(channel, "sampling_frequency", buf, sizeof(buf));
            // if (ret < 0) {
            //     printf("Could not read %s attribute\n",name_channel);
            //     iio_context_destroy(ctx);
            //     return -1;
            // }
            // printf("rf_port_select is now set to: %s\n", buf);
/////////////////////////////////////////////////////////////////////////////////////////////
         







// int main() {
//     // Create a new IIO context
//     struct iio_context *ctx = iio_create_default_context();

//     // Find the AD9361 IIO device
//     struct iio_device *dev = iio_context_find_device(ctx, "ad9361-phy");

//     int num_attrs = iio_device_get_attrs_count(dev);
//      // Loop through all available attributes and print their names
//     for (int i = 0; i < num_attrs; i++) {
//         const char *name = iio_device_get_attr(dev, i);
//         printf("%s\n", name);
//     }
//     printf("Found %d attributes for device:\n", num_attrs);
//     // Read the value of the "loopback" attribute
//     // char buf[1024];
//     // if (iio_device_attr_read(dev, "loopback", buf, sizeof(buf)) < 0) {
//     //     printf("Error reading attribute.\n");
//     // } else {
//     //     printf("Loopback value: %s\n", buf);
//     // }

//     // Cleanup and exit
//     //iio_device_destroy(dev);
//     iio_context_destroy(ctx);
//     return 0;
// }

