#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#define VISINA 10
#define SIRINA 10

// ================= STRUKTURNE TIPOVE PODATAKA (FAZA 4) =================

typedef struct {
    char tip;
    char naziv[20];
    int cena;
    int prihod;
    int populacija;
    int sreca;
} Zgrada;

typedef struct {
    char mapaTip[VISINA][SIRINA];
    int mapaNivo[VISINA][SIRINA];
    int budzet;
    int populacija;
    int sreca;
    int potezBroj;
    int prihod;
    int troskovi;
    int odrzavanje;
    int negativanBudzetRundi;
    int igraAktivna;
    int zabranjenaGradnja;
} Grad;

// Globalna promenljiva za stanje grada
Grad mojGrad;

// Deklaracije funkcija sa novim potpisima
void azurirajStatistiku(char tip, int nivo, int faktor);
void azuriranjeKomsiluka(void);
void izracunajPrihod(void);
void izracunajOdrzavanje(void);

// ================= INICIJALIZACIJA =================

void inicijalizujMapu(void) {
    for (int i = 0; i < VISINA; i++) {
        for (int j = 0; j < SIRINA; j++) {
            mojGrad.mapaTip[i][j] = ' ';  // Prazno polje je razmak
            mojGrad.mapaNivo[i][j] = 0;   // Nivo je 0 za prazna polja
        }
    }

    mojGrad.mapaTip[0][5] = '~'; //A6
    mojGrad.mapaTip[1][5] = '~'; //B6
    mojGrad.mapaTip[1][6] = '~'; //B7
    mojGrad.mapaTip[2][6] = '~'; //C7
    mojGrad.mapaTip[3][6] = '~'; //D7
    mojGrad.mapaTip[3][7] = '~'; //D8
    mojGrad.mapaTip[4][7] = '~'; //E8
    mojGrad.mapaTip[5][7] = '~'; //F8
    mojGrad.mapaTip[5][8] = '~'; //F9
    mojGrad.mapaTip[5][9] = '~'; //F10
    mojGrad.mapaTip[6][9] = '~'; //G10
}

void ocistiStdin(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF)
        ;
}

// ================= CRTANJE MAPE  =================

void crtajHorizontalnuLiniju(void) {
    printf("   +");
    for(int j = 0; j < SIRINA; j++)
        printf("----+");
    printf("\n");
}

void iscrtajMapu(void) {
    printf("\n      ");
    for(int i = 0; i < SIRINA; i++)
        printf("%d    ", i + 1);
    printf("\n");

    for(int i = 0; i < VISINA; i++) {
        crtajHorizontalnuLiniju();
        printf("%c |", 'A' + i);
        for(int j = 0; j < SIRINA; j++) {
            if(mojGrad.mapaTip[i][j] == ' ')
                printf("    |");
            else if (mojGrad.mapaTip[i][j] == '~')
                printf("  ~ |");
            else
                printf(" %c%d |", mojGrad.mapaTip[i][j], mojGrad.mapaNivo[i][j]);
        }
        printf("\n");
    }
    crtajHorizontalnuLiniju();
}

// ================= NADOGRADNJA =================

void nadogradiZgradu(void) {
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

    if(mojGrad.mapaTip[r][k] == ' ') {
        printf("Na ovom polju nema zgrade!\n");
        return;
    }

    if (mojGrad.mapaTip[r][k] == '~') {
        printf("Greska: ne mozete nadograditi reku!\n");
        return;
    }

    if(mojGrad.mapaTip[r][k] == 'P' || mojGrad.mapaTip[r][k] == 'Z' || mojGrad.mapaTip[r][k] == 'M') {
        printf("Ova zgrada ne moze da se nadogradjuje!\n");
        return;
    }

    if(mojGrad.mapaNivo[r][k] >= 3) {
        printf("Ova zgrada je vec na maksimalnom nivou!\n");
        return;
    }

    int cena = 0;
    if (mojGrad.mapaTip[r][k] == 'S') cena = 100;
    else if (mojGrad.mapaTip[r][k] == 'B') cena = 200;
    else if (mojGrad.mapaTip[r][k] == 'F') cena = 150;
    else if (mojGrad.mapaTip[r][k] == 'K') cena = 120;
    else if (mojGrad.mapaTip[r][k] == 'C') cena = 110;
    else {
        printf("Greska: Nepoznat tip zgrade! Pokusajte ponovo.\n");
        return;
    }

    if(mojGrad.budzet < cena) {
        printf("Nemate dovoljno novca!\n");
        return;
    }

    mojGrad.budzet -= cena;
    mojGrad.mapaNivo[r][k]++;
    azurirajStatistiku(mojGrad.mapaTip[r][k], mojGrad.mapaNivo[r][k], 1);

    printf("Zgrada je uspesno nadogradjena!\n");
}

