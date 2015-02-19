#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
#include <rp.h>
 
 
int main(int argc, char **argv){
 
    if(rp_Init() != RP_OK){
        fprintf(stderr, "Rp api init failed!n");
    }

    rp_GenOutDisable(RP_CH_1);
    rp_GenOutDisable(RP_CH_2);
 

    rp_Release();
}