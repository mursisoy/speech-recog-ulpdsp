/*
 * main.c
 */

#include "5535config.h"
#include "speech_recog.h"

void main(void) {
	
	init5535();
	init_speech();
	

	while(1){

		speech_recog();

	}
}
