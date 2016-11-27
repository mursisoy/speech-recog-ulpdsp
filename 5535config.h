/*
 *
 *
 */
#ifndef _5535config
#define _5535config


#include "SEmP_5515.h"


#define CPU_PRCR *(ioport volatile uint16_t *)0x1C05
#define CPU_PSRCR *(ioport	 volatile uint16_t *)0x1C04

void init5535(void); 




#endif
