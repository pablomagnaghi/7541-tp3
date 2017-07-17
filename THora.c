#include "THora.h"

/* 
 * PRE: cadenaHora con formato 'HH:MM' o 'MM:SS'
 * 
 * POS: Se crea la hora a partir de la cadena dada
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TH_Crear(THora* hora, char cadenaHora[6]){
	// verifico el formato de la hora
	if (esHoraValida(cadenaHora)){
		char cadenaHoras[3];
		cadenaHoras[0] = cadenaHora[0];
		cadenaHoras[1] = cadenaHora[1];
		cadenaHoras[2] = '\0';
		strcpy(hora->horas, cadenaHoras);
		char cadenaMinutos[3];
		cadenaMinutos[0] = cadenaHora[3];
		cadenaMinutos[1] = cadenaHora[4];
		cadenaMinutos[2] = '\0';
		strcpy(hora->minutos, cadenaMinutos);
		return 0;
	}
	else{
		fprintf(stderr, "Error: La hora ingresada no es valida\n");
		strcpy(hora->horas, "");
		strcpy(hora->minutos, "");
		return 1;
	}
}

/* 
 * PRE: -
 * 
 * POS: Devuelve 1 si la cadena tiene formato 'HH:MM' o 'MM:SS' y 0 en caso contrario
 */
int esHoraValida(char cadenaHora[6]){
	int longitudHora = strlen(cadenaHora);
	char cadenaHoras[3];
	cadenaHoras[0] = cadenaHora[0];
	cadenaHoras[1] = cadenaHora[1];
	cadenaHoras[2] = '\0';
	char cadenaMinutos[3];
	cadenaMinutos[0] = cadenaHora[3];
	cadenaMinutos[1] = cadenaHora[4];
	cadenaMinutos[2] = '\0';
	int enteroMinutos = atoi(cadenaMinutos);
	if ((longitudHora == 5)&&(enteroMinutos >= 0)&&(enteroMinutos <= 59)&&(cadenaHora[2] == ':')&&(sonSoloNumeros(cadenaHoras))&&(sonSoloNumeros(cadenaMinutos))){
		return 1;
	}
	else{
		return 0;
	}
}

/* 
 * PRE: cadenaHora con formato 'HH:MM' o 'MM:SS'
 * 
 * POS: Se modifica la hora a partir de la cadena dada
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TH_Modificar(THora* hora, char cadenaHora[6]){
	// verifico el formato de la hora
	if (esHoraValida(cadenaHora)){
		char cadenaHoras[3];
		cadenaHoras[0] = cadenaHora[0];
		cadenaHoras[1] = cadenaHora[1];
		cadenaHoras[2] = '\0';
		strcpy(hora->horas, cadenaHoras);
		char cadenaMinutos[3];
		cadenaMinutos[0] = cadenaHora[3];
		cadenaMinutos[1] = cadenaHora[4];
		cadenaMinutos[2] = '\0';
		strcpy(hora->minutos, cadenaMinutos);
		return 0;
	}
	else{
		fprintf(stderr, "Error: La hora ingresada no es valida\n");
		return 1;
	}
}

/* 
 * PRE: hora creada
 * 
 * POS: Se carga la hora en cadenaHora
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TH_ObtenerHora(THora hora, char cadenaHora[6]){
	strcpy(cadenaHora, "");
	strcat(cadenaHora, hora.horas);
	strcat(cadenaHora, ":");
	strcat(cadenaHora, hora.minutos);
	strcat(cadenaHora, "\0");
	if (esHoraValida(cadenaHora)){
		return 0;
	}
	else{
		strcpy(cadenaHora, "");
		return 1;
	}
}

/* 
 * PRE: hora creada
 * 
 * POS: Se cargan las horas en cadenaHoras
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TH_ObtenerHoras(THora hora, char cadenaHoras[3]){
	strcpy(cadenaHoras, "");
	strcat(cadenaHoras, hora.horas);
	strcat(cadenaHoras, "\0");
	return 0;
}

/* 
 * PRE: hora creada
 * 
 * POS: Se cargan los minutos o segundos en cadenaMinutos
 * 		Devuelve 0 si no hubo ningun error y 1 en caso contrario
 */
int TH_ObtenerMinutos(THora hora, char cadenaMinutos[3]){
	strcpy(cadenaMinutos, "");
	strcat(cadenaMinutos, hora.minutos);
	strcat(cadenaMinutos, "\0");
	int enteroMinutos = atoi(cadenaMinutos);
	if ((enteroMinutos>=0)&&(enteroMinutos<=59)){
		return 0;
	}
	else{
		strcpy(cadenaMinutos, "");
		return 1;
	}
}

/* 
 * PRE: hora creada
 * 
 * POS: Se destruye la hora
 */
int TH_Destruir(THora* hora){
	strcpy(hora->horas, "");
	strcpy(hora->minutos, "");
	return 0;
}