// ================= UPUTSTVO  =================

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
    printf("Za gradnju morate imati DOVOLJNO NOVCA.\n");
}

void azurirajStatistiku(char tip, int nivo, int faktor) {
    float mult = 1.0 + (nivo - 1) * 0.5;

    switch(tip) {
        case 'S':
            mojGrad.populacija += (int)(50 * mult) * faktor;
            mojGrad.sreca += (int)(10 * mult) * faktor;
            mojGrad.troskovi += 10 * faktor;
            break;
        case 'P':
            mojGrad.sreca += (int)(20 * mult) * faktor;
            mojGrad.troskovi += 5 * faktor;
            break;
        case 'B':
            mojGrad.sreca += (int)(30 * mult) * faktor;
            mojGrad.troskovi += 20 * faktor;
            break;
        case 'F':
            mojGrad.prihod += (int)(100 * mult) * faktor;
            mojGrad.sreca -= (int)(35 * mult) * faktor;
            mojGrad.troskovi += 15 * faktor;
            break;
        case 'K':
            mojGrad.prihod += (int)(80 * mult) * faktor;
            mojGrad.troskovi += 12 * faktor;
            break;
        case 'Z':
            mojGrad.populacija += (int)(20 * mult) * faktor;
            mojGrad.sreca += (int)(40 * mult) * faktor;
            mojGrad.troskovi += 8 * faktor;
            break;
        case 'C':
            mojGrad.sreca += (int)(10 * mult) * faktor;
            mojGrad.prihod -= (int)(50 * mult) * faktor;
            mojGrad.troskovi += 8 * faktor;
            break;
        case 'M':
            mojGrad.prihod += (int)(70 * mult) * faktor;
            mojGrad.sreca += (int)(35 * mult) * faktor;
            mojGrad.troskovi += 22 * faktor;
            break;
    }
    if(mojGrad.sreca > 100) mojGrad.sreca = 100;
    if(mojGrad.sreca < 0) mojGrad.sreca = 0;
}

void azuriranjeKomsiluka(void) {
    int i, j, x, y;

    for(i=0; i < VISINA; i++) {
        for(j=0; j < SIRINA; j++) {
            char tip = mojGrad.mapaTip[i][j];

            if(tip == 'P') {
                for(x = i-1; x <= i+1; x++) {
                    for(y = j-1; y <= j+1; y++) {
                        if(x >= 0 && x < VISINA && y >= 0 && y < SIRINA) {
                            if(mojGrad.mapaTip[x][y] != ' ') {
                                mojGrad.sreca += 2;
                            }
                        }
                    }
                }
            }

            if(tip == 'F') {
                for(x = i-2; x<=i+2; x++) {
                    for(y = j-2; y <= j+2; y++) {
                        if (x >=0 && x <VISINA && y >= 0 && y < SIRINA) {
                            if(mojGrad.mapaTip[x][y] != ' ') {
                                mojGrad.sreca -= 3;
                            }
                        }
                    }
                }
            }
        }
    }

    if(mojGrad.sreca > 100) mojGrad.sreca = 100;
    if(mojGrad.sreca < 0) mojGrad.sreca = 0;
}

void izracunajOdrzavanje(void) {
    mojGrad.odrzavanje = 0;

    for(int i = 0; i < VISINA; i++) {
        for(int j = 0; j < SIRINA; j++) {
            char tip = mojGrad.mapaTip[i][j];
            int nivo = mojGrad.mapaNivo[i][j];
            int cena = 0;

            if(tip == 'S') cena = 100;
            if(tip == 'P') cena = 50;
            if(tip == 'B') cena = 200;
            if(tip == 'F') cena = 150;
            if(tip == 'K') cena = 120;
            if(tip == 'Z') cena = 180;
            if(tip == 'C') cena = 110;
            if(tip == 'M') cena = 220;

            mojGrad.odrzavanje += (cena * nivo) / 10;
        }
    }
}

void izracunajPrihod(void) {
    mojGrad.prihod = 0;

    for(int i = 0; i < VISINA; i++) {
        for(int j = 0; j < SIRINA; j++) {
            char tip = mojGrad.mapaTip[i][j];
            int nivo = mojGrad.mapaNivo[i][j];
            float mult = 1.0 + (nivo - 1) * 0.5;

            if(tip == 'F')
                mojGrad.prihod += (int)(100 * mult);
            if(tip == 'K')
                mojGrad.prihod += (int)(80 * mult);
        }
    }
}

