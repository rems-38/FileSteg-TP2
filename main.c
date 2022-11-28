#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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
}

int file_count_vowels(char *filename) {
    int count = 0;
    char vowels[6] = {'a', 'e', 'i', 'o', 'u', 'y'};
    int tab_len = sizeof(vowels)/sizeof(vowels[0]);
    FILE *file = fopen(filename, "r");
    char curr_char;

    while (curr_char != EOF) {
        curr_char = fgetc(file);
        for(int i = 0; i < tab_len; i++) {
            if(curr_char == vowels[i]) {
                count++;
                i = tab_len;
            }
        }
    }    

    return count;
}

void readable_txt(char *filename) {
    FILE *f = fopen(filename, "r");
    char main[100], second[100];
    strcpy(main, filename);
    strcpy(second, "_source.txt");
    FILE *fout = fopen(strcat(main, second), "w");

    char curr_char;
    while (curr_char != EOF) {
        curr_char = fgetc(f);
        if(isalpha(curr_char) != 0) {
            if(isupper(curr_char)) {
                curr_char += 'a' - 'A';
                fputc(curr_char, fout);
            } else {
                fputc(curr_char, fout);
            }
        } else {
            fputc(' ', fout);
        }
    }
}


int main() {
    file_odd(100);

    char filename[30];
    printf("Entrez le nom du fichier : ");
    scanf("%s", filename);
    int counter = file_count_vowels(filename);
    printf("Le nombre de voyelle dans le fichier est : %d\n", counter);

    readable_txt("utils/transporteur.txt");

    return 0;
}