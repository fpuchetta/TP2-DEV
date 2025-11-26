#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "stdlib.h"

#include "src/tp2_est.h"

int main(int argc, char *argv[])
{
	tp2_t *tp2;

	if (argc > 1) {
		tp2 = tp2_crear(argv[1]);
	} else {
		tp2 = tp2_crear(NULL);
	}

	if (!tp2) {
		printf("Error al crear el tp2\n");
		return -1;
	}

	if (!tp2_ejecutar(tp2)) {
		tp2_destruir_todo(tp2);
		printf("Error al correr el tp2\n");
		return -1;
	}

	tp2_destruir_todo(tp2);
	return 0;
}