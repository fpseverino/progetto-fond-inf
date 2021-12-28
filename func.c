//
//  func.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include "func.h"
#include <stdio.h>

unsigned int menùPrincipale() {
    printf("%s", "MENU PRINCIPALE\n"
        " 1 - Aggiungi nuovo conto\n"
        " 2 - Visualizza elenco conti\n"
        " 3 - Modifica conto\n"
        " 4 - Transazione\n"
        " 5 - Elimina account\n"
        " 6 - Vedi dettagli conto\n"
        " 7 - Uscita\n? ");
    
    unsigned int sceltaMenù;
    scanf("%u", &sceltaMenù); // scelta dell'utente
    fflush(stdin);
    return sceltaMenù;
}

void aggiungiNuovoConto(FILE *pFile, Data dataOdierna) {
    printf("%s", "\nInserisci il numero del nuovo account (1 - 100): ");
    unsigned int numeroAccount; // numero del conto
    scanf("%u", &numeroAccount);
    fflush(stdin);

    // controlla che il numero inserito rientri nei limiti del file
    if (numeroAccount < 1 || numeroAccount > 100) {
        puts(" ERRORE: Scelta non consentita, operazione di aggiunta conto annullata.\n");
        return;
    }

    // sposta il puntatore del file al record corretto nel file
    fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET);

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

    // leggi il record dal file
    fread(&account, sizeof(DatiAccount), 1, pFile);

    // stampa un messaggio di errore se il conto esiste già
    if (account.numeroConto != 0) {
        printf(" L'account #%u contiene già informazioni.\n\n", account.numeroConto);
    } else /* crea il record */ {
        // l'utente inserisce i dati
        printf("%s", " Inserisci nome e cognome: ");
        scanf(" %24[^\n]", account.nome);
        fflush(stdin);

        printf("%s", " Inserisci data di nascita (gg/mm/aaaa): ");
        scanf("%u/%u/%u", &account.dataNascita.giorno, &account.dataNascita.mese, &account.dataNascita.anno);
        fflush(stdin);

        printf("%s", " Inserisci codice fiscale: ");
        scanf("%16s", account.codiceFiscale);
        fflush(stdin);

        printf("%s", " Inserisci indirizzo di residenza: ");
        scanf(" %29[^\n]", account.indirizzoResidenza);
        fflush(stdin);

        printf("%s", " Inserisci numero di telefono: ");
        scanf(" %16[^\n]", account.telefono);
        fflush(stdin);

        printf("%s", " Inserisci saldo: ");
        scanf("%lf", &account.saldo);
        fflush(stdin);

        printf("%s", " Inserisci il tipo di conto:\n"
            "  0 - corrente (interessi: 0%)\n"
            "  1 - deposito (interessi: 1% all'anno)\n"
            "  2 - fisso per 1 anno (interessi: 2% all'anno)\n"
            "  3 - fisso per 2 anni (interessi: 2,5% all'anno)\n"
            "  4 - fisso per 3 anni (interessi: 3% all'anno)\n ? ");
        TIPO_CONTO temp;
        scanf("%d", &temp);
        fflush(stdin);
        switch (temp) {
            case 0:
                account.tipoConto = corrente;
                account.interessi = 1.0;
                break;
            case 1:
                account.tipoConto = deposito;
                account.interessi = 1.01;
                break;
            case 2:
                account.tipoConto = fisso1Anno;
                account.interessi = 1.02;
                break;
            case 3:
                account.tipoConto = fisso2Anni;
                account.interessi = 1.025;
                break;
            case 4:
                account.tipoConto = fisso3Anni;
                account.interessi = 1.03;
                break;
            default:
                puts("  ERRORE: Scelta non consentita, operazione di aggiunta conto annullata.\n");
                return;
                break;
        }

        account.dataVersamento.giorno = dataOdierna.giorno;
        account.dataVersamento.mese = dataOdierna.mese;
        account.dataVersamento.anno = dataOdierna.anno;

        account.numeroConto = numeroAccount;

        // sposta il puntatore del file al record corretto nel file
        fseek(pFile, (account.numeroConto - 1) * sizeof(DatiAccount), SEEK_SET);

        // inserisci il record nel file
        fwrite(&account, sizeof(DatiAccount), 1, pFile);

        printf("\nNuovo conto (#%u) aggiunto.\n\n", account.numeroConto);
    }
}

