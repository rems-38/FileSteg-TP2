#include <stdio.h>

void file_odd(int n) {
    int i = 0, c = 0;
    FILE *f = fopen("nombres_impairs.txt", "w");
    while (c < n) {
        if (i % 2 != 0) {
            // char *ci = i + '0';
            fprintf(f, "%d\n", i);
            c++;
        }
        i++;
    }
}

int main() {
    file_odd(100);
    return 0;
}