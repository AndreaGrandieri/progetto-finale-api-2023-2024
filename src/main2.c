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

#include "defines.c"
#include "ElementoRicetta.c"
#include "DLLSPEZ2.c"
#include "BSTSPEZ3.c"
#include "BSTSPEZ4.c"
#include "BSTSPEZ5.c"
#include "BSTSPEZ6.c"

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
