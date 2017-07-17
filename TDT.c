#include "TDT.h"

/* 
 * PRE:  -
 * 
 * POS: Se crea detalleTelefonico a partir del archivo de configuración dado,
 * 		se cargan los árboles y la estructura de gastos generales.
 * 		Se retorna 0 en caso de que no hayan habido errores y 1 en caso contrario.
 * 		Los errores que pudieran ocurrir se imprimirán por stderr.
 */
int TDT_Crear(TDT* detalleTelefonico, FILE* configFile){
	if (configFile){
		int error = 0;
		char linea[100];
		FILE *archivoDetalle;
		
		// abro el arhivo de factura
		fgets(linea, 100, configFile);
		sacarFinDeLinea(linea);
		archivoDetalle = fopen(linea, "r");
		if (!archivoDetalle){
			fprintf(stderr, "Error: Hubo un error en el archivo de factura\n");
			return 1;
		}
		// cargo los gastos generales
		crearGastosGenerales(detalleTelefonico, archivoDetalle);
		// cargo el árbol de detalle
		crearArbolDetalle(detalleTelefonico, archivoDetalle);
		
		// abro los archivos de teléfonos de usuarios y los cargo en una lista
		char palabrasSeparadas[64][64];
		int cantPalabras;
		
		TDatoListaArchivosUsuarios archivoUsuario;
		ls_Crear(&detalleTelefonico->listaUsuarios, sizeof(TDatoListaArchivosUsuarios));
		while ((!feof(configFile))&&(!error)){
			fgets(linea, 100, configFile);
			sacarFinDeLinea(linea);
			separarPalabras(linea, palabrasSeparadas, &cantPalabras);
			strcpy(archivoUsuario.nombre, palabrasSeparadas[0]);
			archivoUsuario.archivo = fopen(palabrasSeparadas[1], "r");
			if (!archivoUsuario.archivo){
				fprintf(stderr, "Error: Hubo un error en un archivo de usuario\n");
				return 1;
			}
			ls_Insertar(&detalleTelefonico->listaUsuarios, LS_PRIMERO, &archivoUsuario);	
		}
		// en base a la lista de archivos de teléfonos de usuarios cargo el árbol de teléfonos de usuarios
		crearArbolTelefonosUsuarios(detalleTelefonico, detalleTelefonico->listaUsuarios);
		// vacío la lista de teléfonos de usuarios
		
		return 0;
	}
	else{
		fprintf(stderr, "Error: Hubo un error en el archivo de configuracion\n");
		return 1;
	}
}

/* 
 * PRE:	detalleTelefonico creado.
 * 		numeroTelefonico creado.
 * 		listaNombres creada.
 * 		
 * POS:	Los nombres de los usuarios que tengan asociado el número telefónico dado serán cargados en listaNombres.
 * 		La cantidad de nombres de usuarios que tienen asociado dicho número telefónico será cargada en cantUsuarios.
 * 		Se retorna 0 en caso de que no hayan habido errores y 1 en caso contrario.
 * 		Los errores que pudieran ocurrir se imprimirán por stderr.
 */
int TDT_ObtenerUsuariosTelefono(TDT detalleTelefonico, TNumeroTelefonico numeroTelefonico, 
								TListaSimple* listaNombres, int* cantUsuarios){
	(*cantUsuarios) = 0;
	char numero[11];
	int error;
	TNT_ObtenerNumero(numeroTelefonico, numero);
	ABO_MoverCte(&detalleTelefonico.arbolUsuarios, RAIZ, &error);
	
	if (!error){
		BuscarUsuariosTelefono(detalleTelefonico.arbolUsuarios, RAIZ, numero, listaNombres, cantUsuarios);
		return 0;
	}
	else{
		fprintf(stderr, "Error: No se encontró el arbol de usuarios\n");
		return 1;
	}
}

