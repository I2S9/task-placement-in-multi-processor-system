# Task placement in a multi-processor system

 This project is an NP problem consisting of placing tasks in a multi-processor system using a genetic algorithm

## 1. Introduction

We have a set of processes (or tasks) $`T_1, . . , T_n`$ of a parallel program and a set of processors $P_1, . . , P_p$

The processors are not identical and we know for each task $`T_i`$ its execution cost $`q_{ij}`$ on $`P_j`$

If $`T_i`$ and $`T_j`$ are not placed on the same processor, then there is a communication cost to the exchange of data between these two tasks

The communication cost between two tasks placed on a single processor is negligeable

**The goal of this project is to minimize the total cost of execution, ie the sum of execution and communication costs**

***Example :***


<p align="center">
  <img src="https://github.com/I2S9/task_placement_in_multi_processor_system/assets/111307883/51c2d454-2a54-498a-a73a-0a17c16a6338" width="26%" height="269%">
</p>

In our case we have 8 tasks and 4 processors :
- Execution cost: $`q_{11} + q_{21} + q_{34} + q_{41} + q_{52} + q_{63} + q_{72} + q_{84}`$

- Cost of communication: $`C_{45} + C_{46} + C_{78}`$

### 1.1 Input

### 1.2 Output

$`C_{14} C_{24}`$ and $`C_{38}`$ (hatched links) are not counted because the corresponding tasks were placed on the same processors

## 2. NP problem

This problem is difficult in the sense of complexity. There is most likely no effective algorithm to find optimal solutions
Thus, since it is difficult to determine optimal solutions in a reasonable time, we will use heuristic (fast) approaches to determine good solutions

## 3. Genetic algorithm

<p align="center">
 <img src="https://github.com/I2S9/task_placement_in_multi_processor_system/assets/111307883/ee55ce33-90e2-4cc4-8760-b359c33e3540" width="30%" height="30%">
</p>

The Darwinian theory of evolution is based on two postulates :

- In each environment, only the best adapted species persist over time, the others being condemned to
disparate

- Within each species, population turnover is mainly due to the best individuals of the species

To **define a genetic algorithm:**

1. A function to optimize and a coding principle of the population element (eligible solution of the problem)
2. A mechanism for generating the initial population
3. Operators allowing the population to cross and diversify :
   - Selection to define individuals who survive
   - Crossing recomposes the genes of two individuals previously selected
   - Mutation allows a better exploration of the space of solutions
  
# 


    
# References 

[Comprendre les algorithmes génétiques](https://igm.univ-mlv.fr/~dr/XPOSE2013/tleroux_genetic_algorithm/fonctionnement.html)

[Les Algorithmes génétiques](https://khayyam.developpez.com/articles/algo/genetic/)
