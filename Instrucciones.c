#include "Instrucciones.h"

void INS_Usr(TDT *tdt,char *usuario)
{
	/* Variable local donde se guarda el total de importe para el usuario.*/	
	double total = 0;

	TListaSimple listaDetallesUsuario;
	TNodoDetalle detalle;

	/* Se crea una lista de detalle y luego se carga.*/
	ls_Crear(&listaDetallesUsuario, sizeof(TNodoDetalle));
	TDT_ObtenerDetalleUsuario(*tdt, usuario, &listaDetallesUsuario);
	
	if (!ls_Vacia(listaDetallesUsuario)){
		printf("\nMuestro el detalle de llamadas para el usuario %s...\n", usuario);
		printf("\nFECHA		TELEFONO	MONTO\n\n");
		
		/* Mientras haya elementos se imprime por pantalla el detalle recuperado.*/
		int mov = ls_MoverCorriente(&listaDetallesUsuario, LS_PRIMERO);
		while (mov)
		{
			ls_ElemCorriente(listaDetallesUsuario, &detalle);
			//acumulo el total.
			total = total + detalle.importe;
			//imprimo el detalle de llamada realizada.
			imprimirDetalle(&detalle);
			//me muevo al siguiente.
			mov = ls_MoverCorriente(&listaDetallesUsuario, LS_SIGUIENTE);
		}
		printf("\n**TOTAL**			%4.2f\n",total);
	}
	else{
		fprintf(stderr, "Error: El usuario especificado no tiene llamadas asignadas");
	}
	ls_Vaciar(&listaDetallesUsuario);
}

void INS_Overview(TDT *tdt)
{
	TListaSimple listaUsuarios, listaNoAsignadas;
	char resumenUsuario[100];
	double totalUsuarios,totalNoAsignadas,totalGenerales,acumulado;
	TNodoDetalle detalle;
	TGastosGenerales gastos;
	
	ls_Crear(&listaUsuarios,sizeof(char[100]));
	ls_Crear(&listaNoAsignadas,sizeof(TNodoDetalle));
	
	totalUsuarios = 0;
	totalNoAsignadas = 0;
	totalGenerales = 0;
	acumulado = 0;
	
/* Se recuperan el resumen para los usuarios.*/	
	TDT_GenerarResumenUsuarios(*tdt,&listaUsuarios,&totalUsuarios);
/* Se recupera el detalle para las llamadas no asignadas.*/	
	TDT_ObtenerDetalleNoAsignada(*tdt,&listaNoAsignadas);
/* Se recupera la sumatoria de los gastos generales.*/	
	TDT_ObetenerDeudaGastosGenerales(*tdt,&gastos,&totalGenerales);
	
	
/* Si se recuperaron usuarios se los muestra por pantalla.*/	
	if (!ls_Vacia(listaUsuarios))
	{
	printf("Imprimo el contenido de los gastos generales...\n");
	printf("\nUSUARIO		MONTO\n");
	int mov = ls_MoverCorriente(&listaUsuarios,LS_PRIMERO);
/* Se imprime el resumen de cada usuario.*/	
	while (mov)
	{
		ls_ElemCorriente(listaUsuarios,resumenUsuario);
		printf("\n%s",resumenUsuario);
		mov = ls_MoverCorriente(&listaUsuarios,LS_SIGUIENTE);
	}
	
/* Se sumariza el total por llamadas no asignadas.*/	
	mov = ls_MoverCorriente(&listaNoAsignadas,LS_PRIMERO);
	while (mov)
	{
		ls_ElemCorriente(listaNoAsignadas,&detalle);
		totalNoAsignadas = totalNoAsignadas + detalle.importe;
		mov = ls_MoverCorriente(&listaNoAsignadas,LS_SIGUIENTE);
	}
	
	printf("\nNO ASIGNADO\t%4.2f\n",totalNoAsignadas);
	printf("\nGASTOS GENERALES\t%4.2f\n",totalGenerales);
	// obtengo el total.
	acumulado = totalUsuarios + totalNoAsignadas + totalGenerales;
	printf("\n\n** TOTAL **\t%4.2f\n",acumulado);
	}
	else
		printf("\nNo existen usuarios cargados\n");
	
}

