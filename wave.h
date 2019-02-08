
#ifndef wave_h
#define wave_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>
#include <math.h>
#include <stdbool.h>


typedef struct {
	uint32_t* canal;
}Tab_bloc;

typedef struct {
	char riff[4];
	uint32_t size_file;
	char wave[4];
	char fmt[4];
	uint32_t number16;
	uint16_t PCM;
	uint16_t number_canal;
	uint32_t frequency;
	uint32_t octet_second;
	uint16_t octet_bloc;
	int16_t precision; //8, 16 ou 24
	char data[4];
	int32_t size_data;
	
	
}header;

typedef struct {
	header* head;
	uint32_t number_Bloc;
	Tab_bloc* blocs; // Tableau de blocs
}pointeurWave;

typedef struct {
	pointeurWave* p;
}wave_t;


//les fonctions à utiliser******************************************

wave_t wave_new (uint32_t f, uint16_t p, uint16_t c, uint32_t B); 

void wave_delete(wave_t w);

int32_t wave_get ( wave_t w, uint32_t i, uint16_t j);

void wave_set (wave_t w, uint32_t i , uint16_t j,int32_t a);

double round( double value);

void add_signal ( wave_t w, double (*s) (double));

wave_t wave_load ( const char * fname);

bool wave_save(const char* fname, wave_t w);

void wave_canal(wave_t w, uint16_t c);

void wave_reverse(wave_t w);

wave_t* wave_split(wave_t w, int* pc);

wave_t wave_merge(wave_t* w, int c);

wave_t wave_crop(wave_t w, uint32_t start, uint32_t length);

wave_t wave_scale ( wave_t w, double s);

void convert_To16 (wave_t w);

void convert_To8 (wave_t w);

void convert_To24 (wave_t w);

void modify_frequency(wave_t w, uint32_t f);

void modify_volume(wave_t w, double factor);


#endif

