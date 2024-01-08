#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
// #include "structures.h"
// #include "functions.h"

#define POPULATION_SIZE 100
#define SELECTION_RATE 66
#define CROSS_RATE 40
#define MUTATION_RATE 20
#define NOMBRE_GENERATIONS 14

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
void sortPopulation(taskRepartition **Population, int taillePopulation);
void displayPopulation(taskRepartition **Population, int taillePopulation);


void mutate(taskRepartition* individu, int nbTask, int nbProc);
taskRepartition* cross(taskRepartition* parent1, taskRepartition* parent2, int nbTask);
void addNewGeneration(taskRepartition** currentPopulation, int nbTask, int nbProc, int** execCost, int** commCost);

int main(void)
{
    srand(time(NULL));
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
    sortPopulation(Population, taillePopulation);
    displayPopulation(Population, taillePopulation);
    for (int i = 0; i < taillePopulation; i++) {
            free(Population[i]);
    }

    taskRepartition **currentPopulation = malloc(POPULATION_SIZE * sizeof(taskRepartition *));
    if (currentPopulation == NULL) {
        fprintf(stderr, "Erreur d'allocation de memoire pour currentPopulation\n");
        return 1;
    }
    for (int i = 0; i < POPULATION_SIZE; i++) {
        currentPopulation[i] = getRandomTaskRepartition(nbTask, nbProc);
        currentPopulation[i]->totalCost = getTotalExecCost(currentPopulation[i], execCost,
                commCost, nbTask, nbProc);
    }

    for (int generation = 0; generation < NOMBRE_GENERATIONS; generation++) {
        addNewGeneration(currentPopulation, nbTask, nbProc, execCost, commCost);
        printf("Generation %d, cout minimal actuel : %f\n", generation, currentPopulation[0]->totalCost);
    }

    printf("Cout de placement minimal (total): %f\n", currentPopulation[0]->totalCost);
    printf("Le placement final correspondant est donc : \n");
    for (int i = 0; i < nbTask; i++) {
        printf("T%d : P%d ", i, currentPopulation[0]->procRepartition[i]);
    }
    printf("\n");

    for (int i = 0; i < POPULATION_SIZE; i++) {
        free(currentPopulation[i]->procRepartition);
        free(currentPopulation[i]);
    }
    free(currentPopulation);
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

// void mutate(taskRepartition* individu, int nbTask, int nbProc) {
//     int taskToMutate = rand() % nbTask;
//     int newProc = rand() % nbProc;
//     individu->procRepartition[taskToMutate] = newProc;
// }

void mutate(taskRepartition* individu, int nbTask, int nbProc) {
    int taskToMutate = rand() % nbTask;
    int newProc;
    do {
        newProc = rand() % nbProc;
    } while (newProc == individu->procRepartition[taskToMutate]);
    
    individu->procRepartition[taskToMutate] = newProc;
}

taskRepartition* cross(taskRepartition* parent1, taskRepartition* parent2, int nbTask) {
    taskRepartition* enfant = malloc(sizeof(taskRepartition));
    enfant->procRepartition = malloc(nbTask * sizeof(int));

    for (int i = 0; i < nbTask; i++) {
        enfant->procRepartition[i] = (rand() % 2 == 0) ? parent1->procRepartition[i] : parent2->procRepartition[i];
    }

    return enfant;
}

// void addNewGeneration(taskRepartition** currentPopulation, int nbTask, int nbProc, int** execCost, int** commCost) {
//     int X = POPULATION_SIZE * SELECTION_RATE / 100;

//     sortPopulation(currentPopulation, POPULATION_SIZE);

//     taskRepartition** newPopulation = malloc(POPULATION_SIZE * sizeof(taskRepartition*));
//     for (int i = 0; i < X; i++) {
//         newPopulation[i] = currentPopulation[i];
//     }

//     for (int i = X; i < POPULATION_SIZE; i++) {
//         int parent1Idx = rand() % X;
//         int parent2Idx = rand() % POPULATION_SIZE;

//         newPopulation[i] = cross(currentPopulation[parent1Idx], currentPopulation[parent2Idx], nbTask);
//         mutate(newPopulation[i], nbTask, nbProc);
//         // newPopulation[i]->totalCost = getTotalExecCost(newPopulation[i], execCost, commCost, nbTask);
//         newPopulation[i]->totalCost = getTotalExecCost(newPopulation[i], execCost, commCost, nbTask, nbProc);

//     }

//     for (int i = 0; i < POPULATION_SIZE; i++) {
//         if (i >= X) {
//             free(currentPopulation[i]->procRepartition);
//             free(currentPopulation[i]);
//         }
//         currentPopulation[i] = newPopulation[i];
//     }

//     free(newPopulation);
// }


void addNewGeneration(taskRepartition** currentPopulation, int nbTask, int nbProc, int** execCost, int** commCost) {
    int X = POPULATION_SIZE * SELECTION_RATE / 100;

    sortPopulation(currentPopulation, POPULATION_SIZE);

    taskRepartition** newPopulation = malloc(POPULATION_SIZE * sizeof(taskRepartition*));
    for (int i = 0; i < X; i++) {
        newPopulation[i] = currentPopulation[i];
    }

    for (int i = X; i < POPULATION_SIZE; i++) {
        int parent1Idx = rand() % X;
        int parent2Idx = rand() % POPULATION_SIZE;

        newPopulation[i] = cross(currentPopulation[parent1Idx], currentPopulation[parent2Idx], nbTask);

        do {
            mutate(newPopulation[i], nbTask, nbProc);
            newPopulation[i]->totalCost = getTotalExecCost(newPopulation[i], execCost, commCost, nbTask, nbProc);
        } while (newPopulation[i]->totalCost == 0);

    }

    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (i >= X) {
            free(currentPopulation[i]->procRepartition);
            free(currentPopulation[i]);
        }
        currentPopulation[i] = newPopulation[i];
    }

    free(newPopulation);
}

