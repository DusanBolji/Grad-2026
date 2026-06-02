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
                printf("\nNova igra u razvoju (Faza 1 stub)...\n");
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
