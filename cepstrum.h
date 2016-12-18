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

void fft_tester(signed int *audio, signed int *out);

void fft_norm(signed int *fftC, signed int *abs_fft, unsigned int fftsize);
void std_norm(signed int *vec, signed int *vec_norm, unsigned int vecsize);
void cepstrum_vec(signed int *audio, signed int *out);

#endif /* speech_recog_h */
