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
	long contadorNegro, contadorBlanco, cantidadPixeles;
	int* auxLong = (long*)malloc(sizeof(long));
	int muestreo, clasificador;
	int UCla;
	int numero;
	size_t fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int* aux2=(int*)malloc(sizeof(int));

	printf("\n\n\n##################################################################\n");
	printf("##################################################################\n");
	printf("###### CLASIFICADOR DE IMAGENES: ANALISIS DE PROPIEDADES #########\n");
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
	printf("Cantidad de Blancos: %d\n", contadorBlanco);

	//LECTURA DEL CONTADOR DE NEGROS
	read(STDIN_FILENO, auxLong, 8);
	contadorNegro = auxLong[0];
	printf("Cantidad de Negros: %d\n", contadorNegro);

	//LECTURA DEL FLAG
	read(STDIN_FILENO, aux2, 1);
	muestreo = aux2[0];
	printf("Bandera de muestreo: %d\n", muestreo);

	//LECTURA DEL UMBRAL DE CLASIFICACIÓN
	read(STDIN_FILENO, aux2, 1);
	UCla = aux2[0];
	printf("Umbral de Clasificacion: %d\n", UCla);

	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////

	///////////////////////////////////////////////////
	//EJECUCIÓN DEL PROCESO: ANALISIS DE PROPIEDADES //
	///////////////////////////////////////////////////

	cantidadPixeles = fileSize/4;
	printf("Cantidad de pixeles: %d\n", cantidadPixeles);
	if(muestreo==1){
		printf(" ---> Proporcion Pixeles Negros : %ld de %ld. %f del total\n", contadorNegro, cantidadPixeles, (float)contadorNegro/(float)cantidadPixeles);
		printf(" ---> Proporcion Pixeles Blancos : %ld de %ld. %f del total\n", contadorBlanco, cantidadPixeles, (float)contadorBlanco/(float)cantidadPixeles);
		if((float)contadorNegro/(float)cantidadPixeles * 100 >= UCla) clasificador = 1;
		else clasificador = 0;
		if(clasificador == 1) printf("\n---> La imagen %d es nearly black <---\n\n", numero);
		else printf("\n---> La imagen %d no es nearly black <---\n\n", numero);
	}
	printf("##################################################################\n");
	printf("##################################################################\n");
	printf("##################################################################\n");
	printf("##################################################################\n");
	printf("##################################################################\n");
	printf("##################################################################\n");
	printf("##################################################################\n");
	printf("##################################################################\n\n");

	///////////////////////////////////////////////////////
	//FIN EJECUCIÓN DEL PROCESO: ANALISIS DE PROPIEDADES //
	///////////////////////////////////////////////////////
	return 0;
}