void TDT_ObtenerDetalleUsuario(TDT detalleTelefonico, char nombre[65], TListaSimple* listaDetallesUsuario){
	int error;
	ABO_MoverCte(&detalleTelefonico.arbolDetalle, RAIZ, &error);
	
	if (!error){
		BuscarDetallesUsuario(detalleTelefonico, RAIZ, nombre, listaDetallesUsuario);
	}
	else{
		fprintf(stderr, "Error: No se encontró el arbol de detalles de llamadas\n");
	}
}

/* 
 * PRE:	TDT creado
 * 		
 * POS:	devuelve en gastosGenerales, los gastos que se encuentran en el TDT.
 */
void TDT_ObetenerDeudaGastosGenerales(TDT TDT, TGastosGenerales *gastosGenerales, double *total){
	gastosGenerales->abono = TDT.gastos.abono;
	gastosGenerales->concepto = TDT.gastos.concepto;
	gastosGenerales->detalleLlamadas = TDT.gastos.detalleLlamadas;
	gastosGenerales->detectorLlamadas = TDT.gastos.detectorLlamadas;
	
	*total = TDT.gastos.abono + TDT.gastos.concepto + TDT.gastos.detalleLlamadas + 
			 TDT.gastos.detectorLlamadas;
}

void TDT_ObtenerDetalleNoAsignada(TDT detalleTelefonico, TListaSimple* listaDetallesNoAsignados){
	int error;
	ABO_MoverCte(&detalleTelefonico.arbolDetalle, RAIZ, &error);
	
	if (!error){
		BuscarDetallesNoAsignados(detalleTelefonico, RAIZ, listaDetallesNoAsignados);
	}
	else{
		fprintf(stderr, "Error: No se encontró el arbol de detalles de llamadas\n");
	}
}


void TDT_GenerarResumenUsuarios(TDT TDT, TListaSimple* listaResumen, double *acumulado)
{
	char resumen[100];
	TDatoListaArchivosUsuarios usuario;
	double total = 0;
	TListaSimple listaDetalles;
	TNodoDetalle detalle;
	
	resumen[0]='\0';
	
	ls_Crear(&listaDetalles,sizeof(TNodoDetalle));
	
	int mov = ls_MoverCorriente(&TDT.listaUsuarios,LS_PRIMERO);
/* Recorro la lista de usuarios del TDT.*/	
	while (mov)
	{
		ls_ElemCorriente(TDT.listaUsuarios,&usuario);
/* Obtengo el detalle para el usuario.*/		
		TDT_ObtenerDetalleUsuario(TDT, usuario.nombre, &listaDetalles);
		//Recorro el detalle sumando los importes por llamadas.
		mov = ls_MoverCorriente(&listaDetalles, LS_PRIMERO);
		total = 0;
		while (mov)
		{
			ls_ElemCorriente(listaDetalles, &detalle);
			//acumulo el total.
			total = total + detalle.importe;
			//me muevo al siguiente.
			mov = ls_MoverCorriente(&listaDetalles, LS_SIGUIENTE);
		}
		//sumo el acumulado total por usuarios.
		*acumulado = *acumulado + total;
		sprintf(resumen,"%s\t\t%4.2f",usuario.nombre,total);
		// Inserto el dato de resumen.
		ls_Insertar(listaResumen,LS_SIGUIENTE,resumen);
		// vacio la lista de detalle recuperada.
		ls_Vaciar(&listaDetalles);
		//Me muevo al siguiente usuario.
		mov = ls_MoverCorriente(&TDT.listaUsuarios,LS_SIGUIENTE);
	}
	
}

/*funcion: obtiene el detalle de consumos para todos los numeros telefonicos que 
  comiencen con la secuencia de numeros indicada (fecha, duracion, numero y monto
  para cada elemento del detalle). La primitiva no espera necesariamente un codigo 
  de area en si, sino la forma en que comienzan los numeros*/

/*pre: TDT creado y la secuencia posee como maximo 10 caracteres
  post: devuelve en una lista el detalle de todos los numeros que comienzan con esa secuencia*/
