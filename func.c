//
//  func.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 27/12/21.
//

#include "func.h"
#include "func_date.h"
#include "transazione.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

unsigned int menuPrincipale() {
    printf("%s", "*** MENU PRINCIPALE ***\n"
        " 1 - Aggiungi nuovo conto\n"
        " 2 - Visualizza elenco conti\n"
        " 3 - Modifica conto\n"
        " 4 - Transazione\n"
        " 5 - Elimina account\n"
        " 6 - Vedi dettagli conto\n"
        " 7 - Visualizza elenco transazioni\n"
        " 8 - Uscita\n? ");
    unsigned int sceltaMenu;
    fflush(stdin);
    scanf("%u", &sceltaMenu);
    return sceltaMenu;
}

void aggiungiNuovoConto(FILE *pFile, Data dataOdierna) {
    printf("%s", "\nInserisci il numero del nuovo account (1 - 100): ");
    unsigned int numeroAccount;
    scanf("%u", &numeroAccount);

    // controlla che il numero inserito rientri nei limiti del file
    if (numeroAccount < 1 || numeroAccount > 100) {
        puts(" ERRORE: Scelta non consentita, operazione di aggiunta conto annullata.\n");
        return;
    }

    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
    // crea DatiAccount con informazioni predefinite
    DatiAccount account = {
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
    fread(&account, sizeof(DatiAccount), 1, pFile); // leggi il record dal file

    if (account.numeroConto != 0) {
        printf(" L'account #%u contiene già informazioni.\n\n", account.numeroConto);
    } else /* crea il record */ {
        printf("%s", " Inserisci nome e cognome: ");
        fflush(stdin);
        scanf("%24[^\n]", account.nome);

        bool checkDataNascita = false;
        while (!checkDataNascita) {
            printf("%s", " Inserisci data di nascita (gg/mm/aaaa): ");
            fflush(stdin);
            scanf("%u%*c%u%*c%u", &account.dataNascita.giorno, &account.dataNascita.mese, &account.dataNascita.anno);
            if (!(checkDataNascita = controllaData(account.dataNascita))) {
                puts("  ERRORE: Inserisci una data valida.");
            }
        }

        printf("%s", " Inserisci codice fiscale: ");
        fflush(stdin);
        scanf("%16s", account.codiceFiscale);
        inMaiuscolo(account.codiceFiscale);

        printf("%s", " Inserisci indirizzo di residenza: ");
        fflush(stdin);
        scanf("%29[^\n]", account.indirizzoResidenza);

        printf("%s", " Inserisci numero di telefono: ");
        fflush(stdin);
        scanf("%16[^\n]", account.telefono);

        printf("%s", " Inserisci saldo: ");
        fflush(stdin);
        scanf("%lf", &account.saldo);

        printf("%s", " Inserisci il tipo di conto:\n"
            "  1 - corrente          (interessi: 0)             (deposito, prelievo, bonifico)\n"
            "  2 - deposito          (interessi: 1% all'anno)   (deposito, prelievo)\n"
            "  3 - fisso per 1 anno  (interessi: 2% all'anno)   (deposito, prelievo dopo un anno)\n"
            "  4 - fisso per 2 anni  (interessi: 2,5% all'anno) (deposito, prelievo dopo due anni)\n"
            "  5 - fisso per 3 anni  (interessi: 3% all'anno)   (deposito, prelievo dopo tre anni)\n");
        TIPO_CONTO temp;
        bool checkTipoConto = false;
        while (!checkTipoConto) {
            printf("%s", " ? ");
            fflush(stdin);
            scanf("%d", &temp);
            switch (temp) {
                case 1:
                    account.tipoConto = corrente;
                    account.interessi = 1.0;
                    checkTipoConto = true;
                    break;
                case 2:
                    account.tipoConto = deposito;
                    account.interessi = 1.01;
                    checkTipoConto = true;
                    break;
                case 3:
                    account.tipoConto = fisso1Anno;
                    account.interessi = 1.02;
                    checkTipoConto = true;
                    break;
                case 4:
                    account.tipoConto = fisso2Anni;
                    account.interessi = 1.025;
                    checkTipoConto = true;
                    break;
                case 5:
                    account.tipoConto = fisso3Anni;
                    account.interessi = 1.03;
                    checkTipoConto = true;
                    break;
                default:
                    puts("  ERRORE: Scelta non consentita.");
                    break;
            }
        }
        
        account.dataAperturaConto.giorno = dataOdierna.giorno;
        account.dataAperturaConto.mese = dataOdierna.mese;
        account.dataAperturaConto.anno = dataOdierna.anno;

        account.numeroConto = numeroAccount;

        fseek(pFile, (account.numeroConto - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
        fwrite(&account, sizeof(DatiAccount), 1, pFile); // inserisci il record nel file
        printf("\nNuovo conto (#%u) aggiunto.\n\n", account.numeroConto);
    }
}

void visualizzaElencoConti(FILE *pFile) {
    rewind(pFile);
    int numeroAccountLetti = 0;
    // leggi tutti i record del file
    while (!feof(pFile)) {
        // crea DatiAccount con informazioni predefinite
        DatiAccount account = {
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
        int risultato = fread(&account, sizeof(DatiAccount), 1, pFile); // leggi il record dal file e passa eventuali errori
        if (risultato != 0 && account.numeroConto != 0) {
            printf("\n---- Dati dell'account #%u ----------------\n", account.numeroConto);
            printf("%-27s%s\n", " Nome: ", account.nome);
            printf("%-27s%s\n", " Indirizzo di residenza: ", account.indirizzoResidenza);
            printf("%-27s%s\n", " Telefono: ", account.telefono);
            numeroAccountLetti++;
        }
    }
    if (numeroAccountLetti == 0) puts("\nNon ci sono account salvati.");
    puts("");
}

void modificaConto(FILE *pFile) {
    printf("%s", "\nInserisci il numero dell'account da modificare (1 - 100): ");
    unsigned int numeroAccount;
    scanf("%u", &numeroAccount);

    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
    // crea DatiAccount con informazioni predefinite
    DatiAccount account = {
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
    fread(&account, sizeof(DatiAccount), 1, pFile); // leggi il record dal file

    if (account.numeroConto == 0) {
        printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount);
    } else /* modifica il record */ {
        printf("\n---- Dati dell'account #%u ----------------\n", account.numeroConto);
        printf("%-27s%s\n", " Nome: ", account.nome);
        printf("%-27s%s\n", " Indirizzo di residenza: ", account.indirizzoResidenza);
        printf("%-27s%s\n", " Telefono: ", account.telefono);

        printf("%s", "\nInserisci nuovo indirizzo di residenza: ");
        fflush(stdin);
        scanf("%29[^\n]", account.indirizzoResidenza);

        printf("%s", "Inserisci nuovo numero di telefono: ");
        fflush(stdin);
        scanf("%16[^\n]", account.telefono);

        fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
        fwrite(&account, sizeof(DatiAccount), 1, pFile); // scrivi il record aggiornato al posto del vecchio record nel file
        printf("\nConto #%u modificato.\n\n", account.numeroConto);
    }
}

void transazione(FILE *pFile, Data dataOdierna) {
    FILE *pFileTransazioni; // puntatore al file "transazioni.txt"

    printf("%s", "\nInserisci il numero dell'account su cui operare (1 - 100): "); 
    unsigned int numeroAccount;
    scanf("%u", &numeroAccount);

    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
    // crea DatiAccount con informazioni predefinite
    DatiAccount account = {
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
    fread(&account, sizeof(DatiAccount), 1, pFile); // leggi il record dal file

    if (account.numeroConto == 0) {
        printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount);
    } else {
        if ((pFileTransazioni = fopen("transazioni.txt", "a+")) == NULL) {
            puts("\nERRORE: Il file non può essere aperto.\n");
        } else {
            stampaDettagliConto(account, dataOdierna);
            unsigned int sceltaTransazione;
            while ((sceltaTransazione = menuTransazioni(account.numeroConto)) != 5) {
                switch (sceltaTransazione) {
                    case 1: funcDeposito(pFile, pFileTransazioni, &account, dataOdierna);
                            break;
                    case 2: prelievo(pFile, pFileTransazioni, &account, dataOdierna);
                            break;
                    case 3: bonifico(pFile, pFileTransazioni, &account, dataOdierna);
                            break;
                    case 4: cambiaAccount(pFile, &account, dataOdierna);
                            break;
                    default: puts("\nERRORE: Scegli un'opzione dal menù.\n");
                            break;
                }
            }
            fclose(pFileTransazioni);
            puts("\nUscita dal menu transazioni.\n");
        }
    }
}

void eliminaAccount(FILE *pFile, Data dataOdierna) {
    printf("%s", "\nInserisci il numero dell'account da eliminare (1 - 100): ");
    unsigned int numeroAccount;
    scanf("%u", &numeroAccount);

    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
    // crea DatiAccount con informazioni predefinite
    DatiAccount account = {
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
    fread(&account, sizeof(DatiAccount), 1, pFile); // leggi il record dal file

    if (account.numeroConto == 0) {
        printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount);
    } else /* cancella il record */ {
        stampaDettagliConto(account, dataOdierna); // prima stampa i dati dell'account per sicurezza dell'utente
        printf("Sei sicuro di voler cancellare l'account #%u? (y = si, n = no)\n? ", account.numeroConto);
        fflush(stdin);
        char conferma = getchar();
        switch (conferma) {
            case 'n':
            case 'N':
                puts("\nEliminazione annullata.\n");
                return;
                break;
            case 'y':
            case 'Y':
                fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
                DatiAccount accountVuoto = {
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
                fwrite(&accountVuoto, sizeof(DatiAccount), 1, pFile); // sostituisci il record esistente con il record vuoto
                printf("\nConto #%u eliminato.\n\n", account.numeroConto);
                break;
            default:
                puts("\nERRORE: Scegli un'opzione valida, eliminazione annullata.\n");
                break;
        }
    }
}

void vediDettagliConto(FILE *pFile, Data dataOdierna) {
    printf("%s", "\nInserisci il numero dell'account da visionare (1 - 100): ");
    unsigned int numeroAccount;
    scanf("%u", &numeroAccount);

    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET); // sposta il puntatore del file al record corretto nel file
    // crea DatiAccount con informazioni predefinite
    DatiAccount account = {
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
    fread(&account, sizeof(DatiAccount), 1, pFile); // leggi il record dal file

    if (account.numeroConto == 0) {
        printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount);
    } else {
        stampaDettagliConto(account, dataOdierna);
    }
}

void visualizzaElencoTransazioni() {
    FILE *pTxn; // puntatore al file "transazioni.txt"
    if ((pTxn = fopen("transazioni.txt","r")) == NULL) {
        puts("\nERRORE: Il file delle transazioni non esiste.\n");
    } else {
        char c = fgetc(pTxn);
        if (c == EOF) {
            puts("\nNon ci sono transazioni salvate.\n");
            return;
        }
        puts("\n---- ELENCO TRANSAZIONI --------");
        while (c != EOF) {
            printf("%c", c);
            c = fgetc(pTxn);
        }
        puts("");
    }
    fclose(pTxn);
}

void inMaiuscolo(char * string) {
    int lunghezzaString = strlen(string);
    for (int i = 0; i < lunghezzaString; i++) {
        string[i] = toupper(string[i]);
    }
}

void stampaDettagliConto(DatiAccount account, Data dataOdierna) {
    printf("\n---- Dati dell'account #%u ----------------\n", account.numeroConto);
    printf("%-27s%s\n", " Nome: ", account.nome);
    printf("%-27s%u/%u/%u\n", " Data di nascita: ", account.dataNascita.giorno, account.dataNascita.mese, account.dataNascita.anno);
    printf("%-27s%s\n", " Codice fiscale: ", account.codiceFiscale);
    printf("%-27s%s\n", " Indirizzo di residenza: ", account.indirizzoResidenza);
    printf("%-27s%s\n", " Telefono: ", account.telefono);
    printf("%-27s%.2lf\n", " Saldo: ", account.saldo);
    printf("%-27s", " Tipo di conto: ");
    switch (account.tipoConto) {
        case corrente:
            puts("corrente");
            break;
        case deposito:
            puts("deposito");
            break;
        case fisso1Anno:
            puts("fisso per 1 anno");
            break;
        case fisso2Anni:
            puts("fisso per 2 anni");
            break;
        case fisso3Anni:
            puts("fisso per 3 anni");
            break;
        default:
            puts("ERRORE");
            break;
    }
    printf("%-27s%u\n", " Numero del conto: ", account.numeroConto);
    printf("%-27s%u/%u/%u\n", " Data di versamento: ", account.dataAperturaConto.giorno, account.dataAperturaConto.mese, account.dataAperturaConto.anno);
    // calcola gli interessi
    double importoInteressi = account.interessi * account.saldo * anniPassati(account.dataAperturaConto, dataOdierna) - account.saldo * anniPassati(account.dataAperturaConto, dataOdierna);
    printf("%-27s%.2lf\n\n", " Importo degli interessi: ", importoInteressi);
}