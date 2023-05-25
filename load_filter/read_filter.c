#include <stdio.h>
#include <stdlib.h>
#include <iio.h>

int main() {
    // Create a context and open the device
    struct iio_context *ctx;
    struct iio_device *dev;

    ctx = iio_create_default_context();
    if (!ctx) {
        printf("Failed to create context\n");
        return -1;
    }

    dev = iio_context_find_device(ctx, "ad9361-phy");
    if (!dev) {
        printf("Failed to find device\n");
        iio_context_destroy(ctx);
        return -1;
    }

    // Read the filter configuration attribute
    char buffer[4096];
    ssize_t read_bytes = iio_device_attr_read(dev, "filter_fir_config", buffer, sizeof(buffer) - 1);
    if (read_bytes < 0) {
        printf("Failed to read filter configuration attribute\n");
        iio_context_destroy(ctx);
        return -1;
    }

    // Null-terminate the buffer
    buffer[read_bytes] = '\0';

    // Print the filter configuration
    printf("Loaded filter configuration:\n%s\n", buffer);

    // Cleanup and close the device
    iio_context_destroy(ctx);

    return 0;
}