// ================= NASUMICNI DOGADJAJI (BONUS) =================

void okiniNasumicniDogadjaj(void) {
    int sansa = rand() % 10;

    if (sansa > 2) {
        return;
    }

    int tipDogadjaja = rand() % 3;
    printf("\n--- VANREDNE VESTI U GRADU! ---\n");

    switch(tipDogadjaja) {
        case 0: {
            mojGrad.budzet += 200;
            printf("[DONACIJA] Anonimni investitor je uplatio 200 EUR u budzet grada!\n");
            break;
        }
        case 1: {
            mojGrad.sreca -= 10;
            if (mojGrad.sreca < 0) mojGrad.sreca = 0;
            printf("[EPIDEMIJA] Grip hara gradom! Sreca stanovnistva je opala za 10%%.\n");
            break;
        }
        case 2: {
            int fabrikaUnistena = 0;
            for (int i = 0; i < VISINA; i++) {
                for (int j = 0; j < SIRINA; j++) {
                    if (mojGrad.mapaTip[i][j] == 'F') {
                        mojGrad.mapaTip[i][j] = ' ';
                        mojGrad.mapaNivo[i][j] = 0;
                        azurirajStatistiku('F', 1, -1);

                        printf("[POZAR] Izbio je veliki pozar! Fabrika na poziciji %c%d je potpuno unistena.\n", 'A' + i, j + 1);
                        fabrikaUnistena = 1;
                        break;
                    }
                }
                if (fabrikaUnistena) break;
            }

            if (!fabrikaUnistena) {
                printf("[VREME] Jaka kisa je pogodila grad, ali nema materijalne stete.\n");
            }
            break;
        }
    }
    printf("--------------------------------\n");
}

void zavrsiPotez(void) {
    okiniNasumicniDogadjaj();
    mojGrad.troskovi = 0;

    azuriranjeKomsiluka();
    izracunajPrihod();
    izracunajOdrzavanje();

    mojGrad.budzet += mojGrad.prihod;
    mojGrad.budzet -= mojGrad.troskovi;
    mojGrad.budzet -= mojGrad.odrzavanje;

    if(mojGrad.budzet < 0) {
        mojGrad.budzet = 0;
        mojGrad.zabranjenaGradnja = 1;
    } else {
        mojGrad.zabranjenaGradnja = 0;
    }
    mojGrad.potezBroj++;
}

void proveriKrajIgre(void) {
    if (mojGrad.budzet < 0)
        mojGrad.negativanBudzetRundi++;
    else
        mojGrad.negativanBudzetRundi = 0;

    if (mojGrad.negativanBudzetRundi >= 3) {
        printf("=====================================\n");
        printf("KRAJ IGRE: BANKROT!\n");
        printf("Vas budzet je bio u minusu 3 poteza zaredom.\n");
        printf("=====================================\n");
        mojGrad.igraAktivna = 0;
    } else if (mojGrad.sreca < 20) {
        printf("=====================================\n");
        printf("KRAJ IGRE: POBUNA GRADJANA!\n");
        printf("Sreca stanovnistva je pala ispod 20%.\n");
        printf("=====================================\n");
        mojGrad.igraAktivna = 0;
    }
}

// ================= UCITAVANJE I SACUVANJE IGRE =================

void sacuvajIgru(const char *imeFajla) {
    FILE *f = fopen(imeFajla, "w");

    if(f == NULL) {
        printf("Greska pri cuvanju igre!\n");
        return;
    }

    fprintf(f, "%d %d %d %d\n", mojGrad.budzet, mojGrad.populacija, mojGrad.sreca, mojGrad.potezBroj);

    for(int i = 0; i < VISINA; i++) {
        for(int j = 0; j < SIRINA; j++) {
            fprintf(f, "%c %d\n", mojGrad.mapaTip[i][j], mojGrad.mapaNivo[i][j]);
        }
    }

    fclose(f);
    printf("Igra je sacuvana!\n");
}

void ucitajIgru(const char *imeFajla) {
    FILE *f = fopen(imeFajla, "r");

    if(f == NULL) {
        printf("Ne postoji sacuvana igra!\n");
        return;
    }

    fscanf(f, "%d %d %d %d", &mojGrad.budzet, &mojGrad.populacija, &mojGrad.sreca, &mojGrad.potezBroj);
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
                mojGrad.mapaTip[i][j] = ' ';
                sscanf(linija + 1, "%d", &mojGrad.mapaNivo[i][j]);
            } else {
                sscanf(linija, " %c %d", &mojGrad.mapaTip[i][j], &mojGrad.mapaNivo[i][j]);
            }
        }
    }

    fclose(f);

    mojGrad.prihod = 0;
    mojGrad.troskovi = 0;
    mojGrad.odrzavanje = 0;
    mojGrad.negativanBudzetRundi = 0;
    mojGrad.zabranjenaGradnja = 0;

    izracunajPrihod();
    izracunajOdrzavanje();
    printf("Igra je ucitana!\n");
}

