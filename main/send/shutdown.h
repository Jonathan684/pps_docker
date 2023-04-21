#ifndef   Shutdown_H_
#define   Shutdown_H_
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <iio.h>

#include "definition.h"

void load_structure(struct iio_context *,struct iio_channel * ,struct iio_channel *,struct iio_channel *,struct iio_channel *,struct iio_buffer  *,struct iio_buffer  *);



void handle_sig(int);
void errchk(int , const char* );
bool cfg_ad9361_streaming_ch(struct stream_cfg *, enum iodev , int );
void wr_ch_lli(struct iio_channel *, const char* , long long );
void wr_ch_str(struct iio_channel *, const char* , const char* );
char* get_ch_name(const char* , int );
struct iio_device* get_ad9361_phy();
bool get_ad9361_stream_dev(enum iodev , struct iio_device **);
bool get_ad9361_stream_ch(enum iodev , struct iio_device *, int , struct iio_channel **);
bool get_phy_chan(enum iodev , int , struct iio_channel **chn,struct iio_context *);
bool get_lo_chan(enum iodev , struct iio_channel **,struct iio_context *);
void shutdown();

#endif