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
  int cantidadImagenes;
  int umbralBinarizacion;
  int umbralClasificacion;
  int flagMuestreo;

  opterr = 0;

  while ((argument = getopt (argc, argv, "c:u:n:b:")) != -1)
    switch (argument)
      {
      case 'c':
        cantidadImagenes = atoi(optarg);
        break;

      case 'u':
        umbralBinarizacion = atoi(optarg);
        break;

      case 'n':
        umbralClasificacion = atoi(optarg);
        break;

      case 'b':
        flagMuestreo = atoi(optarg);
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
	if (flagMuestreo > 0) flagMuestreo = 1;
	else flagMuestreo = 0;


  printf ("Cantidad de imagenes = %d\nUmbral de binarizacion = %d\nUmbral de clasificacion = %d\nBandera de muestreo = %d\n",
          cantidadImagenes, umbralBinarizacion, umbralClasificacion, flagMuestreo);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////FIN LECTURA DE PARÁMETROS///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

	unsigned char *img;
	long fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int file;
	int clasificador;
	int i;
	int cantidadPixeles=0;
	int j=0;
	imgStruct * imagen = (imgStruct*)malloc(sizeof(imgStruct));
	long imgWidth;
	long imgHeight;
	int headerSize;

	//FIN DECLARACIÓN DE VARIABLES//

	fileSize = 500;//Se leen los primeros 500 carácteres(número arbitrario) para obtener de la data, el ancho, el largo y el tamaño del header
	file = abrirImagen("ngc2023.bmp");//Abrir archivo y obtener file descriptor.
	img = leerImagen(file, fileSize);//leerImagen devuelve un puntero a un array de unsigned char con los bytes de datos obtenidos de la imagen.
	imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,3) + img[25]*pow(256,4);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	close(file);
	printf("Tamano del header: %d\n", headerSize);
	printf("Ancho de la imagen: %ld\n", imgWidth);
	printf("Largo de la imagen: %ld\n", imgHeight);
	fileSize = (imgWidth * imgHeight)*4 + headerSize;
	imagen->pixeles = (pixel**)malloc(sizeof(pixel*)*imgWidth*imgHeight);
	printf("Tamano del archivo: %ld\n", fileSize);
	printf("Tamano");
	printf("Tamano");
	printf("Tamano");
	printf("Tamano");
	printf("Tamano");
	img = leerImagen(file, fileSize);//Se relee la imagen con el tamaño del archivo definido.
	close(file);

	//////////////////////////////////////////////////
	//PIPELINE DE LECTURA, CONVERSION Y BINARIZACION//
	//////////////////////////////////////////////////
	printf("inicio pipeline");
	for(i = headerSize; i < fileSize ; i++)//Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
	{
		printf("%d, ", i);
		pixel * pix = (pixel*)malloc(sizeof(pixel));
		pix -> B = img[i];// Pixel Azul
		pix -> G = img[i+1];// Pixel Verde
		pix -> R = img[i+2];// Pixel Rojo
		pix -> Y = pix->R*0.3+pix->G*0.59+pix->B*0.11;//Conversion a escala de grises
		if(pix->Y > umbralBinarizacion)
		{
 			pix -> binPixel = 1; // Si el pixel de escala de grises es mayor que el umbral, el pixel binarizado es 1.
			printf("binarizado!");
		}
		else (pix -> binPixel = 0);//Sino, 0.
		imagen->pixeles[cantidadPixeles] = pix;//Se guarda el pixel en la estructura de datos de imagenes.
		i = i + 3;
		cantidadPixeles=cantidadPixeles+1;
		printf("Y%f-B%d ",imagen->pixeles[cantidadPixeles]->Y, imagen->pixeles[cantidadPixeles]->binPixel);
	}
	//////////////////////////////////////////////////////
	//FIN PIPELINE DE LECTURA, CONVERSION Y BINARIZACION//
	//////////////////////////////////////////////////////
	imagen -> nPixeles = cantidadPixeles;
	printf("Cantidad de pixeles: %d\n", imagen -> nPixeles);
	printf("CLASIFICACION INICIANDO\n");
	clasificador = clasificacion(imagen);
	printf("CLASIFICACION TERMINADA\n");
	if(clasificador == 1) printf("La imagen es nearly black.\n");
	else if(clasificador == -1) printf("Ha ocurrido un error en la clasificacion, algun pixel esta mal definido.\n");
	else if(clasificador == -1) printf("Ha ocurrido un error en la clasificacion\n");
	else printf("La imagen no es nearly black.\n");


}//END MAIN





















