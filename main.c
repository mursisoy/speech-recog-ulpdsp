/*
 * main.c
 */

#include "5535config.h"
#include "speech_recog.h"

void main(void) {
	
	init5535();
	init_speech();
	
	/*int i;

	DATA *tofft;
	DATA test_out[256];

	tofft = malloc(400*sizeof(int));

	for(i = 0; i < 400; i++)
			tofft[i] = 1;
	//tofft[0] = 16000;

	fft_tester(tofft, test_out);

	for(i = 0; i < 256; i++)
		printf("[%d]=%d\n",i,test_out[i]);
*/

	while(1){

		speech_recog();

	}
}
