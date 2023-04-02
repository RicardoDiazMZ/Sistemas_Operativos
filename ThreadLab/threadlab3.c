#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define tam_arreglo 5

int array[tam_arreglo] = {0};

void *consumer() {
    int i = 0;
    while (1) {
        if (array[i] != 0) {
            printf("Valor encontrado: %d\n", array[i]);
            array[i] = 0;
            if (i == tam_arreglo - 1) {
                pthread_exit(NULL);
            }
        }
        i = (i + 1) % tam_arreglo;
    }
}

int main() {
    int i;
    pthread_t thread;

    printf("Ingresar %d valores: ", tam_arreglo);
    for (i = 0; i < tam_arreglo; i++) {
        scanf("%d", &array[i]);
    }

    pthread_create(&thread, NULL, consumer, NULL);
    pthread_join(thread, NULL);

    printf("Procesos finalizados.\n");

    return 0;
}
