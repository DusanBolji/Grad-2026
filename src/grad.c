#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define VISINA 10
#define SIRINA 10

// ================= STRUKTURE PODATAKA =================

typedef struct {
    char mapaTip[VISINA][SIRINA];
    int mapaNivo[VISINA][SIRINA];
    int budzet;
    int populacija;
    int sreca;
    int potezBroj;
    int prihod;
    int troskovi;
    int negativanBudzetRundi;
    int igraAktivna;
    int odrzavanje;
    int zabranjenaGradnja;
} Grad;

// ================= DEKLARACIJE FUNKCIJA =================

void inicijalizujMapu(Grad *g);
void ocistiStdin(void);
void crtajHorizontalnuLiniju(void);
void iscrtajMapu(Grad *g);
void azurirajStatistiku(Grad *g, char tip, int nivo, int faktor);
void nadogradiZgradu(Grad *g);
void prikazi_uputstvo(void);
void azuriranjeKomsiluka(Grad *g);
void izracunajOdrzavanje(Grad *g);
void izracunajPrihod(Grad *g);
void okiniNasumicniDogadjaj(Grad *g);
void zavrsiPotez(Grad *g);
void proveriKrajIgre(Grad *g);
void sacuvajIgru(Grad *g, const char *imeFajla);
void ucitajIgru(Grad *g, const char *imeFajla);
void pokreniIgru(Grad *g);

// ================= INICIJALIZACIJA =================

void inicijalizujMapu(Grad *g) {
    for (int i = 0; i < VISINA; i++) {
        for (int j = 0; j < SIRINA; j++) {
            g->mapaTip[i][j] = ' ';  // Prazno polje je razmak
            g->mapaNivo[i][j] = 0;   // Nivo je 0 za prazna polja
        }
    }

    g->mapaTip[0][5] = '~'; //A6
    g->mapaTip[1][5] = '~'; //B6
    g->mapaTip[1][6] = '~'; //B7
    g->mapaTip[2][6] = '~'; //C7
    g->mapaTip[3][6] = '~'; //D7
    g->mapaTip[3][7] = '~'; //D8
    g->mapaTip[4][7] = '~'; //E8
    g->mapaTip[5][7] = '~'; //F8
    g->mapaTip[5][8] = '~'; //F9
    g->mapaTip[5][9] = '~'; //F10
    g->mapaTip[6][9] = '~'; //G10
}

void ocistiStdin(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF)
        ;
}

// ================= CRTANJE MAPE =================

void crtajHorizontalnuLiniju(void) {
    printf("   +");
    for(int j = 0; j < SIRINA; j++)
        printf("----+");
    printf("\n");
}

void iscrtajMapu(Grad *g) {
    printf("\n      ");
    for(int i = 0; i < SIRINA; i++)
        printf("%d    ", i + 1);
    printf("\n");

    for(int i = 0; i < VISINA; i++) {
        crtajHorizontalnuLiniju();
        printf("%c |", 'A' + i);
        for(int j = 0; j < SIRINA; j++) {
            if(g->mapaTip[i][j] == ' ')
                printf("    |");
            else if (g->mapaTip[i][j] == '~')
                printf("  ~ |");
            else
                printf(" %c%d |", g->mapaTip[i][j], g->mapaNivo[i][j]);
        }
        printf("\n");
    }
    crtajHorizontalnuLiniju();
}

// ================= NADOGRADNJA =================

