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