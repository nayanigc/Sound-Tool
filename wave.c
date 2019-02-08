#include "wave.h"


// fonction créant en mémoire un WAVE

wave_t wave_new (uint32_t f, uint16_t p, uint16_t c, uint32_t B){
	if ( (p==8 || p==16 || p==24) && c<=6) {	
		wave_t w;
		w.p = malloc(sizeof(pointeurWave));
		w.p->head = malloc(sizeof(header));
		strcpy(w.p->head->riff,"RIFF");
		strcpy(w.p->head->wave,"WAVE");
		strcpy(w.p->head->fmt,"fmt ");
		strcpy(w.p->head->data,"data");
		w.p->head->size_file = ((B * c * (p / 8)) + 44 - 8);
		w.p->head->number16 = 16;
		w.p->head->PCM = 1;
		w.p->head->number_canal = c;
		w.p->head->frequency = f;
		w.p->head->octet_bloc = c * (p / 8);	
		w.p->head->octet_second = (f * w.p->head->octet_bloc);
		w.p->head->precision = p;
		w.p->head->size_data = B * (w.p->head->octet_bloc);
		w.p->number_Bloc = B;
		w.p->blocs = malloc(sizeof(Tab_bloc) * B);
		uint32_t i;
		if (w.p->blocs == NULL){
			fprintf(stderr, "\n allocation failed(bloc)\n");
		} 
		for (i=0; i< (w.p->number_Bloc); i++){
			w.p->blocs[i].canal = malloc(sizeof(int32_t)*(c));
			if (w.p->blocs[i].canal == NULL){
				printf("%d\n",i);
				fprintf(stderr, "\n allocation failed(bloc)\n");
			}
		}
		return w;
	}
	fprintf(stderr, "\nimpossible to create wav structure with these data \n");
	wave_t error;
	error.p = NULL;
	return error;	
}

// fonction libérant la place mémoire occupée par le wave

void wave_delete(wave_t w){
	uint32_t i;
	for (i=0; i< (w.p->number_Bloc); i++){
		free(w.p->blocs[i].canal);
	}
	free(w.p->blocs);
	free(w.p->head);
	free(w.p);
}

// fonction qui renvoie l'amplitude de l'echantillon/ bloc i sur le canal j de wave w

int32_t wave_get ( wave_t w, uint32_t i, uint16_t j){
	if (i>(w.p->number_Bloc) || j>(w.p->head->number_canal)) {
		fprintf(stderr, "\nimpossible to find\n");
		return 0;
	}
	int32_t amplitude;
	amplitude = w.p->blocs[i-1].canal[j-1];
	return amplitude;
}

// fonction affectant la nouvelle amplitude a à l'echantillon\bloc i sur le canal j du wave w

void wave_set (wave_t w, uint32_t i , uint16_t j, int32_t a){			
	if ( i>(w.p->number_Bloc) || j>(w.p->head->number_canal) ) {
		fprintf(stderr, "\nImpossible to inser\n");
		return;
	}	
	w.p->blocs[i-1].canal[j-1] = a;

}

// fonction qui arrondi une valeur donnée 

double round (double value){
	return floor(value+0.5);
}

// ajoutant au wave w le signal s, s une fonction R->[-1;1] associant à chaque seconde une amplitude normalisée dans [-1;1]
// on ne peut utiliser cette fonction que si w a était allouée  avec la fonction wave_new. Elle ne remplie que le premier canal.

void add_signal ( wave_t w, double (*s) (double)){
	double period = 1/((double)w.p->head->frequency);
	uint32_t i;
	double cursor =0;
	double value =0.0;
	for(i=1; i<=w.p->number_Bloc;i++){
		value = s(cursor);
		double res = (value>=0)?value*(pow(2,w.p->head->precision-1)-1.0):value*pow(2,w.p->head->precision-1);
		int32_t res_i = round(res);
		wave_set(w, i, 0, res_i);
		cursor+=period;
	}
}


// fonction qui charge un wave à partir d'un fichier, renvera NULL en cas d'échec