void nadogradiZgradu(Grad *g) {
    char red;
    int kolona;

    printf("Unesite koordinatu: ");

    if(scanf(" %c %d", &red, &kolona) != 2) {
        ocistiStdin();
        printf("Pogresan unos!\n");
        return;
    }

    red = (char)toupper((unsigned char)red);

    if(red < 'A' || red > 'J' || kolona < 1 || kolona > 10) {
        printf("Koordinate van mape!\n");
        return;
    }

    int r = red - 'A';
    int k = kolona - 1;

    if(g->mapaTip[r][k] == ' ') {
        printf("Na ovom polju nema zgrade!\n");
        return;
    }

    if (g->mapaTip[r][k] == '~') {
        printf("Greska: ne mozete nadograditi reku!\n");
        return;
    }

    if(g->mapaTip[r][k] == 'P' || g->mapaTip[r][k] == 'Z' || g->mapaTip[r][k] == 'M') {
        printf("Ova zgrada ne moze da se nadogradjuje!\n");
        return;
    }

    if(g->mapaNivo[r][k] >= 3) {
        printf("Ova zgrada je vec na maksimalnom nivou!\n");
        return;
    }

    int cena = 0;
    if (g->mapaTip[r][k] == 'S') cena = 100;
    else if (g->mapaTip[r][k] == 'B') cena = 200;
    else if (g->mapaTip[r][k] == 'F') cena = 150;
    else if (g->mapaTip[r][k] == 'K') cena = 120;
    else if (g->mapaTip[r][k] == 'C') cena = 110;
    else if (g->mapaTip[r][k] == 'M') cena = 220;
    else {
        printf("Greska: Nepoznat tip zgrade! Pokusajte ponovo.\n");
        return;
    }

    if(g->budzet < cena || g->zabranjenaGradnja) {
        printf("Nemate dovoljno novca ili je grad u bankrotu!\n");
        return;
    }

    g->budzet -= cena;
    g->mapaNivo[r][k]++;
    azurirajStatistiku(g, g->mapaTip[r][k], g->mapaNivo[r][k], 1);

    printf("Zgrada je uspesno nadogradjena!\n");
}

// ================= UPUTSTVO =================

void prikazi_uputstvo(void) {
    printf("=====================================\n");
    printf("            UPUTSTVO\n");
    printf("=====================================\n");
    printf("Cilj igre je razvoj grada.\n");
    printf("Gradite zgrade, unapredjujte ih i upravljajte budzetom dok pratite srecu i populaciju!\n");
    printf("=====================================\n");
    printf("SIMBOLI ZGRADA:\n");
    printf("S - Stambena zgrada(100 EUR), +50 populacije, +10 srece na tom polju\n");
    printf("P - Park(50 EUR), +20 srece\n");
    printf("B - Bolnica(200 EUR), +30 srece svim stanovnicima\n");
    printf("F - Fabrika(150 EUR), +100 prihoda, -35 srece u komsiluku\n");
    printf("K - Kancelarije(120 EUR), +80 prihoda\n");
    printf("Z - Zabavni park(180 EUR), +40 srece, +20 populacije\n");
    printf("C - Kazino(110 EUR), +10 srece, -50 prihod\n");
    printf("M - Muzej(220 EUR), +35 srece, +70 prihod\n");
    printf("=====================================\n");
    printf("TOK IGRE:\n");
    printf("1 - Postavi zgradu\n");
    printf("2 - Ukloni zgradu\n");
    printf("3 - Prikazi statistike\n");
    printf("0 - Sledeci potez\n");
    printf("=====================================\n");
    printf("Koordinate se unose u formatu:\n");
    printf("- SLOVO BROJ -\n");
    printf("A 2\n");
    printf("C 6\n");
    printf("=====================================\n");
    printf("Maksimalan nivo zgrade je 3.\n");
    printf("Cena nadogradnje je ista kao originalna cena zgrade, svi efekti rastu za 50%!\n");
    printf("Nije moguce nadograditi: park, zabavni centar i muzej.\n");
    printf("Za gradnju se koriste PRAZNA POLJA.\n");
}

