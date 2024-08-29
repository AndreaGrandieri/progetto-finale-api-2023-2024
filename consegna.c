// Andrea Grandieri https://andreagrandieri.github.io/
// Visita il mio nuovo sito: https://giusp.com

//////////////////////////////////////////////////////

/*
Discussioni sulla sicurezza:

Assunzione: tutti gli input del tester sono sani.
*/

//////////////////////////////////////////////////////

/*
SPEZN perkaholic map

DLLSPEZ2: specializza la struct Ordine all'interno della DLL
BSTSPEZ3: specializza la struct Scorta all'interno del BST
BSTSPEZ4: specializza la struct Ricetta all'interno del BST
BSTSPEZ5: specializza la struct Ingrediente all'interno del BST
BSTSPEZ6: specializza il BST utilizzato come "bstSpedizione"
*/

/*
Direct NodeSPEZN bad references

DLLSPEZ2: NodeSPEZ4 no more. Now it uses CoreSPEZ4 that is safe.
BSTSPEZ3: safe
BSTSPEZ4: safe
BSTSPEZ5: safe
BSTSPEZ6: safe
*/

//////////////////////////////////////////////////////

/*
Topological sort for inclusion in single source code file

1) <string.h>
2) <stdlib.h>
3) <stdio.h>
4) "defines.c"
5) Z3
6) Z5
7) "ElementoRicetta.c"
8) Z4
9) Z2
10) Z6
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// "defines.c"

#define MALLOC(T) (T*)malloc(sizeof(T))
#define MAX_LINE_LENGTH 25451

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// "BSTSPEZ3.c"

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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// "BSTSPEZ5.c"

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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// "ElementoRicetta.c"

typedef struct ElementoRicetta {
    NodeSPEZ5* ingrediente;
    int quantita;
} ElementoRicetta;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// "BSTSPEZ4.c"

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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// "DLLSPEZ2.c"

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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
// "BSTSPEZ6.c"

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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////

// Altre struct

//////////////////////////////////////////////////////

// Globali
NodeSPEZ5* dbIngredientiro;
NodeSPEZ4* dbRicettero;
DLLSPEZ2Root* dbOrdini;

NodeSPEZ6* bstSpedizionero;

int istanteTempoUniversale = -1;
int periodicitaCorriere = -1;
int capienzaCorriere = -1;
char* globalReadBuf = NULL;
char* globalHelperBuf = NULL;

//////////////////////////////////////////////////////

// OK
NodeSPEZ4* creaEAggiungiAlDBRicette(char* nome, int s) {
    NodeSPEZ4* n = BSTSPEZ4_createNode(nome, 0, s);
    BSTSPEZ4_addNode(&dbRicettero, n);
    return n;
}

// OK
int rimuoviDalDBRicette(NodeSPEZ4* nRicetta) {
    if (nRicetta->d->ordiniPendenti > 0) {
        return -1;
    }

    NodeSPEZ4* removedNRicetta = BSTSPEZ4_removeNode(&dbRicettero, nRicetta, NULL);

    /// FREE ///
    free(removedNRicetta->d->ingredienti);
    free(removedNRicetta->d->nome);
    BSTSPEZ4_destroyCore(removedNRicetta->d);
    BSTSPEZ4_destroyNode(removedNRicetta);
    /// //// ///

    return 0;
}

// OK
NodeSPEZ5* creaEAggiungiAlDBIngredienti(char* nome) {
    NodeSPEZ5* n = BSTSPEZ5_createNode(nome, 0, NULL);
    BSTSPEZ5_addNode(&dbIngredientiro, n);
    return n;
}

/*
int rimuoviDalDBIngredienti(NodeSPEZ5* nIngrediente) {
    int succIsDel = -1;
    NodeSPEZ5* removedNIngrediente = BSTSPEZ5_removeNode(dbIngredienti, nIngrediente, &succIsDel);

    // Un ingrediente rimosso con questa funzione è garantito avere:
    // - usatoCount a 0
    // - 0 scorte

    /// FREE ///
    BSTSPEZ3_destroyRoot(removedNIngrediente->scorte);
    BSTSPEZ5_destroyNode(removedNIngrediente);
    /// //// ///

    return succIsDel;
}
*/

// OK
void creaEAggiungiAllaRicetta(NodeSPEZ4* nRicetta, char* nome, int quantita, int offset) {
    NodeSPEZ5* nIngrediente = BSTSPEZ5_search(dbIngredientiro, nome);

    if (!nIngrediente) {
        nIngrediente = creaEAggiungiAlDBIngredienti(nome);
    }

    (nRicetta->d->ingredienti + offset)->ingrediente = nIngrediente;
    (nRicetta->d->ingredienti + offset)->quantita = quantita;

    // nIngrediente->usatoCount++;
}

