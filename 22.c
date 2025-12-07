#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

// --- Configuration ---
#define NUM_STUDENTS 5
#define MAX_CHAIRS 3 // Bounded buffer size

// --- Synchronization Primitives ---
pthread_mutex_t mutex;
sem_t Faculty_sem;               // Used by students to wake the Faculty (init 0)
sem_t student_sem[NUM_STUDENTS]; // Used by Faculty to signal help finished for a specific student (init 0)

// --- Shared State (Protected by mutex) ---
int waiting_students = 0;
// Queue to hold the IDs of students waiting in the chairs (First-Come, First-Served)
int student_queue[MAX_CHAIRS];
int queue_head = 0;
int queue_tail = 0;

/**
 * @brief Utility function to simulate random time spent programming or helping.
 * @param min_ms Minimum sleep time in milliseconds.
 * @param max_ms Maximum sleep time in milliseconds.
 */
void rand_sleep(int min_ms, int max_ms) {
    int sleep_ms = (rand() % (max_ms - min_ms + 1)) + min_ms;
    usleep(sleep_ms * 1000); // usleep takes microseconds
}

// --- Queue Operations (MUST be called while mutex is locked) ---

void enqueue_student(int student_id) {
    student_queue[queue_tail] = student_id;
    queue_tail = (queue_tail + 1) % MAX_CHAIRS;
}

int dequeue_student() {
    int student_id = student_queue[queue_head];
    queue_head = (queue_head + 1) % MAX_CHAIRS;
    return student_id;
}

// --- Faculty Thread Function ---

void *Faculty_teaching(void *arg) {
    printf("Faculty thread started.\n");
    while (1) {
        // 1. Faculty is sleeping until notification is arrived (wait on own semaphore)
        printf("\nFaculty: All chairs clear, going to sleep...\n");
        sem_wait(&Faculty_sem); 

        // Faculty has been woken up by a student.
        printf("Faculty: Woke up! Starting continuous help cycle.\n");

        // Loop to help all students currently waiting in the queue
        while (1) {
            int current_student_ID;
            
            // Acquire lock to safely check shared state and dequeue
            pthread_mutex_lock(&mutex);
            
            if (waiting_students == 0) {
                // If chairs are empty, Faculty is done with this cycle
                printf("Faculty: All waiting students helped. Breaking help cycle.\n");
                pthread_mutex_unlock(&mutex);
                break; // Exit inner while loop, go back to sem_wait(&Faculty_sem) (sleeping)
            }
            
            // Dequeue student and update chair count
            current_student_ID = dequeue_student();
            waiting_students--;
            
            printf("Faculty: Helping Student %d. Remaining in chairs: %d.\n", current_student_ID, waiting_students);
            
            // Release lock while performing the helping task (sleep)
            pthread_mutex_unlock(&mutex);

            // 3. Helping task (random time)
            rand_sleep(300, 800); 
            
            // 4. Signal the student that help is finished (using the student's own semaphore)
            sem_post(&student_sem[current_student_ID]);
            
            printf("Faculty: Finished help for Student %d.\n", current_student_ID);
        }
    }
    return NULL;
}

// --- Student Thread Function ---

void *stu_programming(void *arg) {
    int student_ID = *(int *)arg;
    printf("Student %d thread started.\n", student_ID);
    
    while (1) {
        // 1. Students are programming (waiting in random time)
        printf("Student %d: Programming...\n", student_ID);
        rand_sleep(500, 1500); 
        
        printf("Student %d: Finished programming, seeking help.\n", student_ID);

        // Acquire lock to check and update shared state
        pthread_mutex_lock(&mutex);
        
        // 2. Check if chairs are available
        if (waiting_students < MAX_CHAIRS) {
            // A. Chair available - take a seat
            enqueue_student(student_ID);
            waiting_students++;
            
            printf("Student %d: Waiting in chair. Total waiting: %d.\n", student_ID, waiting_students);
            
            // Release lock (critical section finished)
            pthread_mutex_unlock(&mutex);
            
            // 3. Wake up the Faculty (notify using Faculty's semaphore)
            sem_post(&Faculty_sem); 

            // 4. Wait for Faculty to finish helping *this* student (wait on own semaphore)
            sem_wait(&student_sem[student_ID]);
            
            printf("Student %d: Got help, returning to programming.\n", student_ID);
            
        } else {
            // B. Chairs full - go back to programming
            printf("Student %d: Chairs full (%d/%d). Returning to programming.\n", student_ID, MAX_CHAIRS, MAX_CHAIRS);
            
            // Release lock
            pthread_mutex_unlock(&mutex);
            // Will repeat the loop and rand_sleep() before trying again
        }
    }
    return NULL;
}

// --- Main Function ---

int main() {
    pthread_t Faculty_tid;
    pthread_t student_tids[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];
    
    // Seed the random number generator
    srand(time(NULL));

    // Initialize Mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("Mutex initialization failed");
        return 1;
    }

    // Initialize Semaphores
    if (sem_init(&Faculty_sem, 0, 0) != 0) { // Shared 0 (process local), Initial value 0 (sleeping)
        perror("Faculty semaphore initialization failed");
        return 1;
    }
    for (int i = 0; i < NUM_STUDENTS; i++) {
        if (sem_init(&student_sem[i], 0, 0) != 0) { // Shared 0, Initial value 0 (waiting for help)
            perror("Student semaphore initialization failed");
            return 1;
        }
        student_ids[i] = i; // Assign ID
    }

    // Create Faculty thread
    if (pthread_create(&Faculty_tid, NULL, Faculty_teaching, NULL) != 0) {
        perror("Faculty thread creation failed");
        return 1;
    }

    // Create Student threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        if (pthread_create(&student_tids[i], NULL, stu_programming, &student_ids[i]) != 0) {
            perror("Student thread creation failed");
            return 1;
        }
    }

    // Wait for all threads to finish (they run indefinitely, so this is mostly for cleanup if they were ever terminated)
    pthread_join(Faculty_tid, NULL);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(student_tids[i], NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&Faculty_sem);
    for (int i = 0; i < NUM_STUDENTS; i++) {
        sem_destroy(&student_sem[i]);
    }

    return 0;
}
