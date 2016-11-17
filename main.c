/*
 * main.c
 */
int main(void) {
	
	//configuracion paginas vectores interrupcion
	CPU_IVPD = 0x04DE;
	CPU_IVPH = 0x04DE;

	// INICIO CONFIGURACIÓN C5535

	// Reset de Periféricos
	CPU_PSRCR = 0x020;
	CPU_PRCR = 0x0081;   //8--> PG4_RST 1-->I2C_RST
	asm(" RPT #65535 ");
	asm(" NOP ");
	//Damos valor a los registros para que el reloj llegue a todos los perifericos
	CPU_PCGR1 = 0x0000;
	CPU_PCGR2 = 0x0000;
	//Configuramos la PLL
	PLL_CNTL1 = 0x8BE8;// PLL power up. PLL Multiplier M = 1000     ||    PG1.4: 0x82FA;
	PLL_CNTL2 = 0x8000;// Bypass reference divider
	PLL_CNTL3 = 0x0806;// initialization bits
	PLL_CNTL4 = 0x0000;// Bypass output divider
	//External bus selection register
	CPU_EBSR = 0x1A00;// Paralel port Mode 1 (SPI, GPIO, UART, and I2S2), serial ports to GPIOs

	/* I2S2 settings */
	I2S2_SRGR = 0x0;
	I2S2_CR = 0x8010;
	I2S2_ICMR = 0x24;

	// FIN CONFIGURACIÓN C5535
	

	return 0;
}