// OK
NodeSPEZ4* ricettaByName(char* nome) {
    return BSTSPEZ4_search(dbRicettero, nome);
}

// OK
int calcolaPesoOrdine(DLLSPEZ2* nOrdine) {
    int peso = 0;

    for (int i = 0; i < nOrdine->ricetta->size; i++) {
        peso += nOrdine->ricetta->ingredienti[i].quantita;
    }

    peso *= nOrdine->elementiOrdinati;

    return peso;
}

// OK
DLLSPEZ2* creaEAggiungiAlDBOrdini(NodeSPEZ4* nRicetta, int elementiOrdinati) {
    DLLSPEZ2* nOrdine = DLLSPEZ2_createNode(nRicetta->d, elementiOrdinati, istanteTempoUniversale, 0);
    nOrdine->peso = calcolaPesoOrdine(nOrdine);

    nRicetta->d->ordiniPendenti++;

    DLLSPEZ2_addNodeToHead(dbOrdini, nOrdine);
    return nOrdine;
}

// OK
void creaEAggiungiScorta(char* nome, int quantita, int istanteScadenza) {
    NodeSPEZ5* nIngrediente = BSTSPEZ5_search(dbIngredientiro, nome);

    if (!nIngrediente) {
        nIngrediente = creaEAggiungiAlDBIngredienti(nome);
    }

    nIngrediente->tot += quantita;

    NodeSPEZ3* nScorta = BSTSPEZ3_createNode(istanteScadenza, quantita);
    BSTSPEZ3_addNode(&nIngrediente->scortero, nScorta);
}

// OK
int rimuoviScorta(NodeSPEZ5* nIngrediente, NodeSPEZ3* nScorta) {
    int succIsDel = -1;
    nIngrediente->tot -= nScorta->quantita;
    NodeSPEZ3* removedNScorta = BSTSPEZ3_removeNode(&nIngrediente->scortero, nScorta, &succIsDel);

    /// FREE ///
    BSTSPEZ3_destroyNode(removedNScorta);
    /// //// ///

    return succIsDel;
}

// OK
int cleanScorte(NodeSPEZ5* nIngrediente) {
    NodeSPEZ3* nScorta = BSTSPEZ3_min(nIngrediente->scortero);
    NodeSPEZ3* nNext = NULL;

    while (nScorta) {
        nNext = BSTSPEZ3_successor(nScorta);

        if (nScorta->quantita == 0) {
            // Scorta esaurita: rimuovo
            if (rimuoviScorta(nIngrediente, nScorta)) {
                continue;
            } else {
                nScorta = nNext;
                continue;
            }
        }

        if (nScorta->istanteScadenza <= istanteTempoUniversale) {
            // Scorta scaduta: rimuovo
            if (rimuoviScorta(nIngrediente, nScorta)) {
                continue;
            } else {
                nScorta = nNext;
                continue;
            }
        } else {
            break;
        }

        nScorta = nNext;
    }

    /*
    if (nIngrediente->usatoCount == 0 && !nIngrediente->scorte->ro) {
        // Rimuovo ingrediente dal DBIngredienti
        return rimuoviDalDBIngredienti(nIngrediente);
    }
    */

    return -1;
}

// OK
void cleanScorteScoped(DLLSPEZ2* nOrdine) {
    for (int i = 0; i < nOrdine->ricetta->size; i++) {
        cleanScorte(nOrdine->ricetta->ingredienti[i].ingrediente);
    }
}

// OK
void cleanScorteWide() {
    NodeSPEZ5* nIngrediente = BSTSPEZ5_min(dbIngredientiro);

    while (nIngrediente) {
        if (cleanScorte(nIngrediente) == 1) {
            continue;
        } else {
            nIngrediente = BSTSPEZ5_successor(nIngrediente);
        }
    }
}

// OK
int checkDisponibilitaScorta(NodeSPEZ5* nIngrediente, int quantitaRichiesta) {
    return quantitaRichiesta <= nIngrediente->tot;
}

