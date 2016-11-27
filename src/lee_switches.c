   /* lee_switches.c
 *
 *   Incluye 4 versiones de la funci�n lee_switches:
 *
 *   		lee_switches1() => Utiliza instrucciones condicionales para implamentar la MEF
 *   		lee_switches2() => Utiliza tabla de transici�n y salida para implamentar la MEF
 *   		lee_switches3() => Utiliza punteros a funciones para implamentar la MEF
 *   		lee_switches4() => Utiliza punteros a funciones en la MEF y filtra la pulsacion doble
 *
 *  �ltima edicion 15/10/2013
 */



#include "../SEmP_5515.h"
#include "../lee_switches.h"




/*
* unsigned int lee switches()
*
* 	Esta funci�n devuelve los valores siguientes:
* 			0 -> si no hay ningun switch pulsado / error en lectura
* 			1 -> si se ha pulsado el SW1
* 			2 -> si se ha pulsado el SW2
* 			3 -> si se han pulsado los dos switches
*
* 	en caso de repetir el valor anterior devuelve un 0
*
*   Los switches est�n conectados al canal 3 del conversor ADC.
*     las lecturas del conversor en funci�n del switch pulsado
*     son las siguientes:
*
* 			~1.8v (~1023) -> Si no hay ninguno pulsado
* 		 	~1.2v (~ 682) -> Si esta pulsado SW1
*			~0.9v (~ 511) -> Si esta pulsado SW2
* 			~0.7v (~ 409) -> Si est�n pulsados SW1 y SW2
*/
//#define SW1   0x2AA  // Switch 1
//#define SW2   0x1FF  // Switch 2
//#define SW12  0x199  // Switch 1+2
//#define NoKey 0x3FF  // No pulsacion
//#define Error 0x4
//#define rng   12     //
//
//#define MAX_CNT  100

/******************************************************************
 * Opcion 1: Instrucciones condicionales anidadas                 *
 ******************************************************************/

unsigned int lee_switches1 ()
{
	typedef  enum  {pls_reposo, pls_1, pls_2, pls_12} pls_estados_t;
	static pls_estados_t pls_estado=pls_reposo;
	unsigned int dato_ADC,dato_leido,pulsacion;


	SARCTRL=0xB400;              // Inicio conversi�n canal 3
    while (SARDATA & 0x8000);    // Conversi�n finalizada
	dato_ADC=SARDATA & 0x3FF;
    dato_leido=Error;
	if ((dato_ADC > (SW1-rng))   && (dato_ADC < (SW1+rng)))   dato_leido=1;
	if ((dato_ADC > (SW2-rng))   && (dato_ADC < (SW2+rng)))   dato_leido=2;
	if ((dato_ADC > (SW12-rng))  && (dato_ADC < (SW12+rng)))  dato_leido=3;
	if ((dato_ADC > (NoKey-rng))) dato_leido=0;

	pulsacion=0;
	switch (pls_estado)
	{
	case pls_reposo: if (dato_leido==1)      {pls_estado=pls_1; pulsacion=1;}
	                 else if (dato_leido==2) {pls_estado=pls_2; pulsacion=2;}
	                 else if (dato_leido==3) {pls_estado=pls_12;pulsacion=3;}
 	break;
	case pls_1:      if ((dato_leido!=1)&&(dato_leido!=4)) pls_estado=pls_reposo;
 	break;
	case pls_2:	 if ((dato_leido!=2)&&(dato_leido!=4)) pls_estado=pls_reposo;
 	break;
	case pls_12:	 if ((dato_leido!=3)&&(dato_leido!=4)) pls_estado=pls_reposo;
	}

	return pulsacion;
}

/******************************************************************
 * Opcion 2: Tabla de transici�n y de salida                      *
 ******************************************************************/

unsigned int lee_switches2 ()
{
	typedef  enum  {pls_reposo, pls_1, pls_2, pls_12} pls_estados_t;
	const  pls_estados_t trans [4][5] =
	    {{ pls_reposo,      pls_1,      pls_2,     pls_12,   pls_reposo},
		 { pls_reposo,      pls_1, pls_reposo, pls_reposo,        pls_1},
		 { pls_reposo, pls_reposo,      pls_2, pls_reposo,        pls_2},
		 { pls_reposo, pls_reposo, pls_reposo,     pls_12,       pls_12}};

	const int    salida[4][5]    ={ {0,1,2,3,0},
							        {0,0,0,0,0},
							        {0,0,0,0,0},
							        {0,0,0,0,0}};


	static pls_estados_t pls_estado=pls_reposo;
	unsigned int dato_ADC,dato_leido,pulsacion;


	SARCTRL=0xB400;              // Inicio conversi�n canal 3
    while (SARDATA & 0x8000);    // Conversi�n finalizada
	dato_ADC=SARDATA & 0x3FF;
    dato_leido=Error;
	if ((dato_ADC > (SW1-rng))   && (dato_ADC < (SW1+rng)))   dato_leido=1;
	if ((dato_ADC > (SW2-rng))   && (dato_ADC < (SW2+rng)))   dato_leido=2;
	if ((dato_ADC > (SW12-rng))  && (dato_ADC < (SW12+rng)))  dato_leido=3;
	if ((dato_ADC > (NoKey-rng)))  dato_leido=0;


	pulsacion=salida[pls_estado][dato_leido];
    pls_estado=trans[pls_estado][dato_leido];

	return pulsacion;
}

/******************************************************************
 * Opcion 3: Puntero a funci�n, una funci�n para cada estado      *
 ******************************************************************/

typedef unsigned int (*fsm_handler_t)();

