#include "../voice_record.h"

linkl *link0 = NULL;

void rx_windowing(signed int rx, uint16_t reset){

    int i = 0;

    static unsigned int c[3] = {0, 0, 0};
    static int list_ = 0;
    static signed int ci = 0;
    signed int index = 0;

    if (reset) {
    	remove_list();
    	list_ = 0;
    	ci = 0;
    	c[0]=0;
    	c[1]=0;
    	c[2]=0;
    }

    if (link0 == NULL){
    	if( (link0 = malloc(sizeof(linkl))) == NULL){
    	}
		link0->id = 0;
		link0->next = NULL;
    }

    for (i = 0; i < 3; i++){

        index = ((ci - i) < 0)? 3 + (ci - i): ci - i;

        if ((list_ - i > -1) && ( c[index] < WIN_SAMPLES) ) {
            write_list(list_ - i, c[index], rx);
            c[index]++;
        }
    }

    if (c[ci] == WIN_STEP ){
        list_++;
        ci = (ci == 2)? 0: ci + 1;
        c[ci] = 0;
        add_list();
    }
    return;
}

int read_list(int lista, int posicion){

	linkl *current_link = link0;

    int r;

    while(current_link->id < lista){
        current_link = current_link->next;
    }

    r = current_link->dat[posicion];
    current_link = link0;
    return r;
}

void write_list(int lista, int posicion, int dato){


	linkl *current_link = link0;

    while(current_link->id < lista){
        current_link = current_link->next;
    }

    current_link->dat[posicion] = dato;
}

void remove_list(){

	linkl *current_link = link0;

	while(current_link != NULL){
		link0 = current_link->next;
		free(current_link);
		current_link = link0;
	}
}

void add_list(){


	linkl *current_link = link0;


    while(current_link->next != NULL){
        current_link = current_link->next;
    }


    if( (current_link->next =  malloc(sizeof(linkl)) ) == NULL){
    }
    current_link->next->id = current_link->id + 1;
    current_link->next->next = NULL;
}

int list_length(){
    
    unsigned int r = 0;
    
    linkl *current_link = link0;

    while(current_link != NULL){
        current_link = current_link->next;
        r++;
    }
    current_link = link0;
    return r;
}

void lowen_sf_dr_adj(unsigned int th_scale) {
	int N = list_length();
	unsigned int window_cnt = 0;
	unsigned int sample_cnt;
	unsigned long int *window_energy; // Max sum is 25 bits 16+9
	unsigned long int energy_threshold = 0;
	unsigned int max_value=0, abs_sample, signal_scale;
	linkl *prev_link;
	linkl *current_link;


	window_energy = (unsigned long int *) malloc(N * sizeof(*window_energy));

	current_link = link0;

	while (current_link != NULL) {
		*(window_energy + window_cnt) = 0;
		for (sample_cnt = 0; sample_cnt < WIN_SAMPLES; sample_cnt++) {
			abs_sample = abs(current_link->dat[sample_cnt]);
			*(window_energy + window_cnt) += abs_sample;
			max_value = (max_value < abs_sample)? abs_sample : max_value;
		}
		energy_threshold += *(window_energy + window_cnt);

		window_cnt++;
		current_link = current_link->next;
	}

	energy_threshold /= (N * th_scale);

	if(max_value < DYN_RNG_ADJ) {
		signal_scale = DYN_RNG_ADJ/max_value;
	} else {
		signal_scale = 0;
	}


	current_link = link0;
	prev_link = NULL;
	window_cnt = 0;
	while (current_link != NULL) {
		if (*(window_energy + window_cnt) < energy_threshold) {
			if (prev_link == NULL) {
				link0 = current_link->next;
				free(current_link);
				current_link = link0;
			} else {
				prev_link->next = current_link->next;
				free(current_link);
				current_link = prev_link->next;
			}
		} else {
			if (signal_scale > 1) {
				for (sample_cnt = 0; sample_cnt < WIN_SAMPLES; sample_cnt++) {
					current_link->dat[sample_cnt] *= signal_scale;
				}
			}

			prev_link = current_link;
			current_link = current_link->next;
		}

		window_cnt++;

	}
	free(window_energy);
}
