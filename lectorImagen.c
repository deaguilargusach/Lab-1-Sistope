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
	long fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int file;
	char path[30];
	char ubin[30];
	char UCla[30];
	char muestreo[2];
	unsigned char *img;
	int imgWidth;
	int imgHeight;
	int headerSize;
	int offset;
	int aux;
	imgStruct *imagen = (imgStruct*)malloc(sizeof(imgStruct));


	///////////////////
	//PIPE IN SECTION//
	///////////////////
	int* aux2=(int*)malloc(sizeof(int));
	char buffer[100];
	char buffer1[100];
	char buffer2[100];
	char buffer3[100];
	read(STDIN_FILENO,aux2,1);
	read(STDIN_FILENO, buffer, aux2[0]);//Lectura del path
	strcpy(path, buffer);
	path[aux2[0]]='\0';
	printf("Path: %s\n", path);
	read(STDIN_FILENO,aux2,1);
	read(STDIN_FILENO, buffer1, aux2[0]);
	strcpy(UCla, buffer1);
	UCla[aux2[0]]='\0';
	printf("UClasificacion: %s\n", UCla);
	read(STDIN_FILENO,aux2,1);
	read(STDIN_FILENO, buffer2, aux2[0]);
	strcpy(uBin, buffer2);
	uBin[aux2[0]]='\0';
	printf("UBinarizacion: %s\n", ubin);
	read(STDIN_FILENO, buffer3, 1);
	strcpy(muestreo, buffer3);
	muestreo[2]='\0';
	printf("Flag: %s\n", muestreo);
	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////


	//PROCESAMIENTO//
	path[12]='\0';
	fileSize = 500;//Se leen los primeros 500 carácteres(número arbitrario) para obtener de la data, el ancho, el largo y el tamaño del header
	file = abrirImagen(path);//Abrir archivo y obtener file descriptor.
	img = leerImagen(file, fileSize);//leerImagen devuelve un puntero a un array de unsigned char con los bytes de datos obtenidos de la imagen.
	offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
	imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	imagen->pixeles = (pixel**)malloc(sizeof(pixel*)*imgWidth*imgHeight);
	close(file);
	printf("Tamano del header: %d\n", headerSize);
	printf("Ancho de la imagen: %d\n", imgWidth);
	printf("Largo de la imagen: %d\n", imgHeight);
	fileSize = (imgWidth * imgHeight)*4 + headerSize;
	file = abrirImagen(path);//Abrir archivo y obtener file descriptor.
	img = leerImagen(file, fileSize);//Se relee la imagen con el tamaño del archivo definido.
	printf("Tamano del archivo: %ld\n", fileSize);
	printf("inicio pipeline\n");

	////////////////////
	//PIPE OUT SECTION//
	////////////////////
	int pipefd[2];
	pipe(pipefd);
	int pid;
	long fileSize;
	printf("Creando hijo\n");
	pid = fork();// CREANDO HIJO
	if(pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);//EL OUT DE ESTE PIPE SERÁ FD1
		close(pipefd[0])
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