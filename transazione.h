//
//  transazione.h
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 27/12/21.
//

#ifndef TRANSAZIONE_H
#define TRANSAZIONE_H

#include "func.h"
#include "func_date.h"
#include <stdio.h>

unsigned int menuTransazioni(unsigned int numeroAccount);

void funcDeposito(FILE *pFile, FILE *pTra, DatiAccount * account, Data dataOdierna);
void prelievo(FILE *pFile, FILE *pTra, DatiAccount * account, Data dataOdierna);
void bonifico(FILE *pFile, FILE *pTra, DatiAccount * account, Data dataOdierna);
void cambiaAccount(FILE *pFile, DatiAccount * account, Data dataOdierna);

#endif