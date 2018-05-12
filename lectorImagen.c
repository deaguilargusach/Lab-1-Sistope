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
	int UBin;
	int UCla;
    int muestreo;
	unsigned char *img;
	int imgWidth;
	int imgHeight;
	int headerSize;
	int offset;
	int * aux;
	int status;
	int numero;

	printf("\n\n\n##################################################################\n");
	printf("##################################################################\n");
	printf("######### CLASIFICADOR DE IMAGENES: LECTOR DE IMAGENES ###########\n");
	printf("##################################################################\n");
	printf("##################################################################\n\n\n");

	///////////////////
	//PIPE IN SECTION//
	///////////////////
	int* aux2=(int*)malloc(sizeof(int));
	char buffer[100];

	//LECTURA DE NUMERO DE IMAGEN
	read(STDIN_FILENO, aux2, 1);
	numero = aux2[0];
	printf("Imagen #: %d\n", numero);

	//LECTURA DE TAMAÑO DEL PATH
	read(STDIN_FILENO,aux2,1);

	//LECTURA DEL PATH
	read(STDIN_FILENO, buffer, aux2[0]);
	strcpy(path, buffer);
	path[aux2[0]]='\0';
	printf("Nombre de la imagen: %s\n", path);

	//LECTURA DEL UMBRAL DE CLASIFICACION
	read(STDIN_FILENO, aux2, 1);
	UCla = aux2[0];
	printf("Umbral de Clasificacion: %d\n", UCla);

	//LECTURA DEL UMBRAL DE BINARIZACION
	read(STDIN_FILENO, aux2, 2);
	UBin = aux2[0];
	printf("Umbral de Binarizacion: %d\n", UBin);

	//LECTURA DE LA FLAG
	read(STDIN_FILENO, aux2, 1);
	muestreo = aux2[0];
	printf("Bandera de Muestreo: %d\n", muestreo);

	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////

	////////////////////////////////////////////
	//EJECUCIÓN DEL PROCESO: LECTOR DE IMAGEN //
	////////////////////////////////////////////

	path[12]='\0';
	fileSize = 500;//Se leen los primeros 500 carácteres(número arbitrario) para obtener de la data, el ancho, el largo y el tamaño del header
	file = abrirImagen(path);//Abrir archivo y obtener file descriptor.
	img = leerImagen(file, fileSize);//leerImagen devuelve un puntero a un array de unsigned char con los bytes de datos obtenidos de la imagen.
	offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
	printf("Tamano del header: %d\n", offset);
	imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	printf("Ancho de la imagen: %d\n", imgWidth);
	imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	printf("Largo de la imagen: %d\n", imgHeight);
	headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	close(file);
	fileSize = (imgWidth * imgHeight)*4 + headerSize;
	file = abrirImagen(path);//Abrir archivo y obtener file descriptor.
	img = leerImagen(file, fileSize);//Se relee la imagen con el tamaño del archivo definido.
	printf("Tamano del archivo: %ld bytes\n", fileSize);
	close(file);

	////////////////////////////////////////////////
	//FIN EJECUCIÓN DEL PROCESO: LECTOR DE IMAGEN //
	////////////////////////////////////////////////

	////////////////////
	//PIPE OUT SECTION//
	////////////////////


	int pipefd[2];
	pipe(pipefd);
	int pid;
	printf("Creando hijo...\n\n");
	pid = fork();// CREANDO HIJO
	if(pid == 0)//Esta condición se activa únicamente para el proceso hijo
	{
		dup2(pipefd[0], STDIN_FILENO);//EL OUT DE ESTE PIPE SERÁ FD1
		close(pipefd[0]);
		execl("imagenGris", "ls","-al", NULL);//Se ejecuta comando execl para proceder al siguiente proceso
        perror("exec ls failed\n");
        exit(EXIT_FAILURE);
	}
	else
	{
		//En esta sección el padre envía datos a cada hijo

		//ESCRITURA DEL NUMERO DE IMAGEN
		aux2[0]=numero;
		write(pipefd[1],aux2,1);

		//ESCRITURA DEL TAMAÑO DE ARCHIVO
		aux2[0]=fileSize;
		write(pipefd[1], aux2, sizeof(fileSize));

		//ESCRITURA DEL UMBRAL DE CLASIFICACIÓN
		aux2[0] = UCla;
		write(pipefd[1], aux2, 1);

		//ESCRITURA DEL UMBRAL DE BINARIZACIÓN
		aux2[0] = UBin;
		write(pipefd[1], aux2, 2);

		//ESCRITURA DEL FLAG
		aux2[0] = muestreo;
		write(pipefd[1], aux2, 1);

		//ESCRITURA DEL HEADER SIZE
		aux2[0] = headerSize;
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