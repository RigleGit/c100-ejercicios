#include <stdio.h>

int main(void) {
    int n;

    printf("Introduce el número de filas: ");
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}
