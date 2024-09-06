#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    int id;
    char status[10];
} Process;

typedef struct {
    Process* processes;
    int n;
} BullyAlgoExample2;

void ring(BullyAlgoExample2* bully) {
    printf("Enter total number of processes: ");
    scanf("%d", &(bully->n));

    bully->processes = (Process*)malloc(bully->n * sizeof(Process));
    for (int i = 0; i < bully->n; i++) {
        printf("Enter ID for process %d: ", i);
        scanf("%d", &(bully->processes[i].id));
        strcpy(bully->processes[i].status, "active");
    }
}

int getMaxValue(BullyAlgoExample2* bully) {
    int mxId = -99;
    int mxIdIndex = 0;
    for (int i = 0; i < bully->n; i++) {
        if (strcmp(bully->processes[i].status, "active") == 0 && bully->processes[i].id > mxId) {
            mxId = bully->processes[i].id;
            mxIdIndex = i;
        }
    }
    return mxIdIndex;
}

void performElection(BullyAlgoExample2* bully) {
    sleep(1);

    int maxIndex = getMaxValue(bully);
    printf("Process having id %d fails\n", bully->processes[maxIndex].id);
    strcpy(bully->processes[maxIndex].status, "inactive");

    int idOfInitiator = 0;
    int overStatus = 1;

    while (overStatus) {
        int higherProcesses = 0;

        for (int i = idOfInitiator + 1; i < bully->n; i++) {
            if (strcmp(bully->processes[i].status, "active") == 0) {
                printf("Process %d Passes Election(%d) message to process %d\n", idOfInitiator, idOfInitiator, bully->processes[i].id);
                higherProcesses = 1;
            }
        }

        if (higherProcesses) {
            for (int i = idOfInitiator + 1; i < bully->n; i++) {
                if (strcmp(bully->processes[i].status, "active") == 0) {
                    printf("Process %d Passes OK(%d) message to process %d\n", bully->processes[i].id, bully->processes[i].id, idOfInitiator);
                }
            }
            idOfInitiator++;
        } else {
            int coord = bully->processes[getMaxValue(bully)].id;
            printf("Finally Process %d Becomes Coordinator\n", coord);

            for (int i = coord - 1; i >= 0; i--) {
                if (strcmp(bully->processes[i].status, "active") == 0) {
                    printf("Process %d Passes Coordinator(%d) message to process %d\n", coord, coord, bully->processes[i].id);
                }
            }

            printf("End of Election\n");
            overStatus = 0;
            break;
        }
    }
}

int main() {
    BullyAlgoExample2 bully;
    ring(&bully);
    performElection(&bully);
    free(bully.processes);
    return 0;
}
