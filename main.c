#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void* read_file(void *arg) {
    int i;
    int* loop_count = (int*)arg;

    for (i = 0; i <= *loop_count; i++) {
        printf("%d\n", i);
    }

    pthread_exit("Loop finalizado!");
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
    int i;
    int n_threads = 16;
    char* in_path[1000] = {"arq1.dat", "arq2.dat", "arq3.dat"};
    char* out_path = "saida.dat";

    int n_loop = 100000;

    if (argc >= 1) n_threads = atoi(argv[1]);

    for (i = 2; i < argc; ++i) {
        if (argv[i] == "-o") {
            out_path = argv[i+1];
            break;
        }

        in_path[i - 2] = argv[i]; // Tem que arrumar isso aqui
    }

    thread_id = malloc(n_threads * sizeof(pthread_t));

    // Aloca os threads
    for(i = 0; i < n_threads; i++) {
        thread_status = pthread_create((thread_id + i), NULL, read_file, (void*)(&n_loop));

        thereAnError(thread_status, "Error create!");
    }

    // Executa e espera a resposta dos threads
    for(i = 0; i < n_threads; i++) {
        thread_status = pthread_join(*(thread_id + i), &thread_response);

        thereAnError(thread_status, "Error join!");

        response = (char *)thread_response;

        printf("%d) %s\n", i+1, response);

    }

    free(thread_id);

    return 0;
}