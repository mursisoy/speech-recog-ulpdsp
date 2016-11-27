/*
 * speech_config.c
 *
 *  Created on: Nov 27, 2016
 *      Author: detroit
 */

#include "../speech_config.h"



void init_speech(void){

	AIC3204_init();
	USBSTK5515_I2C_init();

	IODIR1 = 0xC000;
	IODIR2 = 0x03;

}


