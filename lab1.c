#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "structs.c"
#include "functions.c"

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

char *img;
img = leerImagen("ngc2023.bmp");
printf("asd");
int i;
for(i = 0; i < 20000; i++)
{
	printf("%d .. ", img[i]);
}


}//END MAIN





















