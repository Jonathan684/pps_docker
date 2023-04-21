

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <iio.h>
#include "shutdown.h"
#include "definition.h"


struct iio_context *ctx  ;
struct iio_channel *rx0_i;
struct iio_channel *rx0_q;
struct iio_channel *tx0_i;
struct iio_channel *tx0_q;
struct iio_buffer  *rxbuf;
struct iio_buffer  *txbuf;

void load_structure(struct iio_context *ctx,struct iio_channel *rx0_i,struct iio_channel *rx0_q,struct iio_channel *tx0_i,struct iio_channel *tx0_q,struct iio_buffer  *rxbuf,struct iio_buffer  *txbuf)
{
	ctx  = ctx;
	rx0_i = rx0_i;
	rx0_q = rx0_q;
	tx0_i = tx0_i;
	tx0_q = tx0_q;
	rxbuf = rxbuf;
	txbuf = txbuf;
}


void handle_sig(int sig)
{
	printf("Waiting for process to finish... Got signal %d\n", sig);
	stop = true;
}
/* check return value of attr_write function */
void errchk(int v, const char* what) {
	if (v < 0) { fprintf(stderr, "Error %d writing to channel \"%s\"\nvalue may not be supported.\n", v, what); 
	 	shutdown(ctx,rx0_i ,rx0_q,tx0_i,tx0_q,rxbuf,txbuf );
 	}
}

/* write attribute: long long int */
void wr_ch_lli(struct iio_channel *chn, const char* what, long long val)
{
	errchk(iio_channel_attr_write_longlong(chn, what, val), what);
}
/* write attribute: string */
void wr_ch_str(struct iio_channel *chn, const char* what, const char* str)
{
	errchk(iio_channel_attr_write(chn, what, str), what);
}
/* helper function generating channel names */
char* get_ch_name(const char* type, int id)
{
	snprintf(tmpstr, sizeof(tmpstr), "%s%d", type, id);
	return tmpstr;
}
/* returns ad9361 phy device */
struct iio_device* get_ad9361_phy()
{
	struct iio_device *dev =  iio_context_find_device(ctx, "ad9361-phy");
	IIO_ENSURE(dev && "No ad9361-phy found");
	return dev;
}
/*
iio:device0: ad9361-phy -> 
iio:device1: xadc
iio:device2: cf-ad9361-dds-core-lpc (buffer capable) -> TX
iio:device3: cf-ad9361-lpc (buffer capable) -> RX
 finds AD9361 streaming IIO devices */
bool get_ad9361_stream_dev(enum iodev d, struct iio_device **dev)
{
	switch (d) {
	case TX: *dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc"); return *dev != NULL;
	case RX: *dev = iio_context_find_device(ctx, "cf-ad9361-lpc");  return *dev != NULL;
	default: IIO_ENSURE(0); return false;
	}
}
/* finds AD9361 streaming IIO channels */
bool get_ad9361_stream_ch(enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn)
{
	*chn = iio_device_find_channel(dev, get_ch_name("voltage", chid), d == TX);
	if (!*chn)
		*chn = iio_device_find_channel(dev, get_ch_name("altvoltage", chid), d == TX);
	return *chn != NULL;
}
/* finds AD9361 phy IIO configuration channel with id chid */
bool get_phy_chan(enum iodev d, int chid, struct iio_channel **chn,struct iio_context *ctx )
{
	switch (d) {
	case RX: *chn = iio_device_find_channel(get_ad9361_phy(ctx), get_ch_name("voltage", chid), false); return *chn != NULL;
	case TX: *chn = iio_device_find_channel(get_ad9361_phy(ctx), get_ch_name("voltage", chid), true);  return *chn != NULL;
	default: IIO_ENSURE(0); return false;
	}
}
/* finds AD9361 local oscillator IIO configuration channels */
bool get_lo_chan(enum iodev d, struct iio_channel **chn,struct iio_context *ctx)
{
	switch (d) {
	 // LO chan is always output, i.e. true
	case RX: *chn = iio_device_find_channel(get_ad9361_phy(ctx), get_ch_name("altvoltage", 0), true); return *chn != NULL;
	case TX: *chn = iio_device_find_channel(get_ad9361_phy(ctx), get_ch_name("altvoltage", 1), true); return *chn != NULL;
	default: IIO_ENSURE(0); return false;
	}
}
/* applies streaming configuration through IIO */
bool cfg_ad9361_streaming_ch(struct stream_cfg *cfg, enum iodev type, int chid)
{
	struct iio_channel *chn = NULL;

	// Configure phy and lo channels
	printf("* Acquiring AD9361 phy channel %d\n", chid);
	if (!get_phy_chan(type, chid, &chn,ctx)) {	return false; }
	wr_ch_str(chn, "rf_port_select",     cfg->rfport);
	wr_ch_lli(chn, "rf_bandwidth",       cfg->bw_hz);
	wr_ch_lli(chn, "sampling_frequency", cfg->fs_hz);

	// Configure LO channel
	printf("* Acquiring AD9361 %s lo channel\n", type == TX ? "TX" : "RX");
	if (!get_lo_chan(type, &chn,ctx)) { return false; }
	wr_ch_lli(chn, "frequency", cfg->lo_hz);
	return true;
}






















//static bool stop;

// cleanup and exit 
void shutdown()
{
	printf("* Destroying buffers\n");
	if (rxbuf) { iio_buffer_destroy(rxbuf); }
	if (txbuf) { iio_buffer_destroy(txbuf); }

	printf("* Disabling streaming channels\n");
	if (rx0_i) { iio_channel_disable(rx0_i); }
	if (rx0_q) { iio_channel_disable(rx0_q); }
	if (tx0_i) { iio_channel_disable(tx0_i); }
	if (tx0_q) { iio_channel_disable(tx0_q); }

	printf("* Destroying context\n");
	if (ctx) { iio_context_destroy(ctx); }
	exit(0);
}


// /* check return value of attr_write function */
// void errchk(int v, const char* what) {
// 	 if (v < 0) { fprintf(stderr, "Error %d writing to channel \"%s\"\nvalue may not be supported.\n", v, what); shutdown(); }
// }
