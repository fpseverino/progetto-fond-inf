//
//  func.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include "func.h"
#include <stdio.h>

void stampaMenùPrincipale() {
    // stampa le opzioni di richiesta
    printf("%s", "Inserisci richiesta\n"
        " 1 - Aggiungi nuovo conto\n"
        " 2 - Visualizza elenco conti\n"
        " 3 - Modifica conto\n"
        " 4 - Transazione\n"
        " 5 - Elimina account\n"
        " 6 - Vedi dettagli conto\n"
        " 7 - Uscita\n? ");
}