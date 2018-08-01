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


	///////////////////
	//PIPE IN SECTION//
	///////////////////

	//LECTURA DE NUMERO DE IMAGEN
	read(STDIN_FILENO, aux2, 1);
	numero = aux2[0];


	//LECTURA DEL TAMAÑO DE ARCHIVO
	read(STDIN_FILENO, auxLong, 8);
	fileSize = auxLong[0];
	

	//LECTURA DEL CONTADOR DE BLANCOS
	read(STDIN_FILENO, auxLong, 8);
	contadorBlanco = auxLong[0];

	//LECTURA DEL CONTADOR DE NEGROS
	read(STDIN_FILENO, auxLong, 8);
	contadorNegro = auxLong[0];

	//LECTURA DEL FLAG
	read(STDIN_FILENO, aux2, 1);
	muestreo = aux2[0];


	//LECTURA DEL UMBRAL DE CLASIFICACIÓN
	read(STDIN_FILENO, aux2, 1);
	UCla = aux2[0];


	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////

	///////////////////////////////////////////////////
	//EJECUCIÓN DEL PROCESO: ANALISIS DE PROPIEDADES //
	///////////////////////////////////////////////////

	cantidadPixeles = fileSize/4;
	if(muestreo==1){
		if((float)contadorNegro/(float)cantidadPixeles * 100 >= UCla) clasificador = 1;
		else clasificador = 0;
		if(clasificador == 1) printf("\n---> La imagen %d es nearly black <---\n\n", numero);
		else printf("\n---> La imagen %d no es nearly black <---\n\n", numero);
	}


	///////////////////////////////////////////////////////
	//FIN EJECUCIÓN DEL PROCESO: ANALISIS DE PROPIEDADES //
	///////////////////////////////////////////////////////
	return 0;
}