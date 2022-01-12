//
//  func_date.h
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 27/12/21.
//

#ifndef FUNC_DATE_H
#define FUNC_DATE_H

#include <stdbool.h>

typedef struct {
    unsigned int giorno, mese, anno;
} Data;

int anniPassati(Data primaData, Data secondaData); // funzione che ritorna gli anni passati fra due date
bool bisestile(int anno);
bool controllaData(Data data); // funzione che verifica che una data sia valida

#endif