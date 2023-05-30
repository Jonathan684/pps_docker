#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iio.h>

int main(int argc, char **argv)
{
    // Open IIO context and find device
    struct iio_context *ctx = iio_create_default_context();
    if (!ctx) {
        printf("Error creating IIO context\n");
        return -1;
    }

    struct iio_device *dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc");

    // Set the frequency to 2.4 GHz
    int32_t frequency_hz = 2400000000;
    struct iio_channel *chn = iio_device_find_channel(dev, "altvoltage0", true);
    iio_channel_attr_write_longlong(chn, "frequency", frequency_hz);

    // Set the amplitude to 0 dBFS
    float amplitude_dbfs = 0;
    chn = iio_device_find_channel(dev, "altvoltage0", false);
    iio_channel_attr_write_double(chn, "scale", pow(10, amplitude_dbfs / 20));

    // Set the waveform to a pulse
    int samples_count = 1024;
    int16_t waveform[samples_count];
    memset(waveform, 0, sizeof(waveform));
    waveform[samples_count / 2] = INT16_MAX;

    // Configure TX buffer
    struct iio_buffer *txbuf = iio_device_create_buffer(dev, samples_count, true);
    if (!txbuf) {
        printf("Error creating TX buffer\n");
        iio_context_destroy(ctx);
        return -1;
    }
    void *p_dat, *p_end;
    ssize_t p_inc;
    p_inc = iio_buffer_step(txbuf);
    p_end = iio_buffer_end(txbuf);
    for (p_dat = iio_buffer_first(txbuf, dev, IIO_CHANNEL_TX, true); p_dat < p_end; p_dat += p_inc) {
        memcpy(p_dat, waveform, samples_count * sizeof(int16_t));
    }

    // Enable TX and TX_LO frequency and start streaming
    iio_channel_enable(chn);
    chn = iio_device_find_channel(dev, "altvoltage1", true);
    iio_channel_enable(chn);
    iio_device_attr_write_bool(dev, "tx_hardwaregain_enable", true);
    iio_device_attr_write_bool(dev, "out_altvoltage0_TX_LO_enable", true);
    iio_device_attr_write_bool(dev, "out_altvoltage1_TX_LO_enable", true);
    iio_buffer_push(txbuf);
    iio_device_attr_write_bool(dev, "tx_en", true);

    // Wait for pulse to finish and stop streaming
    usleep(100000);
    iio_device_attr_write_bool(dev, "tx_en", false);
    iio_device_attr_write_bool(dev, "out_altvoltage0_TX_LO_enable", false);
    iio_device_attr_write_bool(dev, "out_altvoltage1_TX_LO_enable", false);
    iio_buffer_destroy(txbuf);
    iio_context_destroy(ctx);

    return 0;
}