void TDT_ObtenerDetalleDeTelefonosPorAreaFisica (TDT TDT, char secuencia[10], TListaSimple* listaDetallesSecuencia){
	int error;
	/*muevo el corriente a la raiz*/
	ABO_MoverCte (&TDT.arbolDetalle, RAIZ, &error);
	
	if (!error){ /*si no hubo error*/
		BuscarDetallesSecuencia(TDT, RAIZ, secuencia, listaDetallesSecuencia);
	}
	else{
		fprintf(stderr, "Error: No se encontró el arbol de detalles de llamadas\n");
	}
}



void BuscarDetallesSecuencia (TDT detalleTelefonico, int MOV, char secuencia[10], TListaSimple* listaDetallesSecuencia){
	TNodoDetalle eabo;
	TNumeroTelefonico numeroTelefonicoAux; 
 	int error;
 	
 	ABO_MoverCte(&detalleTelefonico.arbolDetalle, MOV, &error);
 	if (!error){
		/*comienzo a buscar en la izquierda del arbol*/
		BuscarDetallesSecuencia(detalleTelefonico, IZQ, secuencia, listaDetallesSecuencia);
		
		ABO_ElemCte(detalleTelefonico.arbolDetalle, &eabo);
		TNT_Crear (&numeroTelefonicoAux, secuencia, "");
		/*si comienzan con la misma secuencia agrego a la lista los elementos del TNodo Detalle*/
		if (TNT_EmpiezanIgual(numeroTelefonicoAux, eabo.numero)){
			ls_Insertar(listaDetallesSecuencia, LS_PRIMERO, &eabo);
		}
		
		/*ahora busco en la derecha del arbol*/
		BuscarDetallesSecuencia (detalleTelefonico, DER, secuencia, listaDetallesSecuencia);
	}
}

void crearGastosGenerales(TDT* TDT, FILE *p_archivo_facturas){
	char linea[100];
	int i,j,k;
	int encontrado = 0;
	char cadena_num [10];

	k = 1;
	while ((!encontrado) && (!feof(p_archivo_facturas))){ /*mientras no sea el fin del archivo*/
		/* se levanta una linea del archivo*/
		fgets(linea,100,p_archivo_facturas);
		i=0;
		j=0;
		
		if (k==5){ /*si llego a la linea **Detalle** */
			encontrado = 1;
		} 
		else{
			/* leo para crear los datos del TDT*/
			while (linea[i]!='\t'){	
				i++;
			}
			//printf ("%s\n", linea);
			i++; /*sigue leyendo a partir del siguiente caractear al caracter de tabulacion*/
			while (linea[i] != '\0'){
				cadena_num[j] = linea [i]; /*obtengo en cadena_num el numero como cadena*/
				i++;
				j++;
			}   
			/*convierto la cadena_num en un double y la agrego donde corresponde en el TDT*/
			if (k==1){
				TDT->gastos.abono = atof(cadena_num);
			}
			else{
				if (k==2){
					TDT->gastos.concepto = atof(cadena_num);
				}
				else{ 
					if (k==3){
						TDT->gastos.detalleLlamadas = atof(cadena_num);
					}
					else{
						if (k==4){ 
							TDT->gastos.detectorLlamadas = atof(cadena_num);  
						}
					}
				}
			}
		} /*fin del else*/
		k++;
	} /*fin del ciclo while*/
}

