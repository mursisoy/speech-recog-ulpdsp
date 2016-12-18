/*
 * speech_recog.c
 *
 *  Created on: Dec 4, 2016
 *      Author: detroit
 */
#include "../speech_recog.h"

enum {IDLE, RECORDING, PROCESSING} speech_status = IDLE;
uint16_t VOICE_RECORDING_RESET = 1;


void init_speech(void){

	AIC3204_init();
	set_sampling_frequency_and_gain(16000,0); // 16 kHz
	USBSTK5515_I2C_init();

	//Esto es para ADC para pulsadores
	// Velocidad de conversión a 2Mhz
	SARCLKCTRL = 0x0031;

	// Habilitación ADC SAR
	SARPINCTRL = 0x3100;

	// Entradas analógicas
	SARGPOCTRL = 0;

	IODIR1 = 0xC000;
	IODIR2 = 0x03;

	CPU_IER0 |= (1<<4); // TIMER INTERRUPT ENABLE
	CPU_IER1 = 0;
	_enable_interrupts();

	TIM0PRD1 = 0x4B3F;
	TIM0PRD2 = 0x004C; // 100ms
	TIM0TCR = 0x8003; // Timer Start

}

void speech_recog(){

	switch(speech_status) {
		case IDLE:
			IODATAOUT1 = 0x8000; // Y OFF, B ON
			IODATAOUT1 = 0x0003; // G OFF, R OFF
			break;

		case RECORDING:
			IODATAOUT1 = 0x4000; // Y ON , B OFF
			IODATAOUT1 = 0x0003; // G OFF, R OFF
			break;

		case PROCESSING:
			speech_status = IDLE;
			break;
	}


}

interrupt void ISR_TINT0(){
	static uint16_t pls_mask = 0, ADC_data, recording_cnt = 0;

	if (TIAFR & 0x1) {
		SARCTRL=0xB400;              // Start converison channel 3
		while (SARDATA & 0x8000);    // End conversion
		ADC_data = SARDATA & 0x3FF;

		if (speech_status == IDLE) {
			if ((ADC_data > (SW2-rng)) && (ADC_data < (SW2+rng))){
				pls_mask = (pls_mask<<1) | 1;
			} else {
				pls_mask <<= 1;
			}

			if ( pls_mask & 0x1F ) { // 500ms being pressed => Start recording
				speech_status = RECORDING;
				_disable_interrupts();
				CPU_IER0 |= (1<<14); // Enable I2S2 RX interrupt
				_enable_interrupts();
			}
		} else if (speech_status == RECORDING){ // In RECORDING state, watch for SW2 release or time threshold

			if (~((ADC_data > (SW2-rng)) && (ADC_data < (SW2+rng))) ||  recording_cnt >= 20 ) {
				speech_status = PROCESSING;
				_disable_interrupts();
				CPU_IER0 &= ~(1<<14); // Disable I2S RX interrupt
				_enable_interrupts();

				VOICE_RECORDING_RESET = 1;
			} else {
				recording_cnt++;
			}

		}
	}

}

interrupt void ISR_I2S_rx(void){

	rx_windowing(I2S2_W0_MSW_R);

}