// OK
void consumaScorta(NodeSPEZ5* nIngrediente, int quantitaRichiesta) {
    // Questa funzione deve essere chiamata con la promessa di possibile consumazione totale

    NodeSPEZ3* nScorta = BSTSPEZ3_min(nIngrediente->scortero);

    // Operazione sicura basata sulla promessa
    nIngrediente->tot -= quantitaRichiesta;

    while (nScorta) {
        if (quantitaRichiesta <= nScorta->quantita) {
            nScorta->quantita -= quantitaRichiesta;
            quantitaRichiesta = 0;
            return;
        } else {
            quantitaRichiesta -= nScorta->quantita;
            nScorta->quantita = 0;
        }

        nScorta = BSTSPEZ3_successor(nScorta);
    }
}

// OK
void preparaOrdine(DLLSPEZ2* nOrdine) {
    for (int i = 0; i < nOrdine->ricetta->size; i++) {
        if (!checkDisponibilitaScorta(nOrdine->ricetta->ingredienti[i].ingrediente, nOrdine->ricetta->ingredienti[i].quantita * nOrdine->elementiOrdinati)) {
            return;
        }
    }

    for (int i = 0; i < nOrdine->ricetta->size; i++) {
        consumaScorta(nOrdine->ricetta->ingredienti[i].ingrediente, nOrdine->ricetta->ingredienti[i].quantita * nOrdine->elementiOrdinati);
    }

    nOrdine->evaso = 1;
}

// OK
void preparaOrdiniInAttesa() {
    DLLSPEZ2* t = dbOrdini->t;

    while (t) {
        if (!t->evaso) {
            preparaOrdine(t);
        }

        t = t->p;
    }
}

// OK
void caricaNelCorriere(DLLSPEZ2* nOrdine) {
    printf("%d %s %d\n", nOrdine->istanteTempo, nOrdine->ricetta->nome, nOrdine->elementiOrdinati);
}

// OK
int arrivaIlCorriere() {
    if (istanteTempoUniversale < periodicitaCorriere) {
        return 0;
    }

    return (istanteTempoUniversale % periodicitaCorriere) == 0;
}

// OK
void compilaBstSpedizione(int capienzaCorriere) {
    DLLSPEZ2* t = dbOrdini->t;
    DLLSPEZ2* p = NULL;

    while (t) {
        p = t->p;

        if (t->evaso) {
            if (t->peso > capienzaCorriere) {
                break;
            }

            capienzaCorriere -= t->peso;
            t->ricetta->ordiniPendenti--;

            DLLSPEZ2* removedNOrdine = DLLSPEZ2_removeNode(dbOrdini, t);
            BSTSPEZ6_addNodeOv(&bstSpedizionero, BSTSPEZ6_createNode(removedNOrdine));
        }

        t = p;
    }
}

// OK
void spedisci_cb(NodeSPEZ6* n) {
    caricaNelCorriere(n->ordine);
}

// OK
void spedisci() {
    if (!bstSpedizionero) {
        printf("camioncino vuoto\n");
        return;
    }

    rinorderSPEZ6(bstSpedizionero, &spedisci_cb);
}

// OK
void decompilaBSTSpedizione_cb(NodeSPEZ6** bstro, NodeSPEZ6* n) {
    // succIsDel non può risultare mai vero in quanto la decompilazione dell'albero
    // avviene secondo il flusso traverse di postorder2SPEZ6
    BSTSPEZ6_removeNode(bstro, n, NULL);

    /// FREE ///
    DLLSPEZ2_destroyNode(n->ordine);
    BSTSPEZ6_destroyNode(n);
    /// //// ///
}

// OK
void decompilaBSTSpedizione() {
    postorder2SPEZ6(&bstSpedizionero, bstSpedizionero, &decompilaBSTSpedizione_cb);
}

//////////////////////////////////////////////////////

// Altri supporti di stampa

//////////////////////////////////////////////////////

// Supporto alla lettura dell'input

// OK
char* readFullLine() {
    char* ret = fgets(globalReadBuf, MAX_LINE_LENGTH, stdin);

    if (!ret) {
        return NULL;
    }

    // Rimozione di '\n'
    int len = strlen(globalReadBuf);
    globalReadBuf[len - 1] = '\0';

    return ret;
}

// OK
char* pullFirstToken() {
    return strtok(globalReadBuf, " ");
}

// OK
char* pullToken() {
    return strtok(NULL, " ");
}

// OK
int cycleAndCount(char* buf, char ch) {
    int count = 0;

    char* r = strchr(buf, ch);

    while (r) {
        count++;
        r++;
        r = strchr(r, ch);
    }

    return count;
}

//////////////////////////////////////////////////////

