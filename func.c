//
//  func.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include "func.h"
#include <stdio.h>

unsigned int menùPrincipale() {
    // stampa le opzioni di richiesta
    printf("%s", "\nInserisci richiesta\n"
        " 1 - Aggiungi nuovo conto\n"
        " 2 - Visualizza elenco conti\n"
        " 3 - Modifica conto\n"
        " 4 - Transazione\n"
        " 5 - Elimina account\n"
        " 6 - Vedi dettagli conto\n"
        " 7 - Uscita\n? ");
    
    unsigned int sceltaMenù;
    scanf("%u", &sceltaMenù);
    fflush(stdin);
    return sceltaMenù;
}

void aggiungiNuovoConto(FILE *pFile) {
    // ottieni il numero del conto da creare
    printf("%s", "Inserisci il numero del nuovo account (1 - 100): ");
    unsigned int numeroAccount; // numero del conto
    scanf("%u", &numeroAccount);
    fflush(stdin);
    // sposta il puntatore del file al record corretto nel file
    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET);
    // crea DatiAccount con informazioni predefinite
    DatiAccount account = {
        "",
        {0, 0, 0},
        "",
        "",
        0,
        0.0,
        0,
        0,
        {0, 0, 0},
        0.0
    };
    // leggi il record dal file
    fread(&account, sizeof(DatiAccount), 1, pFile);
    // stampa un messaggio di errore se il conto esiste già
    if (account.numeroConto != 0) {
        printf("L'account #%u contiene già informazioni.\n", account.numeroConto);
    } else /* crea il record */ {
        // l'utente inserisce i dati
        printf("%s", "Inserisci nome e cognome\n? ");
        scanf(" %24[^\n]", account.nome);
        fflush(stdin);

        printf("%s", "Inserisci data di nascita (gg/mm/aaaa)\n? ");
        scanf("%u/%u/%u", &account.dataNascita.giorno, &account.dataNascita.mese, &account.dataNascita.anno);
        fflush(stdin);

        printf("%s", "Inserisci codice fiscale\n? ");
        scanf("%16s", account.codiceFiscale);
        fflush(stdin);

        printf("%s", "Inserisci indirizzo di residenza\n? ");
        scanf(" %24[^\n]", account.indirizzoResidenza);
        fflush(stdin);

        printf("%s", "Inserisci numero di telefono\n? ");
        scanf("%d", &account.telefono);
        fflush(stdin);

        printf("%s", "Inserisci saldo\n? ");
        scanf("%lf", &account.saldo);
        fflush(stdin);

        printf("%s", "Inserisci il tipo di conto\n"
            " 0 - risparmio\n"
            " 1 - corrente\n"
            " 2 - fisso per 1 anno\n"
            " 3 - fisso per 2 anni\n"
            " 4 - fisso per 3 anni\n? ");
        scanf("%d", &account.tipoConto);
        fflush(stdin);

        printf("%s", "Inserisci data odierna (gg/mm/aaaa)\n? ");
        scanf("%u/%u/%u", &account.dataVersamento.giorno, &account.dataVersamento.mese, &account.dataVersamento.anno);
        fflush(stdin);

        account.numeroConto = numeroAccount;

        // sposta il puntatore del file al record corretto nel file
        fseek(pFile, (account.numeroConto - 1) * sizeof(DatiAccount), SEEK_SET);

        // inserisci il record nel file
        fwrite(&account, sizeof(DatiAccount), 1, pFile);
    }
}

void vediDettagliConto(FILE *pFile) {
    // ottieni il numero del conto da visionare
    printf("%s", "Inserisci il numero dell'account da visionare (1 - 100): ");
    unsigned int numeroAccount; // numero del conto
    scanf("%u", &numeroAccount);
    // sposta il puntatore del file al record corretto nel file
    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET);
    // crea DatiAccount con informazioni predefinite
    DatiAccount account = {
        "",
        {0, 0, 0},
        "",
        "",
        0,
        0.0,
        0,
        0,
        {0, 0, 0},
        0.0
    };
    // leggi il record dal file
    fread(&account, sizeof(DatiAccount), 1, pFile);
    // stampa un messaggio di errore se il conto non esiste
    if (account.numeroConto == 0) {
        printf("L'account #%u non contiene informazioni.\n", numeroAccount);
    } else /* stampa i dettagli dell'account */ {
        printf("Nome: %s\n", account.nome);
        printf("Data di nascita: %u/%u/%u\n", account.dataNascita.giorno, account.dataNascita.mese, account.dataNascita.anno);
        printf("Codice fiscale: %s\n", account.codiceFiscale);
        printf("Indirizzo di residenza: %s\n", account.indirizzoResidenza);
        printf("Telefono: %d\n", account.telefono);
        printf("Saldo: %.2lf\n", account.saldo);
        printf("Tipo di conto: %d\n", account.tipoConto);
        printf("Numero del conto: %u\n", account.numeroConto);
        printf("Data ultimo versamento: %u/%u/%u\n", account.dataVersamento.giorno, account.dataVersamento.mese, account.dataVersamento.anno);
    }
}