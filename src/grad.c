#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define VISINA 10
#define SIRINA 10

char mapaTip[VISINA][SIRINA];
int mapaNivo[VISINA][SIRINA];

// Globalne promenljive
int budzet;
int populacija;
int sreca;
int potezBroj;

// ================= INICIJALIZACIJA =================

void inicijalizujMapu(void) {
    for (int i = 0; i < VISINA; i++) {
        for (int j = 0; j < SIRINA; j++) {
            mapaTip[i][j] = ' ';  // Prazno polje je razmak
            mapaNivo[i][j] = 0;   // Nivo je 0 za prazna polja
        }
    }
}

// Pomocna funkcija za ciscenje unosa (mora da bude tu zbog scanf-a u meniju)
void ocistiStdin(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF)
        ;
}

// ================= CRTANJE MAPE  =================

void crtajHorizontalnuLiniju(void) {
    printf("  +");
    for(int j = 0; j < SIRINA; j++)
        printf("----+");
    printf("\n");
}

void iscrtajMapu(void) {
    printf("\n     ");
    for(int i = 0; i < SIRINA; i++)
        printf("%d    ", i + 1);
    printf("\n");

    for(int i = 0; i < VISINA; i++) {
        crtajHorizontalnuLiniju();
        printf("%c |", 'A' + i);
        for(int j = 0; j < SIRINA; j++) {
            if(mapaTip[i][j] == ' ')
                printf("    |");
            else
                printf(" %c%d |", mapaTip[i][j], mapaNivo[i][j]);
        }
        printf("\n");
    }
    crtajHorizontalnuLiniju();
}

// ================= GLAVNA FUNKCIJA GRADA =================

