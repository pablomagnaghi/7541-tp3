#ifndef TABO_H_
#define TABO_H_

#include <stdlib.h>

#define IZQ 1	
#define DER 2	
#define PAD 3
#define RAIZ 4

typedef struct TNodoABO{
	void* elem;
	struct TNodoABO *izq, *der;
	}TNodoABO;

typedef struct TAB{
		TNodoABO *raiz,*cte;
		int tamdato;
		int (*fcomp)(void *,void *);
	}TABO;

void ABO_Crear(TABO *a, int tdato, int (*comp)(void *,void *));

int buscar_lugar(TABO *a, TNodoABO **p, void *elem);

void ABO_Insertar(TABO *a, void *elem, int *error);

void ABO_ElemCte(TABO a, void *elem);

void ABO_ModifCte(TABO *a, void *elem);

void ABO_MoverCte(TABO *a, int mov, int *error);

void ABO_Vaciar(TABO *a);

int ABO_Vacio(TABO a);
#endif /*TABO_H_*/
