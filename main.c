#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void* read_file(void *arg) {
    int i;
    int* loop_count = (int*)arg;

    for (i = 0; i < *loop_count; i++) {
        printf("%d\n", i);
    }

    pthread_exit("Loop finalizado!");
}

int main() {
    pthread_t* thread_id;
    void* thread_response;
    int thread_status;
    char* response;
    int i;
    int n_threads = 16;
    int n_loop = 1000000;

    thread_id = malloc(n_threads * sizeof(pthread_t));

    for(i = 0; i < n_threads; i++) {
        thread_status = pthread_create((thread_id + i), NULL, read_file, (void*)(&n_loop));

        if (thread_status != 0) {
            printf("Error create!\n");
            exit(thread_status);
        }
    }

    for(i = 0; i < n_threads; i++) {
        thread_status = pthread_join(*(thread_id + i), &thread_response);

        if (thread_status != 0) {
            printf("Error join!\n");
            exit(thread_status);
        }

        response = (char *)thread_response;

        printf("%d) %s\n", i+1, response);

    }

    free(thread_id);

    // thread_status = pthread_create(&thread_id_2, NULL, read_file, (void*)(&n_loop));

    // if (thread_status != 0) {
    //     printf("Error create!\n");
    //     exit(thread_status);
    // }

    // thread_status = pthread_create(&thread_id_3, NULL, read_file, (void*)(&n_loop));

    // if (thread_status != 0) {
    //     printf("Error create!\n");
    //     exit(thread_status);
    // }

    // thread_status = pthread_join(thread_id_1, &thread_response);

    // if (thread_status != 0) {
    //     printf("Error join!\n");
    //     exit(thread_status);
    // }

    // response = (char *)thread_response;

    // printf("1) %s\n", response);

    // thread_status = pthread_join(thread_id_2, &thread_response);

    // if (thread_status != 0) {
    //     printf("Error join!\n");
    //     exit(thread_status);
    // }

    // response = (char *)thread_response;

    // printf("2) %s\n", response);

    // thread_status = pthread_join(thread_id_3, &thread_response);

    // if (thread_status != 0) {
    //     printf("Error join!\n");
    //     exit(thread_status);
    // }

    // response = (char *)thread_response;

    // printf("3) %s\n", response);

    return 0;
}