void azurirajStatistiku(Grad *g, char tip, int nivo, int faktor) {
    float mult = 1.0 + (nivo - 1) * 0.5;

    switch(tip) {
        case 'S':
            g->populacija += (int)(50 * mult) * faktor;
            g->sreca += (int)(10 * mult) * faktor;
            g->troskovi += 10 * faktor;
            break;
        case 'P':
            g->sreca += (int)(20 * mult) * faktor;
            g->troskovi += 5 * faktor;
            break;
        case 'B':
            g->sreca += (int)(30 * mult) * faktor;
            g->troskovi += 20 * faktor;
            break;
        case 'F':
            g->prihod += (int)(100 * mult) * faktor;
            g->sreca -= (int)(35 * mult) * faktor;
            g->troskovi += 15 * faktor;
            break;
        case 'K':
            g->prihod += (int)(80 * mult) * faktor;
            g->troskovi += 12 * faktor;
            break;
        case 'Z':
            g->populacija += (int)(20 * mult) * faktor;
            g->sreca += (int)(40 * mult) * faktor;
            g->troskovi += 8 * faktor;
            break;
        case 'C':
            g->sreca += (int)(10 * mult) * faktor;
            g->prihod -= (int)(50 * mult) * faktor;
            g->troskovi += 8 * faktor;
            break;
        case 'M':
            g->prihod += (int)(70 * mult) * faktor;
            g->sreca += (int)(35 * mult) * faktor;
            g->troskovi += 22 * faktor;
            break;
    }
    if(g->sreca > 100) g->sreca = 100;
    if(g->sreca < 0) g->sreca = 0;
}

// ================= PROMENA 1: ISPRAVLJENO RAČUNANJE KOMŠILUKA =================

void azuriranjeKomsiluka(Grad *g) {
    int i, j, x, y;

    // Resetujemo srecu na baznu vrednost pre nego sto ponovo uracunamo komsiluk
    // Time sprecavamo da sreca ide u beskonacnost svakim potezom
    int imaStambenih = 0;
    int baznaSreca = 0;

    for(i=0; i < VISINA; i++) {
        for(j=0; j < SIRINA; j++) {
            if(g->mapaTip[i][j] == 'S') {
                float mult = 1.0 + (g->mapaNivo[i][j] - 1) * 0.5;
                baznaSreca += (int)(50 * mult); // Faza 2: Pocetna sreca stanovnika je 50%
                imaStambenih++;
            }
        }
    }

    if(imaStambenih > 0) {
        g->sreca = baznaSreca / imaStambenih;
    } else {
        g->sreca = 50;
    }

    // Sada primenjujemo bonuse/penale iz neposredne okoline
    for(i=0; i < VISINA; i++) {
        for(j=0; j < SIRINA; j++) {
            char tip = g->mapaTip[i][j];

            if(tip == 'P') {
                for(x = i-1; x <= i+1; x++) {
                    for(y = j-1; y <= j+1; y++) {
                        if(x >= 0 && x < VISINA && y >= 0 && y < SIRINA) {
                            if(g->mapaTip[x][y] == 'S') { // Utice samo na stambene zgrade
                                g->sreca += 2;
                            }
                        }
                    }
                }
            }

            if(tip == 'F') {
                for(x = i-2; x<=i+2; x++) {
                    for(y = j-2; y <= j+2; y++) {
                        if (x >=0 && x <VISINA && y >= 0 && y < SIRINA) {
                            if(g->mapaTip[x][y] == 'S') { // Fabrika kvari srecu stanarima
                                g->sreca -= 3;
                            }
                        }
                    }
                }
            }
        }
    }

    if(g->sreca > 100) g->sreca = 100;
    if(g->sreca < 0) g->sreca = 0;
}

void izracunajOdrzavanje(Grad *g) {
    g->odrzavanje = 0;

    for(int i = 0; i < VISINA; i++) {
        for(int j = 0; j < SIRINA; j++) {
            char tip = g->mapaTip[i][j];
            int nivo = g->mapaNivo[i][j];
            int cena = 0;

            if(tip == 'S') cena = 100;
            if(tip == 'P') cena = 50;
            if(tip == 'B') cena = 200;
            if(tip == 'F') cena = 150;
            if(tip == 'K') cena = 120;
            if(tip == 'Z') cena = 180;
            if(tip == 'C') cena = 110;
            if(tip == 'M') cena = 220;

            g->odrzavanje += (cena * nivo) / 10;
        }
    }
}

void izracunajPrihod(Grad *g) {
    g->prihod = 0;

    for(int i = 0; i < VISINA; i++) {
        for(int j = 0; j < SIRINA; j++) {
            char tip = g->mapaTip[i][j];
            int nivo = g->mapaNivo[i][j];
            float mult = 1.0 + (nivo - 1) * 0.5;

            if(tip == 'F') g->prihod += (int)(100 * mult);
            if(tip == 'K') g->prihod += (int)(80 * mult);
            if(tip == 'M') g->prihod += (int)(70 * mult);
            if(tip == 'C') g->prihod -= (int)(50 * mult);
        }
    }
}

