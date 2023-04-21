#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <iio.h>

int main()
{
            // Find the IIO context
        struct iio_context *ctx = iio_create_default_context();

        // Find the device
        //struct iio_device *dev = iio_context_find_device(ctx, "ad9361-phy");
        struct iio_device *dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");

        // Find the channel
        struct iio_channel *chn = iio_device_find_channel(dev, "voltage0", true);

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

        // Cleanup
        //iio_channel_free(chn);
        //iio_device_destroy(dev);
        iio_context_destroy(ctx);
     
    // const char *device_name = "cf-ad9361-dds-core-lpc";
    // // Open the IIO context and get the Pluto device
    // // Find the IIO context
    // struct iio_context *ctx = iio_create_default_context();

    // // Find the device
    // struct iio_device *dev = iio_context_find_device(ctx, device_name);

    // // Find the channel
    // struct iio_channel *chn = iio_device_find_channel(dev, "voltage0", true); /*Para cf-ad9361-dds-core-lpc true*/
    // value = iio_channel_attr_read_double(chn, device_name, value_ptr);
    // // Check if the channel was found successfully
    // if (!chn) {
    //     printf("Error finding channel.\n");
    //     return -1;
    // }

    // double value;
    // const char *attr_name = "sampling_frequency";
    // //const char *device_name2 = "cf-ad9361-dds-core-lpc";
    // // Assume chn has been initialized
    // value = iio_channel_attr_read_double(chn, attr_name, &value);
    // printf("2. -The value of %s on device %s is %f\n", attr_name, device_name, value);

///////////////////////////////////////////////////////////////////////77

    // const char *device_name = "iio:device2";
    // // Open the IIO context and get the Pluto device
    // // Find the IIO context
    // struct iio_context *ctx = iio_create_default_context();

    // // Find the device
    // struct iio_device *dev = iio_context_find_device(ctx, "xadc");
    
    // //struct iio_device *dev =  iio_context_find_device(ctx, );

    // // Find the channel
    // struct iio_channel *chn = iio_device_find_channel(dev, "voltage0", NULL);

    // // Check if the channel was found successfully
    // if (!chn) {
    //     printf("Error finding channel.\n");
    //     return -1;
    // }

    
    // //const char *attr_name = "sampling_frequency";
    // double value;
    // double *value_ptr = &value;
    // const char *attr_name = "sampling_frequency";
    // // Assume chn has been initialized
    // //int iio_channel_attr_read_double(const struct iio_channel *chn,const char *attr, double *val)
    // value = iio_channel_attr_read_double(chn, device_name, value_ptr);
    // printf("The value of %s on device %s is %f\n", attr_name, device_name, value);
    
    return 0;
}
/* struct iio_context *ctx = iio_create_default_context();
    struct iio_device *dev = iio_context_find_device(ctx, "ad9361-phy");
    double freq;

    int ret = iio_channel_attr_read_double(chn, attr, &freq);
    
    // Get the current configuration of the TX channel
    struct iio_channel *tx_chn = iio_device_find_channel(dev, "voltage0", true);
    printf("Current configuration of the TX channel:\n");
    printf("RF port enabled: %f\n", iio_channel_attr_read_double(tx_chn, "rf_port_en"));
    printf("Hardware gain control enabled: %f\n", iio_channel_attr_read_double(tx_chn, "hardwaregain"));
    printf("RF bandwidth: %f\n", iio_channel_attr_read_double(tx_chn, "rf_bandwidth"));
    printf("Sampling frequency: %f\n", iio_channel_attr_read_double(tx_chn, "sampling_frequency"));
    printf("Carrier frequency: %f\n", iio_channel_attr_read_double(tx_chn, "frequency"));

    // Get the current configuration of the RX channel
    struct iio_channel *rx_chn = iio_device_find_channel(dev, "voltage0", false);
    printf("\nCurrent configuration of the RX channel:\n");
    printf("RF port enabled: %f\n", iio_channel_attr_read_double(rx_chn, "rf_port_en"));
    printf("Hardware gain control enabled: %f\n", iio_channel_attr_read_double(rx_chn, "hardwaregain"));
    printf("RF bandwidth: %f\n", iio_channel_attr_read_double(rx_chn, "rf_bandwidth"));
    printf("Sampling frequency: %f\n", iio_channel_attr_read_double(rx_chn, "sampling_frequency"));
    printf("Carrier frequency: %f\n", iio_channel_attr_read_double(rx_chn, "frequency"));

    // Cleanup and exit
    iio_channel_release(tx_chn);
    iio_channel_release(rx_chn);
    */