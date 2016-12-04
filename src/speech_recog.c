/*
 * speech_recog.c
 *
 *  Created on: Dec 4, 2016
 *      Author: detroit
 */
#include "../speech_recog.h"


void init_speech(void){

	AIC3204_init();
	USBSTK5515_I2C_init();

	IODIR1 = 0xC000;
	IODIR2 = 0x03;

	CPU_IER0 |= (1<<15);
	CPU_IER1 = 0;
	_enable_interrupts();

}


interrupt void ISR_I2S_rx(void){}