void crearArbolDetalle(TDT* TDT, FILE *p_archivo_facturas){/*variables locales*/
	char linea[100];
	char palabrasSeparadas[64][64];
	int cantPalabras;
	TNodoDetalle elem_abo_detalle;
	int error = 0;
	int error_abo = 0;
	TpFuncionComparar* pFuncionComparar = &compararDetalle;

	ABO_Crear(&TDT->arbolDetalle, sizeof(TNodoDetalle), pFuncionComparar);

	while (!feof(p_archivo_facturas)){ /*mientras no sea el fin del archivo*/ 
		/* se levanta una linea del archivo*/
		fgets(linea, 100, p_archivo_facturas);
		separarPalabras(linea, palabrasSeparadas, &cantPalabras);
		if (cantPalabras == 4){
			/*aca llamo a la funcion del tda TNumeroTelefonico*/
			error = TNT_Crear(&elem_abo_detalle.numero, palabrasSeparadas[0], "");
			/* 0 si es correcto,  si hubo error*/
			if (error){
				fprintf(stderr, "Error: Hubo un error en la carga del arbol de detalle\n");
				return;
			}
			
			/*aca llamo a la funcion del tda TFecha*/
			error = TF_Crear(&elem_abo_detalle.fecha, palabrasSeparadas[1]);
			/* 0 si es correcto,  si hubo error*/
			if (error){ 
				fprintf(stderr, "Error: Hubo un error en la carga del arbol de detalle\n");
				return;
			}
			
			/*aca llamo a la funcion del tda THora*/
			error = TH_Crear(&elem_abo_detalle.hora, palabrasSeparadas[2]);
			if (error){ 
				fprintf(stderr, "Error: Hubo un error en la carga del arbol de detalle\n");
				return;
			}
			
			elem_abo_detalle.importe = atof(palabrasSeparadas[3]);
			/*primero paso a double la cadena y luego la agrego al struct*/

			ABO_Insertar(&TDT->arbolDetalle, &elem_abo_detalle, &error_abo);
			if (error_abo){
				fprintf(stderr, "Error: Hubo un error en la carga del arbol de detalle\n");
				return;
			}
		}
	} /*fin del ciclo while*/
}

/* 
 * PRE:	listaArchivosUsuarios creada.
 * 		
 * POS:	Los números telefónicos encontrados en cada uno de los archivos de la lista son
 * 		asociados con el nombre del archivo e insertados en el árbol de telefonos de usuarios 
 *		que posee detalleTelefonico.
 *		Se retorna 0 en caso de que no hayan habido errores y 1 en caso contrario.
 * 		Los errores que pudieran ocurrir se imprimirán por stderr.
 */
