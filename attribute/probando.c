#include <stdio.h>
#include <iio.h>

int main()
{
    // Initialize the IIO context and device
    struct iio_context *ctx = iio_create_default_context();
    struct iio_device *dev = iio_context_find_device(ctx, "out");

    // Get the debug attribute for loopback value
    struct iio_channel *chn = iio_device_find_channel(dev, "voltage0", true);
    //channel = iio_device_get_channel(dev, i);

    //__api __check_ret __pure const char * iio_channel_find_attr(
	//	const struct iio_channel *chn, const char *name);
    
    //const char *attr = iio_channel_find_attr(chn, "debug-attr9");

    // Read the current value of the debug attribute
    char value[256];
    //  iio_device_attr_read_bool(const struct iio_device *dev,
	//	const char *attr, bool *val);

    int ret = iio_channel_attr_read(chn, "debug-attr9", value,sizeof(value));
    
    if (ret < 0) {
        perror("Error reading debug attribute: \n");
        return ret;
    }

    // Modify the value of the debug attribute to 1
    
    //value[0] = '1';

    // Write the modified value back to the debug attribute
    //ret = iio_attribute_write(attr, value, sizeof(value));
    if (ret < 0) {
        printf("Error writing debug attribute: \n");
        return ret;
    }

    // Clean up resources
    iio_context_destroy(ctx);

    return 0;
}