// ================= NASUMICNI DOGADJAJI (BONUS) =================

void okiniNasumicniDogadjaj(Grad *g) {
    int sansa = rand() % 10;
    if (sansa > 2) return; // 30% sansa za dogadjaj

    int tipDogadjaja = rand() % 3;
    printf("\n--- VANREDNE VESTI U GRADU! ---\n");

    switch(tipDogadjaja) {
        case 0:
            g->budzet += 200;
            printf("[DONACIJA] Anonimni investitor je uplatio 200 EUR u budzet grada!\n");
            break;
        case 1:
            g->sreca -= 10;
            if (g->sreca < 0) g->sreca = 0;
            printf("[EPIDEMIJA] Grip hara gradom! Sreca stanovnistva je opala za 10%%.\n");
            break;
        case 2: {
            int fabrikaUnistena = 0;
            for (int i = 0; i < VISINA; i++) {
                for (int j = 0; j < SIRINA; j++) {
                    if (g->mapaTip[i][j] == 'F') {
                        g->mapaTip[i][j] = ' ';
                        g->mapaNivo[i][j] = 0;
                        azurirajStatistiku(g, 'F', 1, -1);
                        printf("[POZAR] Izbio je pozar! Fabrika na poziciji %c%d je unistena.\n", 'A' + i, j + 1);
                        fabrikaUnistena = 1;
                        break;
                    }
                }
                if (fabrikaUnistena) break;
            }
            if (!fabrikaUnistena) {
                printf("[VREME] Jaka kisa je pogodila grad, ali nema stete.\n");
            }
            break;
        }
    }
    printf("--------------------------------\n");
}

// ================= PROMENA 2: SREĐENO UPRAVLJANJE BUDŽETOM KOD ZAVRŠETKA POTEZA =================

void zavrsiPotez(Grad *g) {
    okiniNasumicniDogadjaj(g);

    azuriranjeKomsiluka(g);
    izracunajPrihod(g);
    izracunajOdrzavanje(g);

    g->budzet += g->prihod;
    g->budzet -= g->troskovi;    // Troskovi se ovde ne resetuju na 0 pre vremena
    g->budzet -= g->odrzavanje;

    g->troskovi = 0;             // Resetujemo troskove tek na samom kraju poteza za sledeci potez

    if(g->budzet < 0) {
        g->zabranjenaGradnja = 1; // Kada je u minusu, zabranjeno je graditi nove stvari
    } else {
        g->zabranjenaGradnja = 0;
    }
    g->potezBroj++;
}

void proveriKrajIgre(Grad *g) {
    if (g->budzet < 0) {
        g->negativanBudzetRundi++;
    } else {
        g->negativanBudzetRundi = 0;
    }

    if (g->negativanBudzetRundi >= 3) {
        printf("=====================================\n");
        printf("KRAJ IGRE: BANKROT!\n");
        printf("Vas budzet je bio u minusu 3 poteza zaredom.\n");
        printf("=====================================\n");
        g->igraAktivna = 0;
    } else if (g->sreca < 20) {
        printf("=====================================\n");
        printf("KRAJ IGRE: POBUNA GRADJANA!\n");
        printf("Sreca stanovnistva je pala ispod 20%%.\n");
        printf("=====================================\n");
        g->igraAktivna = 0;
    }
}

// ================= UCITAVANJE I SACUVANJE IGRE =================

void sacuvajIgru(Grad *g, const char *imeFajla) {
    FILE *f = fopen(imeFajla, "w");
    if(f == NULL) {
        printf("Greska pri cuvanju igre!\n");
        return;
    }

    fprintf(f, "%d %d %d %d\n", g->budzet, g->populacija, g->sreca, g->potezBroj);

    for(int i = 0; i < VISINA; i++) {
        for(int j = 0; j < SIRINA; j++) {
            fprintf(f, "%c %d\n", g->mapaTip[i][j], g->mapaNivo[i][j]);
        }
    }
    fclose(f);
    printf("Igra je sacuvana!\n");
}

