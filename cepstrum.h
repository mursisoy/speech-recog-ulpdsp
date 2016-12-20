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

DATA **cep;
DATA mel_filter_bank[256*24];
DATA dct_mat[24*12];//filas x columnas

void fft_tester(DATA *audio, DATA *out);

void fft_norm(DATA *fftC, DATA *abs_fft, unsigned int fftsize);
void std_norm(DATA *vec, DATA *vec_norm, unsigned int vecsize);
void cepstrum_vec(DATA *audio, DATA *out);
void cepstrum_gen();
signed int sqrt32(unsigned long int num);
void std_norm(signed int *vec, signed int *out);

#endif /* speech_recog_h */
