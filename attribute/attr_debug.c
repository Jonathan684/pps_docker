#include <stdio.h>
#include <iio.h>
#include <string.h>
#include <stdbool.h>

int main(){
struct iio_context *ctx = NULL;
ctx = iio_create_default_context();
if (!ctx) {
    printf("Unable to create IIO context\n");
    return -1;
}

struct iio_device *dev = NULL;
dev = iio_context_find_device(ctx, "ad9361-phy");
if (!dev) {
    printf("Unable to find Pluto device\n");
    iio_context_destroy(ctx);
    return -1;
}
char buffer[50];
char buffer2[50];
//char src[50];

//LOOPBACK
int i = iio_device_get_debug_attrs_count(dev);
const char * attr = iio_device_find_debug_attr(dev, "loopback");
iio_device_debug_attr_read(dev,"loopback",buffer,sizeof(buffer));


//__api __check_ret ssize_t iio_device_debug_attr_write(const struct iio_device *dev,
//		const char *attr, const char *src);

iio_device_debug_attr_write(dev,"loopback","1");//0-1-2
//
printf("attribute count ---------->%d\n",i);
printf("attr name %s\n",attr);
printf("value %s\n",buffer);
iio_device_debug_attr_read(dev,"loopback",buffer,sizeof(buffer));
printf("value %s\n",buffer);
iio_device_debug_attr_read(dev,"sampling_frequency",buffer2,sizeof(buffer2));
printf("sampling_frequency %s\n",buffer2);
// int bytes_read = iio_device_attr_read(dev, "loopback", buffer, sizeof(buffer));
// if (bytes_read < 0) {
//     printf("Unable to read debug attribute\n");
//     iio_context_destroy(ctx);
//     return -1;
//}

//printf("Debug attribute value: %s\n", buffer);
iio_context_destroy(ctx);
}