int main(int argc, char const* argv[]) {
    // Inizializzazione globali
    dbIngredientiro = NULL;
    dbRicettero = NULL;
    dbOrdini = DLLSPEZ2_createRoot();
    bstSpedizionero = NULL;

    // Inizializzazione buffer globali
    globalReadBuf = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
    globalHelperBuf = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));

    //////////////////////////////////////////////////////

    int inutile;

    // Primo input: due interi sono attesi
    inutile = scanf("%d", &periodicitaCorriere);
    inutile = scanf("%d", &capienzaCorriere);

    // Discard di \n leftover dalle due letture di integer con scanf "grezzo"
    getchar();

    //////////////////////////////////////////////////////

    char* token = NULL;
    char* token2 = NULL;
    char* token3 = NULL;

    while (1) {
        istanteTempoUniversale++;

        // Se arriva il corriere gestisco la sua scena prima di qualsiasi altro comando
        if (arrivaIlCorriere()) {
            compilaBstSpedizione(capienzaCorriere);
            spedisci();
            decompilaBSTSpedizione();
        }

        if (!readFullLine()) {
            break;
        }

        strcpy(globalHelperBuf, globalReadBuf);

        token = pullFirstToken();

        // Big "switch"
        if (strcmp(token, "aggiungi_ricetta") == 0) {
            // Rilevato comando: aggiungi_ricetta
            int quantita;

            // Leggo <nome_ricetta>
            token = pullToken();

            if (ricettaByName(token)) {
                printf("ignorato\n");
            } else {
                // Inserisco nuova ricetta vuota nel ricettario
                int sizeAlloc = cycleAndCount(globalHelperBuf, ' ') / 2;
                int offset = 0;
                NodeSPEZ4* nRicetta = creaEAggiungiAlDBRicette(token, sizeAlloc);

                while (1) {
                    // Leggo <nome_ingrediente>
                    token = pullToken();

                    if (!token) {
                        break;
                    }

                    // Leggo <quantita>
                    token2 = pullToken();
                    quantita = atoi(token2);

                    creaEAggiungiAllaRicetta(nRicetta, token, quantita, offset);

                    offset++;
                }

                printf("aggiunta\n");
            }

            continue;
        }

        if (strcmp(token, "rimuovi_ricetta") == 0) {
            // Rilevato comando: rimuovi_ricetta

            // Leggo <nome_ricetta>
            token = pullToken();

            // Trova nodo della ricetta
            NodeSPEZ4* nRicetta = ricettaByName(token);

            if (!nRicetta) {
                // Non trovato
                printf("non presente\n");
                continue;
            }

            int status = rimuoviDalDBRicette(nRicetta);

            if (status == 0) {
                // Trovato e rimosso
                printf("rimossa\n");
            } else if (status == -1) {
                // Ordini in sospeso
                printf("ordini in sospeso\n");
            }

            continue;
        }

        if (strcmp(token, "rifornimento") == 0) {
            // Rilevato comando: rifornimento
            cleanScorteWide();

            int quantita;
            int istanteScadenza;

            while (1) {
                // Leggo <nome_ingrediente>
                token = pullToken();

                if (!token) {
                    break;
                }

                // Leggo <quantita>
                token2 = pullToken();
                quantita = atoi(token2);

                // Leggo <scadenza>
                token3 = pullToken();
                istanteScadenza = atoi(token3);

                // Importante controllo per non accettare scorte gi� scadute!
                if (istanteScadenza > istanteTempoUniversale) {
                    creaEAggiungiScorta(token, quantita, istanteScadenza);
                }
            }

            printf("rifornito\n");

            preparaOrdiniInAttesa();

            continue;
        }

        if (strcmp(token, "ordine") == 0) {
            // Rilevato comando: ordine
            int elementiOrdinati;

            // Leggo <nome_ricetta>
            token = pullToken();

            // Leggo <numero_elementi_ordinati>
            token2 = pullToken();
            elementiOrdinati = atoi(token2);

            // Lookup
            NodeSPEZ4* nRicetta = ricettaByName(token);

            if (!nRicetta) {
                printf("rifiutato\n");
                continue;
            }

            // Inserisco ordine
            DLLSPEZ2* nOrdine = creaEAggiungiAlDBOrdini(nRicetta, elementiOrdinati);

            printf("accettato\n");
                
            cleanScorteScoped(nOrdine);
            preparaOrdine(nOrdine);

            continue;
        }

        // Non dovrebbe mai capitare...
        // Letto token invalido. Non conto il tempo.
        istanteTempoUniversale--;
    }
    
    // Deallocazione di massa non necessaria: time consuming: lascio fare al sistema operativo.

    inutile = inutile + 1;
}

// per Chiara
