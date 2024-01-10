#ifndef STRUCTURES_H
#define STRUCTURES_H

#define POPULATION_SIZE 24
#define SELECTION_RATE 66
#define CROSS_RATE 40
#define MUTATION_RATE 20
#define NOMBRE_GENERATIONS 100

typedef struct taskRepartition taskRepartition;

struct taskRepartition{
    int* procRepartition;
    double totalCost;
};

typedef taskRepartition* taskPopulation;

#endif // STRUCTURES_H
