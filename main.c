//
//  main.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include <stdio.h>

//  definizione della struct Data per la gestione delle date
struct DATA {
    unsigned int giorno;
    unsigned int mese;
    unsigned int anno;
};
typedef struct DATA Data;

// definizione dell'enum TipoConto dei vari tipi di conto
enum TIPO_CONTO {
    risparmio,
    corrente,
    fisso1Anno,
    fisso2Anni,
    fisso3Anni,
};
typedef enum TIPO_CONTO TipoConto;

// definizione della struct DatiAccount dei dati di ogni account
struct DATI_ACCOUNT {
    char nome[10];
    char cognome[15];
    Data dataNascita;
    char codiceFiscale[16];
    char indirizzoResidenza[20];
    int telefono[15];
    double saldo;
    TipoConto tipoConto;
    unsigned int numeroConto;
    Data dataVersamento;
    double importoInteressi;
};
typedef struct DATI_ACCOUNT DatiAccount;

void stampaMenùPrincipale();

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
            puts("ERRORE\n");
            break;
        }

        stampaMenùPrincipale();
        scanf("%u", &richiesta);    
    }

    puts("Chiusura programma.");
    
    return 0;
}

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