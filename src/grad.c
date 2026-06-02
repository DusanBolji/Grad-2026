#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Globalne promenljive
int budzet;
int populacija;
int sreca;
int potezBroj;

// Pomocna funkcija za ciscenje unosa (mora da bude tu zbog scanf-a u meniju)
void ocistiStdin(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(void) {
    int izbor;

    printf("=====================================\n");
    printf("           CITY BUILDER\n");
    printf("=====================================\n");

    do {
        printf("\n1 Nova igra\n");
        printf("2 Uputstvo\n");
        printf("0 Exit\n");
        printf("Izbor: ");

        if(scanf("%d", &izbor) != 1) {
            ocistiStdin();
            continue;
        }

        switch(izbor) {
            case 1:
                printf("     1    2    3    4    5    6    7    8    9   10\n");

    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("A | S1 |    | P2 |    | F1 |    |    | B1 |    |    |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("B |    | K1 |    | S2 |    |    | P1 |    | F2 |    |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("C | P1 |    |    |    | Z1 |    |    |    |    | S1 |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("D |    | F1 |    | B2 |    | P2 |    |    | K1 |    |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("E | S1 |    |    |    |    |    | F1 |    |    | P1 |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("F |    | Z1 |    | P1 |    | S2 |    |    | B1 |    |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("G | F2 |    |    |    | P1 |    |    | S1 |    |    |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("H |    |    | K1 |    |    | F1 |    | P2 |    | S1 |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("I | P1 | S1 |    |    | B1 |    |    |    | F1 |    |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");
    printf("J |    | F1 |    | P1 |    |    | S2 |    |    | K1 |\n");
    printf("  +----+----+----+----+----+----+----+----+----+----+\n");

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
