#ifndef __ListaOrdenada_h__
	#define __ListaOrdenada_h__
	#if !defined(NULL)
		#define NULL 0
	#endif
	#if !defined(FALSE)
		#define FALSE 0
	#endif
	#if !defined(TRUE)
		#define TRUE 1
	#endif

	typedef enum{
		LO_PRIMERO,
		LO_SIGUIENTE,
		LO_ANTERIOR
	}TMovimiento_Lo;

	typedef struct TNodoOrdenada{
		void* Elem;
		struct TNodoOrdenada *Siguiente;
	}TNodoOrdenada;

	typedef struct{
		TNodoOrdenada *Primero, *Corriente;
		int TamanioDato;
		int (*fcompara)(void*, void*);
	}TListaOrdenada;

	/* lo_Crear
	Pre: Lo no fue creada.
	Post: Lo creada y vacia */
	void lo_Crear(TListaOrdenada *pLo, int TamanioDato, int (*funcion)(void*, void*));

	/* lo_Vaciar
	Pre: Lo creada.
	Post: Lo vacia.*/
	void lo_Vaciar(TListaOrdenada *pLo);

	/* lo_Vacia
	Pre: Lo creada.
	Post: Si Ls tiene elementos devuelve FALSE sino TRUE.*/
	int lo_Vacia(TListaOrdenada Lo);

	/* lo_ElemCorriente
	Pre: Lo creada y no vacia.
	Post: Se devuelve en E el elemento corriente de la lista.*/
	void lo_ElemCorriente(TListaOrdenada Lo, void* pE);

	/* lo_ModifCorriente
	Pre: Lo creada y no vacia.
	Post: El contenido del elemento actual quedo actualizado con E. */
	void lo_ModifCorriente(TListaOrdenada *pLo, void* pE);

	/* lo_MoverCorriente
	Pre: Lo creada y no vacia.
	Post: Si Ls esta vacia, devuelve FALSE. Sino:
	Si M = LO_PRIMERO, el nuevo elemento corriente es el primero. Devuelve TRUE
	Si M = LO_SIGUIENTE, el nuevo elemento corriente es el siguiente al
	anterior. Si estaba en el ultimo elemento, devuelve FALSE, sino TRUE.
	Si M = LO_ANTERIOR, devuelve FALSE. */
	int lo_MoverCorriente(TListaOrdenada *pLo, TMovimiento_Lo M);

	/* lo_BorrarCorriente
	Pre: Lo creada y no vacia.
	Post: Se elimino el elemento corriente, El nuevo elemento es el siguiente o
	el anterior si el corriente era el ultimo elemento.*/
	void lo_BorrarCorriente(TListaOrdenada *pLo);

	/* lo_Insertar
	Pre: Lo creada.
	Post: E se agrego a la lista y es el actual elemento corriente.
	Si M=LO_PRIMERO: se inserto como primero de la lista.
	Si M=LO_SIGUIENTE: se inserto despues del elemento corriente.
	Si M=LO_ANTERIOR: se inserto antes del elemento corriente.
	Si pudo insertar el elemento devuelve TRUE, sino FALSE.*/
	int lo_Insertar(TListaOrdenada *pLo, void* E);
	
	int lo_Buscar (TListaOrdenada *pLo, void* pE);

#endif

