make compile: 
	gcc main.c -o main
	gcc lectorImagen.c -o lectorImagen
	gcc imagenGris.c -o imagenGris
	./main -c 2 -u 254 -n 50 -b
