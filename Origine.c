
/*

Un�agenzia di un servizio corriere espresso vuole realizzare un programma per la gestione delle spedizioni. In particolare,
le richieste giornaliere di ritiro di pacchi da spedire sono inizialmente memorizzate in un file di testo (un esempio di file � fornito come allegato all'attivita').
Nel file ogni riga riporta, separati da spazio le seguenti informazioni:
- l'identificativo del mittente (stringa senza spazi)
- l'orario in cui � stata effettuata la richiesta (stringa in formato HH:MM)
- la sigla della provincia di destinazione (stringa)


Implementare un programma in ANSI C che contiene le seguenti funzioni:
1) lettura e caricamento dei dati contenuti nel file in una lista ordinata in base all'orario della richiesta. 
La funzione deve inoltre restituire il numero di elementi letti e caricati nella lista

2) funzione che restituisce il numero di spedizioni richieste nelle seguenti 3 fasce orarie: 
mattina (fino alle 11:59), pomeriggio (dalle 12:00 alle 17:59), sera (dopo le 18:00).

3) funzione che, fornita in ingresso una provincia, restituisca la lista delle spedizioni per quella provincia

Si implementi inoltre un main che richiami in maniera opportuna le funzioni sopra elencate, seguendo lo schema fornito nel codice.

La descrizione dettagliata di ogni funzione � riportata come commento immediatamente prima della definizione della funzione nello scheletro di programma fornito
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>  // Includere gli header di libreria necessari
#include <stdlib.h>
#include <string.h>
// Dichiarazione Struttura dati

typedef struct spedizione {
    char  mittente[32];
    char  orario[6]; //nel formato HH:MM
    char  provincia[3];
} TSpedizione;

typedef struct nodo {
    TSpedizione	info;
    struct nodo* next;
} TNodo;

typedef TNodo* TLista;


/* InserisciTestaLista
    inserisce un elemento in testa alla lista passata come primo parametro.
    Restituisce 1 se l'operazione va a buon fine, 0 altrimenti */
int InserisciTestaLista(TLista* lis, TSpedizione elem) {
    TLista paux;

    if ((paux = (TLista)malloc(sizeof(TNodo))) == NULL)
        return 0;

    paux->info = elem;
    paux->next = *lis;
    *lis = paux;

    return 1;
}

/* InserisciInListaOrdinata
    inserisce un elemento in lista in maniera ordinata rispetto al campo orario */
void InserisciInListaOrdinata(TLista* lis, TSpedizione elem) {

    if (*lis == NULL || strcmp((*lis)->info.orario, elem.orario) > 0)
        InserisciTestaLista(lis, elem);
    else
        InserisciInListaOrdinata(&((*lis)->next), elem);
}

/*
   1. CaricaDaFile:
   carica da file la richieste in una lista di spedizioni (ordinata in base all'orario).
   Restituisce il numero di elementi letti da file e caricati nella lista
*/
int CaricaDaFile(TLista* plis, char* nomefile) {
    // ... corpo della funzione da completare a cura dello studente ...
    FILE* f;
    TSpedizione sped;
    int cont = 0;

    f = fopen(nomefile, "r");
    if (f == NULL) return 0;
    while (fscanf(f, "%s%s%s", sped.mittente, sped.orario, sped.provincia) == 3) {
        InserisciInListaOrdinata(plis, sped);
        cont++;
    }
    return cont;
}


/*
    2. NumeroSpedizioni
    funzione che restituisce il numero di spedizioni richieste nelle fasce orarie: mattina (fino alle 11:59), pomeriggio (dalle 12:00 alle 17:59), sera (dopo le 18:00). 
    Tali risultati vengono restituiti in un opportuno vettore di tre elementi:0 mattina, 1 pomeriggio, 2 sera.
*/
void NumeroSpedizioni(TLista lis,int* vettore) {
    int ora, minuti;

    while (lis != NULL) {
        
        sscanf(lis->info.orario, "%d:%d", &ora, &minuti);
        if (ora > 00 && ora < 12) { //FASCIA MATTINA
            vettore[0]++;
        }
        if (ora >= 12 && ora < 18) { //FASCIA POME
            vettore[1]++;
        }
        if (ora >= 18 && ora < 24) { //FASCIA CENA
            vettore[2]++;
        }
        lis = lis->next;
    }
}

/*
    3. SpedizioniProvincia:
    funzione che prevede come parametri di ingresso: la lista delle spedizioni, ed una provincia.
    Essa deve restituire la lista delle spedizioni per la provincia data
*/
TLista SpedizioniProvincia(TLista lis, char* prov) {
    // ... corpo della funzione da completare a cura dello studente ...
    TLista aux=NULL;
    while (lis != NULL) {
        if (strcmp(lis->info.provincia, prov) == 0) {
            InserisciInListaOrdinata(&aux, lis->info);
        }
        lis = lis->next;
    }
    return aux;

}

/*******menu' di scelta *********/
int Menu() {
    int scelta;

    printf("\n\n*** M E N U ***\n"
        "1 - Carica dati da file\n"
        "2 - Numero spedizioni per fasce orarie\n"
        "3 - Spedizioni per provincia\n"
        "4 - Stampa lista spedizioni\n"
        "0 - Uscita\n\n"
        "Scelta: ");
    scanf("%d", &scelta);

    return scelta;
}

/*** funzione di utilit� per visualizzare i dati della lista ***/
void VisualizzaLista(TLista lis) {

    // ... corpo della funzione da completare a cura dello studente ...
    while (lis != NULL) {
        printf("\nMittente %s alle %s da %s", lis->info.mittente, lis->info.orario, lis->info.provincia);
        lis = lis->next;
    }
}


/****** main *********/
int main(void) {
    //TLista listaspedizioni;
    int scelta;

    /* inserire le ulteriori variabili necessarie */
    TLista listaspedizioni = NULL;
    char nome_file[128];
    int vettore[3] = { 0,0,0 }; //inizializzo il vettore per il quesito 2
    char provincia[128];
    TLista select = NULL; //quesito 3


    do {
        scelta = Menu();

        switch (scelta) {
        case 1:
            // Acquisire il nome del file
            printf("File: ");
            scanf("%s", nome_file);
            // Invocare la funzione CaricaDaFile e visualizzare il numero di elementi caricati o un messaggio in caso di errore
            int es = CaricaDaFile(&listaspedizioni, nome_file);
            printf("Caricati %d elementi ", es);
            // Visualizzare i dati caricati nella lista
            VisualizzaLista(listaspedizioni);
            break;

        case 2:
            // Invocare la funzione NumeroSpedizioni
            NumeroSpedizioni(listaspedizioni, vettore);
            // Visualizzare i dati restituiti dalla funzione 
            for (int i = 0; i < 3; i++) {
                if (i == 0)
                    printf("\nMattina : %d |", vettore[0]);
                if (i == 1)
                    printf(" Pomeriggio : %d |", vettore[1]);
                if (i == 2)
                    printf(" Sera : %d \n", vettore[2]);
            }
            break;

        case 3:
            // Acquisire la provincia
            printf("Provincia: ");
            scanf("%s", provincia);
            // Invocare la funzione SpedizioniProvincia
            select=SpedizioniProvincia(listaspedizioni, provincia);
            // Visualizzare i dati della lista restituita dalla funzione
            VisualizzaLista(select);
            break;
        case 4:
            VisualizzaLista(listaspedizioni);
            break;
        }

    } while (scelta != 0);

    // disallocare la lista
    free(listaspedizioni);

    return 0;
}
