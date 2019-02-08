
#ifndef menu_h 
#define menu_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>




//action
typedef struct {
	char* nomAction;
	void(*action)();
} action_t;

//item
typedef union {
	struct menu* sousMenuItem;
	action_t actionItem;
} choixMenu;

typedef enum {
	sousMenu = 0,
	Action = 1,
} typeMenu; 

typedef struct {
	choixMenu choix;
	typeMenu type;   
} item_t;

//MENU***********************
typedef struct menu {
	struct menu* parent;
	char* nomMenu;
	item_t tabItem[9];
	int compteur;
} menu_t;

                                                                                
//les fonctions à utiliser*************************************

menu_t* createMenu(const char* text);

void addMenuAction(menu_t *m, const char* text, void ( *f)() );

void addSubMenu(menu_t* m, menu_t *sm);

void deleteMenu(menu_t *m);

void launchMenu(menu_t *m);

void empty_buffer ();

void quit();


#endif
