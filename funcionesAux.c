#include "funcionesAux.h"

int parametrosInvalidos(char *opcion,char *comando,char *parametro1, char *parametro2)
{
/* Se valida parametro a parametro. En caso de ocurrir un error se devuelve un int
 * por parametro de salida para informar el comando que es invalido.*/
	
	// El parametro opcion debe ser igual a -o.
	if (strcmp(opcion,"-o")!= 0) return 1;
	// El parametro comando debe ser igual a cualquiera de los 5 comandos posibles. 
	else
		if (strcmp(comando,"usr") == 0)
		{
			// Si no se indico ningun usuario se devuelve error 3.
			if (strlen(parametro1) < 1) return 3;
			else return 0;
		}
			// Este comando no posee parametros.
		else if (strcmp(comando,"overview") == 0)
			return 0;
		else if (strcmp(comando,"unknown") == 0)
		{
			// El comando unknown posee como parametros validos a "fecha" y "numero".
			// Sino es ninguno de estos valores se devuelve error 4.
			if ((strcmp(parametro1,"numero") != 0) &&
			    (strcmp(parametro1,"fecha") != 0) )
					return 4;
			else
				return 0;
		}
		else if (strcmp(comando,"begin") == 0)
		{
			// El comando begin posee 2 parametros. El primero un numero, y el segundo
			// idem al parametro uno del comando anterior.
			// Si no se indico ningun numero se devuelve error 3.
			if (strlen(parametro1) < 1) return 3;
			else
			{
				if ((strcmp(parametro2,"numero") != 0) &&
							    (strcmp(parametro2,"fecha") != 0) )
									return 5;
				else
					return 0;
			}
			
		}
		else if (strcmp(comando,"num") == 0)
		{
			if (strlen(parametro1) < 1) return 3;
			else
				return 0;
		}
		else return 2;

}

/* Funcion para informar por pantalla errores ocurridos al ingresar
 * los parametros de ejecucion.*/
void informarError(int error)
{
	switch (error)
	{
		case 1: printf("\nEl primero comando debe ser '-o'.\n");
				break;
		case 2: printf("\nSegundo comando invalido.\n");
				break;
		case 3: printf("\nDebe ingresar un tercer parametro.\n");
				break;
		case 4: printf("\nEl primer parametro debe ser 'fecha' o 'numero'.\n");
				break;
		case 5: printf("\nEl segundo parametro debe ser 'fecha' o 'numero'.\n");
				break;
	}
		
}

void ejecutarInstruccion(TDT *tdt,FILE *config,char *comando,char *parametro1,
						 char *parametro2)
{
/* Se crea el TDA Detalle Telefonico a ser utilizado por la instruccion.*/	
	TDT_Crear(tdt,config);
/* Se identifica que instruccion fue ejecutada.*/	
	if 		(strcmp(comando,"usr") == 0)
		INS_Usr(tdt,parametro1);
	else if (strcmp(comando,"overview") == 0)
		INS_Overview(tdt);
	else if (strcmp(comando,"unknown") == 0)
		INS_Unknown(tdt,parametro1);
	else if (strcmp(comando,"begin") == 0)
		INS_Begin(tdt,parametro1,parametro2);
	else if (strcmp(comando,"num") == 0)
		INS_Num(tdt,parametro1);
}
