#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "structs.c"
#include "functions.c"
#include <math.h>




void main (int argc, char **argv)
{
  int index;
  int argument;
  int cantidadImagenes;
  int flagMuestreo;
  opterr = 0;

  while ((argument = getopt (argc, argv, "c:h:u:n:b")) != -1)
    switch (argument)
      {
      case 'c':
        cantidadImagenes = atoi(optarg);
        break;

      case 'h':
        cantidadHebras = atoi(optarg);
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
        else if (optopt == 'h')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'u')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (optopt == 'n')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt)&&optopt!='b')
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        
        else if(optopt != 'b'){
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        }
      }

  printf ("Cantidad de imagenes = %d\nUmbral de binarizacion = %d\nUmbral de clasificacion = %d\nBandera de muestreo = %d\n",
          cantidadImagenes, umbralBinarizacion, umbralClasificacion, flagMuestreo);
  printf("cantidadHebras: ");
  printf("%d\n", cantidadHebras );
  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////FIN LECTURA DE PARÁMETROS///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
	unsigned char *img;
	long fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int outfile;
	int clasificador;
	int i;
	int cantidadPixeles;
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
	void* entradahebra;
	void* imghebra;
	if (cantidadHebras<=0){
		exit(0);
	}
	pthread_t* hebras=(pthread_t*)malloc(sizeof(pthread_t)*cantidadHebras);
	pthread_barrier_init(&mybarrier,NULL,cantidadHebras);


	//FIN DECLARACIÓN DE VARIABLES//
	//inicio de bucle de ejecucion para las n imagenes {
	/////////////////////////////////////////////////////////
	//INICIO PIPELINE DE LECTURA, CONVERSION Y BINARIZACION//
	/////////////////////////////////////////////////////////
	while(aux<cantidadImagenes+1){
		white=0;
		black=0;
		cantidadPixeles=0;
		path[7]='\0';
		bm[5]='\0';
		sprintf(numero,"%d",aux);
		strcat( path, numero );
		strcat( path, bm );
		entradahebra=(void*)path;
		/////////////////////////////////////////////////////////
							// LECTURA //
		/////////////////////////////////////////////////////////
		if (pthread_create(&hebras[0],NULL,leerImagenHebras,entradahebra)){
			exit(0);
		}
		if(pthread_join(hebras[0],&imghebra)){
			exit(0);
		}
		img=(unsigned char*)imghebra;
		offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
		imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
		imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
		headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
		fileSize = (imgWidth * imgHeight)*4 + headerSize;
		/////////////////////////////////////////////////////////
							// CONVERSION //
		/////////////////////////////////////////////////////////
		for ( i = 0; i < cantidadHebras; i=i+1){
			pthread_mutex_lock(&lock);
			img[fileSize]=(unsigned char)i;
			entradahebra=(void*)img;
			if (pthread_create(&hebras[i],NULL,hebrasgris,entradahebra)){
			exit(0);
			}
			if(pthread_join(hebras[i],&imghebra)){
			exit(0);
			}
			img=(unsigned char*)imghebra;
		}
		/////////////////////////////////////////////////////////
							//BINARIZACION//
		/////////////////////////////////////////////////////////
		for ( i = 0; i < cantidadHebras; i=i+1){
			pthread_mutex_lock(&lock);
			img[fileSize]=(unsigned char)i;
			entradahebra=(void*)img;
			if (pthread_create(&hebras[i],NULL,hebrasbinario,entradahebra)){
			exit(0);
			}
			if(pthread_join(hebras[i],&imghebra)){
			exit(0);
			}
			img=(unsigned char*)imghebra;
		}

		/////////////////////////////////////////////////////////
		//INICIO PIPELINE DE LECTURA, CONVERSION Y BINARIZACION//
		/////////////////////////////////////////////////////////

		cantidadPixeles = fileSize/4;
		if(flagMuestreo==1){
		if(black/cantidadPixeles * 100 >= umbralClasificacion) clasificador = 1;
		else clasificador = 0;
		if(clasificador == 1) printf("\n---> La imagen %d es nearly black <---\n\n", aux);
		else printf("\n---> La imagen %d no es nearly black <---\n\n", aux);
	}
	
	//printf("Tamano del archivo: %ld\n", fileSize);

	//////////////////////////////////////////////////////
	//FIN PIPELINE DE LECTURA, CONVERSION Y BINARIZACION//
	//////////////////////////////////////////////////////
		path2[7]='\0';
		bm[5]='\0';
		sprintf(numero,"%d",aux);
		strcat( path2, numero );
		strcat( path2, bm );
		outfile=crearSalida(path2);
		outfile=crearSalida(path2);
		int counter = 1;
		for (i = 0; i < fileSize; i=i+1){	
			escrito[0]=img[i];
			if(i == fileSize/10*counter){
				counter = counter + 1;
			}
		write(outfile,escrito,sizeof(unsigned char));
		}
		lseek(outfile,offset,SEEK_SET);

		close(outfile);
		aux=aux+1;

	}

}//END MAIN





