// ================= POKRETANJE =================

void pokreniIgru(void) {
    int opcija;

    while (mojGrad.igraAktivna) {
        printf("\n==================================================\n");
        printf(" POTEZ %d | Budzet: %d EUR\n", mojGrad.potezBroj, mojGrad.budzet);
        printf("==================================================\n");

        iscrtajMapu();

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

                    if (tip == 'X') {
                        break;
                    }

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

                    if (mojGrad.mapaTip[r][k] == '~') {
                        printf("Greska: Na ovom polju tece reka! Gradnja je nemoguca.\n");
                        continue;
                    }

                    if (mojGrad.mapaTip[r][k] != ' ') {
                        printf("Greska: Polje %c%d je vec zauzeto (%c%d)!\n", red, kolona, mojGrad.mapaTip[r][k], mojGrad.mapaNivo[r][k]);
                        continue;
                    }

                    if (mojGrad.budzet < cena || mojGrad.zabranjenaGradnja) {
                        printf("Greska: Grad je u finansijskoj krizi. Nemate dovoljno novca!\n");
                        continue;
                    } else {
                        mojGrad.budzet -= cena;
                        mojGrad.mapaTip[r][k] = tip;
                        mojGrad.mapaNivo[r][k] = 1;
                        azurirajStatistiku(tip, 1, 1);
                        printf("Uspesno postavljena zgrada %c na %c%d! Preostali budzet: %d EUR\n", tip, red, kolona, mojGrad.budzet);
                        uspesnaGradnja = 1;

                        zavrsiPotez();
                        proveriKrajIgre();
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

                if(mojGrad.mapaTip[r][k] == ' ') {
                    printf("Nema zgrade na tom polju!\n");
                    break;
                }

                if (mojGrad.mapaTip[r][k] == '~') {
                    printf("Greska: ne mozete rusiti prirodne objekte!\n");
                    break;
                }

                int cena = 0;
                if(mojGrad.mapaTip[r][k] == 'S') cena = 100;
                else if(mojGrad.mapaTip[r][k] == 'P') cena = 50;
                else if(mojGrad.mapaTip[r][k] == 'B') cena = 200;
                else if(mojGrad.mapaTip[r][k] == 'F') cena = 150;
                else if(mojGrad.mapaTip[r][k] == 'K') cena = 120;
                else if(mojGrad.mapaTip[r][k] == 'Z') cena = 180;
                else if(mojGrad.mapaTip[r][k] == 'C') cena = 110;
                else if(mojGrad.mapaTip[r][k] == 'M') cena = 220;

                mojGrad.budzet += (cena * mojGrad.mapaNivo[r][k]) / 2;

                char tip = mojGrad.mapaTip[r][k];
                int nivo = mojGrad.mapaNivo[r][k];

                azurirajStatistiku(tip, nivo, -1);

                mojGrad.mapaTip[r][k] = ' ';
                mojGrad.mapaNivo[r][k] = 0;

                printf("Zgrada uklonjena!\n");

                zavrsiPotez();
                proveriKrajIgre();
                break;
            }

            case 3:
                nadogradiZgradu();
                zavrsiPotez();
                proveriKrajIgre();
                break;

            case 4:
                printf("\n--- STATISTIKA GRADA ---\n");
                printf("Potez broj: %d\n", mojGrad.potezBroj);
                printf("Budzet: %d EUR\n", mojGrad.budzet);
                printf("Populacija: %d\n", mojGrad.populacija);
                printf("Sreca: %d%%\n", mojGrad.sreca);
                break;

            case 5:
                sacuvajIgru("save.txt");
                break;

            case 0:
                printf("Prelaz na sledeci potez...\n");
                zavrsiPotez();
                proveriKrajIgre();
                break;

            default:
                printf("Odabrana opcija ne postoji!\n");
        }
    }
}

// ================= GLAVNA FUNKCIJA =================

int main(void) {
    int izbor;
    srand(time(NULL));

    printf("=====================================\n");
    printf("             CITY BUILDER\n");
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

                inicijalizujMapu();
                pokreniIgru();
                break;

            case 2:
                prikazi_uputstvo();
                break;

            case 3:
                ucitajIgru("save.txt");
                mojGrad.igraAktivna = 1;
                pokreniIgru();
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
