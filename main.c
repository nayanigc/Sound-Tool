#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "wave.h"
#include "menu.h"
#include "args.h"

wave_t w;
char currentfile[100];
bool q;

void open() {
	if (w.p != NULL) {
		printf("do you like to save the audio file? 'y' or 'n'\n");
		char c = getchar();
		empty_buffer();
		if(c=='y'){
			bool save = wave_save(currentfile, w);
			if (save!=true){
				printf("impossible to save the file\n");		
			}
			printf("successed to save\n");
		}
	}
	printf("give a audio file name\n");
	scanf("%s", currentfile);
	empty_buffer();
	w = wave_load(currentfile);
	return;
}

void save_mono(){
	convert_To8(w);
	wave_canal(w,1);
	w.p->W->frequency = 11.025;	
}

void save_stereo(){
	convert_To16(w);
	wave_canal(w,2);
	w.p->W->frequency = 44.1;
	
}

/*void save_5.1(){

}*/

void save_customise(){

	int16_t precision;
	printf("insert precision\n");
	scanf ("%hd",&precision);
	
	uint32_t frequency;
	printf("insert frequency\n");
	scanf ("%d",&frequency);

	uint16_t canal;
	printf("insert canal\n");
	scanf ("%hd",&canal);

	if ( precision == 8){
		convert_To8(w);
	}	
	
	if (precision == 16){
		convert_To16(w);	
	}
	
	
}
void info(){
	if ( w.p != NULL){
		printf( "File Name : %s\n",currentfile);
		printf(	"Number of Canals : %d\n",w.p->W->number_canal);
		printf(	"Frequency : %d Hz\n",w.p->W->frequency); 
		int32_t size_data = w.p->W->size_data;
		uint32_t octet_second=  w.p->W->octet_second;
		uint32_t seconds;
		seconds = size_data/octet_second;
		int32_t hours = 0;
		int32_t minutes = 0;
		hours = seconds / 3600;
		seconds -= hours * 3600;
		minutes = seconds / 60;
		seconds -= minutes * 60;
		printf("Approximatif Time : %d:%d:%d\n", hours, minutes, seconds); 
	}else{
		printf("Error information\n"); 
	}
}
void quit(){
	q = true;
	printf("do you like to save the audio file? 'y' or 'n'\n");
	char c = getchar();
	empty_buffer();
	if(c=='y'){
		if(w.p!=NULL){
			bool save = wave_save(currentfile, w);
			if (save!=true){
				printf("impossible to save the file\n");		
			}
			printf("successed to save\n");
			return;
		}
		printf("nothing to save\n");
	}
	return;
}


void reverse(){
	if ( w.p != NULL){
		wave_reverse(w);
		bool save = wave_save(currentfile, w);
		if (save!=true){
			printf("impossible to save the file\n");		
		}
		printf("successed to save in the same file\n");
		return;
	}
	else {
		printf("Error reverse\n");
		return;
	}
}
void cut(){
	char c;
	uint32_t start;
	printf("date to start : \n");
	scanf("%d",&start);
	uint32_t length;
	printf("length : \n");
	scanf("%d",&length);
	char name[100];
	if ( w.p != NULL){
		wave_t crop = wave_crop(w, start, length);
		printf("do you want create a new file ? 'y' or 'n'\n");
		scanf("%s", &c);
		empty_buffer();
		if(c=='y'){
			printf("choose the name\n");
			scanf("%s",name);
			bool save = wave_save(name, crop);
			if (save==false){
				printf("impossible to save the new file\n");		
			}
			printf("successed to save the new file\n");
			return;
		}
		else if(c=='n') {
			wave_delete(w); 
			w.p = crop.p;	
			bool save = wave_save(currentfile, w);
			if (save!=true){
				printf("impossible to save the file\n");		
			}
			printf("successed to save in the same file\n");
			return;
	
		} else { printf("Invalide choice\n"); }			
	}
	else {
		printf("Error cut\n");
		return;
	}

}

void resize(){
	double s;
	char c;
	printf("insert value to modify the sound\n");
	scanf("%lf",&s);
	char name[100];
	if ( w.p != NULL){
		wave_t redim = wave_scale(w, s);
		printf("do you want create a new file ? 'y' or 'n'\n");
		scanf("%s", &c);
		empty_buffer();
		if(c=='y'){
			printf("choose the name\n");
			scanf("%s",name);
			empty_buffer();
			bool save = wave_save(name, redim);
			if (save==false){
				printf("impossible to save the new file\n");		
			}
			printf("successed to save the new file\n");
			return;
		}
		else if(c=='n') {
			wave_delete(w); 
			w.p = redim.p;	
			bool save = wave_save(currentfile, w);
			if (save!=true){
				printf("impossible to save the file\n");		
			}
			printf("successed to save in the same file\n");
			return;
	
		} else { printf("Invalide choice\n"); }	
	}
	else {
		printf("Error resize\n");
		return;
	}
}

