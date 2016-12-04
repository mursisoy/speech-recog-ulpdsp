/*
 * sppech_recog.h
 *
 *  Created on: Dec 4, 2016
 *      Author: detroit
 */

#ifndef SPPECH_RECOG_H_
#define SPPECH_RECOG_H_

#include <stdlib.h>
#include <stdint.h>
#include "lee_switches.h"
#include "dsplib.h"
#include "misc.h"
#include "usbstk5515_i2c.h"
#include "aic3204.h"

void init_speech(void);

void speech_recog();

interrupt void ISR_I2S_rx(void);

#endif /* SPPECH_RECOG_H_ */
