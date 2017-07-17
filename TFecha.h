#ifndef TFECHA_H_
#define TFECHA_H_

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

typedef struct{
	char dia[3];
	char mes[3];
	char anio[3];
}TFecha;

int TF_Crear(TFecha* fecha, char cadenaFecha[9]);

int TF_Modificar(TFecha* fecha, char cadenaFecha[9]);

int TF_ObtenerFecha(TFecha fecha, char cadenaFecha[9]);

int TF_ObtenerDia(TFecha fecha, char cadenaDia[3]);

int TF_ObtenerMes(TFecha fecha, char cadenaMes[3]);

int TF_ObtenerAnio(TFecha fecha, char cadenaAnio[3]);

int TF_Comparar(void* fecha1, void* fecha2);

int TF_Destruir(TFecha* fecha);

int esFechaValida(char cadenaFecha[9]);

#endif /*TFECHA_H_*/
