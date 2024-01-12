#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "functions.h"
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
//#include <conio.h>

/*
Instructions de compilation : 
 gcc -Wall -ansi -std=c99 main.c functions.c -o main.exe
 ./main.exe
 */

int main(void)
{
    srand(time(NULL));
    char filename[] = "fichier_test.dat"; 
    char ch;
    int nbProc;
    int nbTask;
    int** execCost = NULL;
    int** commCost = NULL;
    loadData(filename, &nbProc, &nbTask, &execCost, &commCost);
    displayExecCost(execCost, nbProc, nbTask);
    displaycommCost(commCost, nbProc, nbTask);


    //Déclaration de kbhit et getch - ou inclure conio.h ligne 10

    int kbhit(void){
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
        return FD_ISSET(STDIN_FILENO, &fds);
    }

    int getch(void) {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }

    int taillePopulation = POPULATION_SIZE;
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
        if (kbhit()){
            ch = getch();
            if (ch =='\n'){ //Appuyer sur "ENTREE" pour arrêter la génération des coûts minimaux obtenus par génération
                break;
            }
            /*}else if (ch == '\t'){
                continue;
            }*/
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