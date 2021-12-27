//
//  func.h
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#ifndef func_h
#define func_h

#include <stdio.h>

//  definizione della struttura Data per la gestione delle date
typedef struct {
    unsigned int giorno;
    unsigned int mese;
    unsigned int anno;
} Data;

// definizione dell'enumerazione TipoConto dei vari tipi di conto
typedef enum { risparmio, corrente, fisso1Anno, fisso2Anni, fisso3Anni } TIPO_CONTO;

// definizione della struttura DatiAccount dei dati di ogni account
typedef struct {
    char nome[25];
    Data dataNascita;
    char codiceFiscale[17];
    char indirizzoResidenza[25];
    int telefono;
    double saldo;
    TIPO_CONTO tipoConto;
    unsigned int numeroConto;
    Data dataVersamento;
    double importoInteressi;
} DatiAccount;

unsigned int menùPrincipale();
void aggiungiNuovoConto(FILE *pFile);
void vediDettagliConto(FILE *pFile);

#endif