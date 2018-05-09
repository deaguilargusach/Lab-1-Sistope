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
	path2[7]='\0';
	bm[5]='\0';
	sprintf(numero,"%d",aux);
	strcat( path2, numero );
	strcat( path2, bm );
	printf("%s\n",path2 );
	outfile=crearSalida(path2);
	printf("el offset es: ");
	printf("%d\n",offset );
	//FIN TRATAMIENTO DEL PATH
	imagen -> nPixeles = cantidadPixeles;
	for ( i = 0; i < headerSize; i=i+1){	
		escrito[0]=img[i];
		write(outfile,escrito,sizeof(unsigned char));
	}
	lseek(outfile,offset,SEEK_SET);

	for (i = 0; i < cantidadPixeles; i=i+1){
		//
		if (imagen->pixeles[i]->binPixel==1)
			{
				escrito[0]=255;
			}	
		else{ 
			escrito[0]=0;}
		write(outfile,escrito,sizeof(unsigned char));
		write(outfile,escrito,sizeof(unsigned char));
		write(outfile,escrito,sizeof(unsigned char));
		escrito[0]=imagen->pixeles[i]->alpha;
		write(outfile,escrito,sizeof(unsigned char));
	}
	close(outfile);
}