void ucitajIgru(Grad *g, const char *imeFajla) {
    FILE *f = fopen(imeFajla, "r");
    if(f == NULL) {
        printf("Ne postoji sacuvana igra!\n");
        return;
    }

    fscanf(f, "%d %d %d %d", &g->budzet, &g->populacija, &g->sreca, &g->potezBroj);
    fgetc(f);

    for(int i = 0; i < VISINA; i++) {
        for(int j = 0; j < SIRINA; j++) {
            char linija[20];
            if(fgets(linija, sizeof(linija), f) == NULL) {
                printf("Greska pri ucitavanju!\n");
                fclose(f);
                return;
            }

            if(linija[0] == ' ') {
                g->mapaTip[i][j] = ' ';
                sscanf(linija + 1, "%d", &g->mapaNivo[i][j]);
            } else {
                sscanf(linija, " %c %d", &g->mapaTip[i][j], &g->mapaNivo[i][j]);
            }
        }
    }
    fclose(f);

    g->prihod = 0;
    g->troskovi = 0;
    g->odrzavanje = 0;
    g->negativanBudzetRundi = 0;
    g->zabranjenaGradnja = 0;

    izracunajPrihod(g);
    izracunajOdrzavanje(g);
    printf("Igra je ucitana!\n");
}

// ================= GLAVNA PETLJA IGRE =================

