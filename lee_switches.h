/*
 * lee_switches.h
 *
 *  Created on: 12/11/2013
 *      Author: isidro
 */

/*
* unsigned int lee switches()
*
* 	Esta función devuelve los valores siguientes:
* 			0 -> si no hay ningun switch pulsado / error en lectura
* 			1 -> si se ha pulsado el SW1
* 			2 -> si se ha pulsado el SW2
* 			3 -> si se han pulsado los dos switches
*
* 	en caso de repetir el valor anterior devuelve un 0
*
*   Los switches están conectados al canal 3 del conversor ADC.
*     las lecturas del conversor en función del switch pulsado
*     son las siguientes:
*
* 			~1.8v (~1023) -> Si no hay ninguno pulsado
* 		 	~1.2v (~ 682) -> Si esta pulsado SW1
*			~0.9v (~ 511) -> Si esta pulsado SW2
* 			~0.7v (~ 409) -> Si están pulsados SW1 y SW2
*/





#ifndef LEE_SWITCHES_H_
#define LEE_SWITCHES_H_

#define SW1   0x2AA  // Switch 1
#define SW2   0x1FF  // Switch 2
#define SW12  0x199  // Switch 1+2
#define NoKey 0x3FF  // No pulsacion
#define Error 0x4
#define rng   12     //

#define MAX_CNT  100


uint16_t lee_switches1(void);
uint16_t lee_switches2(void);
uint16_t lee_switches3(void);
uint16_t lee_switches4(void);




#endif /* LEE_SWITCHES_H_ */
