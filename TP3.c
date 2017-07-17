
#include "TDT.h"
#include "funcionesAux.h"


int main(int argc, char *argv[])
{
/*Si el programa recibio menos de 3 parametros se lo considera
 * un error y se lo informa por pantalla al usuario.*/	
	
  char PathConfig[100];
  char opcion[4];
  char comando[10];
  char parametro1[100];
  char parametro2[100];
  
  int error;
  
  FILE *config;
  
  TDT	tdt;
  
/* Se inicializan los strings de parametros.*/  
  strcpy(PathConfig,"\0");
  strcpy(opcion,"\0");
  strcpy(comando,"\0");	
  strcpy(parametro1,"\0");
  strcpy(parametro2,"\0");

/* En primer lugar se valida que se haya ingresado la cantidad minima de parametros.*/  
  if (argc < 3)
  {
	printf("\nError: Comando desconocido\n");
	return -1;
  }
  else
  {
	strcpy(PathConfig,argv[1]);
	strcpy(opcion,argv[2]);
	strcpy(comando,argv[3]);

/* Se toman las instrucciones 4 y 5 solo si fueron ingresadas.*/	
	if(argc >=5)   strcpy(parametro1,argv[4]);
	if (argc == 6) strcpy(parametro2,argv[5]);
/* Se valida la ruta del archivo ce configuracion*/	
	if( (config = fopen(PathConfig,"r")) != NULL )
	{
	  error = parametrosInvalidos(opcion,comando,parametro1,parametro2);
	  if(!error)
	  {
		
		// Si los parametros son validos se procede a ejecutar la instruccion.
		ejecutarInstruccion(&tdt,config,comando,parametro1,parametro2);
	    return 0;
	  }
	  // Si se produjo algun error en los parametros se lo informa por pantalla.
	  else 
	  {
		  informarError(error);
		  return -1;
	  }
	}
	else
	{
	  printf("\nLA ruta del archivo de configuración es invalida.\n");
	  return -1;
	}
		
  }
}

