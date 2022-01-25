//
//  func_date.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 27/12/21.
//

#include "func_date.h"
#include <stdbool.h>

static const int GIORNI_MESE[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int anniPassati(Data primaData, Data secondaData) {
    // conta giorni dalla prima data all'anno zero
    int n1 = primaData.anno * 365 + primaData.giorno;
    for (int i = 0; i < primaData.mese - 1; i++) {
        n1 += GIORNI_MESE[i];
    }
    // conta giorni dalla seconda data all'anno zero
    int n2 = secondaData.anno * 365 + secondaData.giorno;
    for (int i = 0; i < secondaData.mese - 1; i++) {
        n2 += GIORNI_MESE[i];
    }
    // calcola gli anni fra le due date
    int anni = 0;
    if (n2 > n1) {
        anni = (n2 - n1) / 365;
    }
    return anni;
}

bool bisestile(int anno) {
    return (((anno % 4 == 0) && (anno % 100 != 0)) || (anno % 400 == 0)); // ritorna true se l'anno è bisestile
}

bool controllaData(Data data) {
    if (data.anno > 3000 || data.anno < 1) return false;
    if (data.mese < 1 || data.mese > 12) return false;
    if (data.giorno < 1 || data.giorno > 31) return false;
    // controlla febbraio negli anni bisestili
    if (data.mese == 2) {
        if (bisestile(data.anno)) {
            return (data.giorno <= 29);
        } else return (data.giorno <= 28);
    }
    // controlla mesi con 30 giorni
    if (data.mese == 4 || data.mese == 6 || data.mese == 9 || data.mese == 11) {
        return (data.giorno <= 30);
    }
    return true;
}