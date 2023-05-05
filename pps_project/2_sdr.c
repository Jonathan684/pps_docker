#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
//#include <errno>

#define IIO_CHANNEL_TX 0x1
#define SAMPLE_COUNT 1024 // Para el buffer
bool stop;


void handle_sig(int sig)
{
	printf("Waiting for process to finish... Got signal %d\n", sig);
	stop = true;
}


int main(){
    signal(SIGINT, handle_sig);
    struct iio_context *ctx = NULL;
    ctx = iio_create_default_context();
    if (!ctx) {
        printf("Unable to create IIO context\n");
        return -1;
    }

    struct iio_device *dev;
	struct iio_channel *rx0_i, *rx0_q;
	struct iio_buffer *rxbuf;
    
    struct iio_device *dev2;
    
    /* Set sample rate */


    dev2 = iio_context_find_device(ctx, "ad9361-phy"); //dispositivo para enviar
    struct iio_channel *chnn_device0 = iio_device_find_channel(dev2, "voltage0", false);//input 
    int ret = iio_channel_attr_write(chnn_device0, "sampling_frequency", "4999999");//3999999
    if (ret < 0) {
       perror("Error setting TX I sample rate: ");
        return ret;
    }

	dev = iio_context_find_device(ctx, "cf-ad9361-lpc"); // Dispositivo de recepcion
 
	rx0_i = iio_device_find_channel(dev, "voltage0", 0);
	rx0_q = iio_device_find_channel(dev, "voltage1", 0);
    
    /* Set sample rate
    int ret = iio_channel_attr_write(rx0_i, "sampling_frequency", "2000000");
    if (ret < 0) {
        perror("Error setting RX I sample rate: \n");
        return ret;
    }
    ret = iio_channel_attr_write(rx0_q, "sampling_frequency", "2000000");
    if (ret < 0) {
        perror("Error setting RX Q sample rate: %s\n");
        return ret;
    } */

	iio_channel_enable(rx0_i);
	iio_channel_enable(rx0_q);
 
	rxbuf = iio_device_create_buffer(dev, 4096, false);
	if (!rxbuf) {
		perror("Could not create RX buffer");
        //shutdown();
        iio_context_destroy(ctx);
    }
    
    //int16_t a;
    //int16_t b;
    int flag = 0;
    while (!stop) 
    {
		void *p_dat, *p_end, *r_dat;
		ptrdiff_t p_inc;
 
		iio_buffer_refill(rxbuf);
 
		p_inc = iio_buffer_step(rxbuf);
		p_end = iio_buffer_end(rxbuf);
        if(flag==0)
        {
            printf(" %d\n",p_inc);
        }
        for (p_dat = iio_buffer_first(rxbuf, rx0_i), r_dat = iio_buffer_first(rxbuf, rx0_q);
             p_dat < p_end; p_dat += p_inc, r_dat += p_inc) 
        {
            int16_t *buf_i = p_dat;
            int16_t *buf_q = r_dat;
            
            if(flag == 0 )
            {
            printf(" a: %d b:%d\n",(int)buf_i,(int)buf_q);
            flag ++;
            }
            // for( ; ; )
            /* Process here */
           // if(flag == 0 )
           // {
            
          /*  flag ++;
            }
            if(flag == 1 )
            {
            printf(" a: %d b:%d\n",buf_i,buf_q);
            flag ++;
            }
            if(flag == 2)
            {
            printf(" a: %d b:%d\n",buf_i,buf_q);
            flag ++;
            }*/
        }
        printf(" a: %d b:%d\n",(int)p_dat,(int)r_dat);
    }
    printf("== Finish Sdr_2 ==\n");
    iio_buffer_destroy(rxbuf);

    iio_channel_disable(rx0_i);
    iio_channel_disable(rx0_q);
    iio_context_destroy(ctx);

    return 0;
}