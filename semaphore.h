#include <threads.h>

typedef struct Semaphore { // Structure of a semaphore
	int count;   // no of threads/processes able to access the resource guarded by the semaphore
	mtx_t mutex; // guards access to counter
	cnd_t cv;    // used to signal other threads using the semaphore
}

semaphore; // structure declaration

// Semaphore functions

// initializes the semaphore
int sem_init(semaphore* sem, unsigned int value) { // pointer to semaphore and value to initialize the counter to
	if (sem == NULL) {
		return -1;     // if semaphore points to null then return -1
	}

	mtx_init(&sem->mutex, mtx_plain); // initializes the mutex
	cnd_init(&sem->cv);               // initializes the comditional variable
	sem->count = value;               // sets the counter to the initialized value

	return 0;
}

int P(semaphore* sem) {
	if (sem == NULL) {
		return -1;     // if semaphore points to null then return -1
	}

	mtx_lock(&sem->mutex); // Lock the mutex

	while (sem->count == 0) {
		cnd_wait(&sem->cv, &sem->mutex); // If counter is zero then wait till it is free to use
	}

	sem->count--; // Decrement the counter as it is in use by the process/thread

	// Usage is being simulated by putting the thread to sleep for a duration

	mtx_unlock(&sem->mutex); // Unlock the mutex
	return 0;
}

int V(semaphore* sem) {
	if (sem == NULL) {
		return -1;     // if semaphore points to null then return -1
	}

	mtx_lock(&sem->mutex); // Lock the mutex
	sem->count++;          // Increment the counter

	cnd_signal(&sem->cv);  // signal the conditional variable to allow other threads/processes
	                       // a chance at accessing the critical section
	mtx_unlock(&sem->mutex); // Unlock the mutex

	return 0;
}

int sem_destroy(semaphore* sem) { // Destroys the semaphore after completion to prevent memory leaks
	if (sem == NULL) {
		return -1;     // if semaphore points to null then return -1
	}

	mtx_destroy(&sem->mutex); // Destroys the mutex
	cnd_destroy(&sem->cv);    // Destroys the conditional variable

	return 0;
}