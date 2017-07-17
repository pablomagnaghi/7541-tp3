#ifndef FUNCIONESAUX_H_
#define FUNCIONESAUX_H_

#include <stdio.h>
#include <string.h>
#include "TDT.h"
#include "Instrucciones.h"

/* Funcion validadora de los parametros recibidos desde consola*/
int parametrosInvalidos(char *opcion,char *comando,char *parametro1, char *parametro2);

/* Funcion que informa por pantalla errores ocurridos al ingresar los parametros.*/
void informarError(int error);

/* Funcion que ejecuta la instruccion ingresada por consola.*/
void ejecutarInstruccion(TDT *tdt,FILE *config,char *comando,
						 char *parametro1,char *parametro2);

#endif /*FUNCIONESAUX_H_*/
