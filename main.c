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
  char* cantidadImagenes=(char*)malloc(sizeof(char)*15);
  char* umbralBinarizacion=(char*)malloc(sizeof(char)*15);
  char* umbralClasificacion=(char*)malloc(sizeof(char)*15);
  char* flagMuestreo=(char*)malloc(sizeof(char)*2);
  int* tamaniocosas=(int*)malloc(sizeof(int));
  flagMuestreo[0]='0';
  flagMuestreo[1]='\0';

  opterr = 0;

  while ((argument = getopt (argc, argv, "c:u:n:b")) != -1)
    switch (argument)
      {
      case 'c':
      		sscanf(optarg,"%s",cantidadImagenes);
//        cantidadImagenes = atoi(optarg);
        break;

      case 'u':
      		sscanf(optarg,"%s",umbralBinarizacion);
//        umbralBinarizacion = atoi(optarg);
        break;

      case 'n':
      		sscanf(optarg,"%s",umbralClasificacion);
//        umbralClasificacion = atoi(optarg);
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

  printf ("Cantidad de imagenes = %s\nUmbral de binarizacion = %s\nUmbral de clasificacion = %s\nBandera de muestreo = %s\n",
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
	while(aux <= atoi(cantidadImagenes)){
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
			//En esta sección el padre envía datos a cada hijo
			//write(pipefd[1], "TE ENVIO ESTE MENSAJE CUALQUIERA", 33);// PRUEBA DE QUE FUNCIONA
			tamaniocosas[0]=strlen(path);
			write(pipefd[1],tamaniocosas,1);
			write(pipefd[1], path, tamaniocosas[0]);//Envío del path de imagen al hijo

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//MATI, AQUI TENEMOS QUE LOGRAR PASAR ESTOS CUATRO PARÁMETROS EN FORMA DE STRING Y CON TAMAÑO DINÁMICO, ES DECIR
			// 1 BYTE SI ES UN PURO NÚMERO, 2 SI SON 2 Y ASÍ
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			tamaniocosas[0]=strlen(umbralClasificacion);
			write(pipefd[1],tamaniocosas,1);
			write(pipefd[1], umbralClasificacion, strlen(umbralClasificacion));
			tamaniocosas[0]=strlen(umbralBinarizacion);
			write(pipefd[1],tamaniocosas,1);
			write(pipefd[1], umbralBinarizacion, strlen(umbralBinarizacion));
			write(pipefd[1], flagMuestreo, 2);
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





