void visualizzaElencoConti(FILE *pFile) {
    // sposta il puntatore del file all'inizio del file
    rewind(pFile);

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

        int risultato = fread(&account, sizeof(DatiAccount), 1, pFile);

        // stampa i dati di un account
        if (risultato != 0 && account.numeroConto != 0) {
            printf("\nDati dell'account #%u:\n", account.numeroConto);

            printf(" Nome: %s\n", account.nome);

            printf(" Indirizzo di residenza: %s\n", account.indirizzoResidenza);

            printf(" Telefono: %s\n", account.telefono);

            printf(" Numero del conto: %u\n", account.numeroConto);
        }
    }
    puts("");
}

void modificaConto(FILE *pFile) {
    printf("%s", "\nInserisci il numero dell'account da modificare (1 - 100): ");
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
        "",
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
        printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount);
    } else /* modifica il record */ {
        printf("%s", " Inserisci nuovo indirizzo di residenza: ");
        scanf(" %29[^\n]", account.indirizzoResidenza);
        fflush(stdin);

        printf("%s", " Inserisci nuovo numero di telefono: ");
        scanf(" %16[^\n]", account.telefono);
        fflush(stdin);

        // sposta il puntatore del file al record corretto nel file
        fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET);

        // scrivi il record aggiornato al posto del vecchio record nel file
        fwrite(&account, sizeof(DatiAccount), 1, pFile);

        printf("\nConto #%u modificato.\n\n", account.numeroConto);
    }
}

void eliminaAccount(FILE *pFile) {
    printf("%s", "\nInserisci il numero dell'account da eliminare (1 - 100): ");
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
        "",
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
        printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount);
    } else /* cancella il record */ {
        // sposta il puntatore del file al record corretto nel file
        fseek(pFile, (numeroAccount - 1) * sizeof(DatiAccount), SEEK_SET);

        // account vuoto
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

        // sostituisci il record esistente con il record vuoto
        fwrite(&accountVuoto, sizeof(DatiAccount), 1, pFile);

        printf("\nConto #%u eliminato.\n\n", account.numeroConto);
    }
}

void vediDettagliConto(FILE *pFile, Data dataOdierna) {
    printf("%s", "\nInserisci il numero dell'account da visionare (1 - 100): ");
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
        "",
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
        printf(" L'account #%u non contiene informazioni.\n\n", numeroAccount);
    } else /* stampa i dettagli dell'account */ {
        printf("\nDati dell'account #%u:\n", account.numeroConto);

        printf(" Nome: %s\n", account.nome);

        printf(" Data di nascita: %u/%u/%u\n", account.dataNascita.giorno, account.dataNascita.mese, account.dataNascita.anno);

        printf(" Codice fiscale: %s\n", account.codiceFiscale);

        printf(" Indirizzo di residenza: %s\n", account.indirizzoResidenza);

        printf(" Telefono: %s\n", account.telefono);

        printf(" Saldo: €%.2lf\n", account.saldo);

        printf("%s", " Tipo di conto: ");
        switch (account.tipoConto) {
        case 0:
            printf("%s", "corrente\n");
            break;
        case 1:
            printf("%s", "deposito\n");
            break;
        case 2:
            printf("%s", "fisso per 1 anno\n");
            break;
        case 3:
            printf("%s", "fisso per 2 anni\n");
            break;
        case 4:
            printf("%s", "fisso per 3 anni\n");
            break;
        default:
            printf("%s", "ERRORE\n");
            break;
        }

        printf(" Numero del conto: %u\n", account.numeroConto);

        printf(" Data di versamento: %u/%u/%u\n", account.dataVersamento.giorno, account.dataVersamento.mese, account.dataVersamento.anno);

        double importoInteressi = account.interessi * account.saldo * anniPassati(account.dataVersamento, dataOdierna);
        if (anniPassati(account.dataVersamento, dataOdierna) != 0) {
            importoInteressi -= account.saldo;
        } 
        printf(" Importo degli interessi: €%.2lf\n", importoInteressi);

        puts("");
    }
}

int anniPassati(Data primaData, Data secondaData) {
    int anni = 0;
    while (secondaData.anno > primaData.anno) {
        if (secondaData.mese >= primaData.mese) {
            if (secondaData.giorno >= primaData.giorno) {
                anni++;
                primaData.anno++;
            } else break;
        } else break;
    }
    return anni;
}