#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

// void* read_file(void *arg) {
//     int i;
//     int* loop_count = (int*)arg;

//     for (i = 0; i <= *loop_count; i++) {
//         printf("%d\n", i);
//     }

//     pthread_exit("Loop finalizado!");
// }

void selectionSort(int* vetor, int size) {  
  int i, hasBeenOrganized, min, swap;

  while(1) {
    hasBeenOrganized = 1;
    for (i = 1; i < size; i++) { 
        if (vetor[i - 1] > vetor[i]) {
            swap = vetor[i];
            vetor[i] = vetor[i - 1];
            vetor[i - 1] = swap;

            hasBeenOrganized = 0;
        }
    }

    if (hasBeenOrganized == 1) {
        break;
    }
  }
}

void* sort(void *arg) {
    int* numbers = (int*)arg;
    // Tentando pegar o vetor numbers para receber a quantidade de entidades do vetor
    // selectionSort(numbers, amount);
}

void populateMatrix(int amount_files, int matrix_result[], int numbers[]) {
    int j;
    for (j = 0; j < amount_files; j++) {
        matrix_result[j] = numbers[j];
    }
}

void thereAnError(int status, char* message) {
    if (status != 0) {
        printf("%s\n", message);
        exit(status);
    }
}

int main(int argc, char *argv[]) {
    pthread_t* thread_id;
    void* thread_response;
    int thread_status;
    char* response;
    char line[1000];
    int i;
    int j;
    int max_n_in_line = 0;
    int n_threads = 16;
    char* in_path[1000] = {"arq1.dat", "arq2.dat", "arq3.dat"};
    char* out_path = "saida.dat";
    int number_files = 0;
    int numbers[1000][1000];
    int matrix_result[1000][1000];
    int amount_files[1000];
    int current_thread;

    int n_loop = 100000;

    FILE* file;

    if (argc >= 1) n_threads = atoi(argv[1]);

    for (i = 2; i < argc; ++i) {
        if (strcmp(argv[i],"-o") == 0) {
            out_path = argv[i+1];
            break;
        } else {
            in_path[i - 2] = argv[i]; // Tem que arrumar isso aqui, esta fixado em 1000 arquivos
            number_files++;
        }
    }

    if (number_files > 0) {
        for (i = 0; i < number_files; i++) {
            file = fopen(in_path[i], "rt");

            if (file == NULL) {
                printf("Problemas na leitura do arquivo\n");
                exit(1);
            }

            j = 0;
            while (!feof(file)) {
                if (fgets(line, 1000, file)) {
                    numbers[i][j] = atoi(line);
                }

                j++;
            }

            if (max_n_in_line < j) {
                max_n_in_line = j;
            }

            amount_files[i] = j;

            fclose(file);
        }
    }

    thread_id = malloc(n_threads * sizeof(pthread_t));

    for (i = 0; i < number_files; i++) {
        for(j = 0; j < max_n_in_line; j++) {
            matrix_result[i][j] = 0;
        }
    }

    // Ordena numbers
    current_thread = 0;
    for (i = 0; i < number_files; i++) {
        thread_status = pthread_create((thread_id + i), NULL, sort, (void*)(&numbers[i]));
    }

    // Adiciona os num na matrix
    current_thread = 0;
    if (number_files > 0) {
        for (i = 0; i < number_files; i++) {
            populateMatrix(amount_files[i], matrix_result[i], numbers[i]);
        }
    }

    free(thread_id);

    char buffer [50];
    file = fopen(out_path, "w+");
    for (i = 0; i < number_files; i++) {
        for(j = 0; j < max_n_in_line; j++) {
            sprintf (buffer, "%d ", matrix_result[i][j]);
            fputs(buffer, file);
        }
        fputs("\n", file);
    }
    fclose(file);


    // exit(1);

    

    // // Aloca os threads
    // for(i = 0; i < n_threads; i++) {
    //     thread_status = pthread_create((thread_id + i), NULL, read_file, (void*)(&n_loop));

    //     thereAnError(thread_status, "Error create!");
    // }

    // // Executa e espera a resposta dos threads
    // for(i = 0; i < n_threads; i++) {
    //     thread_status = pthread_join(*(thread_id + i), &thread_response);

    //     thereAnError(thread_status, "Error join!");

    //     response = (char *)thread_response;

    //     printf("%d) %s\n", i+1, response);

    // }

    return 0;
}