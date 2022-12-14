#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
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

void int_to_bin(unsigned char source, int bin[8]) {
    // Marche niquel
    // INFO: bit[0] = bit de poids faible
    char mask = 1;
    for(int i = 0; i < 8; i++) {
        if((mask << i) & source) {
            bin[i] = 1;
        } else {
            bin[i] = 0;
        }
    }
    // printf("%d -> ", source);
    // for(int i = 0; i < 8; i++) {
    //     printf("%d", bin[7-i]);
    // }
    // printf("\n");
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

char get_bit_faible(char octet) {
    char mask = 1;
    return mask & octet;
}

char set_bit_faible(char octet, int bit_faible) {
    char mask;
    if(bit_faible == 0) {
        mask = 254;
        return mask & octet;
    }
    else {
        mask = 1;
        return mask | octet;
    }
}

char *decode_bmp(char *filename) {
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
        bit[octet] = get_bit_faible(pixel);
        octet++;
        if(octet == 8) {
            fputc(bit_to_int(bit), fout);
            octet = 0;
        }
    }

    fclose(f);
    fclose(fout);

    printf("File '%s' successfully decoded in file '%s'\n\n", filename, main);

    return main;
}

char *encode_file(char *file_source, char *file_secret) {
    FILE *fsource = fopen(file_source, "r");
    FILE *fsecret = fopen(file_secret, "r"); // file extrait de transporteur.bmp avec decode_bmp() -> transporteur.bmp_source.jpg

    char *remake = malloc(strlen(file_secret) + strlen("_remake.bmp") + 1);
    char *second = malloc(strlen("_remake.bmp") + 1);
    strcpy(remake, file_secret);
    strcpy(second, "_remake.bmp");
    FILE *ftransp = fopen(strcat(remake, second), "w"); // nouveau fichier
    
    char temp = fgetc(fsecret);

    fichierEntete *header = malloc(sizeof(fichierEntete));
    imageEntete *imageHeader = malloc(sizeof(imageEntete));

    fread(header, 1, sizeof(fichierEntete), fsource); // c'est l'inverse entre le 1 et le sizeof !!
    fwrite(header, 1, sizeof(fichierEntete), ftransp);

    fread(imageHeader, 1, sizeof(imageEntete), fsource);
    fwrite(imageHeader, 1, sizeof(imageEntete), ftransp);

    int new_bmp_size = 0;
    char octet_source;
    int bin_secret[8];

    unsigned char char_secret = 0;
    char new_octet_in_src = 0;
    
    //Get file length
    fseek(fsecret, 0, SEEK_END);
    long fileLen = ftell(fsecret);
    fseek(fsecret, 0, SEEK_SET);

    for(int counter = 0; counter < fileLen; counter += 1) {
        char_secret = fgetc(fsecret);
        int_to_bin(char_secret, bin_secret);
        for(int i = 0; i < 8; i++) {
            octet_source = fgetc(fsource);
            new_octet_in_src = set_bit_faible(octet_source, bin_secret[7-i]);
            fputc(new_octet_in_src, ftransp);
        }
        for(int i = 0; i < 8; i++) {
            bin_secret[i] = 0;
        }
    }

    fclose(fsource);
    fclose(fsecret);
    fclose(ftransp);

    printf("Secret file '%s' successfully encoded in '%s' with '%s' as original file\n\n", file_secret, remake, file_source);

    return remake;
}


int main() {
    file_odd(100);

    char *filename;
    printf("Fichier voyelles : vowels.txt\n");
    int counter = file_count_vowels("./vowels.txt");
    printf("Le nombre de voyelle dans le fichier est : %d\n\n", counter);

    readable_txt("utils/transporteur.txt");

    char *secret_file = decode_bmp("utils/transporteur.bmp");
    char *file_encode = encode_file("utils/originel.bmp", secret_file);

    decode_bmp(file_encode);

    return 0;
}