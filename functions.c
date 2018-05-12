//Descripción:
//Entrada:
//Salida:
unsigned char * leerImagen(int fileDescriptor, long fileSize)
{
	unsigned char * data = (unsigned char*)malloc(sizeof(unsigned char)*fileSize);
	//File descriptor sirve como puntero de la función read para leer desde el búffer donde open almacena la información de los pixeles
	int numberOfBytes = read(fileDescriptor, data, fileSize);//Número de bytes. Función guarda datos en data.
	return data;
}
int abrirImagen(char * path)
{
	int fileDescriptor = open(path, O_RDONLY);
	return fileDescriptor;
}
int crearSalida(char* path){

	int fileDescriptor = open(path, O_CREAT|O_WRONLY, S_IRWXU);;
	return fileDescriptor;
}
int escribirEnPipe(){


return 0;
}
int leerDePipe(){


return 0;
}
//Descripción:
//Entrada:
//Salida:
pixel* escalaGrises(pixel* pix){
	pix->Y=pix->R*0.3+pix->G*0.59+pix->B*0.11;// ARREGLAR CON VARIABLES AUXILIARES !!!
	return pix;
}
//Descripción:
//Entrada:
//Salida:
imgStruct* imagenAGrises(imgStruct* img){
	int n=img->nPixeles;
	int i;
	for (i = 0; i <n ; i=i+1){
		img->pixeles[i]=escalaGrises(img->pixeles[i]);
	}
	return img;
}
//Descripción:
//Entrada:
//Salida:
pixel* pixelAbinario(pixel*pix, int umbral){
	if (pix->B>umbral){
		pix->B=255;
		pix->G=255;
		pix->R=255;
	}
	else{
		pix->B=0;
		pix->G=0;
		pix->R=0;
	}
	return pix;
}
//Descripción:
//Entrada:
//Salida:
int clasificacion(imgStruct* img, int umbral){
	int i;
	long contadorPixelesBlancos = 0;
	long contadorPixelesNegros = 0;
	long cantidadPixeles = img->nPixeles;
	for (i = 0; i < img->nPixeles ; i++)
	{
		if(img -> pixeles[i] -> binPixel == 0) contadorPixelesNegros++;
		else if(img -> pixeles[i] -> binPixel == 1) contadorPixelesBlancos++;
		else
		{
 			printf("Error en la clasificacion\n");
			printf("Error ocurrido en el pixel n°: %d\n", i);
			printf("Valor del pixel: %d\n", img -> pixeles[i]->binPixel);
			return -1;
		}
	}
	printf("Proporcion Pixeles Negros : %ld de %ld. %f del total\n", contadorPixelesNegros, cantidadPixeles, (float)contadorPixelesNegros/(float)cantidadPixeles);
	printf("Proporcion Pixeles Blancos : %ld de %ld. %f del total\n", contadorPixelesBlancos, cantidadPixeles, (float)contadorPixelesBlancos/(float)cantidadPixeles);
	printf("primera: %f\n", (float)contadorPixelesNegros/(float)cantidadPixeles * 100);
	printf("segunda: %d\n", umbral);
	if((float)contadorPixelesNegros/(float)cantidadPixeles * 100 >= umbral) return 1;
	else return 0;
	return -2;
}

//Descripción:
//Entrada:
//Salida:
imgStruct* imagenAbinario(imgStruct* img, int umbral){
	int n;
	n=img->nPixeles;
	int i;
	for (i = 0; i <n ; i=i+1){
		img->pixeles[i]=pixelAbinario(img->pixeles[i], umbral);
	}
void crearPipeSalida()
{
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
		close(pipefd[0]);
		execl("lectorImagen", "ls","-al", NULL);
        perror("exec ls failed\n");
        exit(EXIT_FAILURE);
	}
	else
	{
		//En esta sección el padre envía datos a cada hijo
	}
	////////////////////////
	//FIN PIPE OUT SECTION//
	////////////////////////
}
}

