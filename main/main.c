#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include "functions.h"

void loadData(char* filename, int* nbProc, int* nbTask, int*** execCost, int*** commCost);
void displayExecCost(int **execCost, int nbProc, int nbTask);
void displaycommCost(int **commCost, int nbProc, int nbTask);

int main(void)
{
    char filename[] = "fichier_test.dat"; 
    int nbProc;
    int nbTask;
    int** execCost = NULL;
    int** commCost = NULL;
    loadData(filename, &nbProc, &nbTask, &execCost, &commCost);
    printf("Fin1");
    displayExecCost(execCost, nbProc, nbTask);
    displaycommCost(commCost, nbProc, nbTask);
    return 0; 
}

void displayExecCost(int **execCost, int nbProc, int nbTask) {
    puts("Matrice des couts d'éxecution");
    for (int p = 0; p < nbProc; p++) {
        for (int t = 0; t < nbTask; t++) {
            printf("%d\t", execCost[p][t]);
        }
        puts("");
    }
}
void displaycommCost(int **commCost, int nbProc, int nbTask) {
    puts("Matrice des couts de communication");
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
        perror("Impossible d'ouvrir le fichier");
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