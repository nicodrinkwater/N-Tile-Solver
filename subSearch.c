//
// Created by nicod on 21/02/2018.
//

#include "Headers/subSearch.h"

char noSubGoalsComplete;
int lookAheadDepth;
struct Node* solution;

int choice = 1;


// first function called, initialises variables and starts search for solution.
struct Node* startSubSearch(struct Node *pRoot, int depth){

    noSubGoalsComplete = 0;
    lookAheadDepth = depth;

    int finished = 0;
    struct Node *pCurrent = pRoot;

    char *pOrder = selectStartingOrder(choice);

    while(!finished) {
        pCurrent = subSearch(pCurrent, pOrder, &finished);
    }
    solution = pCurrent;

    int numberMoves = solution->g;

//    solution = reverseSol(solution);

    // draw solution
//    struct Node *temp = solution;
//    while(temp != NULL){
//        drawTiles(temp->tiles);
//        temp = temp->parent;
//    }

    printf("Solution found, moves = %d\n", numberMoves);
}

void diffOrders(struct Node *pRoot, int depth){
    for (int i = 0; i < 3; ++i) {
        choice = i + 1;
        startSubSearch(pRoot, depth);
    }
}

// returns pNode with added newPath to subgoal.
struct Node* subSearch(struct Node *pNode, char *pOrder, int *pFinished) {

//    struct Node *bestPath;
//    int best = INF;
//    int a = 1;
//    for (int i = 0; i < NUM_TILES - 1; ++i) {
//        int doIt = 1;
//        for (int j = 0; j < noSubGoalsComplete; ++j) {
//            if (a == pOrder[j]) {
//                doIt = 0;
//            }
//        }
//        if(doIt) {
//            pOrder[noSubGoalsComplete] = a;
//            struct Node *pNewPath = solveSubGoal(pNode, lookAheadDepth, pOrder, noSubGoalsComplete);
//            if (pNewPath->g < best) {
//                bestPath = pNewPath;
//                best = pNewPath->g;
//            }
//        }
//        a++;
//    }

    struct Node *pNewPath = solveSubGoal(pNode, lookAheadDepth, pOrder, noSubGoalsComplete);

    printf("Subgoal chosen\n");

    noSubGoalsComplete += lookAheadDepth + 1;

    if(noSubGoalsComplete >= NUM_TILES - 1){
        *pFinished = 1;
    }

    return pNewPath;
}

// returns best path to next goal
// taking into consideration future subgoals
// if depth = 0, only present goal considered.
// if depth = 2, present and next two subgoals considered. etc.
// pOrder = order of all subgoals.
// complete = number of subgoals already completed.
struct Node *solveSubGoal(struct Node *pNode, int depth, char *pOrder, char complete) {

    // goalTiles are those subgoals already complete plus current subgoal.
    char *pGoalTiles = malloc(complete + 1);
    for (int i = 0; i < complete + 1; ++i) {
        pGoalTiles[i] = pOrder[i];
    }
    int noGoals = complete + 1;

    // futureTiles are those future subgoals that are assessed as
    // to whether they are nearer or further from solution
    // when finding best solution to current subgoal.
    char *pFutureTiles = malloc(depth);
    int noFutures = 0;
    for (int j = 0; j < depth; ++j) {
        if (complete + j < NUM_TILES - 3) {   // there is no point taking last tile into consideration
            pFutureTiles[j] = pOrder[complete + 1 + j];
            noFutures++;
        }
    }

    struct Node *pFirstSolution = startSubRBFS(pNode, pGoalTiles, noGoals, pFutureTiles, noFutures);
    return pFirstSolution;

}

// sets the order of subgoals.
// (order of tiles that are put in correct place)
char* selectStartingOrder(int choice) {

    char *pOrder = malloc(NUM_TILES - 1);

    if(choice == 1) {
        for (int i = 0; i < NUM_TILES; ++i) {
            pOrder[i] = i + 1;
        }
    } else if(choice == 2){
        pOrder[0] = 4;
        pOrder[1] = 3;
        pOrder[2] = 2;
        pOrder[3] = 1;
        pOrder[4] = 5;
        pOrder[5] = 9;
        pOrder[6] = 13;
        pOrder[7] = 14;
        pOrder[8] = 10;
        pOrder[9] = 6;
        pOrder[10] = 7;
        pOrder[11] = 8;
        pOrder[12] = 12;
        pOrder[13] = 11;
        pOrder[14] = 15;

    } else if(choice == 3){
        pOrder[0] = 13;
        pOrder[1] = 9;
        pOrder[2] = 5;
        pOrder[3] = 1;
        pOrder[4] = 2;
        pOrder[5] = 3;
        pOrder[6] = 4;
        pOrder[7] = 8;
        pOrder[8] = 7;
        pOrder[9] = 6;
        pOrder[10] = 10;
        pOrder[11] = 14;
        pOrder[12] = 15;
        pOrder[13] = 11;
        pOrder[14] = 12;
    }
    return pOrder;
}
