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
	char* uBin=(char*)malloc(sizeof(char)*15);
	char* UCla=(char*)malloc(sizeof(char)*15);
    char* muestreo=(char*)malloc(sizeof(char)*2);
	unsigned char *img;
	int imgWidth;
	int imgHeight;
	int headerSize;
	int offset;
	int aux;
	int * numero=(int*)malloc(sizeof(int));
	imgStruct* imagen=(imgStruct*)malloc(sizeof(imgStruct));
	


	///////////////////
	//PIPE IN SECTION//
	///////////////////
	int* aux2=(int*)malloc(sizeof(int));
	char buffer1[100];
	char buffer2[100];
	char buffer3[100];

	printf("HIJO 2 STDIN: %d\n", STDIN_FILENO);
	read(STDIN_FILENO, aux2, 2);
	fileSize=aux2[0];
	unsigned char buffer[fileSize];
	printf("fileSize: %zu\n", fileSize);
	img = (unsigned char*)malloc(sizeof(unsigned char)*fileSize);
	read(STDIN_FILENO,aux2,1);
	read(STDIN_FILENO, buffer1, aux2[0]);
	strcpy(UCla, buffer1);
	//UCla[aux2[0]]='\0';
	printf("COMPLETADO\n");
	printf("UClasificacion: %s\n", UCla);
	read(STDIN_FILENO,aux2,1);
	read(STDIN_FILENO, buffer2, aux2[0]);
	strcpy(uBin, buffer2);
	//uBin[aux2[0]]='\0';
	printf("UBinarizacion: %s\n", uBin);
	read(STDIN_FILENO, buffer3, 1);
	strcpy(muestreo, buffer3);
	//muestreo[2]='\0';
	printf("Flag: %s\n", muestreo);
	read(STDIN_FILENO, img, fileSize);//Lectura de la imagen
	//memcpy(img, buffer, fileSize);
	printf("imagen: %u\n", img[2]);
	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////


	//PROCESAMIENTO/
	offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
	imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	imagen->pixeles = (pixel**)malloc(sizeof(pixel*)*imgWidth*imgHeight);
	printf("Tamano del header: %d\n", headerSize);
	printf("Ancho de la imagen: %d\n", imgWidth);
	printf("Largo de la imagen: %d\n", imgHeight);
	return 0;

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
		execl("lectorImagen", "ls","-al", NULL);
        perror("exec ls failed\n");
        exit(EXIT_FAILURE);
	}
	else
	{
		//En esta sección el padre envía datos a cada hijo
		//write(pipefd[1], aux, size);//Se escriben en el pipe "size" bytes de la variable "aux"
	}
	////////////////////////
	//FIN PIPE OUT SECTION//
	////////////////////////
	return 0;
}