int crearArbolTelefonosUsuarios(TDT* detalleTelefonico, TListaSimple listaArchivosUsuarios){
	FILE* fileAux;
	char linea[100];
	char prefijos[5][5];
	int longitudLinea;
	int error = 0;
	TNodoTelefonosUsuarios nodoAux;
	
	TNumeroTelefonico numeroAux;
	TNT_Crear(&numeroAux, "", "");
	
	TDatoListaArchivosUsuarios archivoUsuario;
	int mov = ls_MoverCorriente(&listaArchivosUsuarios, LS_PRIMERO);
	
	TpFuncionComparar* pFuncionComparar = &compararTelefonosUsuarios;
	ABO_Crear(&detalleTelefonico->arbolUsuarios, sizeof(TNodoTelefonosUsuarios), pFuncionComparar);
	
	// leo todos los archivos de la lista de archivos de teléfonos de usuarios
	while ((mov)&&(!error)){
		error = 0;
		strcpy(prefijos[0], "");
		strcpy(prefijos[1], "");
		strcpy(prefijos[2], "");
		strcpy(prefijos[3], "");
		ls_ElemCorriente(listaArchivosUsuarios, &archivoUsuario);
		fileAux = archivoUsuario.archivo;
		// le asigno al nombre del nodo el nombre del archivo
		strcpy(nodoAux.nombre, archivoUsuario.nombre);
		// leo todas las líneas del archivo de teléfonos de usuarios
		while ((!feof(fileAux))&&(!error)){
			error = 0;
			fgets(linea, 100, fileAux);
			sacarFinDeLinea(linea);
			longitudLinea = strlen(linea);
			// si la línea tiene longitud menor o igual a 5 entonces digo que es un prefijo y lo cargo en el arreglo de prefijos
			// si la línea tiene longitud mayor a 4 y menor a 10 le concateno el prefijo correspondiente y agrego el nodo al árbol
			// si la línea tiene longitud igual a 10 agrego el nodo al arbol
			switch (longitudLinea){
				case 1:
					if (sonSoloNumeros(linea)){
						strcpy(prefijos[0], linea);
					}
					else{
						error = 1;
					}
					break;
				case 2:
					if (sonSoloNumeros(linea)){
						strcpy(prefijos[1], linea);
					}
					else{
						error = 1;
					}
					break;
				case 3:
					if (sonSoloNumeros(linea)){
						strcpy(prefijos[2], linea);
					}
					else{
						error = 1;
					}
					break;
				case 4:
					if (sonSoloNumeros(linea)){
						strcpy(prefijos[3], linea);
					}
					else{
						error = 1;
					}
					break;
				case 6:
					if ((strlen(prefijos[3]) == 4)&&(sonSoloNumeros(linea))){
						TNT_ModificarNumero(&numeroAux, linea, prefijos[3]);
						nodoAux.numeroTelefonico = numeroAux;
						ABO_Insertar(&detalleTelefonico->arbolUsuarios, &nodoAux, &error);
					}
					else{
						error = 1;
					}
					break;
				case 7:
					if ((strlen(prefijos[2]) == 3)&&(sonSoloNumeros(linea))){
						TNT_ModificarNumero(&numeroAux, linea, prefijos[2]);
						nodoAux.numeroTelefonico = numeroAux;
						ABO_Insertar(&detalleTelefonico->arbolUsuarios, &nodoAux, &error);
					}
					else{
						error = 1;
					}
					break;
				case 8:
					if ((strlen(prefijos[1]) == 2)&&(sonSoloNumeros(linea))){
						TNT_ModificarNumero(&numeroAux, linea, prefijos[1]);
						nodoAux.numeroTelefonico = numeroAux;
						ABO_Insertar(&detalleTelefonico->arbolUsuarios, &nodoAux, &error);
					}
					else{
						error = 1;
					}
					break;
				case 9:
					if ((strlen(prefijos[0]) == 1)&&(sonSoloNumeros(linea))){
						TNT_ModificarNumero(&numeroAux, linea, prefijos[0]);
						nodoAux.numeroTelefonico = numeroAux;
						ABO_Insertar(&detalleTelefonico->arbolUsuarios, &nodoAux, &error);
					}
					else{
						error = 1;
					}
					break;
				case 10:
					if (sonSoloNumeros(linea)){
						TNT_ModificarNumero(&numeroAux, linea, "");
						nodoAux.numeroTelefonico = numeroAux;
						ABO_Insertar(&detalleTelefonico->arbolUsuarios, &nodoAux, &error);
					}
					else{
						error = 1;
					}
					break;
				default:
					break;
			}
		}
		if (error){
			fprintf(stderr, "Error: Hubo un error en la carga del arbol de telefonos de usuarios\n");
		}
		mov = ls_MoverCorriente(&listaArchivosUsuarios, LS_SIGUIENTE);
	}
	return error;
}

/* 
 * PRE:	abo creado.
 * 		MOV dentro del rango válido.
 * 		listaNombres creada.
 * 		cantUsuarios igual a 0.
 * 		
 * POS:	Los nombres de los usuarios que tengan asociado el número telefónico dado serán
 * 		cargados en listaNombres. La cantidad de los mismos será cargada en cantUsuarios.
 */
