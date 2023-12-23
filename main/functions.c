#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void loadData(char* filename, int* nbProc, int* nbTask, int*** execCost, int*** commCost)
{
    FILE* file = NULL;
    char buffer[1000];
    char delim[] = " ";
    file = fopen(filename, "r");

    if(file == NULL)
    {
        perror("Impossible d'ouvrir le fichier");
    }
    else
    {
        fgets(buffer, 1000, file);
        *nbTask = strtol(buffer, NULL, 10);
        fgets(buffer, 1000, file);
        *nbProc = strtol(buffer, NULL, 10);
        printf("%d, %d", *nbTask, *nbProc);
    }
}

void displayExecCost(int **execCost, int nbProc, int nbTask) {
    puts("Matrice des couts d'éxecution");
    for (int p = 0; p < nbProc; p++) {
        for (int t = 0; t < nbTask; t++) {
            printf("%.0lf\t", execCost[p][t]);
        }
        puts("");
    }
}
void displaycommCost(int **commCost, int nbProc, int nbTask) {
    puts("Matrice des couts de communication");
    for (int t1 = 0; t1 < nbTask; t1++) {
        for (int t2 = 0; t2 < nbTask; t2++) {
            printf("%.0lf\t", commCost[t1][t2]);
        }
        puts("");
    }
}

taskRepartition* getRandomTaskRepartition(int nbTask, int nbProc) {
    taskRepartition *tr = malloc(sizeof(taskRepartition));
    tr->procRepartition = malloc(nbTask * sizeof(int));
    for (int t = 0; t < nbTask; t++) {
        tr->procRepartition[t] = rand() % nbProc;
    }
    tr->totalCost = 0;
    return (tr);
}

int getTotalExecCost(taskRepartition *tr, int **execCost,int **commCost, int nbTask) {
    int cost = 0;
    for (int t = 0; t < nbTask; t++) {
        // printf("execCost[%d][%d]\n",tr->procRepartition[t],t);
        cost += execCost[tr->procRepartition[t]][t];
    }
    for (int t1 = 0; t1 < nbTask - 1; t1++) {
        for (int t2 = t1 + 1; t2 < nbTask; t2++) {
            if (tr->procRepartition[t1] != tr->procRepartition[t2]) {
                cost += commCost[t1][t2];
            }
        }
    }
    return (cost);
}