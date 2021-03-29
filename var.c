#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define FILENAME "message.txt"

typedef struct letter {
    char c;
    struct letter *next;
} letter;

pthread_cond_t cv;
pthread_mutex_t mutex;

// pthread_mutex_lock(&mutex);
// while (1) pthread_cond_wait(&cv, &mutex);
// pthread_mutex_unlock(&mutex);

FILE *fp;
char buffer[BUFFER_SIZE];

void read(letter *head) {

    if ((fp = fopen(FILENAME, "r")) == NULL) {
        printf("error opening file.\n");
        return -1;
    }


    pthread_mutex_lock(&mutex);

    fgets(buffer, BUFFER_SIZE, fp);

    pthread_mutex_unlock(&mutex);

}

int main() {

    read(NULL);

    printf("%s\n", buffer);

}
