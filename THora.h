#ifndef THORA_H_
#define THORA_H_

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "TNumeroTelefonico.h"

typedef struct{
	char horas[3];
	char minutos[3];
}THora;

int TH_Crear(THora* hora, char cadenaHora[6]);

int esHoraValida(char cadenaHora[6]);

int TH_Modificar(THora* hora, char cadenaHora[6]);

int TH_ObtenerHora(THora hora, char cadenaHora[6]);

int TH_ObtenerHoras(THora hora, char cadenaHoras[3]);

int TH_ObtenerMinutos(THora hora, char cadenaMinutos[3]);

int TH_Destruir(THora* hora);

#endif /*THORA_H_*/
