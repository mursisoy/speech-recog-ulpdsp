//
//  speech_recog.h
//  uldsp
//
//  Created by Javier Antoran Cabiscol on 04/12/2016.
//  Copyright © 2016 Javier Antoran Cabiscol. All rights reserved.
//

#ifndef cepstrum_h
#define cepstrum_h

#include <stdio.h>
#include "stdlib.h"
#include "dsplib.h"
#include "misc.h"
#include "voice_record.h"
#include "cepstrum_register.h"

#define FFT_RES 256
#define VOICE_CHANNELS 24
#define DCT_LENGTH 16

extern DATA **cep;
extern DATA mel_filter_bank[FFT_RES*VOICE_CHANNELS];
extern DATA dct_mat[VOICE_CHANNELS*DCT_LENGTH];//filas x columnas

typedef struct cepl{
    DATA dat[DCT_LENGTH];
    unsigned long int power;
    int id;
    struct cepl *next;
} cepl;


extern cepl *cep0;

void fft_tester(DATA *audio, DATA *out);

void fft_norm(DATA *fftC, DATA *abs_fft, unsigned int fftsize);
void cepstrum_vec(DATA *audio, DATA *out);
unsigned int cepstrum_gen();
signed int sqrt32(unsigned long int num);
void std_norm(DATA *vec, DATA *out);
unsigned int rms_error16(DATA *vec1, DATA *vec2);
void cepstrum_comp(DATA cep1[][16], unsigned int N1, DATA cep2[][16], unsigned int N2, unsigned int *error_all);
int cmpfunc (const void * a, const void * b);
void cep_clean(unsigned int N);
void online_cepstrum_gen();
void add_cep();
unsigned long int window_power(int *samples);
void lowen_cf(unsigned int th_scale);
unsigned int cepstrum_mat_gen();
void vec_mv(DATA *out, DATA *in, unsigned int size);


#endif /* speech_recog_h */
