// Andrea Grandieri https://andreagrandieri.github.io/
// Visita il mio nuovo sito: https://giusp.com

#ifndef BSTSPEZ6_C
#define BSTSPEZ6_C

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defines.c"
#include "DLLSPEZ2.c"

//////////////////////////////////////////////////////

typedef struct NodeSPEZ6 {
    // Key: peso
    DLLSPEZ2* ordine;

    struct NodeSPEZ6* p;
    struct NodeSPEZ6* l;
    struct NodeSPEZ6* r;
} NodeSPEZ6;

//////////////////////////////////////////////////////

// Comparator

// OK
int comparatorSPEZ6(int kl, int kr) {
    return (kl == kr ? 0 : (kl < kr ? -1 : 1)); 
}

//////////////////////////////////////////////////////

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ6* BSTSPEZ6_min(NodeSPEZ6* n) {
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
NodeSPEZ6* BSTSPEZ6_max(NodeSPEZ6* n) {
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
void BSTSPEZ6_addNode(NodeSPEZ6** bstro, NodeSPEZ6* n) {
    if (!bstro || !n) {
        return;
    }

    NodeSPEZ6* ro = *bstro;
    NodeSPEZ6* last = NULL;

    while (ro) {
        last = ro;

        if (comparatorSPEZ6(n->ordine->peso, ro->ordine->peso) < 0) {
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

    if (comparatorSPEZ6(n->ordine->peso, last->ordine->peso) < 0) {
        last->l = n;
    }
    else {
        last->r = n;
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
void BSTSPEZ6_addNodeOv(NodeSPEZ6** bstro, NodeSPEZ6* n) {
    if (!bstro || !n) {
        return;
    }

    NodeSPEZ6* ro = *bstro;
    NodeSPEZ6* last = NULL;

    while (ro) {
        last = ro;

        if (comparatorSPEZ6(n->ordine->peso, ro->ordine->peso) <= 0) {
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

    if (comparatorSPEZ6(n->ordine->peso, last->ordine->peso) <= 0) {
        last->l = n;
    }
    else {
        last->r = n;
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ6* BSTSPEZ6_successor(NodeSPEZ6* n) {
    if (!n) {
        return NULL;
    }

    if (n->r) {
        return BSTSPEZ6_min(n->r);
    }

    NodeSPEZ6* np = n->p;

    while (np && n == np->r) {
        n = np;
        np = np->p;
    }

    return np;
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ6* BSTSPEZ6_search(NodeSPEZ6* n, int k) {
    if (!n || !k) {
        return NULL;
    }

    int comparatorRes = comparatorSPEZ6(k, n->ordine->peso);

    if (comparatorRes == 0) {
        return n;
    } else if (comparatorRes < 0) {
        return BSTSPEZ6_search(n->l, k);
    }
    else {
        return BSTSPEZ6_search(n->r, k);
    }
}

// OK
// COSTO: O(h) con h altezza dell'albero; h = log(n) con n numero di nodi nell'albero se l'albero è bilanciato o quasi bilanciato.
NodeSPEZ6* BSTSPEZ6_removeNode(NodeSPEZ6** bstro, NodeSPEZ6* n, int* succIsDel) {
    NodeSPEZ6* del = NULL;
    NodeSPEZ6* swap = NULL;

    // Selezione del nodo da eliminare
    if (!n->l || !n->r) {
        del = n;
    }
    else {
        del = BSTSPEZ6_successor(n);
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
        DLLSPEZ2* ordine = del->ordine;

        del->ordine = n->ordine;

        n->ordine = ordine;

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
NodeSPEZ6* BSTSPEZ6_createNode(DLLSPEZ2* ordine) {
    NodeSPEZ6* n = MALLOC(NodeSPEZ6);
    n->p = NULL;
    n->l = NULL;
    n->r = NULL;

    n->ordine = ordine;
    return n;
}

// OK
void BSTSPEZ6_destroyNode(NodeSPEZ6* n) {
    free(n);
}

//////////////////////////////////////////////////////

// OK
// COSTO O(n) con n numero di nodi nell'albero
void rinorderSPEZ6(NodeSPEZ6* r, void(*cb)(NodeSPEZ6*)) {
    if (!r || !cb) {
        return;
    }

    rinorderSPEZ6(r->r, cb);
    cb(r);
    rinorderSPEZ6(r->l, cb);
}

// OK
// COSTO O(n) con n numero di nodi nell'albero
void postorder2SPEZ6(NodeSPEZ6** bstro, NodeSPEZ6* r, void(*cb)(NodeSPEZ6**, NodeSPEZ6*)) {
    if (!r || !cb) {
        return;
    }

    postorder2SPEZ6(bstro, r->l, cb);
    postorder2SPEZ6(bstro, r->r, cb);
    cb(bstro, r);
}

//////////////////////////////////////////////////////

#endif // !BSTSPEZ6_C
