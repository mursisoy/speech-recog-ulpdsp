/*
 * voice_record.h
 *
 *  Created on: Dec 4, 2016
 *      Author: detroit
 */

#ifndef VOICE_RECORD_H_
#define VOICE_RECORD_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define WIN_SAMPLES 400
#define WIN_STEP 160

typedef struct linkl{ //work in progress
    signed int dat[WIN_SAMPLES];
    int id;
    struct linkl *next;
} linkl;


extern linkl *link0;

void add_list();
void remove_list();
void write_list(int lista, int posicion, int dato);
int read_list(int lista, int posicion);
void rx_windowing(signed int rx, uint16_t reset);
int list_length();

#endif /* VOICE_RECORD_H_ */
