#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "structures.h"
#include <time.h>

void loadData(char* filename, int* nbProc, int* nbTask, int*** execCost, int*** commCost)
{
    FILE* file = NULL;
    char buffer[1000];
    char delim[] = " ";
    file = fopen(filename, "r");

    if(file == NULL)
    {
        perror("Impossible d'ouvrir le fichier\n");
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
    puts("Matrice des couts d'éxecution\n");
    for (int p = 0; p < nbProc; p++) {
        for (int t = 0; t < nbTask; t++) {
            printf("%.0lf\t", execCost[p][t]);
        }
        puts("");
    }
}
void displaycommCost(int **commCost, int nbProc, int nbTask) {
    puts("Matrice des couts de communication\n");
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

double getTotalExecCost(taskRepartition *tr, int **execCost, int **commCost, int nbTask, int nbProc) {
    double cost;
    double *costByProc = malloc(nbProc * sizeof(double));
    for (int p = 0; p < nbProc; p++) {
        cost = 0;
        for (int t = 0; t < nbTask; t++) {
            if (tr->procRepartition[t] == p) {
                cost += execCost[tr->procRepartition[t]][t];
            }
        }
        for (int t1 = 0; t1 < nbTask - 1; t1++) {
            if (tr->procRepartition[t1] == p) {
                for (int t2 = t1 + 1; t2 < nbTask; t2++) {
                    if (tr->procRepartition[t1] != tr->procRepartition[t2]) {
                        cost += commCost[t1][t2];
                    }
                }
            }
        }
        costByProc[p] = cost;
    }
    cost = costByProc[0];
    for (int p = 1; p < nbProc; p++) {
        if (cost < costByProc[p]){
            cost =costByProc[p];
        }
    }
    return (cost);
}

void initializePopulation(taskRepartition** Population, int size, int nbTask, int nbProc, int **execCost, int **commCost) {
    for (int i = 0; i < size; i++) {
        Population[i] = getRandomTaskRepartition(nbTask, nbProc);
        Population[i]->totalCost = getTotalExecCost(Population[i], execCost, commCost, nbTask, nbProc);
        printf("Individu %d: Repartition des taches - [", i);
        for (int j = 0; j < nbTask; j++) {
            printf("%d ", Population[i]->procRepartition[j]);
        }
        printf("], Cout Total - %f\n", Population[i]->totalCost);
    }
}

void sortPopulation(taskRepartition **population, int taillePopulation) {
    for (int i = 0; i < taillePopulation - 1; i++) {
        for (int j = 0; j < taillePopulation - i - 1; j++) {
            if (population[j]->totalCost > population[j + 1]->totalCost) {
                taskRepartition *temp = population[j];
                population[j] = population[j + 1];
                population[j + 1] = temp;
            }
        }
    }
}
void displayPopulation(taskRepartition **population, int taillePopulation) {
    for (int i = 0; i < taillePopulation; i++) {
        printf("Individu %d: Cout Total - %f\n", i, population[i]->totalCost);
    }
}
