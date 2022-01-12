//
//  transazione.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 27/12/21.
//

#include "func.h"
#include "func_date.h"
#include "transazione.h"
#include <stdio.h>

unsigned int menuTransazioni(unsigned int numeroAccount) {
    printf("*** MENU TRANSAZIONI ***\n"
        " 1 - Deposito su #%u\n"
        " 2 - Prelievo da #%u\n"
        " 3 - Bonifico da #%u a un altro account\n"
        " 4 - Cambia account\n"
        " 5 - Uscita\n? ", numeroAccount, numeroAccount, numeroAccount);
    unsigned int sceltaMenu;
    scanf("%u", &sceltaMenu);
    return sceltaMenu;
}

void funcDeposito(FILE *pFile, FILE *pTra, DatiAccount * account, Data dataOdierna) {
    printf("\nImporto da depositare: ");
    double importoDeposito;
    scanf("%lf",&importoDeposito);
    if (importoDeposito < 0.0) {
        puts(" ERRORE: Inserisci un valore positivo, transazione annullata.\n");
    } else {
        account->saldo += importoDeposito;
        // sposta il puntatore del file al record corretto nel file
        fseek(pFile, (account->numeroConto - 1) * sizeof(DatiAccount), SEEK_SET);
        fwrite(account, sizeof(DatiAccount), 1, pFile); // aggiornamento record

        fprintf(pTra, " Tipo di transazione: DEPOSITO\n  Conto #%u\n  Importo: %.2f\n  Data: %u/%u/%u\n", account->numeroConto, importoDeposito, dataOdierna.giorno, dataOdierna.mese, dataOdierna.anno);
        fprintf(pTra, "--------------------------------\n");

        printf("\nOperazione eseguita con successo.\n Saldo dell'account #%u: %.2lf\n\n", account->numeroConto, account->saldo);
    }
}

void prelievo(FILE *pFile, FILE *pTra, DatiAccount * account, Data dataOdierna) {
    // controlla che il tipo di conto permetta il prelievo
    if (account->tipoConto > 2) {
        if (anniPassati(account->dataAperturaConto, dataOdierna) < (account->tipoConto - 2)) {
            puts("\nDato il tipo di conto, non ti è possibile prelevare al momento.");
            printf(" Potrai prelevare a partire dal %u/%u/%u\n\n", account->dataAperturaConto.giorno, account->dataAperturaConto.mese, (account->dataAperturaConto.anno + (account->tipoConto - 2)));
            return;
        }
    }
    printf("\nImporto da prelevare: ");
    double importoPrelievo;
    scanf("%lf",&importoPrelievo);
    if (importoPrelievo < 0.0) {
        puts(" ERRORE: Inserisci un valore positivo, transazione annullata.\n");
    } else if (importoPrelievo > account->saldo) {
        puts(" ERRORE: Inserisci un valore minore del saldo totale, transazione annullata.\n");
    } else {
        account->saldo -= importoPrelievo;
        // sposta il puntatore del file al record corretto nel file
        fseek(pFile, (account->numeroConto - 1) * sizeof(DatiAccount), SEEK_SET);
        fwrite(account, sizeof(DatiAccount), 1, pFile); // aggiornamento record

        fprintf(pTra, " Tipo di transazione: PRELIEVO\n  Conto #%u\n  Importo: %.2f\n  Data: %u/%u/%u\n", account->numeroConto, importoPrelievo, dataOdierna.giorno, dataOdierna.mese, dataOdierna.anno);
        fprintf(pTra, "--------------------------------\n");

        printf("\nOperazione eseguita con successo.\n Saldo dell'account #%u: %.2lf\n\n", account->numeroConto, account->saldo);
    }
}

void bonifico(FILE *pFile, FILE *pTra, DatiAccount * account, Data dataOdierna) {
    if (account->tipoConto == 1) {
        printf("%s", "\nInserisci il numero dell'account che riceverà il bonifico (1 - 100): "); 
        unsigned int numeroAccount2;
        scanf("%u", &numeroAccount2);

        fseek(pFile, (numeroAccount2 - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
        // crea DatiAccount con informazioni predefinite
        DatiAccount account2 = {
            "",
            {0, 0, 0},
            "",
            "",
            "",
            0.0,
            0,
            0,
            {0, 0, 0},
            0.0
        };
        fread(&account2, sizeof(DatiAccount), 1, pFile); // leggi il record dal file

        if (account2.numeroConto == 0) {
            printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount2);
        } else {
            printf(" Inserisci importo: ");
            double importoBonifico;
            scanf("%lf",&importoBonifico);
            account->saldo -= importoBonifico;
            account2.saldo += importoBonifico;

            // sposta il puntatore del file al record corretto nel file
            fseek(pFile, (account->numeroConto - 1) * sizeof(DatiAccount), SEEK_SET);
            fwrite(account, sizeof(DatiAccount), 1, pFile); // aggiornamento record
            // sposta il puntatore del file al record corretto nel file
            fseek(pFile, (numeroAccount2 - 1) * sizeof(DatiAccount), SEEK_SET);
            fwrite(&account2, sizeof(DatiAccount), 1, pFile); // aggiornamento record

            fprintf(pTra, " Tipo di transazione: BONIFICO\n  Dal conto #%u\n  Al conto #%u\n  Importo: %.2f\n  Data: %u/%u/%u\n", account->numeroConto, account2.numeroConto, importoBonifico, dataOdierna.giorno, dataOdierna.mese, dataOdierna.anno);
            fprintf(pTra, "--------------------------------\n");

            printf("\nOperazione eseguita con successo.\n Saldo dell'account #%u: %.2lf\n\n", account->numeroConto, account->saldo);
        }
    } else puts("\nE' possibile effettuare bonifici solo con conti correnti.\n");
}

void cambiaAccount(FILE *pFile, DatiAccount * account, Data dataOdierna) {
    printf("%s", "\nInserisci il numero del nuovo account su cui operare (1 - 100): ");
    unsigned int numeroAccount;
    scanf("%u", &numeroAccount);

    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
    // crea DatiAccount con informazioni predefinite
    DatiAccount accountTemp = {
        "",
        {0, 0, 0},
        "",
        "",
        "",
        0.0,
        0,
        0,
        {0, 0, 0},
        0.0
    };
    fread(&accountTemp, sizeof(DatiAccount), 1, pFile); // leggi il record dal file

    if (accountTemp.numeroConto == 0) {
        printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount);
    } else {
        *account = accountTemp;
        printf(" Cambio account eseguito con successo (nuovo account: %u).\n", account->numeroConto);
        stampaDettagliConto(*account, dataOdierna);
    }
}