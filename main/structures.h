#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct taskRepartition taskRepartition;

struct taskRepartition{
    int* procRepartition;
    double totalCost;
};

typedef taskRepartition* taskPopulation;

#define POPULATION_SIZE 10000
#define SELECTION_RATE 66
#define CROSS_RATE 40
#define MUTATION_RATE 20

#endif // STRUCTURES_H
