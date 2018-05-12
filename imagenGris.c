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
{	int i;
	size_t fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int file;
	char* uBin=(char*)malloc(sizeof(char)*15);
	char* UCla=(char*)malloc(sizeof(char)*15);
    char* muestreo=(char*)malloc(sizeof(char)*2);
	unsigned char *img;
	int imgWidth;
	int imgHeight;
	int headerSize;
	int offset;
	int aux;
	int * numero=(int*)malloc(sizeof(int));
	imgStruct* imagen=(imgStruct*)malloc(sizeof(imgStruct));
	


	///////////////////
	//PIPE IN SECTION//
	///////////////////
	int* aux2=(int*)malloc(sizeof(int));
	char buffer1[100];
	char buffer2[100];
	char buffer3[100];
	printf("HIJO 2 STDIN: %d\n", STDIN_FILENO);
	read(STDIN_FILENO, aux2, 4);
	numero[0]=aux2[0];
	read(STDIN_FILENO, aux2, 4);
	fileSize=aux2[0];
	printf("fileSize: %zu\n", fileSize);
	unsigned char* buffer=(unsigned char*)malloc(sizeof(unsigned char)*fileSize);
	img = buffer;
	read(STDIN_FILENO,aux2,4);
	printf("aux2: ");
	printf("%d\n",aux2[0] );
	read(STDIN_FILENO, buffer1, aux2[0]);
	strcpy(UCla, buffer1);
	//UCla[aux2[0]]='\0';
	printf("COMPLETADO\n");
	printf("UClasificacion: %s\n", UCla);
	read(STDIN_FILENO,aux2,4);
	read(STDIN_FILENO, buffer2, aux2[0]);
	strcpy(uBin, buffer2);
	//uBin[aux2[0]]='\0';
	printf("UBinarizacion: %s\n", uBin);
	read(STDIN_FILENO, buffer3, 4);
	strcpy(muestreo, buffer3);
	//muestreo[2]='\0';
	printf("Flag: %s\n", muestreo);
	read(STDIN_FILENO, img, fileSize);//Lectura de la imagen
	//memcpy(img, buffer, fileSize);
	printf("imagen: %u\n", img[2]);
	///////////////////////
	//FIN PIPE IN SECTION//
	///////////////////////


	//PROCESAMIENTO/
	unsigned char B;
	unsigned char G;
	unsigned char R;
	unsigned char alpha;
	unsigned char gris;
	offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
	imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	printf("Tamano del header: %d\n", headerSize);
	printf("Ancho de la imagen: %d\n", imgWidth);
	printf("Largo de la imagen: %d\n", imgHeight);
	for(i = headerSize; i < fileSize ; i=i+1)//Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
		{  
			//printf("de la imagen\n");
			//printf("%d\n",aux );
			B = img[i];// Pixel Azul
			G = img[i+1];// Pixel Verde
			R = img[i+2];// Pixel Rojo
			alpha = img[i+3];
			gris= R*0.3+G*0.59+B*0.11;//Conversion a escala de grises
			img[i]=gris;// Pixel Azul
			img[i+1]=gris;// Pixel Verde
			img[i+2]=gris;

			i = i + 3;

		}
// aqui empiesan el tumulto de weas que tuve que colocar para printear la imagen
	int outfile;
	char path2[30]= "salida_";
	char bm[5]=".bmp";
	unsigned char* escrito=(unsigned char*)malloc(sizeof(unsigned char));
	path2[7]='\0';
	bm[5]='\0';
	sprintf(numero,"%d",aux);
	printf("hola\n");
	strcat( path2, numero );
	strcat( path2, bm );
	
	printf("%s\n",path2 );
	outfile=crearSalida(path2);

	printf("fileSize: %zu\n", fileSize);
	for ( i = 0; i < fileSize; i=i+1){	
		escrito[0]=img[i];
		write(outfile,escrito,sizeof(unsigned char));
		
	}
	lseek(outfile,offset,SEEK_SET);
	//for ( i = headerSize; i < fileSize; i=i+1){
//			escrito[0]=img[i];
//			write(outfile,escrito,sizeof(unsigned char));
//		}
	close(outfile);
	// aqui termina el tumulto
		printf("sali c:\n");
	return 0;

	////////////////////
	//PIPE OUT SECTION//
	////////////////////
	int pipefd[2];
	pipe(pipefd);
	int pid;
	printf("Creando hijo\n");
	pid = fork();// CREANDO HIJO
	if(pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);//EL OUT DE ESTE PIPE SERÁ FD1
		close(pipefd[0]);
		execl("lectorImagen", "ls","-al", NULL);
        perror("exec ls failed\n");
        exit(EXIT_FAILURE);
	}
	else
	{
		//En esta sección el padre envía datos a cada hijo
		//write(pipefd[1], aux, size);//Se escriben en el pipe "size" bytes de la variable "aux"
	}
	////////////////////////
	//FIN PIPE OUT SECTION//
	////////////////////////
	return 0;
}