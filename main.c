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
        flagMuestreo = 1;
        break;

      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'u')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'n')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt)&&optopt!='b')
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
      default:
        abort ();
      }

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
	int outfile;
	int clasificador;
	int i;
	int cantidadPixeles;
	int j=0;
	int* pixelsalida=(int*)malloc(sizeof(int));
	imgStruct *imagen = (imgStruct*)malloc(sizeof(imgStruct));
	int imgWidth;
	int imgHeight;
	int headerSize;
	int aux=1;
	char* numero=(char*)malloc(sizeof(char)*15);
	char path[30]= "imagen_";
	char path2[30]= "salida_";
	char bm[5]=".bmp";
	unsigned char* escrito=(char*)malloc(sizeof(char));
	int offset;


	//FIN DECLARACIÓN DE VARIABLES//
	//inicio de bucle de ejecucion para las n imagenes {
	while(aux<cantidadImagenes+1){
		cantidadPixeles=0;
		path[7]='\0';
		bm[5]='\0';
		int * escribePixel=(int*)malloc(sizeof(int));
		sprintf(numero,"%d",aux);
		strcat( path, numero );
		strcat( path, bm );
		printf("%s\n",path );
		printf("patata\n");

		fileSize = 500;//Se leen los primeros 500 carácteres(número arbitrario) para obtener de la data, el ancho, el largo y el tamaño del header
		file = abrirImagen(path);//Abrir archivo y obtener file descriptor.
		img = leerImagen(file, fileSize);//leerImagen devuelve un puntero a un array de unsigned char con los bytes de datos obtenidos de la imagen.
		offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
		imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
		imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
		headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
		imagen->pixeles = (pixel**)malloc(sizeof(pixel*)*imgWidth*imgHeight);
		close(file);
		printf("Tamano del header: %d\n", headerSize);
		printf("Ancho de la imagen: %d\n", imgWidth);
		printf("Largo de la imagen: %d\n", imgHeight);
		fileSize = (imgWidth * imgHeight)*4 + headerSize;
		file = abrirImagen(path);//Abrir archivo y obtener file descriptor.
		img = leerImagen(file, fileSize);//Se relee la imagen con el tamaño del archivo definido.
		printf("Tamano del archivo: %ld\n", fileSize);
		printf("inicio pipeline\n");


		/////////////////////////////////////////////////////////
		//INICIO PIPELINE DE LECTURA, CONVERSION Y BINARIZACION//
		/////////////////////////////////////////////////////////

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

	//////////////////////////////////////////////////////
	//FIN PIPELINE DE LECTURA, CONVERSION Y BINARIZACION//
	//////////////////////////////////////////////////////
		path2[7]='\0';
		bm[5]='\0';
		sprintf(numero,"%d",aux);
		strcat( path2, numero );
		strcat( path2, bm );
		printf("%s\n",path2 );
		outfile=crearSalida(path2);
		printf("el offset es: ");
		printf("%d\n",offset );
		imagen -> nPixeles = cantidadPixeles;
		for ( i = 0; i < headerSize; i=i+1){	
			escrito[0]=img[i];
			write(outfile,escrito,sizeof(unsigned char));
		}
		lseek(outfile,offset,SEEK_SET);

		for ( i = 0; i < cantidadPixeles; i=i+1){
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

}//END MAIN




