void BuscarUsuariosTelefono(TABO abo, int MOV, char numero[11], TListaSimple* listaNombres, int* cantUsuarios){
	TNodoTelefonosUsuarios eabo;
	int error, comparacion;
	// me muevo a la posicón dada
	ABO_MoverCte(&abo, MOV, &error);
	// si es una hoja no hago nada y salgo de la función
	if (!error){
		ABO_ElemCte(abo, &eabo);
		char numeroAux[11];
		TNT_ObtenerNumero(eabo.numeroTelefonico, numeroAux);
		comparacion = strcmp(numero, numeroAux);
		
		// si el número que busco es igual al número del corriente entonces ahora busco 
		// a partir sus dos hijos ya que estos podrían tener el mismo número pero distinto nombre
		// si el número que busco es mayor al número del corriente encontes ahora busco
		// a partir de su hijo derecho
		// si el número que busco es mayor al número del corriente encontes ahora busco
		// a partir de su hijo derecho
		switch (comparacion){
			case 0:
				(*cantUsuarios)++;
				ls_Insertar(listaNombres, LS_PRIMERO, &eabo.nombre);
				BuscarUsuariosTelefono(abo, DER, numero, listaNombres, cantUsuarios); 
				BuscarUsuariosTelefono(abo, IZQ, numero, listaNombres, cantUsuarios);
				break;
			case 1:
				BuscarUsuariosTelefono(abo, DER, numero, listaNombres, cantUsuarios);
				break;
			case -1:
				BuscarUsuariosTelefono(abo, IZQ, numero, listaNombres, cantUsuarios);
				break;
		}
	}
}

/* 
 * PRE:	detalleTelefonico creado
 * 		MOV dentro del rango válido.
 * 		listaNombres creada.
 * 		
 * POS:	Los detalles que tengan un numero asociado al usuario dado serán guardados en listaDetallesUsuario.
 */
void BuscarDetallesUsuario(TDT detalleTelefonico, int MOV, char nombre[65], TListaSimple* listaDetallesUsuario){
	TNodoDetalle eabo;
	int error, cantUsuarios, mov;
	char nombreAux[65];
	
	// recorro todo el arbol de detalle segun el algoritmo in orden
	ABO_MoverCte(&detalleTelefonico.arbolDetalle, MOV, &error);
	if (!error){
		BuscarDetallesUsuario(detalleTelefonico, IZQ, nombre, listaDetallesUsuario);
	
		// por cada detalle busco los usuarios que tienen asociado el numero
		ABO_ElemCte(detalleTelefonico.arbolDetalle, &eabo);
		TListaSimple listaNombres;
		ls_Crear(&listaNombres, sizeof(char[65]));
		TDT_ObtenerUsuariosTelefono(detalleTelefonico, eabo.numero, &listaNombres, &cantUsuarios);
		mov = ls_MoverCorriente(&listaNombres, LS_PRIMERO);
		while (mov){
			ls_ElemCorriente(listaNombres, nombreAux);
			// si el usuario esta en la lista de usuarios que tienen asociado ese numero
			if (!strcmp(nombre, nombreAux)){
				// divido el importe por la cantidad de usuarios que tienen asociado ese numero
				eabo.importe = (eabo.importe / cantUsuarios);
				// inserto el detalle a la lista de detalles del usuario
				ls_Insertar(listaDetallesUsuario, LS_PRIMERO, &eabo);
			}
			mov = ls_MoverCorriente(&listaNombres, LS_SIGUIENTE);
		}
		ls_Vaciar(&listaNombres);
		
		BuscarDetallesUsuario(detalleTelefonico, DER, nombre, listaDetallesUsuario);
	}
}

/* 
 * PRE:	detalleTelefonico creado
 * 		MOV dentro del rango válido.
 * 		listaDetallesNoAsignados creada.
 * 		
 * POS:	Los detalles no esten asignados a ningun usuario seran insertados en la lista de detalles no asignados.
 */
