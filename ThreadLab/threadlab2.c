#include <stdio.h>
#include <pthread.h>

#define tam_arreglo 12
#define num_hilos 3

int array[tam_arreglo] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
int search_num = 10;
int found_flag = 0;

void *search_array(void *arg) {
    int i;
    int *sub_array = (int *) arg;
    for (i = 0; i < tam_arreglo/num_hilos; i++) {
        if (sub_array[i] == search_num) {
            found_flag = 1;
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

void create_threads() {
    int i;
    pthread_t threads[num_hilos];
    for (i = 0; i < num_hilos; i++) {
        pthread_create(&threads[i], NULL, search_array, &i);
    }
    for (i = 0; i < num_hilos; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    int i;
    pthread_t threads[num_hilos];
    int sub_arrays[num_hilos][tam_arreglo/num_hilos];

    for (i = 0; i < num_hilos; i++) {
        int j, k;
        k = i * (tam_arreglo/num_hilos);
        for (j = 0; j < tam_arreglo/num_hilos; j++) {
            sub_arrays[i][j] = array[k];
            k++;
        }
    }

    for (i = 0; i < num_hilos; i++) {
        pthread_create(&threads[i], NULL, search_array, (void *) sub_arrays[i]);
    }

    for (i = 0; i < num_hilos; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("El número a encontrar es: %d\n", search_num);

    if (found_flag) {
        printf("El número %d se encuentra en el arreglo.\n", search_num);
    } else {
        printf("El número %d no se encuentra en el arreglo\n", search_num);
    }

    return 0;
}