unsigned int reposo (unsigned int dato_leido,fsm_handler_t *p_state);
unsigned int pls1   (unsigned int dato_leido,fsm_handler_t *p_state);
unsigned int pls2   (unsigned int dato_leido,fsm_handler_t *p_state);
unsigned int pls12  (unsigned int dato_leido,fsm_handler_t *p_state);



unsigned int reposo (unsigned int dato_leido, fsm_handler_t *p_state)
{ unsigned int pulsacion=0;
  if (dato_leido==1)      {*p_state=&pls1; pulsacion=1;}
  else if (dato_leido==2) {*p_state=&pls2; pulsacion=2;}
  else if (dato_leido==3) {*p_state=&pls12;pulsacion=3;}
  return pulsacion;
}

unsigned int pls1 (unsigned int dato_leido, fsm_handler_t *p_state)
{
   if ((dato_leido!=1)&&(dato_leido!=4)) *p_state=&reposo;
   return 0;
}

unsigned int pls2 (unsigned int dato_leido, fsm_handler_t *p_state)
{
  if ((dato_leido!=2)&&(dato_leido!=4)) *p_state=&reposo;
  return 0;
}

unsigned int pls12 (unsigned int dato_leido, fsm_handler_t *p_state)
{
  if ((dato_leido!=3)&&(dato_leido!=4)) *p_state=&reposo;
  return 0;
}



unsigned int lee_switches3 ()
{
	unsigned int dato_ADC,dato_leido;
	static fsm_handler_t p_my_FSM=&reposo;


	SARCTRL=0xB400;              // Inicio conversi�n canal 3
    while (SARDATA & 0x8000);    // Conversi�n finalizada
	dato_ADC=SARDATA & 0x3FF;
    dato_leido=Error;
	if ((dato_ADC > (SW1-rng))   && (dato_ADC < (SW1+rng)))   dato_leido=1;
	if ((dato_ADC > (SW2-rng))   && (dato_ADC < (SW2+rng)))   dato_leido=2;
	if ((dato_ADC > (SW12-rng))  && (dato_ADC < (SW12+rng)))  dato_leido=3;
	if ((dato_ADC > (NoKey-rng))) dato_leido=0;

	return (* p_my_FSM)(dato_leido,&p_my_FSM);
}

/******************************************************************
 * Opci�n 4: Puntero a funci�n, una funci�n para cada estado FSM2 *
 ******************************************************************/

//typedef unsigned int (*fsm_handler_t)();


unsigned int reposo_fsm2   (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt);
unsigned int pre_pls1_fsm2 (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt);
unsigned int pls1_fsm2     (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt);
unsigned int pre_pls2_fsm2 (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt);
unsigned int pls2_fsm2     (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt);
unsigned int pls12_fsm2    (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt);



unsigned int reposo_fsm2 (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt)
{ unsigned int pulsacion=0;
  if (dato_leido==1)        {*p_state=&pre_pls1_fsm2; pulsacion=0;}
    else if (dato_leido==2) {*p_state=&pre_pls2_fsm2; pulsacion=0;}
    else if (dato_leido==3) {*p_state=&pls12_fsm2;    pulsacion=3;}
  *p_ini_cnt=1;
  return pulsacion;
}

unsigned int pre_pls1_fsm2 (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt)
{  unsigned int pulsacion=0;
   if ((dato_leido==1)||(dato_leido==4))
	   {if (cnt==MAX_CNT)
		  { *p_state=&pls1_fsm2;
	        pulsacion=1;}
	   }
   else *p_state=&reposo_fsm2;
   return pulsacion;
}

unsigned int pls1_fsm2 (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt)
{
   if ((dato_leido!=1)&&(dato_leido!=4)) *p_state=&reposo_fsm2;
   return 0;
}


unsigned int pre_pls2_fsm2 (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt)
{  unsigned int pulsacion=0;
   if ((dato_leido==2)||(dato_leido==4))
	   {if (cnt==MAX_CNT)
		 {*p_state=&pls2_fsm2;
	      pulsacion=2;}
	   }
   else *p_state=&reposo_fsm2;
   return pulsacion;
}

unsigned int pls2_fsm2 (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt)
{
  if ((dato_leido!=2)&&(dato_leido!=4)) *p_state=&reposo_fsm2;
  return 0;
}

unsigned int pls12_fsm2 (unsigned int dato_leido, fsm_handler_t *p_state, unsigned int cnt, unsigned int *p_ini_cnt)
{
	if (dato_leido==0) *p_state=&reposo_fsm2;
  return 0;
}


unsigned int lee_switches4 ()
{
	unsigned int dato_ADC,dato_leido;

	static fsm_handler_t p_my_FSM=&reposo_fsm2;
	static unsigned int cnt=0;
	unsigned int ini_cnt=0,pulsacion;


	SARCTRL=0xB400;              // Inicio conversi�n canal 3
    while (SARDATA & 0x8000);    // Conversi�n finalizada
	dato_ADC=SARDATA & 0x3FF;
    dato_leido=Error;
	if ((dato_ADC > (SW1-rng))   && (dato_ADC < (SW1+rng)))   dato_leido=1;
	if ((dato_ADC > (SW2-rng))   && (dato_ADC < (SW2+rng)))   dato_leido=2;
	if ((dato_ADC > (SW12-rng))  && (dato_ADC < (SW12+rng)))  dato_leido=3;
	if ((dato_ADC > (NoKey-rng))) dato_leido=0;



	pulsacion=(* p_my_FSM)(dato_leido,&p_my_FSM,cnt,&ini_cnt);
	cnt++;
	if (ini_cnt) cnt=0;

	return pulsacion;
}


