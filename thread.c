/*

david rademacher
1001469394

this is a modified copy of CSE3320/Substring-Assignment/substring.c,
implementing threads to search for substrings in a large
body of text.

*/

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define MAX 5000000

int n1,n2, total_finds = 0;
char *s1,*s2;

// struct to pass data to threaded function
typedef struct {
    int t_num;
    int num_threads;
} thread_info;

pthread_mutex_t mutex;

FILE *fp;

int readf(char* filename) {
    if((fp=fopen(filename, "r"))==NULL) {
        printf("ERROR: can’t open %s!\n", filename);
        return 0;
    }

    s1=(char *)malloc(sizeof(char)*MAX);
    if (s1==NULL) {
        printf ("ERROR: Out of memory!\n") ;
        return -1;
    }

    s2=(char *)malloc(sizeof(char)*MAX);
    if (s2==NULL) {
        printf ("ERROR: Out of memory\n") ;
        return -1;
    }
    
    // fgets() reads until a newline, so the text body is saved into
    // s1 and the target substring is saved into s2
    s1=fgets(s1, MAX, fp);
    s2=fgets(s2, MAX, fp);
    n1=strlen(s1);   // length of text
    n2=strlen(s2)-1; // length of target
    
    if( s1==NULL || s2==NULL || n1 < n2 ) return -1; // exit if error occurs
}

void *num_substring(void *_info) {

    int j,k, count, i_found = 0;
    thread_info *info = _info;

    // using info->t_num was giving a junk value, but this fixes it somehow
    int t_num = info->t_num;

    int chunk_size = n1 / info->num_threads;
    int chunk_start = t_num * chunk_size;
    int chunk_stop  = chunk_start + chunk_size;

    // all threads except the last should search beyond the chunk boundary
    // to find matches that cross over
    if (t_num != info->num_threads - 1) chunk_stop += n2;
   
    for (int i = chunk_start; i <= chunk_stop; i++) {
        // number of matching characters "so far"
        count = 0;
        
        // search for the next string of size n2
        for(j = i, k = 0; k < n2; j++, k++) {
            if (*(s1+j)!=*(s2+k)) break;
            else count++;

            // number of matching characters equals length of n2...
            if (count==n2) i_found++; // ... found a substring in this step
         }
    }

    // use mutex to ensure that the global total_finds is incremented correctly
    pthread_mutex_lock(&mutex);
    total_finds += i_found;
    pthread_mutex_unlock(&mutex);

    // info is recreated in main() for other threads
    free(info);

    printf(" -thread %d found %d\n", t_num+1, i_found);
}
    
int main(int argc, char *argv[]) {
    if(argc < 3) {
        printf("usage: ./thread [filename] [number of threads]\n\n");
        return -1;
    }

    readf(argv[1]);
    
    struct timeval start, end;
    float mtime; 
    int status, secs, usecs;    

    int nt = atoi(argv[2]);

    printf("file: %s (%d characters)\nsearching for: %s", argv[1], n1, s2);
    printf("number of threads: %d\n", nt);

    gettimeofday(&start, NULL);

    pthread_t threads[nt];
    pthread_mutex_init(&mutex, NULL);

    // loop to create threads
    for (int t_num = 0; t_num < nt; t_num++) {

        thread_info *info = malloc(sizeof(thread_info));
        info->num_threads = nt;
        info->t_num = t_num;

        // create thread, passing the thread number to tell it where to start searching
        status = pthread_create(&threads[t_num], NULL, num_substring, (void *) info);

        if (status) {
            printf("error creating thread %d: %d\n", t_num, status);
            return -1;
        }
    }

    // loop to join threads
    for (int t_num = 0; t_num < nt; t_num++) {
        if (pthread_join(threads[t_num], NULL)) {
            printf("error joining thread %d\n", t_num);
            return -1;
        }
    }

    gettimeofday(&end, NULL);

    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;

    printf("total matches: %d\n" , total_finds);
    printf("elapsed time: %f ms\n", mtime);

    if (s1) free(s1);
    if (s2) free(s2);

    pthread_exit(NULL);
    return 0 ; 
}
