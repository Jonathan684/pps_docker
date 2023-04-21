#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
//#include <errno>
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

//char src[50];

// PASOS 
// 1 .- LOOPBACK SDR 192.168.1.36
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
printf("value :%s\n",buffer);

// Get the "in_voltage" attribute of the device
//struct iio_attribute *attr = iio_device_find_attr(dev, "sampling_frequency");
struct iio_channel *chn_rx = iio_device_find_channel(dev, "voltage0", false);//input() * @param output True if the searched channel is output, False otherwise
struct iio_channel *chn_tx = iio_device_find_channel(dev, "voltage0", true);    //output() mismo nombre para el canal
// Read the attribute value as a double
double value;
int ret = iio_channel_attr_read_double(chn_rx, "sampling_frequency", &value);

if (ret < 0) {
   perror("Error reading attribute:");
   return 1;
}

// Print the value
printf("Attribute value: %f\n", value);

// 2.- SET Frecuencia de muestreo
//ssize_t iio_device_attr_write(const struct iio_device *dev,const char *attr, const char *src);
//iio_device_attr_write(dev,"sampling_frequency","800000");solo para debugg
iio_channel_attr_write(chn_rx,"sampling_frequency","800000");

iio_channel_attr_read_double(chn_rx, "sampling_frequency", &value);
printf("Attribute value 2: %f\n", value);


// 3.- Ancho de banda en RF


double value_RxRfBw;
double value_TxRfBw;

const char *RxRfBw = "16000000"; // Bandwidth of front-end analog filter of TX path [Hz]
const char *TxRfBw = RxRfBw;

//printf("--> %s \n",iio_channel_find_attr(chn_rx,"rf_bandwidth"));

iio_channel_attr_write(chn_rx,"rf_bandwidth",RxRfBw);
iio_channel_attr_write(chn_tx,"rf_bandwidth",TxRfBw);

iio_channel_attr_read_double(chn_rx, "rf_bandwidth", &value_RxRfBw);
iio_channel_attr_read_double(chn_tx, "rf_bandwidth", &value_TxRfBw);

printf("RxRfBw :%f and TxRfBw :%f\n",value_RxRfBw,value_TxRfBw);
//4. Ganancia del Hardware
// Get the TX hardware gain on channel 0 and print it to the console
//RX
double rx_hardwaregain_get;
double rx_hardwaregain = 70;

iio_channel_attr_write_double(chn_rx, "hardwaregain", rx_hardwaregain);
iio_channel_attr_read_double(chn_rx, "hardwaregain", &rx_hardwaregain_get);

double tx_hardwaregain_get;
double tx_hardwaregain = -40.0;


iio_channel_attr_write_double(chn_tx, "hardwaregain", tx_hardwaregain);
iio_channel_attr_read_double(chn_tx, "hardwaregain", &tx_hardwaregain_get);

printf("TX hardware gain on channel 0: %f dB\n", tx_hardwaregain_get);
printf("RX hardware gain on channel 0: %f dB\n", rx_hardwaregain_get);

//const char *rx_GainControlModes = "slow_attack";
char rx_GainControlModes_get[100];

//iio_channel_attr_write(const struct iio_channel *chn,const char *attr, const char *src);
iio_channel_attr_write(chn_rx, "gain_control_mode", "slow_attack");
//ssize_t iio_channel_attr_read(const struct iio_channel *chn,const char *attr, char *dst, size_t len);
iio_channel_attr_read(chn_rx, "gain_control_mode", rx_GainControlModes_get,sizeof(rx_GainControlModes_get));


printf("TX gain_control_mode on channel 0: %s dB\n", rx_GainControlModes_get);
//printf("RX gain_control_mode on channel 0: %f dB\n", rx_gain_control_mode_get);

iio_channel_disable(chn_rx);
iio_channel_disable(chn_tx);
iio_context_destroy(ctx);
return 0;
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iio.h>

int main()
{
    // Connect to the SDR device
    struct iio_context *ctx = iio_create_network_context("ip:192.168.2.1");
    struct iio_device *dev = iio_context_find_device(ctx, "MySDR");

    // Set the TX hardware gain on channel 0 to -40 dB
    double tx_hardwaregain = -40.0;
    struct iio_channel *tx_hardwaregain_chan0 = iio_device_find_channel(dev, "voltage0", true);
    iio_channel_attr_write_double(tx_hardwaregain_chan0, "hardwaregain", tx_hardwaregain);

    // Get the TX hardware gain on channel 0 and print it to the console
    double tx_hardwaregain_get;
    iio_channel_attr_read_double(tx_hardwaregain_chan0, "hardwaregain", &tx_hardwaregain_get);
    printf("TX hardware gain on channel 0: %f dB\n", tx_hardwaregain_get);

    // Disconnect from the SDR device
    iio_channel_release(tx_hardwaregain_chan0);
    iio_device_destroy(dev);
    iio_context_destroy(ctx);

    return 0;
}


*/