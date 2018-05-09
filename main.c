#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "structs.c"
#include "functions.c"
#include <math.h>

#define READ 0
#define WRITE 1

void main (int argc, char **argv)
{
  char *cvalue = NULL;
  int index;
  int argument;
  int cantidadImagenes;
  int umbralBinarizacion;
  int umbralClasificacion;
  int flagMuestreo;

  opterr = 0;

  while ((argument = getopt (argc, argv, "c:u:n:b:")) != -1)
    switch (argument)
      {
      case 'c':
        cantidadImagenes = atoi(optarg);
        break;

      case 'u':
        umbralBinarizacion = atoi(optarg);
        break;

      case 'n':
        umbralClasificacion = atoi(optarg);
        break;

      case 'b':
        flagMuestreo = 1;
        break;

      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'u')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'n')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt)&&optopt!='b')
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
      default:
        abort ();
      }

  printf ("Cantidad de imagenes = %d\nUmbral de binarizacion = %d\nUmbral de clasificacion = %d\nBandera de muestreo = %d\n",
          cantidadImagenes, umbralBinarizacion, umbralClasificacion, flagMuestreo);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////FIN LECTURA DE PARÁMETROS///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

	int outfile;
	int clasificador;
	int i;
	int j=0;
	int cantidadPixeles;

	int aux=1;
	char* numero=(char*)malloc(sizeof(char)*15);
	char path[30]= "imagen_";
	char path2[30]= "salida_";
	char bm[5]=".bmp";
	unsigned char* escrito=(char*)malloc(sizeof(char));



	//FIN DECLARACIÓN DE VARIABLES//
	//inicio de bucle de ejecucion para las n imagenes {
	while(aux <= cantidadImagenes){
		cantidadPixeles=0;
		path[7]='\0';
		bm[5]='\0';
		sprintf(numero,"%d",aux);
		strcat(path, numero);
		strcat(path, bm);
		printf("%s\n",path );
		//printf("patata\n");

		int pipefd[2];
		char buffer[100];
		pipe(pipefd);
		int pid;
		aux ++;
		printf("Creando hijo\n");
		pid = fork();// CREANDO HIJO
		if(pid == 0)
		{
			printf("Soy el hijo...\n");
			dup2(pipefd[0], STDIN_FILENO);//EL OUT DE ESTE PIPE SERÁ FD1
			execl("lectorImagen", "ls","-al", NULL);
	        perror("exec ls failed\n");
	        exit(EXIT_FAILURE);
		}
		else
		{
			write(pipefd[1], "TE ENVIO ESTE MENSAJE CUALQUIERA", 33);
			printf("Soy el padre...\n");
		}
		printf("Salida !\n");
		//////////////////////////////
		//PARTE 1: LECTOR DE IMAGEN.//
		//////////////////////////////

		//////////////////////////////////
		//PARTE 2: CONVERSOR A GRIS+BIN.//
		//////////////////////////////////

		//////////////////////////////////
		//PARTE 3: ANALISIS PROPIEDADES.//
		//////////////////////////////////

		//////////////////////////////////
		//PARTE 4: ESCRITURA RESULTADOS.//
		//////////////////////////////////

	}
}//END MAIN





















