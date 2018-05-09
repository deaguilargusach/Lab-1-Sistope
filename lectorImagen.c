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
	printf("PRUEBA 1");
	char buffer[100];
	read(STDIN_FILENO, buffer, 100);
	printf("Hijo: %s\n", buffer);
	//write(STDOUT_FILENO, "TE ENVIO ESTE MENSAJE CUALQUIERA", 33);
	printf("holi, funcioné !");
	return 0;
	//PROCESAMIENTO//
	long fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int file;
	char path[30];
	unsigned char *img;
	int imgWidth;
	int imgHeight;
	int headerSize;
	int offset;
	imgStruct *imagen = (imgStruct*)malloc(sizeof(imgStruct));

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
	//PROCESAMIENTO//

}