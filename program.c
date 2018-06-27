//
// Created by nicod on 24/01/2018.
//

#include "Headers/program.h"

int seed;
int data8loaded = 0;
int data4loaded = 0;


void beginProgram(){
    // loop until user exits.
    while(1) {
        getInput();
    }
}

// get user input into what type of search is used
int getInput(){
    int input;
    int heur;
    int testNo;


    printf("What search method do you want to use: \n");
    printf("(1): Iterative Deepening A*\n");
    printf("(2): Recursive best first search\n");
    printf("(3): Exit\n");
    scanf("%d", &input);

    if(input == 1){
        searchType = IDA_STAR;
    } else if(input == 2){
        searchType = RBFS;
    } else if(input == 3){
        exit(999);
    }
    printf("What type of heuristic do you want to use?\n");
    printf("(1): Manhattan\n");
    printf("(2): Special\n");
    printf("(3): Sum 4s Database\n");
    printf("(4): Mirror 8s\n");
    printf("(5): Sum + Mirror 8s\n");
    printf("(6): Disjoint 8s\n");
    printf("(7): Disjoint Mirror 8s\n");
    printf("(8): Sum 25 6s\n");
    printf("(9): Weighted 25 6s\n");
    printf("(10): Special 25 6s\n");
    printf("(11): RR heuristic\n");
    scanf("%d", &heur);

    if(heur == 1){
        heuristic = MANHATTAN;
    } else if(heur == 2){
        heuristic = SPECIAL_MAN;
    } else if(heur == 3){
        heuristic = SUM_4s;
        if(data4loaded == 0) {
            loadDB(4);
            printf("Loading data.... \n");
            data4loaded = 1;
        }
    }  else if (heur == 4){
        heuristic = MIRROR_8;
    } else if (heur == 5){
        heuristic = SUM_MIRROR_8;
    } else if (heur == 6){
        heuristic = DISJOINT_16_8;
        if(data8loaded == 0) {
            loadDB(8);
            printf("Loading data.... \n");
            data8loaded = 1;
        }
    } else if (heur == 7){
        heuristic = DISJOINT_16_8_MIRROR;
        if(data8loaded == 0) {
            loadDB(8);
            printf("Loading data.... \n");
            data8loaded = 1;
        }
    } else if (heur == 8){
        heuristic = SUM_25_6;
    } else if (heur == 9){
        heuristic = WEIGHTED_25;
    } else if (heur == 10){
        heuristic = SPECIAL_25_6;
    } else if (heur == 11){
        heuristic = RR;
    }

    printf("How many tests? \n"
                   "0 to go back \n"
                   "99 for set layout \n"
                   "-1 to write starter training data\n"
                   "-2 to write random training data\n");
    scanf("%d", &testNo);

    // go to start of menu
    if(testNo == 0){
        getInput();
    }

    printf("Enter Seed:\n");
    scanf("%d", &seed);
    if(seed == 0){
        seed = time(NULL);
    }

    if(testNo == 99) {
        if (seed == 1) {
            testSet(1);
        } else if (seed == 2) {
            testSet(2);
        } else if (seed == 3) {
            testSet(3);
        } else if (seed == 4) {
            testSet(4);
        } else if (seed == 5) {
            testSet(5);
        } else if (seed == 6) {
            testSet(6);
        }
    } else if(testNo == -1){
        createStartingTrainingSet();
    } else if(testNo == -2){
        createTrainingSet();
    } else {
        testRandom(testNo);
    }
}

// returns results of one random search
struct Results searchOneRandom(double seed){
    struct Node *startingNode = getStartNode(seed);

    if(searchType == IDA_STAR) {
        return testIda(startingNode);
    } else if(searchType == RBFS){
        return testRBFS(startingNode);
    }
}

// returns results of a search with set starting node
struct Results searchOneSet(int number){
    struct Node *startingNode = getSetNode(number);
    if(searchType == IDA_STAR) {
        return testIda(startingNode);
    } else if(searchType == RBFS){
        return testRBFS(startingNode);
    }
}

// test search with set starting nodes. Quantity = 'number'
// Prints results to screen.
void testSet(int number){

    double *nodes = malloc(sizeof(double));
    double *moves = malloc(sizeof(double));
    double *time = malloc(sizeof(double));

    *nodes = 0;
    *moves = 0;
    *time = 0;

    struct Results r = searchOneSet(number);
    *nodes += r.nodes;
    *moves += r.moves;
    *time += r.time;
    seed++;

    printf("Testing Set Number %d Complete.\n\n", number);
    printH();
    printf("Number of moves = %.2lf\n", *moves);
    printf("Nummber of nodes visited = %.2lf\n", *nodes);
    printf("Time taken = %.5lf seconds\n", *time);
    printf("\n");
}

// test search with set starting nodes. Quantity = 'number'
// prints resuts to screen
void testRandom(int number){

    double *nodes = malloc(sizeof(double));
    double *moves = malloc(sizeof(double));
    double *time = malloc(sizeof(double));

    *nodes = 0;
    *moves = 0;
    *time = 0;

    int origSeed = seed;

    for (int i = 0; i < number; ++i) {
        struct Results r = searchOneRandom(seed);
        *nodes += r.nodes;
        *moves += r.moves;
        *time += r.time;
        seed++;
    }

    *nodes /= number;
    *moves /= number;
    *time /= number;

    printf("Testing %d Instances Complete.\n", number);
    printf("Seed = %d\n", origSeed);
    printH();
    printf("Average number of moves = %.2lf\n", *moves);
    printf("Average number of nodes visited = %.2lf\n", *nodes);
    printf("Average time taken = %.5lf seconds\n", *time);
    printf("\n");

}


void createTrainingSet(){
    int size = 15000;
    for (int i = 0; i < size; ++i) {
        struct Node *startingNode = getStartNode(seed);
        writeFirstMove(startingNode);
        seed += 1000;
    }
    printf("Training set created \n");
}



void createStartingTrainingSet(){
    int size = 30000;
    int numberMoves = 0;
    for (int i = 0; i < size; ++i) {
        struct Node *startingNode = getEasyNode(numberMoves);
        writeFirstMove(startingNode);
        numberMoves++;
    }
    printf("Starter Training set created \n");
}




void printH(){
    switch(heuristic){
        case MANHATTAN: printf("Heuristic used: Manhattan\n");
            break;
        case SPECIAL_MAN: printf("Heuristic used: Special Manhattan\n");
            break;
        case BEST_8: printf("Heuristic used: Best 8\n");
            break;
        case SUM_8: printf("Heuristic used: Sum 8\n");
            break;
        case SUM_MIRROR_8: printf("Heuristic used: Sum + Mirror 8\n");
            break;
        case SUM_25_6: printf("Heuristic used: Sum 25 6db\n");
            break;
        case WEIGHTED_25: printf("Heuristic used: Best 25 6db\n");
            break;
        case SUM_4s: printf("Heuristic used: Sum 4's\n");
            break;
        case SPECIAL_25_6: printf("Heuristic used: Special 25 6\n");
            break;
        case DISJOINT_16_8: printf("Heuristic used: Disjoint 16 8\n");
            break;
        case CORNER_8: printf("Heuristic used: Corner 8s\n");
            break;
        case DISJOINT_16_8_MIRROR: printf("Heuristic used: Disjoint 16 8 Mirror\n");
            break;

    }
}