void INS_Unknown(TDT *tdt, char *orden)
{
	double total = 0;
	TNodoDetalle detalle;
	TListaSimple listaDetallesNoAsignados;
	ls_Crear(&listaDetallesNoAsignados, sizeof(TNodoDetalle));
	TDT_ObtenerDetalleNoAsignada(*tdt, &listaDetallesNoAsignados);
	if (!strcmp(orden, "fecha")){
		if (!ls_Vacia(listaDetallesNoAsignados)){
			TListaOrdenada listaOrdenada;
			lo_Crear(&listaOrdenada, sizeof(TNodoDetalle), &compararNodoDetallePorFecha);
			int movimiento = ls_MoverCorriente(&listaDetallesNoAsignados, LS_PRIMERO);
			while (movimiento){
				ls_ElemCorriente(listaDetallesNoAsignados, &detalle);
				total += detalle.importe;
				lo_Insertar(&listaOrdenada, &detalle);
				movimiento = ls_MoverCorriente(&listaDetallesNoAsignados, LS_SIGUIENTE);
			}
			movimiento = lo_MoverCorriente(&listaOrdenada, LS_PRIMERO);
			printf("\nEstos son los detalles de las llamadas no asignadas ordenados por fecha...\n");
			printf("\nFECHA		TELEFONO	MONTO\n\n");
			while (movimiento){
				lo_ElemCorriente(listaOrdenada, &detalle);
				imprimirDetalle(&detalle);
				movimiento = lo_MoverCorriente(&listaOrdenada, LS_SIGUIENTE);
			}
			printf("\n**TOTAL**			%4.2f\n", total);
			
			lo_Vaciar(&listaOrdenada);
		}
		else{
			fprintf(stderr, "Error: No se encontraron llamadas no asignadas");
		}
	}
	else{
		if (!strcmp(orden, "numero")){
			if (!ls_Vacia(listaDetallesNoAsignados)){
				TListaOrdenada listaOrdenada;
				lo_Crear(&listaOrdenada, sizeof(TNodoDetalle), &compararNodoDetallePorNumero);
				int movimiento = ls_MoverCorriente(&listaDetallesNoAsignados, LS_PRIMERO);
				while (movimiento){
					ls_ElemCorriente(listaDetallesNoAsignados, &detalle);
					total += detalle.importe;
					lo_Insertar(&listaOrdenada, &detalle);
					movimiento = ls_MoverCorriente(&listaDetallesNoAsignados, LS_SIGUIENTE);
				}
				
				movimiento = lo_MoverCorriente(&listaOrdenada, LS_PRIMERO);
				printf("\nEstos son los detalles de las llamadas no asignadas ordenados por numero...\n");
				printf("\nFECHA		TELEFONO	MONTO\n\n");
				while (movimiento){
					lo_ElemCorriente(listaOrdenada, &detalle);
					imprimirDetalle(&detalle);
					movimiento = lo_MoverCorriente(&listaOrdenada, LS_SIGUIENTE);
				}
				printf("\n**TOTAL**			%4.2f\n", total);
				
				lo_Vaciar(&listaOrdenada);
			}
			else{
				fprintf(stderr, "Error: No se encontraron llamadas no asignadas");
			}
		}
		else{
			fprintf(stderr, "Error: Parametro de ordenamiento invalido");
		}
	}
	ls_Vaciar(&listaDetallesNoAsignados);
}

void INS_Begin(TDT *tdt, char *secuencia, char *orden)
{
	int movimiento;
	double total = 0;
	TNodoDetalle detalle;
	TListaSimple listaDetallesSecuencia;
	ls_Crear(&listaDetallesSecuencia, sizeof(TNodoDetalle));
	TDT_ObtenerDetalleDeTelefonosPorAreaFisica(*tdt, secuencia, &listaDetallesSecuencia);

	if (!strcmp(orden, "fecha")){
		if (!ls_Vacia(listaDetallesSecuencia)){
			TListaOrdenada listaOrdenada;
			lo_Crear(&listaOrdenada, sizeof(TNodoDetalle), &compararNodoDetallePorFecha);
			
			movimiento = ls_MoverCorriente(&listaDetallesSecuencia, LS_PRIMERO);
			while (movimiento){
				ls_ElemCorriente(listaDetallesSecuencia, &detalle);
				total += detalle.importe;
				lo_Insertar(&listaOrdenada, &detalle);
				movimiento = ls_MoverCorriente(&listaDetallesSecuencia, LS_SIGUIENTE);
			}
			
			movimiento = lo_MoverCorriente(&listaOrdenada, LS_PRIMERO);
			printf("\nEstos son los detalles de las llamadas que empiezan con '%s' ordenado por fecha...\n", secuencia);
			printf("\nFECHA		TELEFONO	MONTO\n\n");
			while (movimiento){
				lo_ElemCorriente(listaOrdenada, &detalle);
				imprimirDetalle(&detalle);
				movimiento = lo_MoverCorriente(&listaOrdenada, LS_SIGUIENTE);
			}
			printf("\n**TOTAL**			%4.2f\n", total);
			lo_Vaciar(&listaOrdenada);
		}
		else{
			fprintf(stderr, "Error: No se encontraron llamadas que empiecen con %s ordenado por numero\n", secuencia);
		}
	}
	else{
		if (!strcmp(orden, "numero")){
			if (!ls_Vacia(listaDetallesSecuencia)){
				TListaOrdenada listaOrdenada;
				lo_Crear(&listaOrdenada, sizeof(TNodoDetalle), &compararNodoDetallePorNumero);
				
				int movimiento = ls_MoverCorriente(&listaDetallesSecuencia, LS_PRIMERO);
				while (movimiento){
					ls_ElemCorriente(listaDetallesSecuencia, &detalle);
					total += detalle.importe;
					lo_Insertar(&listaOrdenada, &detalle);
					movimiento = ls_MoverCorriente(&listaDetallesSecuencia, LS_SIGUIENTE);
				}
				
				movimiento = lo_MoverCorriente(&listaOrdenada, LS_PRIMERO);
				printf("\nEstos son los detalles de las llamadas que empiezan con '%s'...\n", secuencia);
				printf("\nFECHA		TELEFONO	MONTO\n\n");
				while (movimiento){
					lo_ElemCorriente(listaOrdenada, &detalle);
					imprimirDetalle(&detalle);
					movimiento = lo_MoverCorriente(&listaOrdenada, LS_SIGUIENTE);
				}
				printf("\n**TOTAL**			%4.2f\n", total);
				lo_Vaciar(&listaOrdenada);
			}
			else{
				fprintf(stderr, "Error: No se encontraron llamadas que empiecen con %s\n", secuencia);
			}
		}
		else{
			fprintf(stderr, "Error: Parametro de ordenamiento invalido");
		}
	}
	ls_Vaciar(&listaDetallesSecuencia);
}

