#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
 
#include <rp.h>
 
#include <sine.h>

const char *gen_waveform_file1="/tmp/gen_ch1.csv";
const char *gen_waveform_file2="/tmp/gen_ch2.csv";
const int *buff_size = 16384;
 
int main(int argc, char **argv){
 
    /* Print error, if rp_Init() function failed */
    if(rp_Init() != RP_OK){
        fprintf(stderr, "Rp api init failed!n");
    }
 
    float ch1_arb[buff_size];
    float ch2_arb[buff_size];

    read_in_file(1, ch1_arb);
    read_in_file(2, ch2_arb);
 
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_ARBITRARY);
    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
 
    rp_GenArbWaveform(RP_CH_1, ch1_arb, buff_size);
    rp_GenArbWaveform(RP_CH_2, ch2_arb, buff_size);
 
    rp_GenAmp(RP_CH_1, 0.7);
    rp_GenAmp(RP_CH_2, 1.0);
 
    rp_GenFreq(RP_CH_1, 4000.0);
    rp_GenFreq(RP_CH_2, 4000.0);
 
    rp_GenOutEnable(RP_CH_1);
    rp_GenOutEnable(RP_CH_2);
 
    /* Releasing resources */
    free(ch1_arb);
    free(ch2_arb);
    rp_Release();
}

int read_in_file(int chann,  float *ch_data)
{
    FILE *fi = NULL;
    int i, read_size, samples_read = 0;

    /* open file */
    if (chann == 1) {

        fi = fopen(gen_waveform_file1, "r+");
        if (fi == NULL) {
            fprintf(stderr, "read_in_file(): Can not open input file (%s): %s\n",
                    gen_waveform_file1, strerror(errno));
            return -1;
        }

    } else {

        fi = fopen(gen_waveform_file2, "r+");
        if (fi == NULL) {
            fprintf(stderr, "read_in_file(): Can not open input file (%s): %s\n",
                    gen_waveform_file2, strerror(errno));
            return -1;
        }
    }

    /* parse at most AWG_SIG_LEN lines and save data to the specified buffers */
    for (i = 0; i < buff_size; i++) {

        read_size = fscanf(fi, "%f \n", &ch_data[i]);
        if((read_size == EOF) || (read_size != 1)) {
            i--;
            break;
        }
    }
    samples_read = i + 1;

    if (samples_read >= buff_size)
        samples_read = buff_size - 1;

    /* check for errors */
    if (i == 0) {
        fprintf(stderr, "read_in_file() cannot read in signal, wrong format?\n");
        fclose(fi);
        return -1;
    }

    /* close a file */
    fclose(fi);

    /* and return the number of parsed lines */
    return samples_read;
}