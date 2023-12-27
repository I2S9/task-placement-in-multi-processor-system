#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "structures.h"
#include <time.h>
#define percentage 0.3

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

void sortPopulation(taskRepartition** Population, int size)
{
 for (int i = 1; i < size; i++) {
        taskRepartition* current = Population[i];
        int j = i - 1;
        while (j >= 0 && Population[j]->totalCost > current->totalCost) {
            Population[j + 1] = Population[j];
            j = j - 1;
        }
        Population[j + 1] = current;
    } 
}

void displayPopulation(taskRepartition** population, int size, int nbTask) {
    printf("Affichage de la population :\n");
    for (int i = 0; i < size; i++) {
        printf("Individu %d: ", i);
        for (int j = 0; j < nbTask; j++) {
            printf("%d ", population[i]->procRepartition[j]);
        }
        printf("\t Coût total : %f\n", population[i]->totalCost);
    }
}

taskRepartition* cross(taskRepartition* parent1, taskRepartition* parent2, int nbTask) {
    taskRepartition* child = malloc(sizeof(taskRepartition));
    child->procRepartition = malloc(nbTask * sizeof(int));

    int pivot = rand() % nbTask;
    for (int i = 0; i < pivot; i++) {
        child->procRepartition[i] = parent1->procRepartition[i];
    }
    for (int i = pivot; i < nbTask; i++) {
        child->procRepartition[i] = parent2->procRepartition[i];
    }
    child->totalCost = 0;
    return child;
}


void initializePopulation(taskRepartition** Population, int size, int **execCost, int commCost, int nbProc, int nbTask, double percentage)
{
    for (int i = 0; i < size; i++) {
        Population[i] = getRandomTaskRepartition(nbTask, nbProc);
        Population[i]->totalCost = getTotalExecCost(Population[i], **execCost, **commCost, nbTask, nbProc);
    }
    while (Population[0]->totalCost > percentage) 
    {
        sortPopulation(Population, size);

        int numSurvivors = (2 * size) / 3;

        for (int i = numSurvivors; i < size; i++) {
            int parent1Idx = rand() % numSurvivors; 
            int parent2Idx = rand() % size; 
        }

    }
    printf("Solution trouvée dans la population avec un coût de %f\n", Population[0]->totalCost);
}
