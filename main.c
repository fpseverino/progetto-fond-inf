//
//  main.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include <stdio.h>
#include "func.h"

int main() {
    FILE *pAccountFile; // puntatore al file accounts.dat

    if ((pAccountFile = fopen("accounts.dat", "rb")) == NULL) {
        // il file non esiste e deve essere creato
        fclose(pAccountFile);

        // crea il file
        if ((pAccountFile = fopen("accounts.dat", "wb")) == NULL) {
            puts("Il file non può essere aperto.");
        } else {
            // crea DatiAccount con informazioni predefinite
            DatiAccount accountVuoto = {
                "",
                {0, 0, 0},
                "",
                "",
                0,
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

            fclose(pAccountFile); // fclose chiude il file
        }
    }

    // fopen apre il file per l'aggiornamento in forma binaria
    if ((pAccountFile = fopen("accounts.dat", "rb+")) == NULL) {
        puts("Il file non può essere aperto.");
    } else {
        unsigned int scelta; // scelta dell'utente
        // consenti all'utente di specificare l'azione
        while ((scelta = menùPrincipale()) != 7) {
            switch (scelta) {
                case 1:
                    // Aggiungi nuovo conto
                    aggiungiNuovoConto(pAccountFile);
                    break;
                case 2:
                    // Visualizza elenco conti
                    puts("\nVisualizzato elenco conti!\n");
                    break;
                case 3:
                    // Modifica conto
                    puts("\nConto modificato!\n");
                    break;
                case 4:
                    // Transazione
                    puts("\nTransazione avvenuta!\n");
                    break;
                case 5:
                    // Elimina account
                    puts("\nAccount eliminato!\n");
                    break;
                case 6:
                    // Vedi dettagli conto
                    vediDettagliConto(pAccountFile);
                    break;
                default:
                    // scelta non valida
                    puts("\nERRORE: Scegli un'opzione dal menù.\n");
                    break;
            }    
        }
        fclose(pAccountFile); // fclose chiude il file
        puts("\nUscita dall'applicazione.");
    }
    return 0;
}