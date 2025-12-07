#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_STUDENTS 5
#define NUM_CHAIRS 3

sem_t faculty_sleep;                  // Faculty semaphore
sem_t student_sem[NUM_STUDENTS];      // Each student has its own semaphore
pthread_mutex_t chair_mutex;          // Mutex to protect chair count

int waiting_chairs = 0;
int chair_queue[NUM_CHAIRS];          // Queue to store waiting students
int front = 0, rear = 0;

void rand_sleep() {
    usleep((rand() % 500 + 200) * 1000);   // 200ms – 700ms
}

// Add student to queue
void enqueue(int id) {
    chair_queue[rear] = id;
    rear = (rear + 1) % NUM_CHAIRS;
}

// Remove student from queue
int dequeue() {
    int id = chair_queue[front];
    front = (front + 1) % NUM_CHAIRS;
    return id;
}

/****************
 * STUDENT CODE *
 ****************/
void* stu_programming(void* arg) {
    int id = *(int*)arg;

    while (1) {
        printf("Student %d is programming...\n", id);
        rand_sleep();

        pthread_mutex_lock(&chair_mutex);

        if (waiting_chairs < NUM_CHAIRS) {
            waiting_chairs++;
            enqueue(id);
            printf("Student %d is waiting... (%d/%d chairs occupied)\n",
                    id, waiting_chairs, NUM_CHAIRS);
            
            // Wake faculty if asleep
            sem_post(&faculty_sleep);

            pthread_mutex_unlock(&chair_mutex);

            // Wait for faculty to help
            sem_wait(&student_sem[id]);

            printf("Student %d is getting help from Faculty...\n", id);
            rand_sleep();
        } 
        else {
            printf("Student %d found no empty chair. Returning to programming.\n", id);
            pthread_mutex_unlock(&chair_mutex);
        }
    }
}

/*****************
 * FACULTY CODE  *
 *****************/
void* faculty_teaching(void* arg) {

    while (1) {
        sem_wait(&faculty_sleep);   // Sleep until notified

        pthread_mutex_lock(&chair_mutex);

        if (waiting_chairs == 0) {
            pthread_mutex_unlock(&chair_mutex);
            continue;
        }

        int student_id = dequeue();
        waiting_chairs--;

        printf("\nFaculty is now helping Student %d. Remaining chairs: %d\n\n",
                student_id, waiting_chairs);

        pthread_mutex_unlock(&chair_mutex);

        // Signal student to start receiving help
        sem_post(&student_sem[student_id]);

        rand_sleep();   // Teaching time
    }
}

/************
 *   MAIN   *
 ************/
int main() {
    srand(time(NULL));

    pthread_t faculty_thread;
    pthread_t students[NUM_STUDENTS];

    pthread_mutex_init(&chair_mutex, NULL);
    sem_init(&faculty_sleep, 0, 0);

    for (int i = 0; i < NUM_STUDENTS; i++) {
        sem_init(&student_sem[i], 0, 0);
    }

    pthread_create(&faculty_thread, NULL, faculty_teaching, NULL);

    int ids[NUM_STUDENTS];
    for (int i = 0; i < NUM_STUDENTS; i++) {
        ids[i] = i;
        pthread_create(&students[i], NULL, stu_programming, &ids[i]);
    }

    pthread_join(faculty_thread, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++)
        pthread_join(students[i], NULL);

    return 0;
}
