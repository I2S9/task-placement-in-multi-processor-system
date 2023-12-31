#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "structures.h"

void loadData(char* filename, int* nbProc, int* nbTask, int*** execCost, int*** commCost);
void displayExecCost(int **execCost, int nbProc, int nbTask);
void displaycommCost(int **commCost, int nbProc, int nbTask);
taskRepartition* getRandomTaskRepartition(int nbTask, int nbProc);
double getTotalExecCost(taskRepartition *tr, int **execCost,int **commCost, int nbTask, int nbProc);

void initializePopulation(taskRepartition** Population, int size, int nbTask, int nbProc,int **execCost, int **commCost);
taskRepartition* getRandomTaskRepartition(int nbTask, int nbProc);
double getTotalExecCost(taskRepartition *tr, int **execCost,int **commCost, int nbTask, int nbProc);
void sortPopulation(Population, taillePopulation);
void displayPopulation(Population, taillePopulation);

void mutate(taskRepartition* individu, int nbTask, int nbProc);
taskRepartition* cross(taskRepartition* parent1, taskRepartition* parent2, int nbTask);
void addNewGeneration(taskRepartition** currentPopulation, int nbTask, int nbProc, int** execCost, int** commCost);
#endif // FUNCTIONS_H
