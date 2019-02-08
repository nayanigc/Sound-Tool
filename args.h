
#ifndef args_h
#define args_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h> 

#define NOOPTION ((option_t*)NULL)

typedef struct option_s {
	const char* keyword;
	enum { OptVoid, OptInt16_32_16, OptString, OptDouble, OptUint32_32, OptUint32, OptUint32_16 } spec;
	union {
		void (*opt_void)();
		void (*opt_int16_32_16)(int16_t, uint32_t, uint16_t);
		void (*opt_string)(const char*);
		void (*opt_double)(double);
		void (*opt_uint32_32)(uint32_t, uint32_t);
		void (*opt_uint32)(uint32_t);
	} fct;
	struct option_s* next;
} option_t;


//les fonctions à utiliser******************************************

option_t* opt_void(option_t* l, const char* kw, void (*f)());

option_t* opt_int16_32_16(option_t* l, const char* kw, void (*f)(int16_t, uint32_t, uint16_t)); 

option_t* opt_double(option_t* l, const char* kw, void (*f)(double));

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));

option_t* opt_uint32_32(option_t* l,const char* kw, void (*f)(uint32_t, uint32_t));

option_t* opt_uint32(option_t* l,const char* kw, void (*f)(uint32_t));

void opt_delete(option_t* l);

void process_arguments(option_t* L, int argc, char* *argv);

#endif
