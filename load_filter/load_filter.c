#include <stdio.h>
#include <stdlib.h>
#include <iio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
/*

NO ESTA EN USO ESTE .C

*/

bool stop;

typedef struct {
    double re;
    double im;
} complex_t;
typedef struct {
    double re;
    double im;
} complex_rx;

void handle_sig(int sig)
{
	printf("Waiting for process to finish... Got signal %d\n", sig);
	stop = true;
}
/*
    Para transmitir verificar:
    - loopback desactivado
    - Frecuencias
    - 
*/
void complexExp(complex_t *signal, int N, double Fc, double Fs);
int main(){
    //FILE *fptr;
    // Open file for writing
    //fptr = fopen("output.txt", "w");

    // Redirect stdout to the file
    //freopen("output.txt", "w", stdout);

    // Write to stdout (will be redirected to file)
    //printf("Hello, world!\n");

    stop = false;

    signal(SIGINT, handle_sig);
    struct iio_context *ctx = NULL;
    ctx = iio_create_default_context();
    if (!ctx) {
        printf("Unable to create IIO context\n");
        return -1;
    }

    
    struct iio_device *main_dev;
    
    int ret_filter;

    main_dev = iio_context_find_device(ctx, "ad9361-phy");
    const void  *filename = "/workspaces/DOCKER/pps_docker/load_filter/filter.txt";
    ret_filter= iio_device_attr_write_raw(main_dev, "filter_fir_config", filename, strlen(filename) + 1);
    if (ret_filter < 0) {
        printf("Error loading filter configuration: %s\n", strerror(-ret_filter));
        iio_context_destroy(ctx);
        return -1;
    }
    printf("Filter configuration loaded successfully\n");
    // const char *filter_fir_config_file = "filter_fir_config";
    // int ret = iio_device_cfg_write(dev, filter_fir_config_file);
    iio_context_destroy(ctx);
    //fclose(fptr);
    //freopen("/dev/tty", "w", stdout);
    return 0;
}


void complexExp(complex_t *signal, int N, double Fc, double Fs) {
    // N : Number of samples to transmit at once
    // Fc: Carrier frequency in Hz
    // Fs: Sampling frequency in Hz
    if (Fs < 2*Fc) {
        printf("Error: Fs must be at least 2 time Fc\n");
        exit(EXIT_FAILURE);
    }
    double Tsamp  = 1/Fs;
    for (int n = 0; n < N; n++) {
        double angle = 2.0*M_PI*Fc*n*Tsamp;
        //printf("angle :%f\n",angle);
    //    printf("angle cos %d: %f\n",n,cos(angle));
    //    printf("angle sin %d: %f\n",n,sin(angle));
        signal[n].re = cos(angle)* pow(2, 14);
        signal[n].im = sin(angle)* pow(2, 14);
    //    printf("angle cos : %f\n",signal[n].re);
    //    printf("angle sin : %f\n",signal[n].im);
    }
    printf("txSignal:\n");
    //for (int i = 0; i < 11; i++) {
    //    printf("txSignal[%d] = (%f, %f)\n", i, (signal[i].re ), (signal[i].im));
    //}
}