void canal(){
	uint32_t number_canal;
	printf("How many canals? \n");
	do{
		scanf("%d",&number_canal);
		empty_buffer();
	} while(number_canal < 1 || number_canal > 6);
	if ( w.p != NULL){
		wave_canal(w, number_canal);			
	}
	else {
		printf("Error canal\n");
		return;
	}
}

//TODO finir
void split(){

}

/*
void merge (){
	char c;
	int number_canal;
	printf("insert number of canal ?\n");
	scanf ("%d",&number_canal)
	char name[100];
	if ( w.p != NULL){
		wave_t merge = wave_merge((w*),number_canal);
		printf("do you want create a new file ? 'y' or 'n'\n");
		scanf("%s", &c);
		empty_buffer();
		if(c=='y'){
			printf("choose the name\n");
			scanf("%s",name);
			bool save = wave_save(name, merge);
			if (save==false){
				printf("impossible to save the new file\n");		
			}
			printf("successed to save the new file\n");
			return;
		}
		else if(c=='n') {
			wave_delete(w); 
			w.p = merge.p;	
			bool save = wave_save(currentfile, w);
			if (save!=true){
				printf("impossible to save the file\n");		
			}
			printf("successed to save in the same file\n");
			return;
	
		} else { printf("Invalide choice\n"); }			
	}
	else {
		printf("Error merge\n");
		return;
	}
}
*/

// son grave
void min_sons(){
	uint32_t i;
	printf( "insert a number of bloc\n");
	scanf("%d",&i);
	uint16_t j;
	printf( "insert a number of canal\n");
	scanf("%hd",&j);
	int64_t amplitude;
	 if (w.p != NULL){
		amplitude = wave_get(w,i,j);
		
		if (amplitude > 10000 ){
			printf( "entre une amplitude inferieur a 10000\n");
			scanf("%ld",&amplitude);
				if ( amplitude>20){
				wave_set(w,i,j,amplitude);
				printf( "la modification a eu lieu\n");	
				}
		}		
	}else {
		printf( " cest deja un son grave\n");
	}
}

void max_sons(){
	uint32_t i;
	printf( "insert a number of bloc\n");
	scanf("%d",&i);
	uint16_t j;
	printf( "insert a number of canal\n");
	scanf("%hd",&j);
	int64_t amplitude;
	 if (w.p != NULL){
		amplitude = wave_get(w,i,j);
		if (amplitude < 10000 ){
			printf( "entre une amplitude superieur a 10000\n");
			scanf("%ld",&amplitude);
			wave_set(w,i,j,amplitude);
			printf( "la modification a eu lieu\n");	
		}		
	}else {
		printf("cest deja un son aigue");
	}
}
int main (){
	q = false;

	menu_t* m;
	menu_t* sm;
	menu_t* ssm;

	m = createMenu("MENU");

	sm = createMenu("File");
	addSubMenu(m,sm);
	addMenuAction(sm,"open",open);
	ssm = createMenu("save");
	addSubMenu(sm,ssm);
	addMenuAction(ssm,"WAVE 8bits Mono 11.025kHz",save_mono);
	addMenuAction(ssm,"WAVE 16bits Stereo 44,1kHz",save_stereo);
	//addMenuAction(ssm,"WAVE 24bits 5.1 192kHz ",save3);
	addMenuAction(ssm,"WAVE customisé",save_customise);
	addMenuAction(sm,"information",info);
	addMenuAction(sm,"Quit",quit);

	sm = createMenu("length and Tempo");
	addSubMenu(m,sm);
	addMenuAction(sm,"reverse",reverse);
	addMenuAction(sm,"cut",cut);
	addMenuAction(sm,"resize",resize);

	sm = createMenu("canal");
	addSubMenu(m,sm);
	addMenuAction(sm,"canal",canal);
	addMenuAction(sm,"split",split);
	//addMenuAction(sm,"merge",merge);


	sm = createMenu("Height");
	addMenuAction(sm,"son grave",min_sons);
	addMenuAction(sm,"son aigue",max_sons);
	addSubMenu(m,sm);

	
	while (q==false){
		launchMenu(m);
	}

	return EXIT_SUCCESS;
}