wave_t wave_load ( const char * fname){
	wave_t w;
	FILE* fdesc = fopen (fname,"rb");
	if (fdesc == NULL){
		fprintf(stderr, "Impossible to open the file");
		exit(0);
	}

	w.p = malloc(sizeof(pointeurWave));
	w.p->head = malloc(sizeof(header));
	fread(&w.p->head->riff, 4, sizeof(char), fdesc); 
	fread(&w.p->head->size_file, sizeof(uint32_t), 1, fdesc);
    	fread(&w.p->head->wave, 4, sizeof(char), fdesc); 
    	fread(&w.p->head->fmt, 4, 1, fdesc); 
    	fread(&w.p->head->number16, sizeof(uint32_t), 1, fdesc); 
    	fread(&w.p->head->PCM, sizeof(uint16_t), 1, fdesc); 
    	fread(&w.p->head->number_canal, sizeof(uint16_t), 1, fdesc); 
    	fread(&w.p->head->frequency, sizeof(uint32_t), 1, fdesc); 
	fread(&w.p->head->octet_second, sizeof(uint32_t), 1, fdesc);
    	fread(&w.p->head->octet_bloc, sizeof(uint16_t), 1, fdesc);
    	fread(&w.p->head->precision, sizeof(int16_t), 1, fdesc);
    	fread(&w.p->head->data, 4, sizeof(char), fdesc);	
    	fread(&w.p->head->size_data, sizeof(int32_t), 1, fdesc);
	
	if(strncmp(w.p->head->riff,"RIFF", 4) != 0){
		printf("RIFF error");
		exit(0);
	}    	
	if(strncmp(w.p->head->wave,"WAVE", 4) != 0){
		printf("WAVE error");
		exit(0);
	}
	if(strncmp(w.p->head->fmt,"fmt ", 4) != 0){
		printf("fmt  error");
		exit(0);
	}
	if((w.p->head->number16) != 16){
		printf("16 error");
		exit(0);
	}
	if((w.p->head->PCM) != 1){
		printf("PCM error");
		exit(0);
	}
	if((w.p->head->number_canal)>6){
		printf("number_CANAUX error");
		exit(0);
	}
	if((w.p->head->frequency) <2000 || (w.p->head->frequency)>1000000){
		printf("Frequence error");
		exit(0);
	}
	if(strncmp(w.p->head->data,"data", 4) != 0){
		printf("Chaine data error");		
		exit(0);
	}
	uint32_t numbre_Blocs = (w.p->head->size_data / w.p->head->octet_bloc);
	w.p->number_Bloc = numbre_Blocs;

	w = wave_new(w.p->head->frequency, w.p->head->precision, w.p->head->number_canal,numbre_Blocs);
	uint32_t i;
	uint16_t j;
	int32_t amplitude = 0;
	

	switch( w.p->head->precision){
		case 8:		for (i = 1; i <= numbre_Blocs; i++){
					for (j = 1; j <= w.p->head->number_canal; j++){
						amplitude = 0;
						fread(&amplitude, 1, 1, fdesc);
						wave_set(w, i, j, amplitude);
					}
				}
		break;
		case 16: 		for (i = 1; i <= numbre_Blocs; i++){
					for (j = 1; j <= w.p->head->number_canal; j++){
						amplitude = 0;
						fread(&amplitude, 2, 1, fdesc);
						wave_set(w, i, j, amplitude);
					}
				}
		break;
		case 24:		for (i = 1; i <= numbre_Blocs; i++){
					for (j = 1; j <= w.p->head->number_canal; j++){
						amplitude = 0;
						fread(&amplitude, 3, 1, fdesc);
						wave_set(w, i, j, amplitude);
					}
				} 
		break;
	}

	printf("successed!\n");
	fclose (fdesc);
	return w;
}

// fonction qui sauvegarde un wave dans un fname, renvera si loperation  a réussi ou non

