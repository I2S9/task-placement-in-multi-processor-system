#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct taskRepartition taskRepartition;

struct taskRepartition{
    int* procRepartition;
    double totalCost;
};

typedef taskRepartition* taskPopulation;


#endif // STRUCTURES_H
