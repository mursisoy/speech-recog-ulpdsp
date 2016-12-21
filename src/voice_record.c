#include "../voice_record.h"

linkl *link0 = NULL;
linkl *current_link = NULL;

void rx_windowing(signed int rx, uint16_t reset){

    int ventana = 400;
    int solape = 160;
    int i = 0;

    static unsigned int c[3] = {0, 0, 0};
    static int list_ = 0;
    static signed int ci = 0;
    signed int index = 0;

    if (reset) {
    	remove_list();
    	list_ = 0;
    	ci = 0;
    }

    if (link0 == NULL){
    	link0 = malloc(sizeof(linkl));
    	link0->id = 0;
    	link0->next = NULL;
    	current_link = link0;
    }

    for (i = 0; i < 3; i++){

        index = ((ci - i) < 0)? 3 + (ci - i): ci - i;

        if ((list_ - i > -1) && ( c[index] < ventana) ) {
            write_list(list_ - i, c[index], rx);
            c[index]++;
        }
    }

    if (c[ci] == solape ){
        list_++;
        ci = (ci == 2)? 0: ci + 1;
        c[ci] = 0;
        add_list();
    }
    return;
}

int read_list(int lista, int posicion){

    int r;

    while(current_link->id < lista){
        current_link = current_link->next;
    }

    r = current_link->dat[posicion];
    current_link = link0;
    return r;
}

void write_list(int lista, int posicion, int dato){


    current_link = link0;//si se maneja bien se puede quitar esta linea

    while(current_link->id < lista){
        current_link = current_link->next;
    }

    current_link->dat[posicion] = dato;
    current_link = link0;
}

void remove_list(){

	linkl *tmp;
	current_link = link0;
	while(current_link != NULL){
		tmp = current_link;
		current_link = current_link->next;
		free(tmp);
	}
	link0 = NULL;

}

void add_list(){


    current_link = link0;


    while(current_link->next != NULL){
        current_link = current_link->next;
    }


    current_link->next =  malloc(sizeof(linkl));
    current_link->next->id = current_link->id + 1;
    current_link->next->next = NULL;
    printf("He creado la ventana %d\n",current_link->next->id);
    current_link = link0;
}

int list_length(){
    
    unsigned int r = 0;
    
    while(current_link->next != NULL){
        current_link = current_link->next;
        r++;
    }
    current_link = link0;
    r++;
    return r;
}