bool wave_save(const char* fname, wave_t w){

	uint32_t size = w.p->head->size_file;
	uint32_t number16 = w.p->head->number16;
	uint16_t PCM = w.p->head->PCM;
	uint16_t number_canal = w.p->head->number_canal;
	uint32_t frequency = w.p->head->frequency;
	uint32_t octet_second = w.p->head->octet_second;
	uint16_t octet_bloc = w.p->head->octet_bloc;
	int16_t precision = w.p->head->precision;
	int32_t size_data = w.p->head->size_data;

	FILE* fdesc = fopen(fname,"wb");
	if(fdesc == NULL){
		fprintf(stderr, "Impossible to save the file");
		return false;	
	}	
	
	fwrite("RIFF", 4, sizeof(char), fdesc); 
	fwrite(&size, sizeof(uint32_t), 1, fdesc);
	fwrite("WAVE", 4, sizeof(char), fdesc); 
	fwrite("fmt ", 4, sizeof(char), fdesc); 
	fwrite(&number16, sizeof(uint32_t), 1, fdesc); 
	fwrite(&PCM, sizeof(uint16_t), 1, fdesc); 
	fwrite(&number_canal, sizeof(uint16_t), 1, fdesc); 
	fwrite(&frequency, sizeof(uint32_t), 1, fdesc); 
	fwrite(&octet_second, sizeof(uint32_t), 1, fdesc); 
	fwrite(&octet_bloc, sizeof(uint16_t), 1, fdesc); 
	fwrite(&precision, sizeof(int16_t), 1, fdesc);
	fwrite("data", 4, sizeof(char), fdesc); 
	fwrite(&size_data, sizeof(int32_t), 1, fdesc);

	uint32_t i;
	uint16_t j;
	switch( w.p->head->precision){

		case 8:
				
				for (i = 1; i <= w.p->number_Bloc; i++){
					for (j = 1; j <= number_canal; j++){
						int32_t amplitude = wave_get(w, i, j);	
						fwrite(&amplitude, 1, 1, fdesc);	
					}
				}
		break;
		case 16:	
				for (i = 1; i <= w.p->number_Bloc; i++){
					for (j = 1; j <= number_canal; j++){
						int32_t amplitude = wave_get(w, i, j);	
						fwrite(&amplitude, 2, 1, fdesc);	
					}
				}
		break;
		case 24:		
				for (i = 1; i <= w.p->number_Bloc; i++){
					for (j = 1; j <= number_canal; j++){
						int32_t amplitude = wave_get(w, i, j);	
						fwrite(&amplitude, 3, 1, fdesc);	
					}
				}
		break;
	}

	fclose(fdesc);
	return true;
}

//porte à c le nombre de canaux dans le WAVE w ; si c<nbr de canaux de w=(canaux sommés) ; si c>(dupliqués)

void wave_canal(wave_t w, uint16_t c){
	
	if ( c>6 ){
		fprintf(stderr, "\n ERROR! number of canal c is false \n");
		return;
	}
	else if (c==(w.p->head->number_canal)){
		return;
	}
	wave_t temp = wave_new (w.p->head->frequency, w.p->head->precision, c, w.p->number_Bloc );
	//fusion
	if( c<(w.p->head->number_canal) ){
		uint32_t i;
		uint16_t j;
		for(i=1; i<=(w.p->number_Bloc); i++){
			for(j=1; j<=c; j++){
				wave_set(temp, i, j, wave_get(w, i, j) );
			}
		}
		int a ;
		for (a=c; a<(w.p->head->number_canal); a++){
			for(i=1; i<=(w.p->number_Bloc); i++){
				for(j=1; j<=c; j++){
					int32_t x = wave_get(temp, i, j);
					int32_t y = wave_get(w, i, a+1);
					wave_set(temp, i, j, x+y);
				}	
			}	
		}
	}
	
	//duplication
	if ( c>(w.p->head->number_canal) ){
		uint32_t i;
		uint16_t j;
		for(i=1; i<=(w.p->number_Bloc); i++){
			for(j=1; j<=(w.p->head->number_canal); j++){
				wave_set(temp, i, j, wave_get(w, i, j));
			}	
		}
		for(i=1;i<=(w.p->number_Bloc); i++){
			int z = 1;
			for(j=(w.p->head->number_canal)+1; j<=c; j++){
				wave_set(temp, i, j, wave_get(w, i, z));
				if(z < w.p->head->number_canal) z++;
				else z = 1;			
			}	
		}		
	}
	*w.p = *temp.p;
}


// fonction qui inverse le sens de lecture 

