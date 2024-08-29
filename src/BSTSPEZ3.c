// Andrea Grandieri https://andreagrandieri.github.io/
// Visita il mio nuovo sito: https://giusp.com

#ifndef BSTSPEZ3_C
#define BSTSPEZ3_C

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defines.c"

//////////////////////////////////////////////////////

typedef struct NodeSPEZ3 {
    // Key: istanteScadenza
    // typedef struct Scorta {
    int quantita;
    int istanteScadenza;
    // } Scorta;

    struct NodeSPEZ3* p;
    struct NodeSPEZ3* l;
    struct NodeSPEZ3* r;
} NodeSPEZ3;

//////////////////////////////////////////////////////

// Comparator

// OK
int comparatorSPEZ3(int kl, int kr) {
    return (kl == kr ? 0 : (kl < kr ? -1 : 1)); 
}

//////////////////////////////////////////////////////

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ3* BSTSPEZ3_min(NodeSPEZ3* n) {
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
NodeSPEZ3* BSTSPEZ3_max(NodeSPEZ3* n) {
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
void BSTSPEZ3_addNode(NodeSPEZ3** bstro, NodeSPEZ3* n) {
    if (!bstro || !n) {
        return;
    }

    NodeSPEZ3* ro = *bstro;
    NodeSPEZ3* last = NULL;

    while (ro) {
        last = ro;

        if (comparatorSPEZ3(n->istanteScadenza, ro->istanteScadenza) < 0) {
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

    if (comparatorSPEZ3(n->istanteScadenza, last->istanteScadenza) < 0) {
        last->l = n;
    }
    else {
        last->r = n;
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
void BSTSPEZ3_addNodeOv(NodeSPEZ3** bstro, NodeSPEZ3* n) {
    if (!bstro || !n) {
        return;
    }

    NodeSPEZ3* ro = *bstro;
    NodeSPEZ3* last = NULL;

    while (ro) {
        last = ro;

        if (comparatorSPEZ3(n->istanteScadenza, ro->istanteScadenza) <= 0) {
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

    if (comparatorSPEZ3(n->istanteScadenza, last->istanteScadenza) <= 0) {
        last->l = n;
    }
    else {
        last->r = n;
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ3* BSTSPEZ3_successor(NodeSPEZ3* n) {
    if (!n) {
        return NULL;
    }

    if (n->r) {
        return BSTSPEZ3_min(n->r);
    }

    NodeSPEZ3* np = n->p;

    while (np && n == np->r) {
        n = np;
        np = np->p;
    }

    return np;
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ3* BSTSPEZ3_search(NodeSPEZ3* n, int k) {
    if (!n || !k) {
        return NULL;
    }

    int comparatorRes = comparatorSPEZ3(k, n->istanteScadenza);

    if (comparatorRes == 0) {
        return n;
    } else if (comparatorRes < 0) {
        return BSTSPEZ3_search(n->l, k);
    }
    else {
        return BSTSPEZ3_search(n->r, k);
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ3* BSTSPEZ3_removeNode(NodeSPEZ3** bstro, NodeSPEZ3* n, int* succIsDel) {
    NodeSPEZ3* del = NULL;
    NodeSPEZ3* swap = NULL;

    // Selezione del nodo da eliminare
    if (!n->l || !n->r) {
        del = n;
    }
    else {
        del = BSTSPEZ3_successor(n);
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
        int quantita = del->quantita;
        int istanteScadenza = del->istanteScadenza;

        del->quantita = n->quantita;
        del->istanteScadenza = n->istanteScadenza;

        n->quantita = quantita;
        n->istanteScadenza = istanteScadenza;

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
NodeSPEZ3* BSTSPEZ3_createNode(int istanteScadenza, int quantita) {
    NodeSPEZ3* n = MALLOC(NodeSPEZ3);
    n->p = NULL;
    n->l = NULL;
    n->r = NULL;

    n->istanteScadenza = istanteScadenza;
    n->quantita = quantita;
    return n;
}

void BSTSPEZ3_destroyNode(NodeSPEZ3* n) {
    free(n);
}

//////////////////////////////////////////////////////

#endif // !BSTSPEZ3_C
