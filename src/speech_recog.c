/*
 * speech_recog.c
 *
 *  Created on: Dec 4, 2016
 *      Author: detroit
 */
#include "../speech_recog.h"

enum {IDLE, RECORDING, PROCESSING, ACCESS_OK, ACCESS_KO} speech_status = IDLE;

uint16_t VOICE_RECORDING_RESET = 1;
uint16_t RECORDING_TH;

void init_speech(void){

	AIC3204_init();
	set_sampling_frequency_and_gain(16000,20); // 16 kHz
	USBSTK5515_I2C_init();

	//Esto es para ADC para pulsadores
	// Velocidad de conversión a 2Mhz
	SARCLKCTRL = 0x0031;

	// Habilitación ADC SAR
	SARPINCTRL = 0x3100;

	// Entradas analógicas
	SARGPOCTRL = 0;

	IODIR1 = 0xC020; // GPIO5 for ACCESS OK OUTPUT
	IODIR2 = 0x0003;
	IODATAOUT1 = 0xC000; // Y OFF, B ON
	IODATAOUT2 = 0x0003;

	CPU_IER0 = 0;
	CPU_IER0 |= (1<<4); // TIMER INTERRUPT ENABLE
	CPU_IER1 = 0;
	CPU_IFR0 = 0xFFFF;
	CPU_IFR1 = 0xFFFF;
	TIAFR = 0xFFFF;


	TIM0PRD1 = 0x4B3F;
	TIM0PRD2 = 0x004C; // 100ms
	TIM0TCR = 0x8003; // Timer Start

	TIM1PRD1 = 0xB27F;
	TIM1PRD2 = 0x0EE6;

	RECORDING_TH = 19; // Timer threshold based on TIMER PERIOD

	_enable_interrupts();

}

void speech_recog(){

	unsigned int N;

	switch(speech_status) {
		case IDLE:
			IODATAOUT1 = 0x8000; // Y OFF, B ON
			IODATAOUT2 = 0x0003; // G OFF, R OFF
			break;

		case RECORDING:

			break;

		case PROCESSING:
			lowen_sf_dr_adj(2); // Filter low energy windows and adjust dynamic range

			N = cepstrum_gen();
			int cep_data[200][16];
			int i,j;
			for (i=0; i<N; i++) {
				for (j=0; j < 16; j++) {
					cep_data[i][j] = *(*(cep+i) + j);
				}
			}

			cep_clean(N);
			speech_status = ACCESS_OK;
			break;

		case ACCESS_OK:
			TIM1TCR = 0x8001;
			IODATAOUT1 |= (1<<5); // Write 1 to GPIO5
			break;
	}


}

interrupt void ISR_TINT0(){
	uint16_t ADC_data;
	static uint16_t pls_mask, recording_cnt = 0;


	if (TIAFR & 0x1) {
		SARCTRL=0xB400;              // Start conversion channel 3
		while (SARDATA & 0x8000);    // End conversion
		ADC_data = SARDATA & 0x3FF;

		if (speech_status == IDLE) {
			if ((ADC_data > (SW2-rng)) && (ADC_data < (SW2+rng))){
				pls_mask = (pls_mask<<1) | 1;
			} else {
				pls_mask <<= 1;
			}

			if ( (pls_mask & 0x1F) == 0x1F ) { // 500ms being pressed => Start recording
				speech_status = RECORDING;
				IODATAOUT1 = 0x4000; // Y ON , B OFF
				IODATAOUT2 = 0x0003; // G OFF, R OFF
				_disable_interrupts();
				CPU_IER0 |= (1<<15); // Enable I2S2 RX interrupt
				_enable_interrupts();

			}
		} else if (speech_status == RECORDING){ // In RECORDING state, watch for SW2 release or time threshold

			if (!((ADC_data > (SW2-rng)) && (ADC_data < (SW2+rng))) ||  recording_cnt >= RECORDING_TH ) {
				_disable_interrupts(); // First Of All
				CPU_IER0 &= ~(1<<15); // Disable I2S RX interrupt
				TIM0TCR &= ~(1<<15); // Disable TIM0
				_enable_interrupts();

				speech_status = PROCESSING;

				IODATAOUT1 = 0x0000; // Y ON , B OFF
				IODATAOUT2 = 0x0000; // G OFF, R OFF

				recording_cnt = 0;
				pls_mask = 0;
				VOICE_RECORDING_RESET = 1;

			} else {
				recording_cnt++;
			}

		}
		TIAFR |= 1;
	} else if (TIAFR & 2) {
		speech_status = IDLE;
		TIM0TCR |= (1<<15);
		IODATAOUT1 &= ~(1<<5); // Write 0 to GPIO5
		TIAFR |= 2;
	}


}

interrupt void ISR_I2S_rx(void){
	if (speech_status == RECORDING) {
		rx_windowing(I2S2_W0_MSW_R, VOICE_RECORDING_RESET);
		VOICE_RECORDING_RESET = 0;
	}
}
