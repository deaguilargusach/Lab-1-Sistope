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
	printf("Cantidad de pixeles: %d\n", imagen -> nPixeles);
	printf("%d\n",imagen->nPixeles);
	if(flagMuestreo==1){
		clasificador = clasificacion(imagen, umbralClasificacion);
		if(clasificador == 1) printf("La imagen es nearly black.\n");
		else if(clasificador == -1) printf("Ha ocurrido un error en la clasificacion, algun pixel esta mal definido.\n");
		else if(clasificador == -1) printf("Ha ocurrido un error en la clasificacion\n");
		else printf("La imagen no es nearly black.\n");
	}
	aux=aux+1;
	for(i = 0 ;i < cantidadPixeles; i=i+1) //Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
	{
		free(imagen->pixeles[cantidadPixeles]); 
	}
	free(imagen->pixeles);
}