void pokreniIgru(Grad *g) {
    int opcija;

    while (g->igraAktivna) {
        printf("\n==================================================\n");
        printf(" POTEZ %d | Budzet: %d EUR | Sreca: %d%% | Pop: %d\n", g->potezBroj, g->budzet, g->sreca, g->populacija);
        printf("==================================================\n");

        iscrtajMapu(g);

        printf("Odaberite opciju:\n");
        printf("1 Postavi zgradu\n");
        printf("2 Ukloni zgradu\n");
        printf("3 Nadogradi zgradu\n");
        printf("4 Prikazi statistike\n");
        printf("5 Sacuvaj igru\n");
        printf("0 Preskoci potez\n");
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
                    printf("Unesite tip zgrade (S, P, B, F, K, Z, C, M): ");
                    scanf(" %c", &tip);
                    tip = (char)toupper((unsigned char)tip);

                    if (tip == 'X') break;

                    if (tip == 'S') cena = 100;
                    else if (tip == 'P') cena = 50;
                    else if (tip == 'B') cena = 200;
                    else if (tip == 'F') cena = 150;
                    else if (tip == 'K') cena = 120;
                    else if (tip == 'Z') cena = 180;
                    else if (tip == 'C') cena = 110;
                    else if (tip == 'M') cena = 220;
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

                    if (g->mapaTip[r][k] == '~') {
                        printf("Greska: Na ovom polju tece reka! Gradnja je nemoguca.\n");
                        continue;
                    }

                    if (g->mapaTip[r][k] != ' ') {
                        printf("Greska: Polje %c%d je vec zauzeto (%c%d)! Pokusajte ponovo.\n",
                               red, kolona, g->mapaTip[r][k], g->mapaNivo[r][k]);
                        continue;
                    }

                    if (g->budzet < cena || g->zabranjenaGradnja) {
                        printf("Greska: Grad je u finansijskoj krizi ili nemate dovoljno novca! Potrebno: %d EUR, Trenutno: %d EUR\n", cena, g->budzet);
                        break;
                    } else {
                        g->budzet -= cena;
                        g->mapaTip[r][k] = tip;
                        g->mapaNivo[r][k] = 1;
                        azurirajStatistiku(g, tip, 1, 1);
                        printf("Uspesno postavljena zgrada %c na %c%d! Preostali budzet: %d EUR\n", tip, red, kolona, g->budzet);
                        uspesnaGradnja = 1;

                        zavrsiPotez(g);
                        proveriKrajIgre(g);
                    }
                }
                break;
            }
            case 2: {
                char red;
                int kolona;

                printf("\n--- UKLANJANJE ZGRADE ---\n");
                printf("Unesi koordinatu (npr. A 2): ");

                if(scanf(" %c %d", &red, &kolona) != 2) {
                    ocistiStdin();
                    printf("Greska: los unos!\n");
                    break;
                }
                red = (char)toupper((unsigned char)red);

                if(red < 'A' || red > 'J' || kolona < 1 || kolona > 10) {
                    printf("Greska: van granica mape!\n");
                    break;
                }

                int r = red - 'A';
                int k = kolona - 1;

                if(g->mapaTip[r][k] == ' ') {
                    printf("Nema zgrade na tom polju!\n");
                    break;
                }

                if (g->mapaTip[r][k] == '~') {
                    printf("Greska: ne mozete rusiti prirodne objekte!\n");
                    break;
                }

                int cena = 0;
                if(g->mapaTip[r][k] == 'S') cena = 100;
                else if(g->mapaTip[r][k] == 'P') cena = 50;
                else if(g->mapaTip[r][k] == 'B') cena = 200;
                else if(g->mapaTip[r][k] == 'F') cena = 150;
                else if(g->mapaTip[r][k] == 'K') cena = 120;
                else if(g->mapaTip[r][k] == 'Z') cena = 180;
                else if(g->mapaTip[r][k] == 'C') cena = 110;
                else if(g->mapaTip[r][k] == 'M') cena = 220;

                g->budzet += (cena * g->mapaNivo[r][k]) / 2;

                char tip = g->mapaTip[r][k];
                int nivo = g->mapaNivo[r][k];

                azurirajStatistiku(g, tip, nivo, -1);

                g->mapaTip[r][k] = ' ';
                g->mapaNivo[r][k] = 0;

                printf("Zgrada uklonjena!\n");

                zavrsiPotez(g);
                proveriKrajIgre(g);
                break;
            }
            case 3:
                nadogradiZgradu(g);
                zavrsiPotez(g);
                proveriKrajIgre(g);
                break;

            case 4:
                printf("\n--- STATISTIKA GRADA ---\n");
                printf("Potez broj: %d\n", g->potezBroj);
                printf("Budzet: %d EUR\n", g->budzet);
                printf("Populacija: %d\n", g->populacija);
                printf("Sreca: %d%%\n", g->sreca);
                break;

            case 5:
                sacuvajIgru(g, "save.txt");
                break;

            case 0:
                printf("Prelaz na sledeci potez...\n");
                zavrsiPotez(g);
                proveriKrajIgre(g);
                break;

            default:
                printf("Odabrana opcija ne postoji!\n");
        }
    }
}

// ================= GLAVNA FUNKCIJA =================

int main(void) {
    srand(time(NULL)); // Inicijalizacija random generatora
    int izbor;
    Grad mojGrad; // Kreiranje instance strukture

    printf("=====================================\n");
    printf("            CITY BUILDER\n");
    printf("=====================================\n");

    do {
        printf("\nGLAVNI MENI:\n");
        printf("1 Nova igra\n");
        printf("2 Uputstvo\n");
        printf("3 Ucitaj igru\n");
        printf("0 Exit\n");
        printf("Vas izbor: ");

        if(scanf("%d", &izbor) != 1) {
            ocistiStdin();
            continue;
        }

        switch(izbor) {
            case 1:
                mojGrad.budzet = 1250;
                mojGrad.populacija = 0;
                mojGrad.sreca = 50;
                mojGrad.potezBroj = 1;
                mojGrad.prihod = 0;
                mojGrad.troskovi = 0;
                mojGrad.odrzavanje = 0;
                mojGrad.negativanBudzetRundi = 0;
                mojGrad.zabranjenaGradnja = 0;
                mojGrad.igraAktivna = 1;

                inicijalizujMapu(&mojGrad);
                pokreniIgru(&mojGrad);
                break;

            case 2:
                prikazi_uputstvo();
                break;

            case 3:
                mojGrad.igraAktivna = 1;
                ucitajIgru(&mojGrad, "save.txt");
                pokreniIgru(&mojGrad);
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
