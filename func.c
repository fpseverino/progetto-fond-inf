//
//  func.c
//  progettoFondInf
//
//  Created by Francesco Paolo Severino and Roberto Giovanni Scolari on 26/12/21.
//

#include "func.h"
#include <stdio.h>

unsigned int menuPrincipale() {
    printf("%s", "*** MENU PRINCIPALE ***\n"
        " 1 - Aggiungi nuovo conto\n"
        " 2 - Visualizza elenco conti\n"
        " 3 - Modifica conto\n"
        " 4 - Transazione\n"
        " 5 - Elimina account\n"
        " 6 - Vedi dettagli conto\n"
        " 7 - Uscita\n? ");
    
    unsigned int sceltaMenu;
    scanf("%u", &sceltaMenu); // scelta dell'utente
    fflush(stdin);
    return sceltaMenu;
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
        if (controllaData(account.dataNascita)) {
            puts("  ERRORE: Inserisci una data valida, operazione di aggiunta conto annullata.\n");
            return;
        }

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

    int nAccount = 0; // salva il numero di account letti

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
            nAccount++;
        }
    }

    if (nAccount == 0) {
        puts("\nNon ci sono account salvati.");
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

void transazione(FILE *pFile, Data dataOdierna) {
    printf("%s", "\nInserisci il numero dell'account su cui operare (1 - 100): ");
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
    } else /* controlla il tipo di conto */ {
        // stampa le informazioni dell'account prima della transazione
        printf("\n#%-5d%-30s€%.2lf\n", account.numeroConto, account.nome, account.saldo);

        switch (account.tipoConto) {
            case corrente:
                /* code */
                break;
            
            case deposito:
            case fisso1Anno:
            case fisso2Anni:
            case fisso3Anni:
                // l'utente sceglie se depositare o prelevare denaro
                printf("%s", "\nScegli il tipo di operazione:\n"
                    " 1 - deposita denaro\n"
                    " 2 - preleva denaro\n? ");
                unsigned int sceltaOperazione; // scelta dell'utente
                scanf("%u", &sceltaOperazione);
                fflush(stdin);
                // deposita o preleva denaro in base alla scelta dell'utente
                switch (sceltaOperazione) {
                    case 1 /* deposita denaro */:
                        printf("%s", "\nInserisci la somma da depositare: ");
                        double versamento;
                        scanf("%lf", &versamento);
                        fflush(stdin);

                        if (versamento <= 0.0) {
                            puts(" ERRORE: Inserisci una somma positiva, transazione annullata.\n");
                            return;
                        }

                        puts(" Transazione avvenuta!"); // debug
                        break;

                    case 2 /* preleva denaro */:
                        if (account.tipoConto > 1) {
                            if (anniPassati(account.dataVersamento, dataOdierna) < (account.tipoConto - 1)) {
                                puts("\nDato il tuo tipo di conto, al momento non ti è permesso ritirare denaro.");
                                puts(" Operazione annullata.\n");
                                return;
                            }
                        }

                        printf("%s", "\nInserisci la somma da prelevare: ");
                        double prelievo;
                        scanf("%lf", &prelievo);
                        fflush(stdin);

                        if (prelievo <= 0.0) {
                            puts(" ERRORE: Inserisci una somma positiva, transazione annullata.\n");
                            return;
                        }
                        
                        puts(" Transazione avvenuta!");
                        break;

                    default:
                        puts(" ERRORE: Scelta non consentita, transazione annullata.\n");
                        return;
                        break;
                }
                break;
            
            default:
                puts(" ERRORE.");
                break;
        }

        // stampa le informazioni dell'account dopo la transazione
        printf("\n#%-5d%-30s€%.2lf\n\n", account.numeroConto, account.nome, account.saldo);
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
        printf(" Numero del conto: %u\n", account.numeroConto);
        printf(" Data di versamento: %u/%u/%u\n", account.dataVersamento.giorno, account.dataVersamento.mese, account.dataVersamento.anno);
        // calcola gli interessi
        double importoInteressi = account.interessi * account.saldo * anniPassati(account.dataVersamento, dataOdierna) - account.saldo * anniPassati(account.dataVersamento, dataOdierna);
        printf(" Importo degli interessi: €%.2lf\n\n", importoInteressi);
    }
}

const int giorniMese[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // numero di giorni in ogni mese

// conta il numero di anni bisestili prima di una data
int contaAnniBisestili(Data data) {
    int anni = data.anno;
    // controlla se l'anno corrente debba essere calcolato o meno
    if (data.mese <= 2) anni--;
    // un anno è bisestile se è non secolare e divisibile per 4 o secolare e divisibile per 400
    return anni / 4 - anni / 100 + anni / 400;
}

unsigned int anniPassati(Data primaData, Data secondaData) {
    // conta giorni dalla prima data all'anno zero
    unsigned int n1 = primaData.anno * 365 + primaData.giorno;
    for (int i = 0; i < primaData.mese - 1; i++) {
        n1 += giorniMese[i];
    }
    n1 += contaAnniBisestili(primaData);
    // conta giorni dalla seconda data all'anno zero
    unsigned int n2 = secondaData.anno * 365 + secondaData.giorno;
    for (int i = 0; i < secondaData.mese - 1; i++) {
        n2 += giorniMese[i];
    }
    n2 += contaAnniBisestili(secondaData);
    // calcola gli anni fra le due date
    unsigned int anni;
    if (n2 > n1) {
        anni = (n2 - n1) / 365;
    } else anni = 0;
    return anni;
}

int isLeapYear(int anno) {
    // ritorna 1 se l'anno è bisestile
    return (((anno % 4 == 0) && (anno % 100 != 0)) || (anno % 400 == 0));
}

int controllaData(Data data) {
    // controlla il range degli anni, mesi e giorni
    if (data.anno > 2070 || data.anno < 1900) return 0;
    // usando il tipo unsigned int nella funzione anniPassati non dovrebbe essere possibile calcolare più di 179 anni di differenza
    if (data.mese < 1 || data.mese > 12) return 0;
    if (data.giorno < 1 || data.giorno > 31) return 0;
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
    return 1;
}