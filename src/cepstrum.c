//
//  speech_recog.c
//  uldsp
//
//  Created by Javier Antoran Cabiscol on 04/12/2016.
//  Copyright © 2016 Javier Antoran Cabiscol. All rights reserved.
//
//#ifdef CEPSTRUM_

#include "../cepstrum.h"

DATA **cep;
DATA mel_filter_bank[FFT_RES*FFT_LENGTH];
DATA dct_mat[FFT_LENGTH*DCT_LENGTH];//filas x columnas

void fft_tester(DATA *audio, DATA *out){
    
    DATA to_fft[512];
    
    unsigned int i = 0;
    
    for(i = 0; i < 512; i++){
        
        to_fft[i] = (i < 400)? audio[i]: 0; //vector para fft
    }

    
    rfft(to_fft, 512, SCALE); //desbordamiento controll
    
    fft_norm(to_fft, out, 512);
    
    
}


void cepstrum_vec(DATA *audio, DATA *out){
    
    DATA to_fft[512];
    DATA to_mel[256];
    DATA to_dct[24];
    
    unsigned int i = 0;
    
    for(i = 0; i < 512; i++){
        
        to_fft[i] = (i < 400)? audio[i]: 0; //vector para fft
    }
    
    rfft(to_fft, 512, SCALE); //desbordamiento controll
    
    fft_norm(to_fft, to_mel, 512);
    
    mmul(to_mel, 1, 256, mel_filter_bank, 256, 24, to_dct);
    
    //logn(); devuleve Q31 para entradas q 15 porque haces log de num decimal
    
    mmul(to_mel, 1, 24, dct_mat, 24, 16, out);
    
    std_norm(out, out);//we keep 16 first dct coeficients
    
    
}

void fft_norm(DATA *fftC, DATA *abs_fft, unsigned int fftsize){
    
    int i;
    signed long int temp1, temp2;
    
    for (i = 0; i < fftsize; i += 2){
        
        temp1 = *(fftC + i) * *(fftC + i);
        temp2 = *(fftC + i + 1) * *(fftC + i + 1);
        *(abs_fft + (i >> 1)) = sqrt32(temp1 + temp2);
    }

}


signed int sqrt32(unsigned long int num){
    
    signed long int dist = 0;
    signed long int dist_check = 0;
    signed int estimation = 0;
    signed int last_est = 0;
    unsigned int i = 0;
    
    
    do{
        
        estimation |= (1 << (14 - i));
        
        dist = (estimation * estimation - num);
        
        if (dist == 0)
            return estimation;
        
        if (dist > 0){
            estimation = last_est;
        }
        
        else{
            last_est = estimation;
        }
        i++;
            }while(i <= 14);
    
    dist_check = ((estimation+1) * (estimation+1) - num);
    dist = (estimation * estimation - num);
    
    if( abs(dist_check) < abs(dist) )
        return (estimation+1);
    else if ( abs(dist_check = ((estimation-1) * (estimation-1) - num)) < abs(dist) )
        return (estimation - 1);
    else
        return estimation;
}

void std_norm(DATA *vec, DATA *out){
    //suponemos vecsize 16
    unsigned int i;
    signed int std;
    signed long int mean = 0;
    signed long long int temp1 = 0;
    
    for( i = 0; i < 16; i++){
        mean += *(vec + i);
    }
     printf("%ld\n",mean);
    //20|15
    mean >>= 4;//16|15
    printf("%ld\n",mean);
    for( i = 0; i < 16; i++){
        temp1 += ( (*(vec+i) - mean) * (*(vec+i) - mean) );
        
    }//35|30
    temp1 >>= 4; //31/30
    
    std = sqrt32((unsigned long int) temp1); //16|15
    
    for(i = 0; i < 16; i++){
         *(out + i) = (*(vec + i) / std);
    }
}


void cepstrum_gen(){
    
    //we still dont know size of input, could be changed
    unsigned int N = list_length();

    printf("there are %d windows\n", N);
    
    unsigned int elementos = 16;
    
    unsigned  int i = 0;
    unsigned int j = 0; //remvove later
    
    //creating cepstrum matrix
    cep = (DATA**) malloc(N * sizeof(DATA*));
    
    while(i < N){
            
        *(cep + i) = (DATA*) malloc(elementos * (sizeof(DATA)));
        
        //filling matrix with info
        //cepstrum_vec(current_link->dat, *(cep+i)); ** castear a data
        for(j = 0; j < elementos; j++){//remove later
            cep[i][j] = (DATA) current_link->dat[j];
            printf("escribiendo %d en [%d][%d]\n", current_link->dat[j], i, j);
        }

        current_link = current_link->next;
        free(link0);
        link0 = current_link;
        i++;
    } 
    
}

unsigned int rms_error16(DATA *vec1, DATA *vec2){
    
    unsigned long int dist = 0;
    unsigned int i = 0;
    
    for( i = 0; i < 16; i++){
        dist += ( (*(vec1+i) - *(vec2+i)) * (*(vec1+i) - *(vec2+i)) );
    }//35|30
    dist >>= 4; //31|30
    return sqrt32(dist);

}

void cepstrum_comp(DATA cep1[][16], unsigned int N1, DATA cep2[][16], unsigned int N2, unsigned int *error_all){
    
    unsigned int i = 0;
    unsigned int j = 0;
    
    for(i = 0; i < N1; i++){
        
        for(j = 0; j < N2; j++){
            
            *(error_all + i*N2 + j) = rms_error16( *(cep1 + i), *(cep2 + j) );
            printf("escribiendo en [%d][%d] = %d\n", i, j, *(error_all + i*N1 + j));
        }
        
    }
}

int cmpfunc (const void * a, const void * b)
{
    int va = *(const int*) a;
    int vb = *(const int*) b;
    return (va > vb) - (va < vb);
}
//#endif
