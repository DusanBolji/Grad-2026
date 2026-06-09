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
int prihod = 0;
int troskovi = 0;
int negativanBudzetRundi = 0;
int igraAktivna = 1;
int odrzavanje = 0;
int zabranjenaGradnja = 0;
void azurirajStatistiku(char tip, int nivo, int faktor);
void azuriranjeKomsiluka();

// ================= INICIJALIZACIJA =================

void inicijalizujMapu(void) {
    for (int i = 0; i < VISINA; i++) {
        for (int j = 0; j < SIRINA; j++) {
            mapaTip[i][j] = ' ';  // Prazno polje je razmak
            mapaNivo[i][j] = 0;   // Nivo je 0 za prazna polja
        }
    }

    mapaTip[0][5] = '~'; //A6
    mapaTip[1][5] = '~'; //B6
    mapaTip[1][6] = '~'; //B7
    mapaTip[2][6] = '~'; //C7
    mapaTip[3][6] = '~'; //D7
    mapaTip[3][7] = '~'; //D8
    mapaTip[4][7] = '~'; //E8
    mapaTip[5][7] = '~'; //F8
    mapaTip[5][8] = '~';//F9
    mapaTip[5][9] = '~'; //F10
    mapaTip[6][9] = '~'; //G10
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
            else if (mapaTip[i][j] == '~')
                printf("  ~ |");
            else
                printf(" %c%d |", mapaTip[i][j], mapaNivo[i][j]);
        }
        printf("\n");
    }
    crtajHorizontalnuLiniju();
}


 // ================= NADOGRADNJA=================
void nadogradiZgradu(void)
{
    char red;
    int kolona;

    printf("Unesite koordinatu: ");

    if(scanf(" %c %d", &red, &kolona) != 2)
    {
        ocistiStdin();
        printf("Pogresan unos!\n");
        return;
    }

    red = (char)toupper((unsigned char)red);

    if(red < 'A' || red > 'J' || kolona < 1 || kolona > 10)
    {
        printf("Koordinate van mape!\n");
        return;
    }

    int r = red - 'A';
    int k = kolona - 1;

    if(mapaTip[r][k] == ' ')
    {
        printf("Na ovom polju nema zgrade!\n");
        return;
    }

    if (mapaTip[r][k] == '~')
    {
        printf("Greska: ne mozete nadograditi reku!");
        return;
    }

    if(mapaTip[r][k] == 'P' || mapaTip[r][k] == 'Z' || mapaTip[r][k] == 'M')
    {
        printf("Ova zgrada ne moze da se nadogradjuje!\n");
        return;
    }

    if(mapaNivo[r][k] >= 3)
    {
        printf("Ova zgrada je vec na maksimalnom nivou!");
        return;
    }

    int cena = 0;
    if (mapaTip[r][k] == 'S') cena = 100;
    else if (mapaTip[r][k] == 'B') cena = 200;
    else if (mapaTip[r][k] == 'F') cena = 150;
    else if (mapaTip[r][k] == 'K') cena = 120;
    else if (mapaTip[r][k] == 'C') cena = 110;
    else {printf("Greska: Nepoznat tip zgrade! Pokusajte ponovo.\n");
    }

    if(budzet < cena)
    {
        printf("Nemate dovoljno novca!\n");
        return;
    }

    budzet -= cena;
    mapaNivo[r][k]++;
    azurirajStatistiku(mapaTip[r][k], mapaNivo[r][k], 1);

    printf("Zgrada je uspesno nadogradjena!\n");
    }


