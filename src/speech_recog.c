/*
 * speech_recog.c
 *
 *  Created on: Dec 4, 2016
 *      Author: detroit
 */
#include "../speech_recog.h"

enum {IDLE, RECORDING, PROCESSING} speech_status = IDLE;


void init_speech(void){

	AIC3204_init();
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
	TIM0TCR = 0x8003;

}

void speech_recog(){

	if (speech_status == IDLE) {
		IODATAOUT1 = 0x8000; // Y OFF, B ON
		IODATAOUT1 = 0x0003; // G OFF, R OFF
	}

}

interrupt void ISR_TINT0(){
	static uint16_t pls_mask = 0, ADC_data;


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
		} else if (speech_status == RECORDING){ // In RECORDING state, watch for SW2 release
			if (~((ADC_data > (SW2-rng)) && (ADC_data < (SW2+rng)))) {
				speech_status = PROCESSING;
				_disable_interrupts();
				CPU_IER0 &= ~(1<<4); // Disable TINT0 interrupt
				_enable_interrupts();
			}
		}
	}

}

interrupt void ISR_I2S_rx(void){

}
