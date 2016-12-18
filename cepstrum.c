//
//  speech_recog.c
//  uldsp
//
//  Created by Javier Antoran Cabiscol on 04/12/2016.
//  Copyright © 2016 Javier Antoran Cabiscol. All rights reserved.
//

#ifdef CEPSTRUM_

#include "cepstrum.h"

signed int mel_filter_bank[256][24];
signed int dct_mat[24][12];//filas x columnas

void fft_tester(signed int *audio, signed int *out){
    
    signed int to_fft[512];
    signed int to_mel[256];
    signed int to_dct[24];
    unsigned int fftscale = 1;
    
    unsigned int i = 0;
    
    for(i = 0; i < 512; i++){
        
        to_fft[i] = (i < 400)? audio[i]: 0; //vector para fft
    }
    printf("fftscaling: %d", fftscale);
    
    rfft(to_fft, 512, fftscale); //desbordamiento controll
    
    fft_norm(to_fft, out, 512);
    
    
}


void cepstrum_vec(signed int *audio, signed int *out){
    
    
    signed int to_fft[512];
    signed int to_mel[256];
    signed int to_dct[24];
    
    unsigned int i = 0;
    
    for(i = 0; i < 512; i++){
        
        to_fft[i] = (i < 400)? audio[i]: 0; //vector para fft
    }
    
    rfft(to_fft, 512, 1); //desbordamiento controll
    
    fft_norm(to_fft, to_mel, 512);
    
    mmul(to_mel, 1, 256, mel_filter_bank, 256, 24, to_dct);
    
    //logn(); preguntar a isidro
    
    mmul(to_mel, 1, 24, dct_mat, 24, 12, out);
    
    std_norm(out, out, 12);
    
    
}

void fft_norm(signed int *fftC, signed int *abs_fft, unsigned int fftsize){
    
    int i;
    signed long int temp1, temp2;
    
    for (i = 0; i < ((fftsize >> 1) - 1); i++){
        
        temp1 = *(fftC + i) * *(fftC + i);
        temp2 = *(fftC + i + 1) * *(fftC + i + 1);
        *(abs_fft + i) = ((temp1 + temp2) >> 15);
    }
    if( sqrt_16(abs_fft, abs_fft, (fftsize << 1)) ){
        printf("ha ocurrido un overflow");
        }
}

void std_norm(signed int *vec, signed int *vec_norm, unsigned int vecsize){
    //suponemos vecsize 12 codificamos 1/12 como (16-18)
    
    signed int a = 21845; // (1/12)*2^18
    unsigned int i;
    signed long int mean, temp1 = 0;
    
    for( i = 0; i < vecsize; i++){
        mean += *(vec + i);
    }
    
    mean *= (a >> 2);
    mean >>= 16;
    
    for( i = 0; i < vecsize; i++){
        temp1 += (((*(vec + i) - mean) >> 1)^2) >> 1;
    }
    
    temp1 *= a; //esto es un 46 46 no se seguir
    
    // falta sqrt de temp1
    
    
    
}

#endif
