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
	long contadorNegro = 0, contadorBlanco = 0, cantidadPixeles;
	int* auxLong = (long*)malloc(sizeof(long));
	size_t fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int headerSize;
	int UBin;
	int UCla;
	int i;
	int numero;
	int status;
	int muestreo;
	int aux;
	unsigned char *img;

	printf("\n\n\n##################################################################\n");
	printf("##################################################################\n");
	printf("############# CLASIFICADOR DE IMAGENES: BINARIZADOR ##############\n");
	printf("##################################################################\n");
	printf("##################################################################\n\n\n");

	///////////////////
	//PIPE IN SECTION//
	///////////////////

	int* aux2=(int*)malloc(sizeof(int));
	
	//LECTURA DE NUMERO DE IMAGEN
	read(STDIN_FILENO, aux2, 1);
	numero = aux2[0];
	printf("Imagen #: %d\n", numero);

	//LECTURA DEL TAMAÑO DE ARCHIVO
	read(STDIN_FILENO, auxLong, 8);
	fileSize=auxLong[0];
	printf("Tamano del archivo: %zu\n", fileSize);

	//LECTURA DEL UMBRAL DE CLASIFICACIÓN
	read(STDIN_FILENO, aux2, 1);
	UCla = aux2[0];
	printf("Umbral de Clasificacion: %d\n", UCla);

	//LECTURA DEL UMBRAL DE BINARIZACIÓN
	read(STDIN_FILENO, aux2, 2);
	UBin = aux2[0];
	printf("Umbral de Binarizacion: %d\n", UBin);

	//LECTURA DEL HEADERSIZE
	read(STDIN_FILENO, aux2, 1);
	headerSize = aux2[0];
	printf("Tamano del Header: %d\n", headerSize);

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

	////////////////////////////////////////
	//EJECUCIÓN DEL PROCESO: BINARIZACION //
	////////////////////////////////////////

	for(i = headerSize; i < fileSize ; i=i+1)//Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
	{  
		if(img[i] > UBin)
		{
			contadorBlanco += 1; // Si el pixel de escala de grises es mayor que el umbral, el pixel binarizado es 1.
			//printf("binarizado!");
		}
		else contadorNegro += 1;//Sino, 0.
		i = i + 3;
		//cantidadPixeles=cantidadPixeles+1;
	}
	printf("Cantidad de Negros: %d\n", contadorNegro);
	printf("Cantidad de Blancos: %d\n", contadorBlanco);

	////////////////////////////////////////////
	//FIN EJECUCIÓN DEL PROCESO: BINARIZACION //
	////////////////////////////////////////////

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
		dup2(pipefd[0], STDIN_FILENO);//EL OUT DE ESTE PIPE SERÁ FD1
		close(pipefd[0]);
		execl("escritor", "ls","-al", NULL);
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

		//ESCRITURA DEL UMBRAL DE CLASIFICACIÓN
		aux2[0] = UCla;
		write(pipefd[1], aux2, 1);

		//ESCRITURA DEL HEADER SIZE
		aux2[0] = headerSize;
		write(pipefd[1], aux2, 1);

		//ESCRITURA DEL FLAG
		aux2[0] = muestreo;
		write(pipefd[1], aux2, 1);


		//ESCRITURA DE LA IMAGEN
		write(pipefd[1], img, fileSize);
		waitpid(pid,&status,0);
	}
	////////////////////////
	//FIN PIPE OUT SECTION//
	////////////////////////
	return 0;
}