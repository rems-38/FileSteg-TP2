#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utils/bitmap.h"

void file_odd(int n) {
    int i = 0, c = 0;
    FILE *f = fopen("nombres_impairs.txt", "w");
    while (c < n) {
        if (i % 2 != 0) {
            fprintf(f, "%d\n", i);
            c++;
        }
        i++;
    }
    printf("Odd numbers generated successfully in file 'nombres_impairs.txt'\n\n");
    fclose(f);
}

int file_count_vowels(char *filename) {
    int count = 0;
    char vowels[12] = {'a', 'e', 'i', 'o', 'u', 'y', 'A', 'E', 'I', 'O', 'U', 'Y'};
    int tab_len = sizeof(vowels)/sizeof(vowels[0]);
    FILE *file = fopen(filename, "r");
    int curr_char = 0;

    while ((curr_char = fgetc(file)) != EOF) {
        for(int i = 0; i < tab_len; i++) {
            if(curr_char == vowels[i]) {
                count++;
                i = tab_len;
            }
        }
    }

    fclose(file);
    return count;
}

int puissance(int a, int b) {
    int res = 1;
    for(int i = 0; i < b; i++) {
        res *= a;
    }
    return res;
}

int bit_to_int(char bit[8]) {
    int tot = 0;
    for(int i = 0; i < 8; i++) {
        if(bit[i] == 1) {
            tot += puissance(2, 7-i);
        }
    }
    return tot;
}

void readable_txt(char *filename) {
    FILE *f = fopen(filename, "r");

    char *main = malloc(strlen(filename) + strlen("_source.txt") + 1);
    char *second = malloc(strlen("_source.txt") + 1);
    strcpy(main, filename);
    strcpy(second, "_source.txt");
    FILE *fout = fopen(strcat(main, second), "w");

    char new_char[8];
    int octet = 0;
    int curr_char = 0;
    while (curr_char != EOF) {
        if(octet == 8) {
            fputc(bit_to_int(new_char), fout);
            octet = 0;
        } else {
            curr_char = fgetc(f);
            if(isalpha(curr_char) != 0) {
                if(isupper(curr_char)) {
                    new_char[octet] = 1;
                } else {
                    new_char[octet] = 0;
                }
                octet++;
            }
        }
    }
    printf("File '%s' converted successfully in file '%s'\n\n", filename, main);
    fclose(f);
    fclose(fout);
}

char bit_faible(char octet) {
    char mask = 1;
    return mask & octet;
}

void decode_bmp(char *filename) {
    FILE *f = fopen(filename, "r");
    
    char *main = malloc(strlen(filename) + strlen("_source.jpg") + 1);
    char *second = malloc(strlen("_source.jpg") + 1);
    strcpy(main, filename);
    strcpy(second, "_source.jpg");
    FILE *fout = fopen(strcat(main, second), "w");

    fichierEntete *header = malloc(sizeof(fichierEntete));
    imageEntete *imageHeader = malloc(sizeof(imageEntete));

    fread(header, 1, sizeof(fichierEntete), f);
    fread(imageHeader, 1, sizeof(imageEntete), f);
    fseek(f, header->offset, SEEK_SET);
    
    char pixel, bit[8];
    int octet = 0;
    for(int i = 0; i < imageHeader->tailleImage; i++) {
        pixel = fgetc(f);
        bit[octet] = bit_faible(pixel);
        octet++;
        if(octet == 8) {
            fputc(bit_to_int(bit), fout);
            octet = 0;
        }
    }

    fclose(f);
    fclose(fout);
}


int main() {
    // file_odd(100);

    // char *filename;
    // printf("Entrez le nom du fichier : ");
    // scanf("%s", filename);
    // int counter = file_count_vowels(filename);
    // printf("Le nombre de voyelle dans le fichier est : %d\n\n", counter);

    // readable_txt("utils/transporteur.txt");

    decode_bmp("utils/transporteur.bmp");

    return 0;
}