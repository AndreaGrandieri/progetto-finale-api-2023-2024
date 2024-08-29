// Andrea Grandieri https://andreagrandieri.github.io/
// Visita il mio nuovo sito: https://giusp.com

#ifndef BSTSPEZ5_C
#define BSTSPEZ5_C

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defines.c"
#include "BSTSPEZ3.c"

//////////////////////////////////////////////////////

typedef struct NodeSPEZ5 {
    // Key: nome
    // typedef struct Ingrediente {
    char* nome;
    int tot;
    // int usatoCount;
    NodeSPEZ3* scortero;
    // } Ingrediente;

    struct NodeSPEZ5* p;
    struct NodeSPEZ5* l;
    struct NodeSPEZ5* r;
} NodeSPEZ5;

//////////////////////////////////////////////////////

// Comparator

// OK
int comparatorSPEZ5(char* kl, char* kr) {
    return strcmp(kl, kr);
}

//////////////////////////////////////////////////////

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ5* BSTSPEZ5_min(NodeSPEZ5* n) {
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
NodeSPEZ5* BSTSPEZ5_max(NodeSPEZ5* n) {
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
void BSTSPEZ5_addNode(NodeSPEZ5** bstro, NodeSPEZ5* n) {
    if (!bstro || !n) {
        return;
    }

    NodeSPEZ5* ro = *bstro;
    NodeSPEZ5* last = NULL;

    while (ro) {
        last = ro;

        if (comparatorSPEZ5(n->nome, ro->nome) < 0) {
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

    if (comparatorSPEZ5(n->nome, last->nome) < 0) {
        last->l = n;
    }
    else {
        last->r = n;
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
void BSTSPEZ5_addNodeOv(NodeSPEZ5** bstro, NodeSPEZ5* n) {
    if (!bstro || !n) {
        return;
    }

    NodeSPEZ5* ro = *bstro;
    NodeSPEZ5* last = NULL;

    while (ro) {
        last = ro;

        if (comparatorSPEZ5(n->nome, ro->nome) <= 0) {
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

    if (comparatorSPEZ5(n->nome, last->nome) <= 0) {
        last->l = n;
    }
    else {
        last->r = n;
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ5* BSTSPEZ5_successor(NodeSPEZ5* n) {
    if (!n) {
        return NULL;
    }

    if (n->r) {
        return BSTSPEZ5_min(n->r);
    }

    NodeSPEZ5* np = n->p;

    while (np && n == np->r) {
        n = np;
        np = np->p;
    }

    return np;
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ5* BSTSPEZ5_search(NodeSPEZ5* n, char* k) {
    if (!n || !k) {
        return NULL;
    }

    int comparatorRes = comparatorSPEZ5(k, n->nome);

    if (comparatorRes == 0) {
        return n;
    } else if (comparatorRes < 0) {
        return BSTSPEZ5_search(n->l, k);
    }
    else {
        return BSTSPEZ5_search(n->r, k);
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ5* BSTSPEZ5_removeNode(NodeSPEZ5** bstro, NodeSPEZ5* n, int* succIsDel) {
    NodeSPEZ5* del = NULL;
    NodeSPEZ5* swap = NULL;

    // Selezione del nodo da eliminare
    if (!n->l || !n->r) {
        del = n;
    }
    else {
        del = BSTSPEZ5_successor(n);
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
        char* nome = malloc(strlen(del->nome) + 1);
        strcpy(nome, del->nome);

        int tot = del->tot;
        // int usatoCount = del->usatoCount;
        NodeSPEZ3* scortero = del->scortero;

        free(del->nome);
        del->nome = malloc(strlen(n->nome) + 1);
        strcpy(del->nome, n->nome);

        del->tot = n->tot;
        // del->usatoCount = n->usatoCount;
        del->scortero = n->scortero;

        free(n->nome);
        n->nome = malloc(strlen(nome) + 1);
        strcpy(n->nome, nome);

        n->tot = tot;
        // n->usatoCount = usatoCount;
        n->scortero = scortero;

        free(nome);

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
NodeSPEZ5* BSTSPEZ5_createNode(char* nome, int tot, /*int usatoCount,*/ NodeSPEZ3* scortero) {
    NodeSPEZ5* n = MALLOC(NodeSPEZ5);
    n->p = NULL;
    n->l = NULL;
    n->r = NULL;

    n->nome = malloc(strlen(nome) + 1);
    strcpy(n->nome, nome);
    n->tot = tot;
    // n->usatoCount = usatoCount;
    n->scortero = scortero;
    return n;
}

// OK
void BSTSPEZ5_destroyNode(NodeSPEZ5* n) {
    free(n->nome);
    free(n);
}

//////////////////////////////////////////////////////

#endif // !BSTSPEZ5_C
