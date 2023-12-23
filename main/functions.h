void loadData(char* filename, int* nbProc, int* nbTask, int*** execCost, int*** commCost);
void displayExecCost(int **execCost, int nbProc, int nbTask);
void displaycommCost(int **commCost, int nbProc, int nbTask);
taskRepartition* getRandomTaskRepartition(int nbTask, int nbProc);
int getTotalExecCost(taskRepartition *tr, int **execCost,int **commCost, int nbTask);