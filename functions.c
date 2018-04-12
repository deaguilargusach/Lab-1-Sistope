//Descripción:
//Entrada:
//Salida:
unsigned char * leerImagen(int fileDescriptor, long fileSize)
{
	//char data[263000];//Se define el array de characters donde se almacenan los pixeles de la imagen leída
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

//Descripción:
//Entrada:
//Salida:
pixel* escalaGrises(pixel* pix){
	pix->B=pix->R*0.3+pix->G*0.59+pix->B*0.11;// ARREGLAR CON VARIABLES AUXILIARES !!!
	pix->G=pix->R*0.3+pix->G*0.59+pix->B*0.11;
	pix->R=pix->R*0.3+pix->G*0.59+pix->B*0.11;
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
		pix->B=1;
		pix->G=1;
		pix->R=1;
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
imgStruct* imagenAbinario(imgStruct* img, int umbral){
	int n;
	n=img->nPixeles;
	int i;
	for (i = 0; i <n ; i=i+1){
		img->pixeles[i]=pixelAbinario(img->pixeles[i], umbral);
	}
}