void BuscarDetallesNoAsignados(TDT detalleTelefonico, int MOV, TListaSimple* listaDetallesNoAsignados){
	TNodoDetalle eabo;
	int error, cantUsuarios;
	
	// recorro todo el arbol de detalle segun el algoritmo in orden
	ABO_MoverCte(&detalleTelefonico.arbolDetalle, MOV, &error);
	if (!error){
		BuscarDetallesNoAsignados(detalleTelefonico, IZQ, listaDetallesNoAsignados);
	
		// por cada detalle busco los usuarios que tienen asociado el numero
		ABO_ElemCte(detalleTelefonico.arbolDetalle, &eabo);
		TListaSimple listaNombres;
		ls_Crear(&listaNombres, sizeof(char[65]));
		TDT_ObtenerUsuariosTelefono(detalleTelefonico, eabo.numero, &listaNombres, &cantUsuarios);
		// si el numero del detalle no esta asociado a ningun usuario
		if (ls_Vacia(listaNombres)){
			// inserto el detalle a la lista de detalles no asignados
			ls_Insertar(listaDetallesNoAsignados, LS_PRIMERO, &eabo);
		}
		ls_Vaciar(&listaNombres);
		
		BuscarDetallesNoAsignados(detalleTelefonico, DER, listaDetallesNoAsignados);
	}	
}

/* 
 * PRE:	elem1 y elem2 distintos de NULL.
 * 		
 * POS:	Devuelve:
 * 		0 si elem1 igual a elem2
 * 		1 si elem1 mayor a elem2
 * 	   -1 si elem2 menor a elem2
 * 
 * 		Clave primaria: numero de teléfono
 *  	Clave secundaria: nombre
 */
int compararTelefonosUsuarios(void* elem1, void* elem2){
	int comparacionTelefonos, comparacionNombres, comparacion;
	TNodoTelefonosUsuarios* eabo1 = (TNodoTelefonosUsuarios*)elem1;
	TNodoTelefonosUsuarios* eabo2 = (TNodoTelefonosUsuarios*)elem2;
	char numero1[11];
	char numero2[11];
	TNT_ObtenerNumero(eabo1->numeroTelefonico, numero1);
	TNT_ObtenerNumero(eabo2->numeroTelefonico, numero2);
	
	// comparo los números
	comparacionTelefonos = strcmp(numero1, numero2);
	comparacion = comparacionTelefonos;
	// si los números son iguales comparo los nombres
	if (comparacionTelefonos == 0){
		comparacionNombres = strcmp(eabo1->nombre, eabo2->nombre);
		comparacion = comparacionNombres;
	}
	return comparacion;
}

/* 
 * PRE:	elem1 y elem2 distintos de NULL.
 * 		
 * POS:	Devuelve:
 * 		0 si elem1 igual a elem2
 * 		1 si elem1 mayor a elem2
 * 	   -1 si elem2 menor a elem2
 * 
 * 		Clave primaria: numero de teléfono
 *  	Clave secundaria: fecha
 */
int compararDetalle (void* elem1, void* elem2){
	int comparacionTelefonos, comparacionFechas, comparacion;
		
	TNodoDetalle* eabo1 = (TNodoDetalle*)elem1;
	TNodoDetalle* eabo2 = (TNodoDetalle*)elem2;
		
	char numero1[11];
	char numero2[11];
		
	TNT_ObtenerNumero(eabo1->numero, numero1);
	TNT_ObtenerNumero(eabo2->numero, numero2);
		
	comparacionTelefonos = strcmp(numero1, numero2);
	comparacion = comparacionTelefonos;
		
	if (comparacionTelefonos == 0){
		char fecha1[9];
		char fecha2[9];
  
		TF_ObtenerFecha(eabo1->fecha, fecha1);
		TF_ObtenerFecha(eabo2->fecha, fecha2);
  
		comparacionFechas = strcmp(fecha1, fecha2);
		comparacion = comparacionFechas;
	}
	
	return comparacion;
}

/* 
 * PRE:	-
 * 		
 * POS:	Reemplaza al final de linea los caracteres \n y \r por \0.
 */
void sacarFinDeLinea(char linea[100]){
	int i;
	int fin = 0;
	for (i = (strlen(linea) - 2); ((i < 100)&&(!fin)&&(i <= strlen(linea))); i++){
		if ((linea[i] == '\n')||(linea[i] == '\r')){
			fin = 1;
			linea[i] = '\0';
		}
	}
}

