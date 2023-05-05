#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <iio.h>

#define RX_BUFFER_SIZE 4096
#define TX_BUFFER_SIZE 4096

int main(int argc, char **argv) {
    struct iio_context *ctx;
    struct iio_device *dev;
    struct iio_channel *rx;
    struct iio_channel *tx;
    struct iio_buffer *rxbuf;
    struct iio_buffer *txbuf;
    int16_t tx_buffer[TX_BUFFER_SIZE];
    int16_t rx_buffer[RX_BUFFER_SIZE];
    ssize_t nbytes_rx, nbytes_tx;

    /* Connect to PlutoSDR */
    ctx = iio_create_network_context("192.168.2.1");
    dev = iio_context_find_device(ctx, "cf-ad9361-lpc"); // Device para recibir
    rx = iio_device_find_channel(dev, "voltage0", true);
    tx = iio_device_find_channel(dev, "voltage0", false);

    /* Configure RX and TX buffers */
    rxbuf = iio_device_create_buffer(dev, RX_BUFFER_SIZE, false);
    txbuf = iio_device_create_buffer(dev, TX_BUFFER_SIZE, false);

    /* Send data */
    for (int i = 0; i < TX_BUFFER_SIZE; i++) {
        tx_buffer[i] = i;
    }
    iio_buffer_set_data(txbuf, tx_buffer);
    nbytes_tx = iio_buffer_push(txbuf);

    /* Receive data */
    nbytes_rx = iio_buffer_refill(rxbuf);

    /* Print received data */
    printf("Received %zd bytes:\n", nbytes_rx);
    for (int i = 0; i < nbytes_rx; i += 2) {
        printf("%d\n", rx_buffer[i]);
    }

    /* Clean up */
    iio_buffer_destroy(rxbuf);
    iio_buffer_destroy(txbuf);
    iio_context_destroy(ctx);

    return 0;
}