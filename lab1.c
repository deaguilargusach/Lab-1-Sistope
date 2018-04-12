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

void main (int argc, char **argv)
{
  char *cvalue = NULL;
  int index;
  int argument;
  int c;
  int u;
  int n;
  int b;

  opterr = 0;

  while ((argument = getopt (argc, argv, "c:u:n:b:")) != -1)
    switch (argument)
      {
      case 'c':
        c = atoi(optarg);
        break;

      case 'u':
        u = atoi(optarg);
        break;

      case 'n':
        n = atoi(optarg);
        break;

      case 'b':
        b = atoi(optarg);
        break;

      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'u')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'n')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'b')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
      default:
        abort ();
      }
	if (b > 0) b = 1;
	else b = 0;


  printf ("Cantidad de imagenes = %d\nUmbral de binarizacion = %d\nUmbral de clasificacion = %d\nBandera de muestreo = %d\n",
          c, u, n, b);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////FIN LECTURA DE PARÁMETROS///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

unsigned char *img;
long fileSize = 2048*2048+150;
int file = abrirImagen("ngc2023.bmp");
int i;
int k=0;
int j=0;
long contador;
long contadorPix;
imgStruct * imagen = (imgStruct*)malloc(sizeof(imgStruct));
imagen -> pixeles = (pixel**)malloc(sizeof(pixel)*fileSize);
img = leerImagen(file, fileSize);
long imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);
long imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,3) + img[25]*pow(256,4);
int headerSize = (int)img[2];//Antes de leer cada archivo BMP es necesario determinar el tamano del header de informacion de la imagen y comenzar a leer los bytes desde
close(file);
//el ultimo byte del header
printf("Header Size: %d\n", headerSize);
printf("Image Width: %ld\n", imgWidth);
printf("Image Height: %ld\n", imgHeight);
  for(i = headerSize; i < 300 ; i++)//Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
  {
	//i = i+16;
  	printf("%d, ", img[i]);
	//printf("%d, ", img[i+1]);
	//printf("%d, ", img[i+2]);
	//printf("%d, ", img[i+3]);
	if(i== 138)printf("\n\n\n");
	if(i%4==3) printf("\n");
	//if((int)img[i]!=0) printf("pixel ! %d, ", img[i]);
	pixel * pix = (pixel*)malloc(sizeof(pixel));
	pix -> B = img[i];
	pix -> G = img[i+1];
	pix -> R = img[i+2];
	imagen -> pixeles[k] = pix;
	i = i + 0;
	k=k+1;
	//printf(" - ");
	//if(k%16==0) printf("\n");
  }
  printf("\n");

}//END MAIN





















