//
//  func.h
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#ifndef func_h
#define func_h

#include <stdio.h>

//  definizione della struct Data per la gestione delle date
typedef struct {
    unsigned int giorno;
    unsigned int mese;
    unsigned int anno;
} Data;

// definizione dell'enum TipoConto dei vari tipi di conto
typedef enum { risparmio, corrente, fisso1Anno, fisso2Anni, fisso3Anni} TipoConto;

// definizione della struct DatiAccount dei dati di ogni account
typedef struct {
    char nome[25];
    Data dataNascita;
    char codiceFiscale[16];
    char indirizzoResidenza[20];
    int telefono[15];
    double saldo;
    TipoConto tipoConto;
    unsigned int numeroConto;
    Data dataVersamento;
    double importoInteressi;
} DatiAccount;

void stampaMenùPrincipale();

#endif