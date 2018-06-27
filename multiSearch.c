//
// Created by nicod on 06/02/2018.
//

#include "Headers/multiSearch.h"

struct Node *pFinalGoal;
struct Node *pRoot;
struct Node *pMiddle;

void startMulti(struct Node *pR, int divisions, float weight){

    pRoot = pR;
    setFinalGoal();

    int g = pRoot->g;
    pRoot->g = 0;

    struct Node *m1;

    pMiddle = getHalfwayNode(m1);

    if (divisions == 2) {
        twoSearch(2);
    }

}


void twoSearch(float weight){

    struct Node *pSolution;

    int totalLength = INF;

    struct Node *pHalfway = pMiddle;

    weight = 1;

    pSolution = startSubRBFS(pRoot, pHalfway, weight);

    int length1stHalf = pSolution->g;

    struct Node *pFirstQ = getHalfwayNode(pSolution);

    pSolution = startSubRBFS(pHalfway, pFinalGoal, weight);

    int length2ndHalf = pSolution->g;

    struct Node *pThirdQ = getHalfwayNode(pSolution);


    //all variables initialised now


    int n = 3;

    while(n > 0) {

        n--;

        totalLength = length1stHalf + length2ndHalf;

        pSolution = startSubRBFS(pFirstQ, pThirdQ, weight);

        pHalfway = getHalfwayNode(pSolution);

        pSolution = startSubRBFS(pRoot, pHalfway, weight);

        length1stHalf = pSolution->g;

        pFirstQ = getHalfwayNode(pSolution);

        pSolution = startSubRBFS(pHalfway, pFinalGoal, weight);

        length2ndHalf = pSolution->g;

        pThirdQ = getHalfwayNode(pSolution);
    }
}

// returns the node of certain place in pPath.
// clears the path from memory.
struct Node *getNthNode(struct Node *pPath, int placeInPath) {
    int length = pPath->g;

    // initialise node.
    struct Node *pNthNode = malloc(sizeof(*pNthNode));
    pNthNode->g = 0;
    pNthNode->parent = NULL;
    pNthNode->f = 0;

    struct Node *pTemp;
    for (int i = 0; i < length; ++i) {
        if(i == placeInPath){
            for (int j = 0; j < NUM_TILES; ++j) {
                pNthNode->tiles[j] = pPath->tiles[j];
            }
        }
        // clear memory and go to next in path
        pTemp = pPath;
        pPath = pPath->parent;
        free(pTemp);
    }
    return pNthNode;
}

// returns the node of certain place in pPath.
// clears the path from memory.
struct Node *getHalfwayNode(struct Node *pPath) {
    int length = pPath->g;

    int halfwayPoint = length / 2;
    // initialise node.
    struct Node *pNthNode = malloc(sizeof(*pNthNode));
    pNthNode->g = 0;
    pNthNode->parent = NULL;
    pNthNode->f = 0;

    struct Node *pTemp;
    for (int i = 0; i < length; ++i) {
        if(i == halfwayPoint){
            for (int j = 0; j < NUM_TILES; ++j) {
                pNthNode->tiles[j] = pPath->tiles[j];
            }
        }
        // clear memory and go to next in path
        pTemp = pPath;
        pPath = pPath->parent;
        free(pTemp);
    }
    return pNthNode;
}

// sets pFinalGoal to correct final tile positions
void setFinalGoal(){

    // the final goal
    struct Node *pFG = malloc(sizeof(*pFG));

    // put tiles in correct final positions
    pFG->tiles[0] = NUM_TILES;
    for (int i = 1; i < NUM_TILES; ++i) {
        pFG->tiles[i] = i;
    }

    pFinalGoal = pFG;
}

// sets pFinalGoal to correct final tile positions
void setRoot(){

    // the final goal
    struct Node *pR = malloc(sizeof(*pR));

    // put tiles in correct final positions
    pR->tiles[0] = NUM_TILES;
    for (int i = 1; i < NUM_TILES; ++i) {
        pR->tiles[i] = i;
    }
    pRoot = pR;
}




