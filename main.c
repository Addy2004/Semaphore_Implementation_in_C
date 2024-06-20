#include <stdio.h>
#include <Windows.h>
#include "semaphore.h"

#define COUNTER_VAL 3   // max number of processes which can access the critical section at a time
#define NUM_OF_PROCESSES 11    // no. of threads trying to access the critical section 
#define SLEEP_DURATION 5000 // duration to put a thread to sleep (to simulate an active process)

// ANSI MACROS
#define AC_RED "\x1b[31m"   // to print in red color
#define AC_GREEN "\x1b[32m" // to print in green color
#define AC_NORMAL "\x1b[m"  // to print normally

semaphore sem;

// run function to assign to a thread/process
int run(int i) {
	//(void)arg;

	P(&sem); // wait
	printf("%sPROCESS %d: Critical Section is being used!\n\n", AC_RED, i+1); // print in red

	Sleep(SLEEP_DURATION); // simulate process/thread execution

	printf("%sPROCESS %d: Critical Section is being exited!\n\n", AC_GREEN, i+1); // print in green
	V(&sem); // signal

	return 0;
}

int main() {
	printf("Semaphore Implementation: \n\n");
	thrd_t threads[NUM_OF_PROCESSES]; // total number of threads to initialize

	sem_init(&sem, COUNTER_VAL); // Initialize the semaphore

	int i;
	for (i = 0; i < NUM_OF_PROCESSES; i++) {
		thrd_create(&threads[i], run, i); // creates new threads and gives them a start function

	}

	int res;
	for (i = 0; i < NUM_OF_PROCESSES; i++) {
		thrd_join(threads[i], &res); // suspends execution of calling thread till target thread finishes
	}

	sem_destroy(&sem); // destroys the semaphore

	printf("%s", AC_NORMAL); // sets print color to normal
	return 0;
}