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

	//////////////////////////////
	//PARTE 1: LECTOR DE IMAGEN.//
	//////////////////////////////

	//////////////////////////////////////////
	//PARTE 2: CONVERSOR A ESCALA DE GRISES.//
	//////////////////////////////////////////

	//////////////////////////
	//PARTE 3: BINARIZACIÓN.//
	//////////////////////////

	//////////////////////////////////
	//PARTE 4: ESCRITURA RESULTADOS.//
	//////////////////////////////////

	//////////////////////////////////
	//PARTE 5: ANALISIS PROPIEDADES.//
	//////////////////////////////////

	int index;
	int argument;
	char* cantidadImagenes=(char*)malloc(sizeof(char)*15);
	char* umbralBinarizacion=(char*)malloc(sizeof(char)*15);
	char* umbralClasificacion=(char*)malloc(sizeof(char)*15);
	char* flagMuestreo=(char*)malloc(sizeof(char)*2);
	int* tamanioCosas=(int*)malloc(sizeof(int));
	flagMuestreo[0]='0';
	flagMuestreo[1]='\0';

	opterr = 0;

  while ((argument = getopt (argc, argv, "c:u:n:b")) != -1)
    switch (argument)
      {
      case 'c':
      		sscanf(optarg,"%s",cantidadImagenes);
        break;

      case 'u':
      		sscanf(optarg,"%s",umbralBinarizacion);
        break;

      case 'n':
      		sscanf(optarg,"%s",umbralClasificacion);
        break;
      case 'b':
        flagMuestreo[0] = '1';
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
        
        else if(optopt != 'b'){
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        }
      }
  printf("\n\n\n##################################################################\n");
  printf("##################################################################\n");
  printf("############# CLASIFICADOR DE IMAGENES: MAIN #####################\n");
  printf("##################################################################\n");
  printf("##################################################################\n\n\n");
  printf ("Cantidad de imagenes = %s\nUmbral de binarizacion = %s\nUmbral de clasificacion = %s\nBandera de muestreo = %s\n",
          cantidadImagenes, umbralBinarizacion, umbralClasificacion, flagMuestreo);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////FIN LECTURA DE PARÁMETROS///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////
	//EJECUCIÓN DEL PROCESO: MAIN //
	////////////////////////////////
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
	//inicio de bucle de ejecucion para las n imagenes
	while(aux <= atoi(cantidadImagenes)){
		cantidadPixeles=0;
		path[7]='\0';
		bm[5]='\0';
		sprintf(numero,"%d",aux);
		strcat(path, numero);
		strcat(path, bm);
		printf("Cargando imagen: %s\n",path );
		int status;

	////////////////////////////////////
	//FIN EJECUCIÓN DEL PROCESO: MAIN //
	////////////////////////////////////

		////////////////////
		//PIPE OUT SECTION//
		////////////////////

		int pipefd[2];
		pipe(pipefd);
		int pid;
		printf("Creando hijo...\n\n");
		pid = fork();// CREANDO HIJO
		if(pid == 0)
		{
			dup2(pipefd[0], STDIN_FILENO);//Se copia la sección de lectura del pipe a la constante STDIN_FILENO, de modo que el hijo pueda leer
			//close(pipefd[0]);
			execl("lectorImagen", "ls","-al", NULL);
	        perror("exec ls failed\n");
	        exit(EXIT_FAILURE);
		}
		else
		{
			//En esta sección el padre envía datos a cada hijo

			//ESCRITURA DEL NUMERO DE IMAGEN
			tamanioCosas[0]=aux;
			write(pipefd[1],tamanioCosas,1);

			//ESCRITURA DEL TAMAÑO DEL PATH
			tamanioCosas[0]=strlen(path);
			write(pipefd[1],tamanioCosas,1);

			//ESCRITURA DEL PATH
			write(pipefd[1], path, strlen(path));//Envío del path de imagen al hijo

			//ESCRITURA DEL UMBRAL DE CLASIFICACIÓN
			tamanioCosas[0]=atoi(umbralClasificacion);
			write(pipefd[1], tamanioCosas, 1);

			//ESCRITURA DEL UMBRAL DE BINARIZACION
			tamanioCosas[0]=atoi(umbralBinarizacion);
			write(pipefd[1], tamanioCosas, 2);

			//ESCRITURA DEL FLAG
			tamanioCosas[0]=atoi(flagMuestreo);
			write(pipefd[1], tamanioCosas, 1);

			waitpid(pid,&status,0);
		}
		aux ++;

		////////////////////
		//PIPE OUT SECTION//
		////////////////////

	}
}//END MAIN





















