#include "TNumeroTelefonico.h"

int TNT_Crear(TNumeroTelefonico* numeroTelefonico, char numero[11], char prefijo[5]){
	
	strcpy(numeroTelefonico->numero, "");
	if (sonSoloNumeros(numero)&&sonSoloNumeros(prefijo)){
		strcpy(numeroTelefonico->numero, prefijo);
		strcat(numeroTelefonico->numero, numero);
		strcat(numeroTelefonico->numero, "\0");
		return 0;
	}
	else{
		return 1;
	}
}

int TNT_ModificarNumero(TNumeroTelefonico* numeroTelefonico, char numero[11], char prefijo[5]){

	strcpy(numeroTelefonico->numero, "");
	if (sonSoloNumeros(numero)&&sonSoloNumeros(prefijo)){
		strcpy(numeroTelefonico->numero, prefijo);
		strcat(numeroTelefonico->numero, numero);
		strcat(numeroTelefonico->numero, "\0");
		return 0;
	}
	else{
		return 1;
	}
}
	
void TNT_ObtenerNumero(TNumeroTelefonico numeroTelefonico, char numero[11]){
	strcpy(numero, numeroTelefonico.numero);
}
	
void TNT_ObtenerPrefijo(TNumeroTelefonico numeroTelefonico, char prefijo[5]){
	prefijo[0] = numeroTelefonico.numero[0];
	prefijo[1] = numeroTelefonico.numero[1];
	prefijo[2] = numeroTelefonico.numero[2];
	prefijo[3] = numeroTelefonico.numero[3];
}

int TNT_Comparar(void* pNumero1, void* pNumero2){
	TNumeroTelefonico* numero1 = (TNumeroTelefonico*)pNumero1;
	TNumeroTelefonico* numero2 = (TNumeroTelefonico*)pNumero2;
	return strcmp(numero1->numero, numero2->numero);
}

int TNT_EmpiezanIgual(TNumeroTelefonico numero1, TNumeroTelefonico numero2){
	if (strlen(numero1.numero) < strlen(numero2.numero)){
 		return !strncmp(numero2.numero, numero1.numero, strlen(numero1.numero));
	}
	else{
		return !strncmp(numero1.numero, numero2.numero, strlen(numero2.numero));
	}
}

void TNT_Destruir(TNumeroTelefonico* numeroTelefonico){
	strcpy(numeroTelefonico->numero, "");
}

int sonSoloNumeros(char linea[100]){
	int longitud = strlen(linea);
	int error = 0;
	int i;
	char c;
	for (i = 0; ((!error)&&(i < longitud)); i++){
		c = linea[i];
		if (!((c == '0')||(c == '1')||(c == '2')||(c == '3')||(c == '4')||(c == '5')||(c == '6')||(c == '7')||(c == '8')||(c == '9')||(c == '\0'))){
			error = 1;
		}
	}
	return (!error);
}
