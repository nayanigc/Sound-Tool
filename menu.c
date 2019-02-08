#include "menu.h"




/*     *****     *****     *****     ***** FONCTION MENU *****     *****     *****     *****    */

// fonction qui vide le buffer
void empty_buffer (){
	char empty;
	while( (empty=getchar())!='\n'&& empty!=EOF);
}

//Création d'un menu

menu_t* createMenu(const char* text){ 
	menu_t* m = (menu_t*) malloc(sizeof(menu_t) ) ;
	(*m).parent = NULL;    //pas de menu parent
	(*m).compteur = 0;
	(*m).nomMenu = (char*) malloc (sizeof(char) * (strlen(text)+1) );
	strcpy ( (*m).nomMenu, text);
	return m;
}


// fonction qui ajoute au menu m un item de type action, descriptif:text = copie, fonctionnalité:f

void addMenuAction(menu_t *m, const char* text, void ( *f)() ){
	if ( (*m).compteur == 9){
		printf("il n'y a plus de place\n");
	} else {
		(*m).tabItem[(*m).compteur].choix.actionItem.nomAction = (char*) malloc (sizeof(char) * (strlen(text)+1) );
		(*m).tabItem[(*m).compteur].type = Action;
		strcpy( (*m).tabItem[(*m).compteur].choix.actionItem.nomAction, text);
		(*m).tabItem[(*m).compteur].choix.actionItem.action = f;
		(*m).compteur =(*m).compteur + 1;
	}
}

// fonction qui ajoute au menu m un item de type menu (sous-menu sm) ; (parent = NULL)

void addSubMenu(menu_t* m, menu_t* sm){
	if ( (*m).compteur == 9){
		printf("il n'y a plus de place\n");
	} else {
		if (sm->parent == NULL){
			(*m).tabItem[(*m).compteur].type = sousMenu;
			(*m).tabItem[(*m).compteur].choix.sousMenuItem = sm;
			sm->parent = m;
			(*m).compteur =(*m).compteur + 1;
		} else {
			printf("Ce sous menu a déjà un menu \n");
		}
	}
}

// fonction qui libère la mémoire occupée par le menu m (et tous ses sous-menus, ses sous-sous-menus,...)

void deleteMenu(menu_t* m){
	int i;
	if ( m->parent == NULL){
		for (i=0; i<(*m).compteur; i++){
			if( (*m).tabItem[i].type == sousMenu){
				(*m).tabItem[i].choix.sousMenuItem->parent = NULL;
				deleteMenu( (*m).tabItem[i].choix.sousMenuItem );
			} 
			else { free( (*m).tabItem[i].choix.actionItem.nomAction); }		
		}
		free (m->nomMenu);
		free (m);
	}
}


//fonction qui lance le menu m 

void launchMenu(menu_t* m) {
		printf("\n   \n");
		printf("\n   \n");

		printf("%s \n",m->nomMenu);
		printf("\n   \n");
		int i;
		for(i=0; i < ( m->compteur ); i++){
			if(m->tabItem[i].type == Action){
				printf("%d  -  %s\n",i , m->tabItem[i].choix.actionItem.nomAction );
				printf("\n   \n");
			}
			if(m->tabItem[i].type == sousMenu){
				printf("%d  -  %s\n",i,  m->tabItem[i].choix.sousMenuItem->nomMenu );
				printf("\n   \n");
			}
		}
		printf("p  -  Return from back \n");

		printf("\n   \n");
		printf("\n*****Make choice*****\n");
		char c = getchar();
		empty_buffer();
		int Choix;
		// si le choix se trouve entre 1 et 9
		if(c>=48 && c<=57){ 
			Choix = c-48;
			if( Choix >= (m->compteur) ){
				printf("\n   \n");
	   			printf(" -> 'Invalide choice' \n");
				printf("\n   \n");
	  		}
			else if (m->tabItem[Choix].type == Action) {            //si c'est une action 
		   		(*m).tabItem[Choix].choix.actionItem.action(); //je lance l'action
		  	}
			else if(m->tabItem[Choix].type == sousMenu){                 //si c'est un sous-menu
		    		launchMenu((*m).tabItem[Choix].choix.sousMenuItem);// je relance le launchMenu avec le sous-menu
			}
		}
		
		//si le choix est 'p'
		else if (c =='p'){ 
			if(m->parent!=NULL){
				launchMenu(m->parent); //je relance le launchMenu avec le parent
			}
			printf("\n   \n");
			printf("-> 'no parent' \n");
		}
	
		//si le choix est un caractère non valide
		else {
			printf(" -> error, insert valid choice\n");		
			printf("\n   \n");
		}
	
}



