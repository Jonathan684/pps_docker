#include <stdio.h>
#include <iio.h>
#include <string.h>


#ifndef IIO_DIR_IN
#define IIO_DIR_IN 0
#endif
#ifndef IIO_DIR_OUT
#define IIO_DIR_OUT 1
#endif

int main() {
    // Create a new IIO context
    struct iio_context *ctx = iio_create_default_context();
    // Find the device
    //struct iio_device *dev = iio_context_find_device(ctx, "ad9361-phy");
    struct iio_device *dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");   
    // Find the channel
    struct iio_channel *chn = iio_device_find_channel(dev, "voltage0", true);  
    

    // Get the number of available devices
    int num_devices = iio_context_get_devices_count(ctx);
    printf("Found %d IIO devices:\n", num_devices);

    // Loop through all available devices and print their names
    for (int i = 0; i < num_devices; i++) {
        struct iio_device *dev = iio_context_get_device(ctx, i);
        const char *name = iio_device_get_name(dev);
        
        printf("======= Prueba ===== %d\n",i);
        if (strstr(name, "input")) {
             printf("Device is an input device.\n");
             } 
        printf("%s\n", name);
    }
    // Check if the channel was found successfully
    if (!chn) {
        printf("Error finding channel.\n");
        return -1;
    }  
    // Read the sampling frequency attribute
    double value;
    if (iio_channel_attr_read_double(chn, "sampling_frequency", &value) < 0) {
        printf("Error reading attribute.\n");
        return -1;
    }  
    printf("The sampling frequency is %f Hz.\n", value);
    // Cleanup and exit
    
    const char *dev_id;

    /* Assume 'dev' is a valid pointer to an IIO device */

    /* Get the device ID */
    dev_id = iio_device_get_id(dev);

    /* Print the device ID */
    printf("Device ID: %s\n", dev_id);


/////////////////////////////////////////////////////////////////////////////////////
    iio_context_destroy(ctx);
    return 0;
}
