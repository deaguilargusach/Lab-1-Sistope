//Descripción:
//Entrada:
//Salida:
pthread_barrier_t mybarrier;
pthread_mutex_t lock;
float white;
float black;
int cantidadHebras;
int umbralBinarizacion;
int umbralClasificacion;
unsigned char * leerImagen(int fileDescriptor, long fileSize)
{
	unsigned char * data = (unsigned char*)malloc(sizeof(unsigned char)*fileSize);
	//File descriptor sirve como puntero de la función read para leer desde el búffer donde open almacena la información de los pixeles
	int numberOfBytes = read(fileDescriptor, data, fileSize);//Número de bytes. Función guarda datos en data.
	return data;
}
void * leerImagenHebras(void* entrada){
	char* path=(char*) entrada;
	unsigned char *img;
	long fileSize;//Tamaño del archivo determinado por (ancho*largo)*4 + tamaño del header.
	int file;
	int offset;
	int clasificador;
	int cantidadPixeles;
	int imgWidth;
	int imgHeight;
	int headerSize;
	fileSize = 500;//Se leen los primeros 500 carácteres(número arbitrario) para obtener de la data, el ancho, el largo y el tamaño del header
	file = abrirImagen(path);//Abrir archivo y obtener file descriptor.
	img = leerImagen(file, fileSize);//leerImagen devuelve un puntero a un array de unsigned char con los bytes de datos obtenidos de la imagen.
	offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
	imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	close(file);
	fileSize = (imgWidth * imgHeight)*4 + headerSize;
	file = abrirImagen(path);
	unsigned char * data = (unsigned char*)malloc(sizeof(unsigned char)*fileSize+2);
	data=leerImagen(file,fileSize);
	entrada=(void*) data;
	return (void*) data;

}
void * hebrasgris(void* entrada){
	unsigned char* img=(unsigned char*) entrada;
	int offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
	int imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	int imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	int headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	int fileSize = (imgWidth * imgHeight)*4 + headerSize;

	int R;
	int G;
	int B;
	int Y;
	int i;
	int numerohebra=img[fileSize];
	int aux1=2;
	int aux2=2;
	if (cantidadHebras>2)
	{
		while(aux1<=cantidadHebras){
		aux1=aux1*2;
		}
		aux1=aux1/2;
		aux2=numerohebra;	
	}
	else{
		aux1=cantidadHebras;
		aux2=numerohebra;
	}
	
	int empezar=(aux2*(imgWidth * imgHeight)*4)/aux1;
	int terminar=empezar + ((imgWidth * imgHeight)*4)/aux1;
	pthread_mutex_unlock(&lock);
	if (terminar>fileSize)
	{
		terminar=fileSize;
	}
	for(i = headerSize+ empezar; i < headerSize+terminar ; i=i+1)//Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
	{  
		B = img[i];// Pixel Azul
		G = img[i+1];// Pixel Verde
		R = img[i+2];// Pixel Rojo
		Y= R*0.3+G*0.59+B*0.11;//Conversion a escala de grises
		img[i]=Y;// Pixel Azul
		img[i+1]=Y;// Pixel Verde
		img[i+2]=Y;

		i = i + 3;
	}
	//pthread_barrier_wait(&mybarrier)
	return (void*) img;


}
void * hebrasbinario(void* entrada){
	unsigned char* img=(unsigned char*) entrada;
	int offset = img[10]*pow(256,0) + img[11]*pow(256,1) + img[12]*pow(256,2);
	int imgWidth = img[18]*pow(256,0) + img[19]*pow(256,1) + img[20]*pow(256,2) + img[21]*pow(256,3);//Los bytes 19, 20, 21 y 22 contienen el ancho de la imagen.
	int imgHeight = img[22]*pow(256,0) + img[23]*pow(256,1) + img[24]*pow(256,2) + img[25]*pow(256,3);//Los bytes 23, 24, 25 y 26 contienen el largo de la imagen.
	int headerSize = (int)img[2];//El tamaño del header esta escrito en el 3er byte de la data.
	int fileSize = (imgWidth * imgHeight)*4 + headerSize;
	int R;
	int Y;
	int i;
	int numerohebra=img[fileSize];
	int aux1=2;
	int aux2=2;
	if (cantidadHebras>2){
		while(aux1<=cantidadHebras){
		aux1=aux1*2;
		}
		aux1=aux1/2;
		aux2=numerohebra;	
	}
	else{
		aux1=cantidadHebras;
		aux2=numerohebra;
	}
	int empezar=(aux2*(imgWidth * imgHeight)*4)/aux1;
	int terminar=empezar + ((imgWidth * imgHeight)*4)/aux1;
	pthread_mutex_unlock(&lock);
	if (terminar>fileSize)
	{
		terminar=fileSize;
	}
	pthread_mutex_lock(&lock);
	for(i = headerSize+ empezar; i < headerSize+terminar ; i=i+1)//Se itera sobre todos los bytes de imagen, los cuales comienzan desde el byte inferior izquierdo
	{  
		if(img[i] > umbralBinarizacion)
		{
			img[i]=255;
			img[i+1]=255;
			img[i+2]=255;
			white= white+1; // Si el pixel de escala de grises es mayor que el umbral, el pixel binarizado es 1.
			
		}
		else {
			img[i]=0;
			img[i+1]=0;
			img[i+2]=0;
			black= black+1;
		}
		i = i + 3;
	}
	pthread_mutex_unlock(&lock);
	//pthread_barrier_wait(&mybarrier);
	return (void*) img;


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

//Descripción:
//Entrada:
//Salida:

//Descripción:
//Entrada:
//Salida:


