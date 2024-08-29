// Andrea Grandieri https://andreagrandieri.github.io/
// Visita il mio nuovo sito: https://giusp.com

#ifndef BSTSPEZ4_C
#define BSTSPEZ4_C

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defines.c"
#include "ElementoRicetta.c"

//////////////////////////////////////////////////////

typedef struct CoreSPEZ4 {
    // typedef struct Ricetta {
    char* nome;
    int ordiniPendenti;
    int size;
    ElementoRicetta* ingredienti;
    // } Ricetta;
} CoreSPEZ4;

typedef struct NodeSPEZ4 {
    // Key: nome
    CoreSPEZ4* d;

    struct NodeSPEZ4* p;
    struct NodeSPEZ4* l;
    struct NodeSPEZ4* r;
} NodeSPEZ4;

//////////////////////////////////////////////////////

// Comparator

// OK
int comparatorSPEZ4(char* kl, char* kr) {
    return strcmp(kl, kr);
}

//////////////////////////////////////////////////////

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ4* BSTSPEZ4_min(NodeSPEZ4* n) {
    if (!n) {
        return NULL;
    }

    while (n->l) {
        n = n->l;
    }

    return n;
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ4* BSTSPEZ4_max(NodeSPEZ4* n) {
    if (!n) {
        return NULL;
    }

    while (n->r) {
        n = n->r;
    }

    return n;
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
void BSTSPEZ4_addNode(NodeSPEZ4** bstro, NodeSPEZ4* n) {
    if (!bstro || !n) {
        return;
    }

    NodeSPEZ4* ro = *bstro;
    NodeSPEZ4* last = NULL;

    while (ro) {
        last = ro;

        if (comparatorSPEZ4(n->d->nome, ro->d->nome) < 0) {
            ro = ro->l;
        }
        else {
            ro = ro->r;
        }
    }

    n->p = last;

    if (!last) {
        // Inserimento in albero vuoto
        *bstro = n;
        return;
    }

    if (comparatorSPEZ4(n->d->nome, last->d->nome) < 0) {
        last->l = n;
    }
    else {
        last->r = n;
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
void BSTSPEZ4_addNodeOv(NodeSPEZ4** bstro, NodeSPEZ4* n) {
    if (!bstro || !n) {
        return;
    }

    NodeSPEZ4* ro = *bstro;
    NodeSPEZ4* last = NULL;

    while (ro) {
        last = ro;

        if (comparatorSPEZ4(n->d->nome, ro->d->nome) <= 0) {
            ro = ro->l;
        }
        else {
            ro = ro->r;
        }
    }

    n->p = last;

    if (!last) {
        // Inserimento in albero vuoto
        *bstro = n;
        return;
    }

    if (comparatorSPEZ4(n->d->nome, last->d->nome) <= 0) {
        last->l = n;
    }
    else {
        last->r = n;
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ4* BSTSPEZ4_successor(NodeSPEZ4* n) {
    if (!n) {
        return NULL;
    }

    if (n->r) {
        return BSTSPEZ4_min(n->r);
    }

    NodeSPEZ4* np = n->p;

    while (np && n == np->r) {
        n = np;
        np = np->p;
    }

    return np;
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ4* BSTSPEZ4_search(NodeSPEZ4* n, char* k) {
    if (!n || !k) {
        return NULL;
    }

    int comparatorRes = comparatorSPEZ4(k, n->d->nome);

    if (comparatorRes == 0) {
        return n;
    } else if (comparatorRes < 0) {
        return BSTSPEZ4_search(n->l, k);
    }
    else {
        return BSTSPEZ4_search(n->r, k);
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ4* BSTSPEZ4_removeNode(NodeSPEZ4** bstro, NodeSPEZ4* n, int* succIsDel) {
    NodeSPEZ4* del = NULL;
    NodeSPEZ4* swap = NULL;

    // Selezione del nodo da eliminare
    if (!n->l || !n->r) {
        del = n;
    }
    else {
        del = BSTSPEZ4_successor(n);
    }

    // Blocco di identificazione del sottoalbero da spostare
    if (del->l) {
        swap = del->l;
    }
    else {
        swap = del->r;
    }

    // Effettivo blocco di spostamento del sottoalbero
    if (swap) {
        swap->p = del->p;
    }

    // Effettivo blocco di eliminazione e spostamento del sottoalbero
    if (!del->p) {
        *bstro = swap;
    }
    else if (del == del->p->l) {
        del->p->l = swap;
    }
    else {
        del->p->r = swap;
    }

    // Blocco di swap di k e d per il caso 3 in caso di eliminazione di del e non di n:
    // salvo k e d di del in n mentre metto in del k e d di n.
    if (del != n) {
        CoreSPEZ4* c = del->d;

        del->d = n->d;

        n->d = c;

        if (succIsDel) {
            *succIsDel = 1;
        }
    } else {
        if (succIsDel) {
            *succIsDel = 0;
        }
    }

    return del;
}

//////////////////////////////////////////////////////

// OK
NodeSPEZ4* BSTSPEZ4_createNode(char* nome, int ordiniPendenti, int size) {
    CoreSPEZ4* c = MALLOC(CoreSPEZ4);
    c->nome = malloc(strlen(nome) + 1);
    strcpy(c->nome, nome);
    c->ordiniPendenti = ordiniPendenti;
    c->size = size;
    c->ingredienti = malloc(size * sizeof(ElementoRicetta));

    NodeSPEZ4* n = MALLOC(NodeSPEZ4);
    n->p = NULL;
    n->l = NULL;
    n->r = NULL;
    n->d = c;

    return n;
}

// OK
void BSTSPEZ4_destroyNode(NodeSPEZ4* n) {
    // free of CoreSPEZ4 is no responsibility of this function

    free(n);
}

// OK
void BSTSPEZ4_destroyCore(CoreSPEZ4* c) {
    free(c);
}

//////////////////////////////////////////////////////

#endif // !BSTSPEZ4_C
