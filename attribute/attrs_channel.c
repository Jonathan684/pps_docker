#include <stdio.h>
#include <stdlib.h>
#include <iio.h>

int main()
{
    // create context
    struct iio_context* ctx = iio_create_default_context();
    if (!ctx) {
        perror("Could not create IIO context");
        return -1;
    }

    // Create a new IIO context
    //struct iio_context *ctx = iio_create_default_context();
    // Find the device
    struct iio_device *dev = iio_context_find_device(ctx, "ad9361-phy");
    //struct iio_device *dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc")
    // get device
    //struct iio_device* dev = iio_context_find_device(ctx, "ad9363-phy");
    if (!dev) {
        perror("Could not find device");
        iio_context_destroy(ctx);
        return -1;
    }

    // get channel
    // struct iio_channel* chn = iio_device_find_channel(dev, "voltage0", true);
    // if (!chn) {
    //     perror("Could not find channel");
    //     //iio_device_destroy(dev);
    //     iio_context_destroy(ctx);
    //     return -1;
    // }
    //const char *loopback_mode = "rf";
    // iio_device_attr_write(const struct iio_device *dev,
	// 	const char *attr, const char *src);

    //int ret = iio_device_attr_write(dev, "loopback", loopback_mode);
    // int ret = iio_device_attr_write(dev, "loopback", "internal");
    //  if (ret < 0) {
    //      perror("Failed to set loopback mode \n");
    //      iio_context_destroy(ctx);
    //      return -1;
    //  }
    // // Set the loopback mode to disabled
    //int lo_mode;
    //lo_mode = 0;
    //struct iio_channel* ch;
    //ch = iio_channel_create(dev, "voltage0", true);
    struct iio_channel *chn = iio_device_find_channel(dev, "voltage0", true); 
    //iio_channel_attr_write(ch, "loopback", "%d", lo_mode);
    if (iio_channel_attr_write(chn, "loopback", "1") < 0) {
        printf("Failed to set loopback mode\n");
        iio_context_destroy(ctx);
        return -1;
    }
    iio_channel_disable(chn);
    // get attributes count

    // __api __check_ret ssize_t iio_channel_attr_read(const struct iio_channel *chn,
	// 	const char *attr, char *dst, size_t len);

    // int attr_count = iio_channel_get_attrs_count(chn);
    // printf("Channel has %d attributes:\n", attr_count);

    // // print attribute names and values
    // for (int i = 0; i < attr_count; i++) {
    //     const char *attr = iio_channel_get_attr(chn, i);
    //     char val[256];
    //     iio_channel_attr_read(chn, attr, val, sizeof(val));
    //     printf(" %s  =   %s\n", attr,val);
    // }

    // cleanup
    //iio_channel_disable(chn);
    //iio_device_destroy(dev);
    iio_context_destroy(ctx);

    return 0;
}
