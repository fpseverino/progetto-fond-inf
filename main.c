//
//  main.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 27/12/21.
//

#include <stdio.h>
#include <stdlib.h>
#include "func.h"

int main() {
    FILE *pAccountFile; // puntatore al file accounts.dat

    if ((pAccountFile = fopen("accounts.dat", "rb")) == NULL) /* il file non esiste e deve essere creato */ {
        // crea il file
        if ((pAccountFile = fopen("accounts.dat", "wb")) == NULL) {
            puts("ERRORE: Il file non può essere aperto.");
        } else {
            DatiAccount accountVuoto = {
                "",
                {0, 0, 0},
                "",
                "",
                "",
                0.0,
                0,
                0,
                {0, 0, 0},
                0.0
            };
            // scrive 100 record vuoti sul file
            for (int i = 1; i <= 100; i++) {
                fwrite(&accountVuoto, sizeof(DatiAccount), 1, pAccountFile);
            }
            fclose(pAccountFile);
        }
    }

    if ((pAccountFile = fopen("accounts.dat", "rb+")) == NULL) {
        puts("ERRORE: Il file non può essere aperto.");
    } else {
        puts("----------------------------------------------------------------");
        printf("%s", "Ciao! Inserisci la data di oggi (gg/mm/aaaa): ");
        Data oggi;
        scanf("%u%*c%u%*c%u", &oggi.giorno, &oggi.mese, &oggi.anno);
        // controlla che la data sia valida
        if (!controllaData(oggi)) {
            puts(" ERRORE: Inserisci una data valida.\n");
            return 0;
        }
        puts("");
        unsigned int scelta;
        // consenti all'utente di specificare l'azione
        while ((scelta = menuPrincipale()) != 8) {
            switch (scelta) {
                case 1: aggiungiNuovoConto(pAccountFile, oggi);
                        break;
                case 2: visualizzaElencoConti(pAccountFile);
                        break;
                case 3: modificaConto(pAccountFile);
                        break;
                case 4: transazione(pAccountFile, oggi);
                        break;
                case 5: eliminaAccount(pAccountFile, oggi);
                        break;
                case 6: vediDettagliConto(pAccountFile, oggi);
                        break;
                case 7: visualizzaElencoTransazioni();
                        break;
                default: /* scelta non valida */ puts("\nERRORE: Scegli un'opzione dal menù.\n");
                        break;
            }    
        }
        fclose(pAccountFile);
        puts("\nUscita dall'applicazione.");
        puts("----------------------------------------------------------------");
    }
    return 0;
}