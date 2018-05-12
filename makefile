make compile: 
	gcc main.c -o main
	gcc lectorImagen.c -o lectorImagen
	gcc imagenGris.c -o imagenGris
	gcc Binarizacion.c -o Binarizacion
	gcc escritor.c -o escritor
	gcc analisis.c -o analisis
	./main -c 3 -u 90 -n 50 -b 0
