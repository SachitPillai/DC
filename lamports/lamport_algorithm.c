#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PROCESSES 3

// Structure to represent a request
typedef struct {
    int timestamp;
    int process_id;
} Request;

// Structure to represent each process
typedef struct {
    int process_id;
    int timestamp;
    int replies_received;
    Request request_queue[NUM_PROCESSES];
    int queue_size;
    pthread_mutex_t lock;
} Process;

Process processes[NUM_PROCESSES]; // Array to represent all processes

// Function to send request to other processes
void send_request(Process *p) {
    p->timestamp++; // Increment logical clock
    printf("Process %d is requesting access to the critical section at time %d\n", p->process_id, p->timestamp);

    Request req;
    req.timestamp = p->timestamp;
    req.process_id = p->process_id;

    // Add request to its own queue
    p->request_queue[p->queue_size++] = req;

    // Send request to other processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (processes[i].process_id != p->process_id) {
            pthread_mutex_lock(&processes[i].lock);
            processes[i].request_queue[processes[i].queue_size++] = req;
            printf("Process %d received request from Process %d with timestamp %d\n",
                   processes[i].process_id, req.process_id, req.timestamp);
            pthread_mutex_unlock(&processes[i].lock);
        }
    }
}

// Function to send reply to other process
void send_reply(Process *p, int requester_id) {
    printf("Process %d sends reply to Process %d\n", p->process_id, requester_id);
}

// Function to check if process can enter the critical section
void try_enter_critical_section(Process *p) {
    while (p->replies_received < NUM_PROCESSES - 1) {
        // Wait until replies are received from all other processes
        sleep(1);
    }

    // Check if the process is at the head of its own request queue
    int can_enter = 1;
    for (int i = 0; i < p->queue_size; i++) {
        if (p->request_queue[i].process_id == p->process_id) {
            for (int j = 0; j < p->queue_size; j++) {
                if (p->request_queue[j].timestamp < p->request_queue[i].timestamp) {
                    can_enter = 0;
                    break;
                }
            }
            break;
        }
    }

    if (can_enter) {
        printf("Process %d is entering the critical section\n", p->process_id);
        sleep(2); // Simulate critical section
        printf("Process %d is leaving the critical section\n", p->process_id);
        p->replies_received = 0;
        p->queue_size = 0;
    }
}

// Thread function for each process
void *process_thread(void *arg) {
    Process *p = (Process *)arg;
    send_request(p);

    // Simulate receiving replies
    for (int i = 0; i < NUM_PROCESSES; i++) {
        if (processes[i].process_id != p->process_id) {
            send_reply(p, processes[i].process_id);
            p->replies_received++;
        }
    }

    try_enter_critical_section(p);
    return NULL;
}

int main() {
    pthread_t threads[NUM_PROCESSES];

    // Initialize processes
    for (int i = 0; i < NUM_PROCESSES; i++) {
        processes[i].process_id = i + 1;
        processes[i].timestamp = 0;
        processes[i].replies_received = 0;
        processes[i].queue_size = 0;
        pthread_mutex_init(&processes[i].lock, NULL);
    }

    // Create threads for each process
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pthread_create(&threads[i], NULL, process_thread, (void *)&processes[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy mutex locks
    for (int i = 0; i < NUM_PROCESSES; i++) {
        pthread_mutex_destroy(&processes[i].lock);
    }

    return 0;
}
