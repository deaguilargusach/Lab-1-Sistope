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
	int cantidadPixeles;
	for(i = headerSize; i < fileSize ; i=i+1)//Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
	{  
		//printf("estoy en la iteracion"); 
		//printf("%d\n",cantidadPixeles );
		//printf("de la imagen\n");
		//printf("%d\n",aux );

		pixel * pix = (pixel*)malloc(sizeof(pixel));
		pix -> B = img[i];// Pixel Azul
		pix -> G = img[i+1];// Pixel Verde
		pix -> R = img[i+2];// Pixel Rojo
		pix -> alpha = img[i+3];
		pix -> Y = pix->R*0.3+pix->G*0.59+pix->B*0.11;//Conversion a escala de grises

		if(pix -> Y > umbralBinarizacion)
		{
				pix -> binPixel = 1; // Si el pixel de escala de grises es mayor que el umbral, el pixel binarizado es 1.
			//printf("binarizado!");
		}
		else (pix -> binPixel = 0);//Sino, 0.
		imagen->pixeles[cantidadPixeles] = pix;//Se guarda el pixel en la estructura de datos de imagenes.
		i = i + 3;
		cantidadPixeles=cantidadPixeles+1;
	}
}