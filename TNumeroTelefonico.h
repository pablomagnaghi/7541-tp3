#ifndef TNUMEROTELEFONICO_H_
#define TNUMEROTELEFONICO_H_
	
#include "string.h"

typedef struct{
	char numero[11];
}TNumeroTelefonico;
	
int TNT_Crear(TNumeroTelefonico* numeroTelefonico, char numero[11], char prefijo[5]);

int TNT_ModificarNumero(TNumeroTelefonico* numeroTelefonico, char numero[11], char prefijo[5]);
	
void TNT_ObtenerNumero(TNumeroTelefonico numeroTelefonico, char numero[11]);
	
void TNT_ObtenerPrefijo(TNumeroTelefonico numeroTelefonico, char prefijo[5]);
	
int TNT_Comparar(void* pNumero1, void* pNumero2);

int TNT_EmpiezanIgual(TNumeroTelefonico numero1, TNumeroTelefonico numero2);

void TNT_Destruir(TNumeroTelefonico* numeroTelefonico);

int sonSoloNumeros(char linea[100]);

#endif /*TNUMEROTELEFONICO_H_*/
