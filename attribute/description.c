//Description of the attributes:

// filter_fir_en: Indicates whether the FIR filter is enabled or not.
// hardwaregain: Indicates the gain value of the hardware in dB.
// hardwaregain_available: Indicates the range of the gain value that the hardware can take in dB.
// rf_bandwidth: Indicates the bandwidth of the RF signal in Hz.
// rf_bandwidth_available: Indicates the range of RF signal bandwidth in Hz.
// rf_port_select: Indicates the selected RF port for the signal input/output.
// rf_port_select_available: Indicates the available RF ports for signal input/output.
// rssi: Indicates the received signal strength indication value in dB.
// sampling_frequency: Indicates the sampling frequency of the RF signal in Hz.
// sampling_frequency_available: Indicates the range of sampling frequency of the RF signal in Hz.

#include <stdio.h>
#include <iio.h>
#include <string.h>
#include <stdbool.h>

int print_attr(struct iio_channel *chn, const char *attr, const char *val, size_t len, void *data) {
    printf("%s = %s\n", attr, val);
    return 0;
}

int main() {
    // Create a new IIO context
    struct iio_context *ctx = iio_create_default_context();

    // Get the first available IIO device                                //ad9361-phy
    struct iio_device *dev = iio_context_find_device(ctx, "ad9361-phy"); //cf-ad9361-dds-core-lpc

    // Get the first available IIO channel of the device
    struct iio_channel *chn = iio_device_find_channel(dev, "voltage0", true);

    // Read all attributes of the channel and print them
    int ret = iio_channel_attr_read_all(chn, print_attr, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error reading attributes.\n");
        return 1;
    }

    // Cleanup and exit
    iio_context_destroy(ctx);
    return 0;
}
