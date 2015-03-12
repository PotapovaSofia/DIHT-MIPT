#include <stdlib.h>
#include <stdio.h>

// количество правильных скобочных последовательностей
// длины 2 * n == n-e число Каталана

int brackets(int n) {
    int* Arr = (int*)malloc((n + 1) * sizeof(int));
    int i, j;
    Arr[0] = 1;

    for (i = 1; i <= n; i++) {
        Arr[i] = 0;
        for (j = 0; j < i; j++) {
            Arr[i] += Arr[j] * Arr[i - 1 - j];
        }
    }
    int x = Arr[n];
    free(Arr);
    return x;
}

int main() {
    int n;
    scanf("%d", &n);

    printf("%d\n", brackets(n));
    return 0;
}


