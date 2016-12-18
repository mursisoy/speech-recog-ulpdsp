/*
 * voice_record.h
 *
 *  Created on: Dec 4, 2016
 *      Author: detroit
 */

#ifndef VOICE_RECORD_H_
#define VOICE_RECORD_H_

#include <stdlib.h>

typedef struct linkl{ //work in progress
    signed int dat[400];
    int id;
    struct linkl *next;
} linkl;


extern linkl *link0;
extern linkl *current_link;

void add_list();
void write_list(int lista, int posicion, int dato);
int read_list(int lista, int posicion);
void rx_windowing(signed int rx);


#endif /* VOICE_RECORD_H_ */
