#include <stdio.h>
#include <iio.h>

#define RX_BUF_SIZE 1024

int main(void)
{
    struct iio_context *ctx;
    struct iio_device *dev;
    struct iio_channel *rx0_i, *rx0_q;
    struct iio_buffer *rxbuf;
    char *rxbuf_data;
	
    int  n;
    ssize_t nbytes_rx;

    // Create context and connect to device
    ctx = iio_create_local_context();
    if (!ctx) {
        perror("iio_create_local_context");
        return 1;
    }
    dev = iio_context_find_device(ctx, "ad9361-phy");
    if (!dev) {
        perror("iio_context_find_device");
        return 1;
    }

    // Configure RX channels
    rx0_i = iio_device_find_channel(dev, "voltage0", 0);
    rx0_q = iio_device_find_channel(dev, "voltage0", 1);
    if (!rx0_i || !rx0_q) {
        perror("iio_device_find_channel");
        return 1;
    }
    iio_channel_enable(rx0_i);
    iio_channel_enable(rx0_q);
    //iio_device_create_buffer: Invalid argument
    // Configure RX buffer and start streaming 
    rxbuf = iio_device_create_buffer(dev, RX_BUF_SIZE, false);
   
    if (!rxbuf) {
        perror("iio_device_create_buffer");
        return 1;
    }
    iio_buffer_start(rxbuf);
    // if (ret < 0) {
    //     perror("iio_buffer_start");
    //     return 1;
    // }

    // Read data from RX buffer
    rxbuf_data = iio_buffer_start(rxbuf);
    while (1) {
        nbytes_rx = iio_buffer_refill(rxbuf);
        if (nbytes_rx < 0) {
            perror("iio_buffer_refill");
            break;
        }
        n = nbytes_rx / (sizeof(int16_t) * 2); // each sample is 16 bits (I + Q)
        for (int i = 0; i < n; i++) {
            const int16_t i_val = ((int16_t *)rxbuf_data)[i * 2];
            const int16_t q_val = ((int16_t *)rxbuf_data)[i * 2 + 1];
            printf("%d, %d\n", i_val, q_val);
        }
    }

    // Clean up
    iio_buffer_cancel(rxbuf);
    iio_buffer_destroy(rxbuf);
    iio_context_destroy(ctx);

    return 0;
}