void wave_reverse(wave_t w){
	wave_t temp = wave_new (w.p->head->frequency, w.p->head->precision, w.p->head->number_canal, w.p->number_Bloc );
	uint32_t i;
	uint16_t j;
	uint32_t l = (w.p->number_Bloc);
	for (i=1; i<=(w.p->number_Bloc) ; i++){
		uint16_t k = (w.p->head->number_canal);
		for (j=1; j<=(w.p->head->number_canal) ; j++){
			wave_set(temp, l, k, wave_get(w, i, j) );
			k--;
		}
		l--;
	}
	*w.p = *temp.p;
}




// fonction qui sépare le WAVE w en plusieurs WAVE mono, c le nbr de canaux de w

wave_t* wave_split(wave_t w, int* pc){
	if (pc!=NULL){
		*pc = (w.p->head->number_canal);
	}
	wave_t* wsplit = malloc(sizeof(wave_t)*(w.p->head->number_canal));
	uint32_t i;
	for (i=1; i<=(w.p->head->number_canal); i++){
		wsplit[i] = wave_new (w.p->head->frequency, w.p->head->precision, 1, w.p->number_Bloc );
		uint16_t j;
		for (j=1; j<=(w.p->number_Bloc); j++){
			wave_set(wsplit[i-1], j, 0, wave_get(w, j, i) );
		}
	}
	return wsplit;
}

//fusionne au sein d'un unique WAVE les c WAVE mono passés en paramètres

wave_t wave_merge(wave_t* w, int c){
	wave_t merge =  wave_new (w[0].p->head->frequency, w[0].p->head->precision, c, w[0].p->number_Bloc );
	uint32_t i;
	uint16_t j;
	uint32_t a = (uint32_t) c;
	for(i=1; i<=a; i++){
		for(j=1; j<=(w[0].p->number_Bloc); j++){
			wave_set(merge, j, i, wave_get(w[i-1], j, 0));
		}
	}
	return merge;
}



// crée un nouveau WAVE à partir de w contenant les length blocs pris à partir de la position start ; la fonction retournera NULL en cas d'erreur

wave_t wave_crop(wave_t w, uint32_t start, uint32_t length){
	if ( (start + length)>(w.p->number_Bloc) ){
		fprintf(stderr, "\nimpossible to create wave_crop\n");
		wave_t error;
		error.p = NULL;
		return error;
	}
	wave_t crop = wave_new(w.p->head->frequency, w.p->head->precision, w.p->head->number_canal, length);
	uint32_t i;
	uint16_t j;
	uint32_t a = start+1;
	for(i=1; i<=length; i++){
		for(j=1; j<=(w.p->head->number_canal); j++){
			wave_set(crop, i, j, wave_get(w, a, j) );
		}
	a++;
	}
	return crop;
}



// crée un nouveau WAVE à partir de w ayant les mêmes caractéristiques, mais jouant le même morceau avec un tempo modifié d'un facteur s

wave_t wave_scale ( wave_t w, double s){
	wave_t temp;
	wave_t error;
	error.p = NULL;
	uint32_t i;
	uint16_t j;
	int k;
	if (s<=0 || s==1){
		return error;
	}
	//fusion // free pour libre lespace et calcul de moyenne dans les canaux pendant la fusion 
	if ( s >=1){
		int a = (int)s;
		if (a==1){
			return error;
		}
		temp =  wave_new ( w.p->head->frequency, w.p->head->precision, w.p->head->number_canal,(w.p->number_Bloc)/a);
		temp.p->number_Bloc=( w.p->number_Bloc/a);
		for(i=1; i<=(w.p->number_Bloc/a); i++){
			for(j=1; j<=(w.p->head->number_canal); j++){	
				int somme =0;
				for ( k = 0; k<a;k++){
					if (i+k <(w.p->number_Bloc)){
						somme+= wave_get(w, i+k, j);
					}
				}
				wave_set(temp, i, j, somme/a);
			}
		}
	}
	// duplication realloue de lespace copie integrale 
	else if ( s <= 1){
		int b = 1/s;
		if ( b ==1){
			return error;
		}
		temp = wave_new ( w.p->head->frequency, w.p->head->precision, w.p->head->number_canal,(w.p->number_Bloc)*b);
		temp.p->number_Bloc=( w.p->number_Bloc*b);
		for(i=0; i<(w.p->number_Bloc); i++){
	 		for(j=0; j<(w.p->head->number_canal); j++){
				for(k=0;k<b;k++){
					wave_set(temp, (i*b)+k+1, j+1, wave_get(w, i+1, j+1) );
				}				
			}
		}	
	}	
	return temp;
}


