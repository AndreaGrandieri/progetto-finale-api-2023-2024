// Andrea Grandieri https://andreagrandieri.github.io/
// Visita il mio nuovo sito: https://giusp.com

#ifndef ELEMENTORICETTA_C
#define ELEMENTORICETTA_C

#include <stdlib.h>

#include "defines.c"
#include "BSTSPEZ5.c"

typedef struct ElementoRicetta {
    NodeSPEZ5* ingrediente;
    int quantita;
} ElementoRicetta;

#endif // !ELEMENTORICETTA_C
