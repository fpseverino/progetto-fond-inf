//
//  main.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include <stdio.h>
#include "func.h"

int main() {
    stampaMenùPrincipale();
    unsigned int richiesta; // numero di richiesta
    scanf("%u", &richiesta);

    // elabora la richiesta dell'utente
    while (richiesta != 7)
    {
        switch (richiesta)
        {
        case 1:
            puts("Aggiunto nuovo conto!\n");
            break;
        case 2:
            puts("Visualizzato elenco conti!\n");
            break;
        case 3:
            puts("Conto modificato!\n");
            break;
        case 4:
            puts("Transazione avvenuta!\n");
            break;
        case 5:
            puts("Account eliminato!\n");
            break;
        case 6:
            puts("Ecco i dettagli del conto!\n");
            break;
        default:
            puts("ERRORE: Scegli un'opzione dal menù.\n");
            break;
        }

        stampaMenùPrincipale();
        scanf("%u", &richiesta);    
    }

    puts("Uscita dall'applicazione.");
    
    return 0;
}