/* 
 * PRE:	-
 * 		
 * POS:	Separa las palabras encontradas en cadena y las almacena en palabrasSeparadas.
 * 		La cantidad de palabras separadas se carga en cantPalabras.
 */
void separarPalabras(char cadena[], char palabrasSeparadas[64][64], int* cantPalabras){
	char palabraAux[64] = "";
	int i;
	*cantPalabras = 0;
	int cantLetras = 0;
	int longitud = strlen(cadena);

	// recorro toda la cadena
	for (i = 0; i <= longitud; i++){
		// si encuentro un caracter de separación copio palabraAux a palabrasSeparadas
		if ((cadena[i] == '\0')||(cadena[i] == ';')||(cadena[i] == ',')||(cadena[i] == '\n')||(cadena[i] == ' ')||(cadena[i] == '	')){
			if (cantLetras > 0){
				int k;
				for (k = 0; k < cantLetras; k++){
					palabrasSeparadas[(*cantPalabras)][k] = palabraAux[k];
				}
				palabrasSeparadas[(*cantPalabras)][cantLetras] = '\0';
				(*cantPalabras)++;
				cantLetras = 0;
			}
		}
		// si no encuentro un caracter de separación concateno el mismo a palabraAux
		else{
			palabraAux[cantLetras] = cadena[i];
			cantLetras++;
		}
	}
}

/* 
 * PRE:	-
 * 		
 * POS:	Imprime por pantalla el número telefónico seguido por el nombre del usuario
 * 		que tiene asociado el mismo.
 */
void imprimirTelefonoUsuario(void* elemento){
	TNodoTelefonosUsuarios* telefonoUsuario = (TNodoTelefonosUsuarios*)elemento;
	char numero[11];
	TNT_ObtenerNumero(telefonoUsuario->numeroTelefonico, numero);
	printf("%s\t%s\n", numero, telefonoUsuario->nombre);
}

/* 
 * PRE:	-
 * 		
 * POS:	Imprime por pantalla una línea de detalle.
 */
void imprimirDetalle(void* elemento){
	
	TNodoDetalle* detalle = (TNodoDetalle*)elemento;
	char linea[100] = "";
	char numeroAux[64];
	char fechaAux[64];
	char duracionAux[64];
	char monto[64];
	
	TNT_ObtenerNumero(detalle->numero, numeroAux);
	TF_ObtenerFecha(detalle->fecha, fechaAux);
	TH_ObtenerHora(detalle->hora, duracionAux);
	
	sprintf(monto, "%4.2f", detalle->importe);
	
	strcat(linea, fechaAux);
	strcat(linea, "\t");
	strcat(linea, numeroAux);
	strcat(linea, "\t");
	strcat(linea, monto);
	printf("%s\n", linea);
}

/* 
 * PRE:	pFuncionImprimir distinto de NULL.
 * 		
 * POS:	Imprime por pantalla el contenido del árbol.
 */
void imprimirArbol(TABO abo, int MOV, TpFuncionImprimir* pFuncionImprimir){
	TNodoTelefonosUsuarios eabo;
	int error;
	ABO_MoverCte(&abo, MOV, &error);
	if (!error){
		imprimirArbol(abo, IZQ, pFuncionImprimir);
		ABO_ElemCte(abo, &eabo);
		(*pFuncionImprimir)(&eabo);
		imprimirArbol(abo, DER, pFuncionImprimir);
	}
}

/* 
 * PRE:	-
 * 		
 * POS:	Imprime por pantalla los gastos generales.
 */
void imprimirGastosGenerales(TGastosGenerales gastosGenerales){
	printf("Abono Línea: %4.2f\n", gastosGenerales.abono);
	printf("Conceptos de Internet: %4.2f\n", gastosGenerales.concepto);
	printf("Detalle llamadas locales: %4.2f\n", gastosGenerales.detalleLlamadas);
	printf("Detector de llamadas: %4.2f\n", gastosGenerales.detectorLlamadas);
}
