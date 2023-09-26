#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include "spend_time.h"

#define MAX_THREADS 1000
#define MAX_RESOURCES 8

// thread variables
pthread_t threads[MAX_THREADS];
int threadData[MAX_THREADS][4]; // 0 - id, 1 - freeTime, 2 - criticTime, 3 - amount of resources
int threadsResources[MAX_THREADS][MAX_RESOURCES]; // resources used by each thread

// resources variables
int resources[MAX_RESOURCES]; // 0 - free, 1 - busy

// sincronization variables
pthread_mutex_t mutex; // mutex for critical section
pthread_cond_t cond; // condition variable for critical section

// function prototypes
void init_recursos(void);
bool canEnterCriticalSection(int tid);
void trava_recursos(int tid);
void libera_recursos(int tid);
void* threadFunction(void *tid);

int main() {
    pthread_cond_init (&cond, NULL); // initializes condition variable
    pthread_mutex_init(&mutex, NULL); // initializes mutex
    init_recursos(); // initializes resources
    int threadID, criticTime, freeTime; // thread data
    int i = 0;
    // reads input
    while (scanf("%d %d %d ", &threadID, &freeTime, &criticTime) != EOF){
        int j = 0;
        char temp;
        // reads resources used by thread
        do{
            scanf("%d%c", &threadsResources[threadID][j], &temp);
            j++;
        }while(temp != '\n');
        // saves thread data
        threadData[threadID][0] = threadID;
        threadData[threadID][1] = freeTime;
        threadData[threadID][2] = criticTime;
        threadData[threadID][3] = j;
        // creates thread
        int error = pthread_create(&(threads[threadID]), NULL, &threadFunction, (void *) threadData[threadID][0]);
        if(error != 0){
            printf("\nThread can't be created :[%s]", strerror(error));
        }
        i++;
    }
    // waits for all threads to finish
    for(int j = 0; j < 1000; j++){
        pthread_join(threads[j], NULL);
    }
    // destroys mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

// initializes resources
void init_recursos(void){
    for(int i = 0; i < MAX_RESOURCES; i++){
        resources[i] = 1;
    }
}

// returns true if all resources are free and thread can enter critical section
bool canEnterCriticalSection(int tid){
    // checks if all resources are free
    for(int i = 0; i < threadData[tid][3]; i++){ 
        if(resources[threadsResources[tid][i]] == 0){
            return false; // thread can't enter critical section
        }
    }
    return true; // thread can enter critical section
}

// locks mutex and enters critical section
void trava_recursos(int tid){
    pthread_mutex_lock(&mutex); // locks mutex
    // waits until thread can enter critical section
    while(!canEnterCriticalSection(tid)){
        pthread_cond_wait(&cond, &mutex);
    }
    // enters critical section
    for(int i = 0; i < threadData[tid][3]; i++){
        resources[threadsResources[tid][i]] = 0;
    }
    pthread_mutex_unlock(&mutex); // unlocks mutex
}

// free resources and signals other threads that they can enter critical section
void libera_recursos(int tid){
    pthread_mutex_lock(&mutex); // locks mutex
    // frees resources
    for(int i = 0; i < threadData[tid][3]; i++){
        resources[threadsResources[tid][i]] = 1;
    }
    pthread_cond_signal(&cond); // signals other threads that they can enter critical section
    pthread_mutex_unlock(&mutex); // unlocks mutex
}

// thread function
void* threadFunction(void *tid){
    int id;
    id = (int) tid;
    spend_time(id, NULL, threadData[id][1]); // spends free time
    trava_recursos(id); // locks mutex and enters critical section
    spend_time(id, "C", threadData[id][2]); // spends critic time
    libera_recursos(id); // frees resources and signals other threads that they can enter critical section
    pthread_exit(NULL); // exits thread
}