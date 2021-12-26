//
//  main.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include <stdio.h>
#include "func.h"

int main() {
    unsigned int richiesta; // numero di richiesta

    // elabora la richiesta dell'utente
    while ((richiesta = menùPrincipale()) != 7)
    {
        switch (richiesta)
        {
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

    puts("\nUscita dall'applicazione.");
    
    return 0;
}