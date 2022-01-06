//
//  func.h
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 27/12/21.
//

#ifndef func_h
#define func_h

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    unsigned int giorno, mese, anno;
} Data;

typedef enum { corrente = 1, deposito, fisso1Anno, fisso2Anni, fisso3Anni } TIPO_CONTO;
    // corrente:   interessi 0%            (manda e ricevi denaro verso e da altri conti)
    // deposito:   interessi 1% all'anno   (solo deposito e ritiro denaro)
    // fisso1Anno: interessi 2% all'anno   (solo deposito, impossibile ritirare denaro prima di un anno)
    // fisso2Anni: interessi 2,5% all'anno (solo deposito, impossibile ritirare denaro prima di due anni)
    // fisso3Anni: interessi 3% all'anno   (solo deposito, impossibile ritirare denaro prima di tre anni)

typedef struct {
    char nome[25];
    Data dataNascita;
    char codiceFiscale[17];
    char indirizzoResidenza[30];
    char telefono[17];
    double saldo;
    TIPO_CONTO tipoConto;
    unsigned int numeroConto;
    Data dataAperturaConto;
    double interessi;
} DatiAccount;

// FUNZIONI PER GESTIONE MENU
unsigned int menuPrincipale();
unsigned int menuTransazioni(unsigned int numeroAccount);
// FUNZIONI PRINCIPALI
void aggiungiNuovoConto(FILE *pFile, Data dataOdierna);
void visualizzaElencoConti(FILE *pFile);
void modificaConto(FILE *pFile);
void transazione(FILE *pFile, Data oggi);
void eliminaAccount(FILE *pFile, Data dataOdierna);
void vediDettagliConto(FILE *pFile, Data dataOdierna);
void visualizzaElencoTransazioni();
// FUNZIONI GESTIONE DATE
int anniPassati(Data primaData, Data secondaData); // funzione che ritorna gli anni passati fra due date
bool isLeapYear(int anno);
bool controllaData(Data data); // funzione che verifica che una data sia valida
// ALTRE FUNZIONI
void inMaiuscolo(char * string, int n); // funzione che riceve una stringa e la modifica rendendo tutti i caratteri maiuscoli

#endif