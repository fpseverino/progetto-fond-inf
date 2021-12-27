//
//  main.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include <stdio.h>
#include "func.h"

int main() {
    FILE *pFile; // puntatore al file accounts.dat
    // fopen apre il file per la scrittura in forma binaria
    if ((pFile = fopen("accounts.dat", "wb")) == NULL) {
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
        // scrive 100 record vuoti su file
        for (int i = 1; i <= 100; i++) {
            fwrite(&accountVuoto, sizeof(DatiAccount), 1, pFile);
        }
        fclose(pFile); // fclose chiude il file
    }

    // fopen apre il file per l'aggiornamento in forma binaria
    if ((pFile = fopen("accounts.dat", "rb+")) == NULL) {
        puts("Il file non può essere aperto.");
    } else {
        unsigned int scelta; // scelta dell'utente
        // consenti all'utente di specificare l'azione
        while ((scelta = menùPrincipale()) != 7) {
            switch (scelta) {
                case 1:
                    // Aggiungi nuovo conto
                    puts("\nAggiunto nuovo conto!");
                    break;
                case 2:
                    // Visualizza elenco conti
                    puts("\nVisualizzato elenco conti!");
                    break;
                case 3:
                    // Modifica conto
                    puts("\nConto modificato!");
                    break;
                case 4:
                    // Transazione
                    puts("\nTransazione avvenuta!");
                    break;
                case 5:
                    // Elimina account
                    puts("\nAccount eliminato!");
                    break;
                case 6:
                    // Vedi dettagli conto
                    puts("\nEcco i dettagli del conto!");
                    break;
                default:
                    // scelta non valida
                    puts("\nERRORE: Scegli un'opzione dal menù.");
                    break;
            }    
        }
        fclose(pFile); // fclose chiude il file
        puts("\nUscita dall'applicazione.");
    }
    
    return 0;
}