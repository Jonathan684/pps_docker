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
    int num_devices; 
    int num_channels;
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
        }
        printf("=========================\n");
    }
    /* Cleanup */
    iio_channel_disable(channel);
    iio_context_destroy(ctx);
    // Close file and restore stdout
    fclose(fptr);
    freopen("/dev/tty", "w", stdout);
    return 0;
}