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
	char path[30];
	char* uBin=(char*)malloc(sizeof(char)*15);
	char* UCla=(char*)malloc(sizeof(char)*15);
    char* muestreo=(char*)malloc(sizeof(char)*2);
	unsigned char *img;
	int imgWidth;
	int imgHeight;
	int headerSize;
	int offset;
	int aux;
	int status=0;
	int * numero=(int*)malloc(sizeof(int));


	///////////////////
	//PIPE IN SECTION//
	///////////////////
	int* aux2=(int*)malloc(sizeof(int));
	char buffer[100];
	char buffer1[100];
	char buffer2[100];
	char buffer3[100];
	printf("HIJO 1 STDIN: %d\n", STDIN_FILENO);
	read(STDIN_FILENO,numero,1);
	read(STDIN_FILENO,aux2,1);
	read(STDIN_FILENO, buffer, aux2[0]);//Lectura del path
	strcpy(path, buffer);
	path[aux2[0]]='\0';
	read(STDIN_FILENO,aux2,1);
	read(STDIN_FILENO, buffer1, aux2[0]);
	strcpy(UCla, buffer1);
	UCla[aux2[0]]='\0';
	printf("UClasificacion: %s\n", UCla);
	read(STDIN_FILENO,aux2,1);
	read(STDIN_FILENO, buffer2, aux2[0]);
	strcpy(uBin, buffer2);
	uBin[aux2[0]]='\0';
	printf("UBinarizacion: %s\n", uBin);
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
	close(file);
	fileSize = (imgWidth * imgHeight)*4 + headerSize;
	file = abrirImagen(path);//Abrir archivo y obtener file descriptor.
	img = leerImagen(file, fileSize);//Se relee la imagen con el tamaño del archivo definido.
	printf("Tamano del archivo: %ld\n", fileSize);
	close(file);

	////////////////////
	//PIPE OUT SECTION//
	////////////////////
	int pipefd[2];
	pipe(pipefd);
	int pid;
	printf("Creando hijo\n\n\n");
	pid = fork();// CREANDO HIJO
	if(pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);//EL OUT DE ESTE PIPE SERÁ FD1
		close(pipefd[0]);
		execl("imagenGris", "ls","-al", NULL);
        perror("exec ls failed\n");
        exit(EXIT_FAILURE);
	}
	else
	{
		printf("pipefd's: 0: %d, 1: %d\n", pipefd[0], pipefd[1]);
		//En esta sección el padre envía datos a cada hijo
		aux2[0]=fileSize;
		write(pipefd[1], aux2, 2);//Se escriben en el pipe "filesize" de la memoria de la variable "aux2"

		aux2[0]=strlen(UCla);
		write(pipefd[1],aux2,1);
		printf("Pasando UCLA: %s\n", UCla);
		write(pipefd[1], UCla, strlen(UCla));
		aux2[0]=strlen(uBin);
		write(pipefd[1],aux2,1);
		printf("Pasando UBin: %s\n", uBin);
		write(pipefd[1], uBin, strlen(uBin));

		write(pipefd[1], muestreo, 1);
		printf("hola\n");

		write(pipefd[1], img, fileSize);
		waitpid(pid,&status,0);
	}
	////////////////////////
	//FIN PIPE OUT SECTION//
	////////////////////////
	return 0;
}