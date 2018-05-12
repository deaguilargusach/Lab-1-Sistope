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

int main (int argc, char **argv)
{	
	int* auxLong = (long*)malloc(sizeof(long));
	long contadorNegro, contadorBlanco, cantidadPixeles;
	size_t fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int i;
	int aux;
	int UCla;
	int headerSize;
	unsigned char *img;
	int status;
	int muestreo;
	int* aux2=(int*)malloc(sizeof(int));
	char* numeroChar=(char*)malloc(sizeof(char)*15);
	int numero;

	printf("\n\n\n##################################################################\n");
	printf("##################################################################\n");
	printf("############# CLASIFICADOR DE IMAGENES: ESCRITOR #################\n");
	printf("##################################################################\n");
	printf("##################################################################\n\n\n");

	///////////////////
	//PIPE IN SECTION//
	///////////////////

	//LECTURA DE NUMERO DE IMAGEN
	read(STDIN_FILENO, aux2, 1);
	numero = aux2[0];
	printf("Imagen #: %d\n", numero);

	//LECTURA DEL TAMAÑO DE ARCHIVO
	read(STDIN_FILENO, auxLong, 8);
	fileSize = auxLong[0];
	printf("Tamano del archivo: %zu bytes\n", fileSize);

	//LECTURA DEL CONTADOR DE BLANCOS
	read(STDIN_FILENO, auxLong, 8);
	contadorBlanco = auxLong[0];

	//LECTURA DEL CONTADOR DE NEGROS
	read(STDIN_FILENO, auxLong, 8);
	contadorNegro = auxLong[0];

	//LECTURA DEL UMBRAL DE CLASIFICACIÓN
	read(STDIN_FILENO, aux2, 1);
	UCla = aux2[0];
	printf("Umbral de Clasificacion: %d\n", UCla);

	//LECTURA DEL HEADERSIZE
	read(STDIN_FILENO, aux2, 1);
	headerSize = aux2[0];

	//LECTURA DEL FLAG
	read(STDIN_FILENO, aux2, 1);
	muestreo = aux2[0];
	printf("Bandera de muestreo: %d\n", muestreo);

	//LECTURA DE LA IMAGEN
	img = (unsigned char*)malloc(sizeof(unsigned char)*fileSize);
	read(STDIN_FILENO, img, fileSize);//Lectura de la imagen

	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////

	////////////////////////////////////
	//EJECUCIÓN DEL PROCESO: ESCRITOR //
	////////////////////////////////////

	int outfile;
	char path2[30]= "salida_";
	char bm[5]=".bmp";
	unsigned char* escrito=(unsigned char*)malloc(sizeof(unsigned char));
	path2[7]='\0';
	bm[5]='\0';
	sprintf(numeroChar,"%d",numero);
	strcat(path2, numeroChar);
	strcat(path2, bm);
	
	printf("Nombre del archivo de salida: %s\n",path2 );
	outfile=crearSalida(path2);
	int counter = 1;
	for (i = 0; i < fileSize; i=i+1){	
		escrito[0]=img[i];
		if(i == fileSize/10*counter)
		{
			printf("---> %d/100...\n", counter*10);
			counter = counter + 1;
		}
		write(outfile,escrito,sizeof(unsigned char));
	}
	lseek(outfile,headerSize,SEEK_SET);

	////////////////////////////////////////
	//FIN EJECUCIÓN DEL PROCESO: ESCRITOR //
	////////////////////////////////////////

	////////////////////
	//PIPE OUT SECTION//
	////////////////////
	int pipefd[2];
	pipe(pipefd);
	int pid;
	printf("Creando hijo\n");
	pid = fork();// CREANDO HIJO
	if(pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);//EL OUT DE ESTE PIPE SERÁ FD1
		close(pipefd[0]);
		execl("analisis", "ls","-al", NULL);
        perror("exec ls failed\n");
        exit(EXIT_FAILURE);
	}
	else
	{
		//En esta sección el padre envía datos a cada hijo

		//ESCRITURA DEL NUMERO DE IMAGEN
		aux2[0]=numero;
		write(pipefd[1],aux2, 1);

		//ESCRITURA DEL TAMAÑO DE ARCHIVO
		aux2[0]=fileSize;
		write(pipefd[1], aux2, sizeof(fileSize));//Se escriben en el pipe "fileSize" de la memoria de la variable "aux2"

		//ESCRITURA DEL CONTADOR DE BLANCOS
		aux2[0]=contadorBlanco;
		write(pipefd[1],aux2, sizeof(contadorBlanco));

		//ESCRITURA DEL CONTADOR DE NEGROS
		aux2[0]=contadorNegro;
		write(pipefd[1],aux2, sizeof(contadorNegro));

		//ESCRITURA DEL FLAG
		aux2[0] = muestreo;
		write(pipefd[1], aux2, 1);

		//ESCRITURA DEL UMBRAL DE CLASIFICACIÓN
		aux2[0] = UCla;
		write(pipefd[1], aux2, 1);

		waitpid(pid,&status,0);
	}
	////////////////////////
	//FIN PIPE OUT SECTION//
	////////////////////////
	return 0;
}