#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>

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
    fclose(f);
}

int file_count_vowels(char *filename) {
    int count = 0;
    char vowels[12] = {'a', 'e', 'i', 'o', 'u', 'y', 'A', 'E', 'I', 'O', 'U', 'Y'};
    int tab_len = sizeof(vowels)/sizeof(vowels[0]);
    FILE *file = fopen(filename, "r");
    char curr_char;

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

void readable_txt(char *filename) {
    FILE *f = fopen(filename, "r");
    char main[sizeof(filename) + sizeof("_source.txt")], second[sizeof("_source.txt")];
    strcpy(main, filename);
    strcpy(second, "_source.txt");
    FILE *fout = fopen(strcat(main, second), "w");

    char new_char[8];
    int octet = 0;
    char curr_char;
    while (curr_char != EOF) {
        if(octet == 8) {
            // conversion du bit en decimal (ex: "01000001" -> 65 (pour la fonction d'après))
            int tot = 0;
            for(int i = 0; i < 8; i++) {
                if(new_char[i] == '1') {
                    tot += puissance(2, 7-i);
                }
            }
            // on écrit le caractère final (btowc convertit un int en char, ex: 65 -> 'A')
            fprintf(fout, "%c", btowc(tot));
            octet = 0;
        } else {
            curr_char = fgetc(f);
            if(isalpha(curr_char) != 0) {
                if(isupper(curr_char)) {
                    new_char[octet] = '1';
                } else {
                    new_char[octet] = '0';
                }
                octet++;
            }
        }
    }
    fclose(f);
    fclose(fout);
}


int main() {
    file_odd(100);

    char *filename;
    printf("Entrez le nom du fichier : ");
    scanf("%s", filename);
    int counter = file_count_vowels(filename);
    printf("Le nombre de voyelle dans le fichier est : %d\n", counter);

    readable_txt("utils/transporteur.txt");

    return 0;
}