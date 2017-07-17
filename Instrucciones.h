#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#include "TDT.h"
#include "ListaOrdenada.h"

void INS_Usr(TDT *tdt,char *usuario);

void INS_Overview(TDT *tdt);

void INS_Unknown(TDT *tdt, char *orden);

void INS_Begin(TDT *tdt,char *secuencia,char *orden);

void INS_Num(TDT *tdt,char *numero);

int compararNodoDetallePorFecha(void* nodo1, void* nodo2);

int compararNodoDetallePorNumero(void* nodo1, void* nodo2);

#endif /*INSTRUCCIONES_H_*/