void INS_Num(TDT *tdt,char *numero)
{	
	printf("\nBusco los usuarios que tienen asociado el numero %s...\n", numero);
	TNumeroTelefonico numeroTelefonicoAux;
	TNodoDetalle detalle;
	int cantUsuarios;
	double importe = 0;
	char nombreAux[65];
	char fechaAux[64];
	char numeroAux[10];
	char usuarios[200];
	usuarios[0] = '\0';
	
	TListaSimple listaNombres, listaDetalles;
	
	//Se crea el tipo de dato numero telefonico apartir del numero recibido.
	TNT_Crear(&numeroTelefonicoAux, numero, "");
/* Se crean las listas de usuarios y detalles para el numero*/
	ls_Crear(&listaNombres, sizeof(char[65]));
	ls_Crear(&listaDetalles, sizeof(TNodoDetalle));

/* Se recuperan los usuarios y detalles relacionados al numero.*/	
	TDT_ObtenerUsuariosTelefono(*tdt, numeroTelefonicoAux, &listaNombres, &cantUsuarios);
	TDT_ObtenerDetalleDeTelefonosPorAreaFisica(*tdt,numero,&listaDetalles);
/* Si se recuperaron llamadas.*/	
	if (!ls_Vacia(listaDetalles))
	{
/* En primer lugar se concatenan los nombres de usuarios del nro.*/		
		int mov = ls_MoverCorriente(&listaNombres, LS_PRIMERO);
		while (mov){
					ls_ElemCorriente(listaNombres, nombreAux);
					strcat(usuarios, nombreAux);
					strcat(usuarios, " ");
					mov = ls_MoverCorriente(&listaNombres, LS_SIGUIENTE);
				}
		//Imprimo encabezado.
		printf("\nFECHA		TELEFONO	MONTO	MONTOxUSUARIO	USUARIO/S\n\n");	
		
		mov = ls_MoverCorriente(&listaDetalles, LS_PRIMERO);
		while (mov)
		{
			ls_ElemCorriente(listaDetalles, &detalle);
			// Saco el importe por usuario si hay usuarios.
			if (cantUsuarios != 0)
			importe = detalle.importe / cantUsuarios;
			
			TF_ObtenerFecha(detalle.fecha, fechaAux);
			TNT_ObtenerNumero(detalle.numero, numeroAux);
			printf("\n%s\t%s\t%4.2f\t%4.2f\t%s\n",fechaAux,numeroAux,detalle.importe,
												importe,usuarios);
			
			mov = ls_MoverCorriente(&listaDetalles, LS_SIGUIENTE);
		}
	}
	else
		printf("\nNo se han realizado llamadas al numero de telefono.\n");
	
	ls_Vaciar(&listaNombres);
	TNT_Destruir(&numeroTelefonicoAux);
	
	printf("\n");
}

int compararNodoDetallePorFecha(void* nodo1, void* nodo2){
	TNodoDetalle* pNodoDetalle1 = (TNodoDetalle*)nodo1;
	TNodoDetalle* pNodoDetalle2 = (TNodoDetalle*)nodo2;
	return TF_Comparar(&pNodoDetalle1->fecha, &pNodoDetalle2->fecha);
}

int compararNodoDetallePorNumero(void* nodo1, void* nodo2){
	TNodoDetalle* pNodoDetalle1 = (TNodoDetalle*)nodo1;
	TNodoDetalle* pNodoDetalle2 = (TNodoDetalle*)nodo2;
	return TNT_Comparar(&pNodoDetalle1->numero, &pNodoDetalle2->numero);
}
