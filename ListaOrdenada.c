#include "ListaOrdenada.h"
#include <malloc.h>
#include <memory.h>

/* lo_Crear
Pre: Lo no fue creada.
Post: Lo creada y vacia */
void lo_Crear(TListaOrdenada *pLo, int TamanioDato, int (*funcion)(void*, void*)){
	pLo->Corriente = NULL;
	pLo->Primero = NULL;
	pLo->TamanioDato = TamanioDato;
	pLo->fcompara = funcion;
}

/* lo_Vaciar
Pre: Lo creada.
Post: Lo vacia.*/
void lo_Vaciar(TListaOrdenada *pLo){
	TNodoOrdenada *pNodo, *siguiente;
	for(pNodo = pLo->Primero; (pNodo); pNodo = siguiente){
		siguiente = pNodo->Siguiente;
		free(pNodo->Elem);
		free(pNodo);
	}
	pLo->Primero = pLo->Corriente=NULL;
}

/* lo_Vacia
Pre: Lo creada.
Post: Si Lo tiene elementos devuelve FALSE sino TRUE.*/
int lo_Vacia(TListaOrdenada Lo){
	return (Lo.Primero == NULL);
}

/* lo_ElemCorriente
Pre: Lo creada y no vacia.
Post: Se devuelve en E el elemento corriente de la lista.*/
void lo_ElemCorriente(TListaOrdenada Lo, void *pE){
	memcpy(pE, Lo.Corriente->Elem, Lo.TamanioDato);
}

/* lo_ModifCorriente
Pre: Lo creada y no vacia.
Post: El contenido del elemento actual quedo actualizado con E. */
void lo_ModifCorriente(TListaOrdenada *pLo, void* pE){
	memcpy(pLo->Corriente->Elem, pE, pLo->TamanioDato);
}

/* lo_MoverCorriente
Pre: Lo creada y no vacia.
Post: Si Ls esta vacia, devuelve FALSE. Sino:
Si M = LO_PRIMERO, el nuevo elemento corriente es el primero. Devuelve TRUE
Si M = LO_SIGUIENTE, el nuevo elemento corriente es el siguiente al
anterior. Si estaba en el ultimo elemento, devuelve FALSE, sino TRUE.
Si M = LO_ANTERIOR, devuelve FALSE. */
int lo_MoverCorriente(TListaOrdenada *pLo, TMovimiento_Lo M){
	switch (M){
		case LO_PRIMERO:   
			pLo->Corriente = pLo->Primero;
			break;
		case LO_SIGUIENTE: 
			if (pLo->Corriente->Siguiente == NULL)
				return FALSE;
			else
				pLo->Corriente = pLo->Corriente->Siguiente;
			break;
		case LO_ANTERIOR:
			return FALSE;
	}
	return TRUE;
}

/* lo_BorrarCorriente
Pre: Lo creada y no vacia.
Post: Se elimino el elemento corriente, El nuevo elemento es el siguiente o
el anterior si el corriente era el ultimo elemento.*/
void lo_BorrarCorriente(TListaOrdenada *pLo){
	TNodoOrdenada *PNodo = pLo->Corriente;
	if (pLo->Corriente == pLo->Primero){
		pLo->Primero = pLo->Corriente->Siguiente;
		pLo->Corriente = pLo->Primero;
	}
	else{
		TNodoOrdenada *PAux = pLo->Primero;
		while (PAux->Siguiente != pLo->Corriente)
			PAux = PAux->Siguiente;
		PAux->Siguiente = pLo->Corriente->Siguiente;
		if (PAux->Siguiente) //Si no es el ultimo
			pLo->Corriente = PAux->Siguiente;
		else
			pLo->Corriente = PAux; //Si es el ultimo queda en el anterior al borrado
		}
	free(PNodo->Elem);
	free(PNodo);
}

/* lo_Insertar
Pre: Lo creada.
Post: E se agrego a la lista y es el actual elemento corriente.
Si M=LO_PRIMERO: se inserto como primero de la lista.
Si M=LO_SIGUIENTE: se inserto después del elemento corriente.
Si M=LO_ANTERIOR: se inserto antes del elemento corriente.
Si pudo insertar el elemento devuelve TRUE, sino FALSE.*/
int lo_Insertar(TListaOrdenada *pLo, void* pElem){
	TNodoOrdenada *pNodo = (TNodoOrdenada*)malloc(sizeof(TNodoOrdenada));
	TNodoOrdenada *pNodoAnterior;
	if (!pNodo){
		return FALSE; //No hay memoria disponible 
	}
	else{
		int insertado = 0;
		if (pLo->Primero == NULL){
			insertado = 1;
			pNodo->Siguiente = NULL;
			pLo->Primero = pNodo;	
		}
		else{
			if (pLo->Primero->Siguiente == NULL){
				if (pLo->fcompara(pElem, pLo->Primero->Elem) <= 0){
					insertado = 1;
					pNodo->Siguiente = pLo->Primero;
					pLo->Primero = pNodo;
				}
				else{
					insertado = 1;
					pNodo->Siguiente = NULL;
					pLo->Primero->Siguiente = pNodo;
				}
			}
			else{
				pNodoAnterior = pLo->Primero;
				pLo->Corriente = pLo->Primero->Siguiente;
				while ((pLo->Corriente != NULL)&&(!insertado)){
					if (pLo->fcompara(pElem, pNodoAnterior->Elem) <= 0){
						pLo->Primero = pNodo;
						pNodo->Siguiente = pNodoAnterior;
						insertado = 1;
					}
					else{
						if (pLo->fcompara(pElem, pLo->Corriente->Elem) <= 0){
							pNodoAnterior->Siguiente = pNodo;
							pNodo->Siguiente = pLo->Corriente;
							insertado = 1;
						}
						else{
							if (pLo->Corriente->Siguiente == NULL){
								pLo->Corriente->Siguiente = pNodo;
								pNodo->Siguiente = NULL;
								insertado = 1;
							}
						}
					}
					pNodoAnterior = pLo->Corriente;
					pLo->Corriente = pLo->Corriente->Siguiente;
				}
			}
		}
		pNodo->Elem = malloc(pLo->TamanioDato);
		memcpy(pNodo->Elem, pElem, pLo->TamanioDato);
		pLo->Corriente = pNodo;
		return TRUE;
	}
}

int lo_Buscar (TListaOrdenada* pLo, void* pElem){
	TNodoOrdenada* pNodo;

	if (pLo->Primero==NULL)
		return 0;
	else{
		pNodo = pLo->Primero;
		while ((pNodo) && (pLo->fcompara(pNodo->Elem, pElem) < 0))  
			/* sigue siendo menor que el que busco*/
			pNodo = pNodo->Siguiente;
		if (pLo->fcompara(pNodo->Elem, pElem) == 0){
			pLo->Corriente = pNodo;
			return 1;
		}
		else
			return 0;
	}
}
