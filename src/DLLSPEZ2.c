// Andrea Grandieri https://andreagrandieri.github.io/
// Visita il mio nuovo sito: https://giusp.com

#ifndef DLLSPEZ2_C
#define DLLSPEZ2_C

#include <stdlib.h>

#include "defines.c"
#include "BSTSPEZ4.c"

//////////////////////////////////////////////////////

typedef struct DLLSPEZ2 {
    // typedef struct Ordine {
    CoreSPEZ4* ricetta;
    int elementiOrdinati;
    int istanteTempo;
    int peso;
    int evaso;
    // } Ordine;

    struct DLLSPEZ2* n;
    struct DLLSPEZ2* p;
} DLLSPEZ2;

typedef struct DLLSPEZ2Root {
    DLLSPEZ2* h;
    DLLSPEZ2* t;
} DLLSPEZ2Root;

//////////////////////////////////////////////////////

// OK
// COSTO COSTANTE
void DLLSPEZ2_addNodeToHead(DLLSPEZ2Root* r, DLLSPEZ2* n) {
    if (!r || !n) {
        return;
    }

    n->p = NULL;
    n->n = r->h;

    if (r->h) {
        r->h->p = n;
    }
    else {
        r->t = n;
    }

    r->h = n;
}

// OK
// COSTO COSTANTE
void DLLSPEZ2_addNodeToTail(DLLSPEZ2Root* r, DLLSPEZ2* n) {
    if (!r || !n) {
        return;
    }

    n->n = NULL;
    n->p = r->t;

    if (r->t) {
        r->t->n = n;
    }
    else {
        r->h = n;
    }

    r->t = n;
}

// OK
// COSTO COSTANTE
DLLSPEZ2* DLLSPEZ2_removeNodeFromHead(DLLSPEZ2Root* r) {
    if (!r) {
        return NULL;
    }

    // Blocco tentativo di rimozione da lista vuota
    if (!r->h) {
        return NULL;
    }

    DLLSPEZ2* n = r->h;
    r->h = n->n;

    if (r->h) {
        r->h->p = NULL;
    }
    else {
        r->t = NULL;
    }

    return n;
}

// OK
// COSTO COSTANTE
DLLSPEZ2* DLLSPEZ2_removeNodeFromTail(DLLSPEZ2Root* r) {
    if (!r) {
        return NULL;
    }

    // Blocco tentativo di rimozione da lista vuota
    if (!r->h) {
        return NULL;
    }

    DLLSPEZ2* n = r->t;
    r->t = n->p;

    if (r->t) {
        r->t->n = NULL;
    }
    else {
        r->h = NULL;
    }

    return n;
}

// OK
// COSTO COSTANTE
void DLLSPEZ2_addBefore(DLLSPEZ2Root* r, DLLSPEZ2* rf, DLLSPEZ2* n) {
    if (!r || !rf || !n) {
        return;
    }

    if (!rf->p) {
        DLLSPEZ2_addNodeToHead(r, n);
        return;
    }

    rf->p->n = n;
    n->p = rf->p;
    
    rf->p = n;
    n->n = rf;
}

// OK
// COSTO COSTANTE
void DLLSPEZ2_addAfter(DLLSPEZ2Root* r, DLLSPEZ2* rf, DLLSPEZ2* n) {
    if (!r || !rf || !n) {
        return;
    }

    if (!rf->n) {
        DLLSPEZ2_addNodeToTail(r, n);
        return;
    }

    rf->n->p = n;
    n->n = rf->n;

    rf->n = n;
    n->p = rf;

}

// OK
// COSTO COSTANTE
DLLSPEZ2* DLLSPEZ2_removeNode(DLLSPEZ2Root* r, DLLSPEZ2* n) {
    if (!r || !n) {
        return NULL;
    }

    // Blocco tentativo di rimozione da lista vuota
    if (!r->h) {
        return NULL;
    }

    if (!n->p) {
        return DLLSPEZ2_removeNodeFromHead(r);
    }

    if (!n->n) {
        return DLLSPEZ2_removeNodeFromTail(r);
    }

    n->p->n = n->n;
    n->n->p = n->p;

    return n;
}

// OK
// COSTO COSTANTE
void DLLSPEZ2_swapNodes(DLLSPEZ2Root* r, DLLSPEZ2* ln, DLLSPEZ2* rn) {
    if (!r || !ln || !rn) {
        return;
    }

    ln->n = rn->n;

    if (!rn->n) {
        r->t = ln;
    }
    else {
        rn->n->p = ln;
    }

    rn->p = ln->p;

    if (!ln->p) {
        r->h = rn;
    }
    else {
        ln->p->n = rn;
    }

    rn->n = ln;
    ln->p = rn;
}

//////////////////////////////////////////////////////

// OK
DLLSPEZ2Root* DLLSPEZ2_createRoot() {
    DLLSPEZ2Root* r = MALLOC(DLLSPEZ2Root);
    r->h = NULL;
    r->t = NULL;
    return r;
}

// OK
DLLSPEZ2* DLLSPEZ2_createNode(CoreSPEZ4* ricetta, int elementiOrdinati, int istanteTempo, int evaso) {
    DLLSPEZ2* n = MALLOC(DLLSPEZ2);
    n->p = NULL;
    n->n = NULL;

    n->ricetta = ricetta;
    n->elementiOrdinati = elementiOrdinati;
    n->istanteTempo = istanteTempo;
    n->evaso = evaso;
    
    return n;
}

// OK
void DLLSPEZ2_destroyRoot(DLLSPEZ2Root* r) {
    free(r);
}

// OK
void DLLSPEZ2_destroyNode(DLLSPEZ2* n) {
    free(n);
}

//////////////////////////////////////////////////////

#endif // !DLLSPEZ2_C
