#ifndef STRUCTURES_H
#define STRUCTURES_H

#define POPULATION_SIZE 500
#define SELECTION_RATE 80
#define CROSS_RATE 70
#define MUTATION_RATE 100
#define NOMBRE_GENERATIONS 200

typedef struct taskRepartition taskRepartition;

struct taskRepartition{
    int* procRepartition;
    double totalCost;
};

typedef taskRepartition* taskPopulation;

#endif // STRUCTURES_H