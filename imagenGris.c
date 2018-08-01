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
	size_t fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int file;
	int UBin;
	int UCla;
    int cantidadPixeles;
	unsigned char *img;
	unsigned char *buffer;
	int imgWidth;
	int imgHeight;
	int headerSize;
	int muestreo;
	int status;
	int offset;
	int aux;
	int* auxLong = (long*)malloc(sizeof(long));
	int i;
	int * numero;
	imgStruct* imagen=(imgStruct*)malloc(sizeof(imgStruct));
	
	
	///////////////////
	//PIPE IN SECTION//
	///////////////////
	int* aux2=(int*)malloc(sizeof(int));
	char buffer1[100];
	char buffer2[100];
	char buffer3[100];

	//LECTURA DE NUMERO DE IMAGEN
	read(STDIN_FILENO, aux2, 1);
	numero = aux2[0];

	//LECTURA DEL TAMAÑO DE ARCHIVO
	read(STDIN_FILENO, auxLong, 8);
	fileSize=auxLong[0];


	//LECTURA DEL UMBRAL DE CLASIFICACIÓN
	read(STDIN_FILENO, aux2, 1);
	UCla = aux2[0];


	//LECTURA DEL UMBRAL DE BINARIZACIÓN
	read(STDIN_FILENO, aux2, 2);
	UBin = aux2[0];

	//LECTURA DEL FLAG
	read(STDIN_FILENO, aux2, 1);
	muestreo = aux2[0];
	
	
	//LECTURA DEL HEADERSIZE
	read(STDIN_FILENO, aux2, 1);
	headerSize = aux2[0];


	//LECTURA DE LA IMAGEN
	img = (unsigned char*)malloc(sizeof(unsigned char)*fileSize);
	read(STDIN_FILENO, img, fileSize);//Lectura de la imagen
	//memcpy(img, buffer, fileSize);
	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////

	///////////////////////////////////////
	//EJECUCIÓN DEL PROCESO: IMAGEN GRIS //
	///////////////////////////////////////

	unsigned char B;
	unsigned char G;
	unsigned char R;
	unsigned char alpha;
	unsigned char Y;
	offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
	imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	for(i = headerSize; i < fileSize ; i=i+1)//Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
	{  
		//printf("de la imagen\n");
		//printf("%d\n",aux );
		B = img[i];// Pixel Azul
		G = img[i+1];// Pixel Verde
		R = img[i+2];// Pixel Rojo
		alpha = img[i+3];
		Y= R*0.3+G*0.59+B*0.11;//Conversion a escala de grises
		img[i]=Y;// Pixel Azul
		img[i+1]=Y;// Pixel Verde
		img[i+2]=Y;

		i = i + 3;
	}

	///////////////////////////////////////////
	//FIN EJECUCIÓN DEL PROCESO: IMAGEN GRIS //
	///////////////////////////////////////////

	////////////////////
	//PIPE OUT SECTION//
	////////////////////
	int pipefd[2];
	pipe(pipefd);
	int pid;
	
	pid = fork();// CREANDO HIJO
	if(pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);//EL OUT DE ESTE PIPE SERÁ FD1
		close(pipefd[0]);
		execl("Binarizacion", "ls","-al", NULL);
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

		//ESCRITURA DEL UMBRAL DE CLASIFICACIÓN
		aux2[0] = UCla;
		write(pipefd[1], aux2, 1);

		//ESCRITURA DEL UMBRAL DE BINARIZACIÓN
		aux2[0] = UBin;
		write(pipefd[1], aux2, 2);

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