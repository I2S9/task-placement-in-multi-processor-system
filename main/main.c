#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <unistd.h>
#include "functions.h"

int main(void)
{
    char ch;
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
        sleep(1);
        if (kbhit())
        {
            ch = getch();
            if (ch == 'q')
            {
               break; 
            }
        }
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
