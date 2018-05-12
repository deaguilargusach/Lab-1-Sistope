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
	///////////////////
	//PIPE IN SECTION//
	///////////////////
	char buffer[100];//Buffer de paso por pipe
	read(STDIN_FILENO, buffer, size_in);//Lectura del archivo enviado por el padre. Desde el descriptor de archivo STDIN_FILENO se leen "size_in" bytes
	//y se almacenan en "buffer"
	strcpy(path, buffer);//Copia del contenido del buffer a variable local para su tratamiento
	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////

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
		write(pipefd[1], aux, size_out);//Se escriben en el pipe "size_out" bytes de la variable "aux"
	}
	////////////////////////
	//FIN PIPE OUT SECTION//
	////////////////////////
}