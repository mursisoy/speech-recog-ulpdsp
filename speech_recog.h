/*
 * sppech_recog.h
 *
 *  Created on: Dec 4, 2016
 *      Author: detroit
 */

#ifndef SPEECH_RECOG_H_
#define SPEECH_RECOG_H_


#include <stdlib.h>
#include <stdint.h>
#include "SEmP_5515.h"
#include "lee_switches.h"
#include "usbstk5515_i2c.h"
#include "aic3204.h"
#include "cepstrum.h"

extern uint16_t VOICE_RECORDING_RESET;
extern uint16_t RECORDING_TH;

void init_speech(void);

void speech_recog();

interrupt void ISR_TINT0();
interrupt void ISR_I2S_rx(void);

#endif /* SPPECH_RECOG_H_ */
