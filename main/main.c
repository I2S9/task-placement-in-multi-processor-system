#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// #include "structures.h"
// #include "functions.h"

typedef struct taskRepartition taskRepartition;

struct taskRepartition{
    int* procRepartition;
    double totalCost;
};

typedef taskRepartition* taskPopulation;


void loadData(char* filename, int* nbProc, int* nbTask, int*** execCost, int*** commCost);
void displayExecCost(int **execCost, int nbProc, int nbTask);
void displaycommCost(int **commCost, int nbProc, int nbTask);

void initializePopulation(taskRepartition** Population, int size, int nbTask, int nbProc,int **execCost, int **commCost);
taskRepartition* getRandomTaskRepartition(int nbTask, int nbProc);
double getTotalExecCost(taskRepartition *tr, int **execCost,int **commCost, int nbTask, int nbProc);

int main(void)
{
    char filename[] = "fichier_test.dat"; 
    int nbProc;
    int nbTask;
    int** execCost = NULL;
    int** commCost = NULL;
    loadData(filename, &nbProc, &nbTask, &execCost, &commCost);
    displayExecCost(execCost, nbProc, nbTask);
    displaycommCost(commCost, nbProc, nbTask);


    int taillePopulation = 100;
    taskRepartition** Population = malloc(taillePopulation * sizeof(taskRepartition*));
    if (Population == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        return 1;
    }
    initializePopulation(Population, taillePopulation, nbTask, nbProc, execCost, commCost);
    for (int i = 0; i < taillePopulation; i++) {
            free(Population[i]);
    }
    free(Population);
    return 0; 
}

void displayExecCost(int **execCost, int nbProc, int nbTask) {
    puts("Matrice des couts d'execution :");
    for (int p = 0; p < nbProc; p++) {
        for (int t = 0; t < nbTask; t++) {
            printf("%d\t", execCost[p][t]);
        }
        puts("");
    }
}
void displaycommCost(int **commCost, int nbProc, int nbTask) {
    puts("Matrice des couts de communication :");
    for (int t1 = 0; t1 < nbTask; t1++) {
        for (int t2 = 0; t2 < nbTask; t2++) {
            printf("%d\t", commCost[t1][t2]);
        }
        puts("");
    }
}

void loadData(char* filename, int* nbProc, int* nbTask, int*** execCost, int*** commCost)
{
    FILE* file = NULL;
    char buffer[1000];
    char delim[] = " ";
    char* ptr;
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
        printf("%d, %d\n", *nbTask, *nbProc);

        *execCost = malloc(*nbProc*sizeof(int*));
        for(int i=0; i<*nbProc; i++)
        {
            (*execCost)[i] = malloc(*nbTask*sizeof(int));
            fgets(buffer, 1000, file);
            printf("%s\n",buffer);
            ptr = strtok(buffer, delim);
            int j=0;
            while(NULL != ptr)
            {
                (*execCost)[i][j] = strtol(ptr, NULL, 10);
                ptr = strtok(NULL, delim);
                j++;
            }
        }
        *commCost = malloc(*nbTask*sizeof(int*));
        for(int i=0; i<*nbTask; i++)
        {
            (*commCost)[i] = malloc(*nbTask*sizeof(int));
            for(int j=0; j<*nbTask; j++)
            {
                (*commCost)[i][j] = 0; 
            }
        }
        for(int i=0; i<*nbTask; i++)
        {
            fgets(buffer, 1000, file);
            printf("%s\n",buffer);
            ptr = strtok(buffer, delim);
            int j=0;
            while(NULL != ptr)
            {
                int val = strtol(ptr, NULL, 10);
                (*commCost)[i][j] = val;
                (*commCost)[j][i] = val;
                ptr = strtok(NULL, delim);
                j++;
            }
        }
    }
    fclose(file);
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
