#ifndef TDT_H_
#define TDT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TABO.h"
#include "ListaSimple.h"
#include "TNumeroTelefonico.h"
#include "TFecha.h"
#include "THora.h"

typedef struct{
     double abono;
     double concepto;
     double detalleLlamadas;
     double detectorLlamadas; 
}TGastosGenerales;

typedef struct{
     TGastosGenerales gastos;
     TABO arbolDetalle;
     TABO arbolUsuarios;
     TListaSimple listaUsuarios;
}TDT;

typedef struct{
	TNumeroTelefonico numeroTelefonico;
	char nombre[65];
}TNodoTelefonosUsuarios;

typedef struct{
     TNumeroTelefonico numero;
     TFecha fecha;
     THora hora;
     double importe;
}TNodoDetalle;

typedef struct{
	FILE* archivo;
	char nombre[65];
}TDatoListaArchivosUsuarios;

typedef void (TpFuncionImprimir)(void* elemento);

typedef int (TpFuncionComparar)(void* elem1, void* elem2);

int TDT_Crear(TDT* detalleTelefonico, FILE *configFile);

int TDT_ObtenerUsuariosTelefono(TDT detalleTelefonico, TNumeroTelefonico numeroTelefonico, TListaSimple* listaNombres, int* cantUsuarios);

void TDT_ObtenerDetalleUsuario(TDT detalleTelefonico, char nombre[65], TListaSimple* listaDetallesUsuario);

void TDT_ObetenerDeudaGastosGenerales(TDT TDT, TGastosGenerales *gastosGenerales, double *total);

void TDT_ObtenerDetalleNoAsignada(TDT detalleTelefonico, TListaSimple* listaDetallesNoAsignados);

void TDT_ObtenerDetalleDeTelefonosPorAreaFisica(TDT TDT, char secuencia[10], TListaSimple* listaDetallesSecuencia);

void TDT_GenerarResumenUsuarios(TDT TDT, TListaSimple* listaResumen, double *acumulado);




void crearGastosGenerales(TDT* TDT, FILE *p_archivo_facturas);

void crearArbolDetalle(TDT* TDT, FILE *p_archivo_facturas);

int crearArbolTelefonosUsuarios(TDT* detalleTelefonico, TListaSimple listaArchivosUsuarios);

void BuscarUsuariosTelefono(TABO abo, int MOV, char numero[11], TListaSimple* listaNombres, int* cantUsuarios);

void BuscarDetallesUsuario(TDT detalleTelefonico, int MOV, char nombre[65], TListaSimple* listaDetallesUsuario);

void BuscarDetallesNoAsignados(TDT detalleTelefonico, int MOV, TListaSimple* listaDetallesNoAsignados);

void BuscarDetallesSecuencia (TDT detalleTelefonico, int MOV, char secuencia[10], TListaSimple* listaDetallesSecuencia);

int compararTelefonosUsuarios(void* elem1, void* elem2);

int compararDetalle (void* elem1, void* elem2);

void imprimirArbol(TABO abo, int MOV, TpFuncionImprimir* pFuncionImprimir);

void imprimirTelefonoUsuario(void* elemento);

void imprimirDetalle(void* elemento);

void imprimirGastosGenerales(TGastosGenerales gastosGenerales);

void separarPalabras(char cadena[], char palabrasSeparadas[64][64], int* cantPalabras);

void sacarFinDeLinea(char linea[100]);

#endif /*TDT_H_*/
