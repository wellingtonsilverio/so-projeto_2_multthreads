#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void* read_file(void *arg) {
    char* file_path = (char*)arg;

    printf("THREAD: %s\n", file_path);

    pthread_exit(arg);
}

int main() {
    pthread_t thread_id;
    void* thread_response;
    int thread_status;

    thread_status = pthread_create(&thread_id, NULL, read_file, (void*)("arquivo.dat"));

    if (thread_status != 0) {
        printf("Error create!\n");
        exit(thread_status);
    }

    thread_status = pthread_join(thread_id, &thread_response);

    if (thread_status != 0) {
        printf("Error join!\n");
        exit(thread_status);
    }

    char* response = (char *)thread_response;

    printf("%s\n", response);

    return 0;
}