// ================= UPUTSTVO  =================
void prikazi_uputstvo(void)
{
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


void azurirajStatistiku(char tip, int nivo, int faktor)
{
    float mult = 1.0 + (nivo - 1) * 0.5;

    switch(tip)
    {
    case 'S':
        populacija +=(int)(50 * mult) * faktor;
        sreca +=(int)(10 * mult) * faktor;
        troskovi += 10 * faktor;
        break;
    case 'P':
        sreca +=(int)(20 * mult) * faktor;
        troskovi += 5 * faktor;
        break;
    case 'B':
        sreca +=(int)(30 * mult) * faktor;
        troskovi += 20 * faktor;
        break;
    case 'F':
        prihod +=(int)(100 * mult) * faktor;
        sreca -=(int)(35 * mult) * faktor;
        troskovi += 15 * faktor;
        break;
    case 'K':
        prihod +=(int)(80 * mult) * faktor;
        troskovi += 12 * faktor;
        break;
    case 'Z':
        populacija +=(int)(20 * mult) * faktor;
        sreca +=(int)(40 * mult) * faktor;
        troskovi += 8 * faktor;
        break;
    case 'C':
        sreca +=(int)(10 * mult) * faktor;
        prihod -=(int)(50 * mult) * faktor;
        troskovi += 8 * faktor;
        break;
    case 'M':
        prihod +=(int)(70 * mult) * faktor;
        sreca +=(int)(35 * mult) * faktor;
        troskovi += 22 * faktor;
        break;
    }
    if(sreca > 100)sreca = 100;
    if(sreca < 0)sreca = 0;
}

//AZURIRANJE STATISTIKE KROZ IGRU//

void azuriranjeKomsiluka()
{
    int i, j, x, y;

    for(i=0; i < VISINA; i++)
    {
        for(j=0; j < SIRINA;j++)
        {
            char tip = mapaTip[i][j];

            if(tip == 'P')
            {
                for(x = i-1; x <= i+1; x++)
                {
                    for(y = j-1; y <= j+1; y++)
                    {
                        if(x >= 0 && x < VISINA && y >= 0 && y < SIRINA)
                        {
                            if(mapaTip[x][y] != ' ')
                            {
                                sreca += 2;
                            }

                        }
                    }
                }
            }

            if(tip == 'F')
            {
                for(x = i-2; x<=i+2; x++)
                {
                    for(y = j-2; y <= j+2; y++)
                    {
                        if (x >=0 && x <VISINA && y >= 0 && y < SIRINA)
                        {
                            if(mapaTip[x][y] != ' ')
                            {
                                sreca -= 3;
                            }
                        }
                    }
                }
            }
        }
    }

    if(sreca > 100)sreca = 100;
    if(sreca < 0)sreca = 0;
}
//AZURIRANJE SRECE KOMSILUKA//


//DODATI PRIHODI I TROSKOVI PRI SVAKOM POTEZU//
void izracunajOdrzavanje()
{
    odrzavanje = 0;

    for(int i = 0; i < VISINA; i++)
    {
        for(int j = 0; j < SIRINA; j++)
        {
            char tip = mapaTip[i][j];
            int nivo = mapaNivo[i][j];

            int cena = 0;

            if(tip == 'S')cena = 100;
            if(tip == 'P')cena = 50;
            if(tip == 'B')cena = 200;
            if(tip == 'F')cena = 150;
            if(tip == 'K')cena = 120;
            if(tip == 'Z')cena = 180;
            if(tip == 'C')cena = 110;
            if(tip == 'M')cena = 220;

            odrzavanje += (cena * nivo) / 10;
        }
    }
}

void izracunajPrihod()
{
    prihod = 0;

    for(int i = 0; i < VISINA; i++)
    {
        for(int j = 0; j < SIRINA; j++)
        {

            char tip = mapaTip[i][j];
            int nivo = mapaNivo[i][j];

            float mult = 1.0 +(nivo -1)* 0.5;

            if(tip == 'F')
                prihod +=(int)(100 * mult);
            if(tip =='K')
                prihod +=(int)(80 * mult);
        }
    }
}

//KRAJ POTEZA

void zavrsiPotez()
{
    troskovi = 0;

    azuriranjeKomsiluka();

    izracunajPrihod();

    izracunajOdrzavanje();

    budzet += prihod;
    budzet -= troskovi;
    budzet -= odrzavanje;

    if(budzet < 0)
    {
        budzet = 0;
        zabranjenaGradnja = 1;
    }
    else
    {
        zabranjenaGradnja = 0;
    }
    potezBroj++;
}

//PROVERA KRAJA IGRE
void proveriKrajIgre()
{
    if (budzet < 0)
        negativanBudzetRundi ++;

    else
        negativanBudzetRundi = 0;

    if (negativanBudzetRundi >= 3)
    {
        printf("=====================================\n");
        printf("KRAJ IGRE: BANKROT!\n");
        printf("Vas budzet je bio u minusu 3 poteza zaredom.\n");
        printf("=====================================\n");
        igraAktivna =  0;
    }

    else if (sreca < 20)
    {
        printf("=====================================\n");
        printf("KRAJ IGRE: POBUNA GRADJANA!\n");
        printf("Sreca stanovnistva je pala ispod 20%.\n");
        printf("=====================================\n");
        igraAktivna =  0;
    }


}

 // ================= UCITAVANJE I SACUVANJE IGRE =================
void sacuvajIgru(const char *imeFajla)
{
    FILE *f = fopen(imeFajla, "w");

    if(f == NULL)
    {
        printf("Greska pri cuvanju igre!\n");
        return;
    }

    fprintf(f, "%d %d %d %d\n", budzet, populacija, sreca, potezBroj);

    for(int i = 0; i < VISINA; i++)
    {
        for(int j = 0; j < SIRINA; j++)
        {
            fprintf(f, "%c %d\n", mapaTip[i][j], mapaNivo[i][j]);
        }
    }

    fclose(f);

    printf("Igra je sacuvana!\n");

}
void ucitajIgru(const char *imeFajla)
    {
        FILE *f = fopen(imeFajla, "r");

        if(f == NULL)
        {
            printf("Ne postoji sacuvana igra!\n");
            return;
        }

        fscanf(f, "%d %d %d %d", &budzet, &populacija, &sreca, &potezBroj);

        fgetc(f);

        for(int i = 0; i < VISINA; i++)
        {
            for(int j = 0; j < SIRINA; j++)
            {
               char linija[20];

               if(fgets(linija, sizeof(linija), f) == NULL)
               {
                   printf("Greska pri ucitavanju!\n");
                   fclose(f);
                   return;
               }

               if(linija[0] == ' ')
               {
                   mapaTip[i][j] = ' ';
                   sscanf(linija + 1, "%d", &mapaNivo[i][j]);
               }
               else
               {
                   sscanf(linija, " %c %d", &mapaTip[i][j], &mapaNivo[i][j]);
               }

            }
        }

        fclose(f);

        prihod = 0;
        troskovi = 0;
        odrzavanje = 0;
        negativanBudzetRundi = 0;
        zabranjenaGradnja = 0;

        izracunajPrihod();
        izracunajOdrzavanje();
        printf("Igra je ucitana!\n");
    }

// ================= POKRETANJE=================
 void pokreniIgru(void)
 {
            int opcija;

                while (igraAktivna) {
                    printf("\n==================================================\n");
                    printf(" POTEZ %d | Budzet: %d EUR\n", potezBroj, budzet);
                    printf("==================================================\n");

                    // Automatsko crtanje trenutnog stanja mape na ekranu
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

                                if (mapaTip[r][k] == '~')
                                    {
                                        printf("Greska: Na ovom polju tece reka! Gradnja je nemoguca.\n");
                                    continue;
                                    }

                                if (mapaTip[r][k] != ' ') {
                                    printf("Greska: Polje %c%d je vec zauzeto (%c%d)! Pokusajte ponovo.\n",
                                           red, kolona, mapaTip[r][k], mapaNivo[r][k]);
                                    continue;
                                }

                                if (budzet < cena || zabranjenaGradnja) {
                                    printf("Greska:Grad je u finansijskoj krizi.Nemate dovoljno novca! Potrebno: %d EUR, Trenutno: %d EUR\n", cena, budzet);
                                    continue;
                                } else {
                                    budzet -= cena;
                                    mapaTip[r][k] = tip;
                                    mapaNivo[r][k] = 1;
                                    azurirajStatistiku(tip, 1, 1);
                                    printf("Uspesno postavljena zgrada %c na %c%d! Preostali budzet: %d EUR\n", tip, red, kolona, budzet);
                                    uspesnaGradnja = 1;

                                    zavrsiPotez();
                                    proveriKrajIgre();
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
                                    printf("Greska: van granica mape!\n");
                                    break;
                                }

                                int r = red - 'A';
                                int k = kolona -1;

                                if(mapaTip[r][k] == ' ')
                                {
                                    printf("Nema zgrade na tom polju!\n");
                                    break;
                                }

                                if (mapaTip[r][k] == '~')
                                {
                                    printf("Greska: ne mozete rusiti prirodne objekte!\n");
                                    break;
                                }

                                int cena = 0;

                                if(mapaTip[r][k] == 'S')cena = 100;
                                else if(mapaTip[r][k] == 'P')cena = 50;
                                else if(mapaTip[r][k] == 'B')cena = 200;
                                else if(mapaTip[r][k] == 'F')cena = 150;
                                else if(mapaTip[r][k] == 'K')cena = 120;
                                else if(mapaTip[r][k] == 'Z')cena = 180;
                                else if(mapaTip[r][k] == 'C')cena = 110;
                                else if(mapaTip[r][k] == 'M')cena = 220;


                                budzet += (cena*mapaNivo[r][k]) / 2;

                                char tip = mapaTip[r][k];
                                int nivo = mapaNivo[r][k];

                                azurirajStatistiku(tip, nivo, -1);

                                mapaTip[r][k] = ' ';
                                mapaNivo[r][k] = 0;

                                printf("Zgrada uklonjena!\n");

                                zavrsiPotez();
                                proveriKrajIgre();

                                break;
                            }
                            break;

                        case 3:
                            nadogradiZgradu();

                            zavrsiPotez();
                            proveriKrajIgre();
                            break;

                        case 4:
                            printf("\n--- STATISTIKA GRADA ---\n");
                            printf("Potez broj: %d\n", potezBroj);
                            printf("Budzet: %d EUR\n", budzet);
                            printf("Populacija: %d\n", populacija);
                            printf("Sreca: %d%%\n", sreca);
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
        printf("3 Ucitaj igru\n");
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

                prihod = 0;
                troskovi = 0;
                odrzavanje = 0;
                negativanBudzetRundi = 0;
                zabranjenaGradnja = 0;
                igraAktivna = 1;

                // Cistimo mapu pre nego sto igra pocne
                inicijalizujMapu();

                int krajPoteza = 0;

                pokreniIgru();

                break;

            case 2:
                prikazi_uputstvo();
                break;
            case 3:
                ucitajIgru("save.txt");

                igraAktivna = 1;
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