// convertion en 8 bits

void convert_To8 (wave_t w){
	uint32_t i;
	uint16_t j;
	int32_t amplitude = 0;
	w.p->head->precision = 8;
	switch( w.p->head->precision){
	case 8: ; break;
	case 16: for(i = 1; i <=  w.p->number_Bloc; i++){
			for(j = 1; j <= w.p->head->number_canal; j++){
				amplitude = wave_get(w, i, j);
				amplitude /= 256;
		 		if (amplitude > 127){
					amplitude = 127;
				}
				amplitude += 127;
				wave_set(w, i, j, amplitude);
			}	
		 }
	break;
	case 24: for(i = 1; i <=  w.p->number_Bloc; i++){
			for(j = 1; j <= w.p->head->number_canal; j++){
				amplitude = wave_get(w, i, j);		
				amplitude /= (256 * pow(2, 8));
		 		if (amplitude > 127){
					amplitude = 127;
				}
				amplitude += 127;
				wave_set(w, i, j, amplitude);
			}	
		 }
	break;
	default :fprintf(stderr ,"Impossible to convert 8 bits");
	}

}

//convertion en 16 bits

void convert_To16 (wave_t w){
	uint32_t i;
	uint16_t j;
	int32_t amplitude = 0;
	w.p->head->precision = 16;
	switch( w.p->head->precision){
	case 8: for (i = 1; i <= w.p->number_Bloc; i++){
			for (j = 1; j <= w.p->head->number_canal; j++){
				amplitude = wave_get(w, i, j);		 		
				if (amplitude < 0){
					amplitude += 256;
				}
				amplitude -= 127;
				amplitude *= 256;
				wave_set(w, i, j, amplitude);
			}
		}
	break;
	case 16: ; break;
	case 24: for (i = 1; i <=  w.p->number_Bloc; i++){
		 	for (j = 1; j <= w.p->head->number_canal; j++){
		 		amplitude = wave_get(w, i, j);
		 		amplitude /= 256;
				wave_set(w, i, j, amplitude);
		 	}	
		 }
	break;
	default :fprintf(stderr ,"Impossible to convert 16 bits");
	}

}


//convertion en 24 bits

void convert_To24 (wave_t w){
	uint32_t i;
	uint16_t j;
	int32_t amplitude = 0;
	w.p->head->precision = 24;
	switch( w.p->head->precision){
	case 8: for(i = 1; i <= w.p->number_Bloc; i++){
			for(j = 1; j <= w.p->head->number_canal; j++){
				amplitude = wave_get(w, i, j);
		 		if (amplitude < 0){
					amplitude += 256;
				}
				amplitude -= 127;
				amplitude *= (256 * pow(2,8));
				wave_set(w, i, j, amplitude);
			}	
		}
	break;
	case 16: for (i = 1; i <= w.p->number_Bloc; i++){
			for (j = 1; j <= w.p->head->number_canal; j++){
				amplitude = wave_get(w, i, j);		 		
				amplitude *= 256;
				wave_set(w, i, j, amplitude);
			}
		 }
	break;
	case 24: ; break;
	default :fprintf(stderr ,"Impossible to convert 16 bits");
	}
}

// modification de la fréquence 

void modify_frequency(wave_t w, uint32_t f){
	w.p->head->frequency = f;
	w.p->head->octet_second = f * w.p->head->octet_bloc;
}

// modification du volume 

void modify_volume(wave_t w, double factor){
	double amp_double = 0.0;
	uint32_t i;
	uint16_t j;		
	for(i=1; i<=(w.p->number_Bloc); i++){
		for(j=1; j<=(w.p->head->number_canal); j++){		
			amp_double = (double) wave_get(w,i,j);
			amp_double *= factor;
			wave_set(w, i, j,  (int32_t) amp_double);
		}
	}
}

