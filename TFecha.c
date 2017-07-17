#include "TFecha.h"

/* 
 * PRE: cadenaFecha con formato 'DD/MM/AA'
 * 
 * POS: Se crea la fecha a partir de la cadena dada
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TF_Crear(TFecha* fecha, char cadenaFecha[9]){
	// verifico el formato de la fecha
	if (esFechaValida(cadenaFecha)){
		char dia[3];
		char mes[3];
		char anio[3];
		dia[0] = cadenaFecha[0];
		dia[1] = cadenaFecha[1];
		dia[2] = '\0';
		mes[0] = cadenaFecha[3];
		mes[1] = cadenaFecha[4];
		mes[2] = '\0';
		anio[0] = cadenaFecha[6];
		anio[1] = cadenaFecha[7];
		anio[2] = '\0';
		strcpy(fecha->dia, dia);
		strcpy(fecha->mes, mes);
		strcpy(fecha->anio, anio);
		return 0;
	}
	else{
		fprintf(stderr, "Error: La fecha ingresada no es valida\n");
		strcpy(fecha->dia, "");
		strcpy(fecha->mes, "");
		strcpy(fecha->anio, "");
		return 1;
	}
}

/* 
 * PRE: -
 * 
 * POS: Devuelve 1 si la cadena tiene formato 'DD/MM/AA' y 0 en caso contrario
 */
int esFechaValida(char cadenaFecha[9]){
	int longitudFecha = strlen(cadenaFecha);
	char cadenaDia[3];
	cadenaDia[0] = cadenaFecha[0];
	cadenaDia[1] = cadenaFecha[1];
	cadenaDia[2] = '\0';
	char cadenaMes[3];
	cadenaMes[0] = cadenaFecha[3];
	cadenaMes[1] = cadenaFecha[4];
	cadenaMes[2] = '\0';
	char cadenaAnio[3];
	cadenaAnio[0] = cadenaFecha[6];
	cadenaAnio[1] = cadenaFecha[7];
	cadenaAnio[2] = '\0';
	int enteroDia = atoi(cadenaDia);
	int enteroMes = atoi(cadenaMes);
	int enteroAnio = atoi(cadenaAnio);
	if ((longitudFecha == 8)&&(enteroDia<=31)&&(enteroDia>=1)&&(enteroMes<=12)&&(enteroMes>=1)&&(enteroAnio<=99)&&(enteroAnio>=0)){
		return 1;
	}
	else{
		return 0;
	}
}

/* 
 * PRE: cadenaFecha con formato 'DD/MM/AA'
 * 
 * POS: Se modifica la fecha a partir de la cadena dada
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TF_Modificar(TFecha* fecha, char cadenaFecha[9]){
	// verifico el formato de la fecha
	if (esFechaValida(cadenaFecha)){
		char dia[3];
		char mes[3];
		char anio[3];
		dia[0] = cadenaFecha[0];
		dia[1] = cadenaFecha[1];
		dia[2] = '\0';
		mes[0] = cadenaFecha[3];
		mes[1] = cadenaFecha[4];
		mes[2] = '\0';
		anio[0] = cadenaFecha[6];
		anio[1] = cadenaFecha[7];
		anio[2] = '\0';
		strcpy(fecha->dia, dia);
		strcpy(fecha->mes, mes);
		strcpy(fecha->anio, anio);
		return 0;
	}
	else{
		fprintf(stderr, "Error: La fecha ingresada no es valida\n");
		return 1;
	}
}

/* 
 * PRE: fecha creada
 * 
 * POS: Se carga la fecha en cadenaFecha
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TF_ObtenerFecha(TFecha fecha, char cadenaFecha[9]){
	strcpy(cadenaFecha, "");
	strcat(cadenaFecha, fecha.dia);
	strcat(cadenaFecha, "/");
	strcat(cadenaFecha, fecha.mes);
	strcat(cadenaFecha, "/");
	strcat(cadenaFecha, fecha.anio);
	strcat(cadenaFecha, "\0");
	// verifico el formato de la fecha
	if (esFechaValida(cadenaFecha)){
		return 0;
	}
	else{
		strcpy(cadenaFecha, "");
		return 1;
	}
}

/* 
 * PRE: fecha creada
 * 
 * POS: Se carga el dia en cadenaDia
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TF_ObtenerDia(TFecha fecha, char cadenaDia[3]){
	strcpy(cadenaDia, "");
	strcat(cadenaDia, fecha.dia);
	strcat(cadenaDia, "\0");
	int enteroDia = atoi(cadenaDia);
	if ((enteroDia>=1)&&(enteroDia<=31)){
		return 0;
	}
	else{
		strcpy(cadenaDia, "");
		return 1;
	}
}

/* 
 * PRE: fecha creada
 * 
 * POS: Se carga el mes en cadenaMes
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TF_ObtenerMes(TFecha fecha, char cadenaMes[3]){
	strcpy(cadenaMes, "");
	strcat(cadenaMes, fecha.mes);
	strcat(cadenaMes, "\0");
	int enteroMes = atoi(cadenaMes);
	if ((enteroMes>=1)&&(enteroMes<=12)){
		return 0;
	}
	else{
		strcpy(cadenaMes, "");
		return 1;
	}
}

/* 
 * PRE: fecha creada
 * 
 * POS: Se carga el año en cadenaAnio
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TF_ObtenerAnio(TFecha fecha, char cadenaAnio[3]){
	strcpy(cadenaAnio, "");
	strcat(cadenaAnio, fecha.anio);
	strcat(cadenaAnio, "\0");
	int enteroAnio = atoi(cadenaAnio);
	if ((enteroAnio>=0)&&(enteroAnio<=99)){
		return 0;
	}
	else{
		strcpy(cadenaAnio, "");
		return 1;
	}
}

int TF_Comparar(void* fecha1, void* fecha2){
	TFecha* pFecha1 = (TFecha*)fecha1;
	TFecha* pFecha2 = (TFecha*)fecha2;
	char cadenaFecha1[9], cadenaFecha2[9];
	TF_ObtenerFecha(*pFecha1, cadenaFecha1);
	TF_ObtenerFecha(*pFecha2, cadenaFecha2);
	return strcmp(cadenaFecha1, cadenaFecha2);
}

/* 
 * PRE: fecha creada
 * 
 * POS: Se destruye la fecha
 */
int TF_Destruir(TFecha* fecha){
	strcpy(fecha->dia, "");
	strcpy(fecha->mes, "");
	strcpy(fecha->anio, "");
	return 0;
}
