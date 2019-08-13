#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

struct Arg_sort {
    int* array;
    int size;
}Arg_sort;

struct Arg_populate_matrix {
    int amount_files;
    int* matrix_result;
    int* numbers;
}Arg_populate_matrix;

void selectionSort(int* array, int size) {  
  int i, hasBeenOrganized = 0, min, swap, k = 0;

  while(hasBeenOrganized == 0) {
    hasBeenOrganized = 1;
    for (i = 1; i < size; i++) { 
        if (array[i - 1] > array[i]) {
            swap = array[i];
            array[i] = array[i - 1];
            array[i - 1] = swap;

            hasBeenOrganized = 0;
        }
    }
  }
}

void* sort(void* arg) {
    struct Arg_sort* args = (struct Arg_sort*)arg;

    selectionSort(args->array, args->size);

    pthread_exit("Sort finalizado!");
}

void* populateMatrix(void* arg) {
    struct Arg_populate_matrix* args = (struct Arg_populate_matrix*)arg;
    
    int j;
    for (j = 0; j < args->amount_files; j++) {
        args->matrix_result[j] = args->numbers[j];
    }

    pthread_exit("Construção da matrix finalizado!");
}

void thereAnError(int status, char* message) {
    if (status != 0) {
        printf("%s\n", message);
        exit(status);
    }
}

void join_threads(int* current_thread, pthread_t* thread_id) {
  for (int j = 0; j < *current_thread; j++) {
      void* thread_response;

      int thread_status = pthread_join(*(thread_id + j), &thread_response);

      thereAnError(thread_status, "Error join!");
  }
  *current_thread = 0;
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
    int current_thread = 0;
    float start_time, end_time;

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

    start_time = (float)clock()/CLOCKS_PER_SEC;

    for (i = 0; i < number_files; i++) {
        for(j = 0; j < max_n_in_line; j++) {
            matrix_result[i][j] = 0;
        }
    }

    // Ordena numbers
    for (i = 0; i < number_files; i++) {
        struct Arg_sort* args = malloc(sizeof(Arg_sort));
        args->array = numbers[i];
        args->size = sizeof(numbers[i]) / sizeof(int);

        thread_status = pthread_create((thread_id + current_thread++), NULL, sort, (void*)(args));
        thereAnError(thread_status, "Error create!");

        if (current_thread >= n_threads) {
            join_threads(&current_thread, thread_id);
        }
    }
    if (current_thread > 0) {
        join_threads(&current_thread, thread_id);
    }

    // Adiciona os num na matrix
    if (number_files > 0) {
        for (i = 0; i < number_files; i++) {
            struct Arg_populate_matrix* args = malloc(sizeof(Arg_populate_matrix));
            args->amount_files = amount_files[i];
            args->matrix_result = matrix_result[i];
            args->numbers = numbers[i];

            thread_status = pthread_create((thread_id + current_thread++), NULL, populateMatrix, (void*)(args));
            thereAnError(thread_status, "Error create!");

            if (current_thread >= n_threads) {
                join_threads(&current_thread, thread_id);
            }
        }

        if (current_thread > 0) {
            join_threads(&current_thread, thread_id);
        }
    }

    end_time = (float)clock()/CLOCKS_PER_SEC;
    
    printf("Time: %f\n", end_time - start_time);

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

    return 0;
}