int main(void) {
    int izbor, opcija;

    printf("=====================================\n");
    printf("            CITY BUILDER\n");
    printf("=====================================\n");

    do {
        printf("\nGLAVNI MENI:\n");
        printf("1 Nova igra\n");
        printf("2 Uputstvo\n");
        printf("0 Exit\n");
        printf("Vas izbor: ");

        if(scanf("%d", &izbor) != 1) {
            ocistiStdin();
            continue;
        }

        switch(izbor) {
            case 1:
                // Pokretanje nove igre - postavljanje pocetnih vrednosti
                budzet = 1250;
                populacija = 0;
                sreca = 50;
                potezBroj = 1;

                // Cistimo mapu pre nego sto igra pocne
                inicijalizujMapu();

                int krajPoteza = 0;
                while (!krajPoteza) {
                    printf("\n==================================================\n");
                    printf(" POTEZ %d | Budzet: %d EUR\n", potezBroj, budzet);
                    printf("==================================================\n");

                    // Automatsko crtanje trenutnog stanja mape na ekranu
                    iscrtajMapu();

                    printf("Odaberite opciju:\n");
                    printf("1 Postavi zgradu\n");
                    printf("2 Ukloni zgradu\n");
                    printf("3 Prikazi statistike\n");
                    printf("0 Sledeci potez\n");
                    printf("Izbor akcije: ");

                    if (scanf("%d", &opcija) != 1) {
                        ocistiStdin();
                        printf("Greska: Unesite broj!\n");
                        continue;
                    }

                    switch(opcija) {
                        case 1: {
                            int uspesnaGradnja = 0;
                            while (!uspesnaGradnja) {
                                char tip, red;
                                int kolona, cena = 0;

                                printf("\n--- GRADNJA ZGRADE (Unesite 'X' za povratak) ---\n");
                                printf("Unesite tip zgrade (S, P, B, F, K, Z): ");
                                scanf(" %c", &tip);
                                tip = (char)toupper((unsigned char)tip);

                                if (tip == 'X') {
                                    break;
                                }

                                if (tip == 'S') cena = 100;
                                else if (tip == 'P') cena = 50;
                                else if (tip == 'B') cena = 200;
                                else if (tip == 'F') cena = 150;
                                else if (tip == 'K') cena = 120;
                                else if (tip == 'Z') cena = 180;
                                else {
                                    printf("Greska: Nepoznat tip zgrade! Pokusajte ponovo.\n");
                                    continue;
                                }

                                printf("Unesite koordinatu (npr. A 2): ");
                                if (scanf(" %c %d", &red, &kolona) != 2) {
                                    ocistiStdin();
                                    printf("Greska: Los format koordinata! Pokusajte ponovo.\n");
                                    continue;
                                }
                                red = (char)toupper((unsigned char)red);

                                if (red < 'A' || red > 'J' || kolona < 1 || kolona > 10) {
                                    printf("Greska: Koordinate su van granica mape! Pokusajte ponovo.\n");
                                    continue;
                                }


                                int r = red - 'A';
                                int k = kolona - 1;

                                if (mapaTip[r][k] != ' ') {
                                    printf("Greska: Polje %c%d je vec zauzeto (%c%d)! Pokusajte ponovo.\n",
                                           red, kolona, mapaTip[r][k], mapaNivo[r][k]);
                                    continue;
                                }

                                if (budzet < cena) {
                                    printf("Greska: Nemate dovoljno novca! Potrebno: %d EUR, Trenutno: %d EUR\n", cena, budzet);
                                    break;
                                } else {

                                    mapaTip[r][k] = tip;
                                    mapaNivo[r][k] = 1;
                                    budzet -= cena;
                                    printf("Uspesno postavljena zgrada %c na %c%d! Preostali budzet: %d EUR\n", tip, red, kolona, budzet);
                                    uspesnaGradnja = 1;
                                }
                            }
                            break;
                        }

                        case 2:
                            {
                                char red;
                                int kolona;

                                printf("\n---UKLANJANJE ZGRADE---\n");
                                printf("Unesi koordinatu (npr. A 2):");

                                if(scanf(" %c %d", &red, &kolona)!=2)
                                {
                                    ocistiStdin();
                                    printf("Greska:los unos!\n");
                                    break;
                                }
                                red=(char)toupper((unsigned char)red);

                                if(red<'A' || red > 'J' || kolona < 1 || kolona > 10)
                                {
                                    printf("Greska:van granica mapre!\n");
                                    break;
                                }

                                int r = red - 'A';
                                int k = kolona -1;

                                if(mapaTip[r][k] == ' ')
                                {
                                    printf("Nema zgrade na tom polju!\n");
                                    break;
                                }
                                int cena = 0;

                                if(mapaTip[r][k] == 'S')cena = 100;
                                else if(mapaTip[r][k] == 'P')cena = 50;
                                else if(mapaTip[r][k] == 'B')cena = 200;
                                else if(mapaTip[r][k] == 'F')cena = 150;
                                else if(mapaTip[r][k] == 'K')cena = 120;
                                else if(mapaTip[r][k] == 'Z')cena = 180;

                                budzet += (cena*mapaNivo[r][k]) / 2;

                                mapaTip[r][k] = ' ';
                                mapaNivo[r][k] = 0;

                                printf("Zgrada uklonjena!\n");
                                break;
                            }
                            break;

                        case 3:
                            printf("\n--- STATISTIKA GRADA ---\n");
                            printf("Potez broj: %d\n", potezBroj);
                            printf("Budzet: %d EUR\n", budzet);
                            printf("Populacija: %d\n", populacija);
                            printf("Sreca: %d%%\n", sreca);
                            break;

                        case 0:
                            printf("Prelaz na sledeci potez...\n");
                            potezBroj++;
                            krajPoteza = 1; // Zavrsava trenutni potez i izlazi iz while petlje
                            break;

                        default:
                            printf("Odabrana opcija ne postoji!\n");
                    }
                }
                break;

            case 2:
                printf("\nUputstvo u razvoju (Faza 1 stub)...\n");
                break;
            case 0:
                printf("Gasenje igre...\n");
                break;
            default:
                printf("Pogresan izbor!\n");
        }

    } while(izbor != 0);

    return 0;
}
