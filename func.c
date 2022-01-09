//
//  func.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 27/12/21.
//

#include "func.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// FUNZIONI PER GESTIONE MENU

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
    scanf("%u", &sceltaMenu);
    return sceltaMenu;
}

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

// FUNZIONI PRINCIPALI

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
        fflush(stdin);
        printf("%s", " Inserisci nome e cognome: ");
        scanf("%24[^\n]", account.nome);

        fflush(stdin);
        printf("%s", " Inserisci data di nascita (gg/mm/aaaa): ");
        scanf("%u%*c%u%*c%u", &account.dataNascita.giorno, &account.dataNascita.mese, &account.dataNascita.anno);
        if (!controllaData(account.dataNascita)) {
            puts("  ERRORE: Inserisci una data valida, operazione di aggiunta conto annullata.\n");
            return;
        }

        fflush(stdin);
        printf("%s", " Inserisci codice fiscale: ");
        scanf("%16s", account.codiceFiscale);
        inMaiuscolo(account.codiceFiscale, strlen(account.codiceFiscale));

        fflush(stdin);
        printf("%s", " Inserisci indirizzo di residenza: ");
        scanf("%29[^\n]", account.indirizzoResidenza);

        fflush(stdin);
        printf("%s", " Inserisci numero di telefono: ");
        scanf("%16[^\n]", account.telefono);

        fflush(stdin);
        printf("%s", " Inserisci saldo: ");
        scanf("%lf", &account.saldo);

        fflush(stdin);
        printf("%s", " Inserisci il tipo di conto:\n"
            "  1 - corrente         (interessi: 0%)\n"
            "  2 - deposito         (interessi: 1% all'anno)\n"
            "  3 - fisso per 1 anno (interessi: 2% all'anno)\n"
            "  4 - fisso per 2 anni (interessi: 2,5% all'anno)\n"
            "  5 - fisso per 3 anni (interessi: 3% all'anno)\n ? ");
        TIPO_CONTO temp;
        scanf("%d", &temp);
        switch (temp) {
            case 1:
                account.tipoConto = corrente;
                account.interessi = 1.0;
                break;
            case 2:
                account.tipoConto = deposito;
                account.interessi = 1.01;
                break;
            case 3:
                account.tipoConto = fisso1Anno;
                account.interessi = 1.02;
                break;
            case 4:
                account.tipoConto = fisso2Anni;
                account.interessi = 1.025;
                break;
            case 5:
                account.tipoConto = fisso3Anni;
                account.interessi = 1.03;
                break;
            default:
                puts("  ERRORE: Scelta non consentita, operazione di aggiunta conto annullata.\n");
                return;
                break;
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
        fflush(stdin);
        printf("%s", " Inserisci nuovo indirizzo di residenza: ");
        scanf("%29[^\n]", account.indirizzoResidenza);

        fflush(stdin);
        printf("%s", " Inserisci nuovo numero di telefono: ");
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
                    default: /* scelta non valida */ puts("\nERRORE: Scegli un'opzione dal menù.\n");
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
    FILE *ptrTra;
    if ((ptrTra = fopen("transazioni.txt","r")) == NULL) {
        puts("\nERRORE: Il file delle transazioni non esiste.\n");
    } else {
        char c = fgetc(ptrTra);
        if (c == EOF) {
            puts("\nNon ci sono transazioni salvate.\n");
            return;
        }
        puts("\n---- ELENCO TRANSAZIONI --------");
        while (c != EOF) {
            printf("%c", c);
            c = fgetc(ptrTra);
        }
        puts("");
    }
    fclose(ptrTra);
}

// FUNZIONI PER LE TRANSAZIONI

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

// FUNZIONI GESTIONE DATE

int anniPassati(Data primaData, Data secondaData) {
    const int giorniMese[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    // conta giorni dalla prima data all'anno zero
    int n1 = primaData.anno * 365 + primaData.giorno;
    for (int i = 0; i < primaData.mese - 1; i++) {
        n1 += giorniMese[i];
    }
    // conta giorni dalla seconda data all'anno zero
    int n2 = secondaData.anno * 365 + secondaData.giorno;
    for (int i = 0; i < secondaData.mese - 1; i++) {
        n2 += giorniMese[i];
    }
    // calcola gli anni fra le due date
    int anni = 0;
    if (n2 > n1) {
        anni = (n2 - n1) / 365;
    }
    return anni;
}

bool isLeapYear(int anno) {
    return (((anno % 4 == 0) && (anno % 100 != 0)) || (anno % 400 == 0)); // ritorna true se l'anno è bisestile
}

bool controllaData(Data data) {
    if (data.anno > 3000 || data.anno < 1) return false;
    if (data.mese < 1 || data.mese > 12) return false;
    if (data.giorno < 1 || data.giorno > 31) return false;
    // controlla febbraio negli anni bisestili
    if (data.mese == 2) {
        if (isLeapYear(data.anno)) {
            return (data.giorno <= 29);
        } else return (data.giorno <= 28);
    }
    // controlla mesi con 30 giorni
    if (data.mese == 4 || data.mese == 6 || data.mese == 9 || data.mese == 11) {
        return (data.giorno <= 30);
    }
    return true;
}

// ALTRE FUNZIONI

void inMaiuscolo(char * string, int n) {
    for (int i = 0; i < n; i++) {
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