//
// Created by nicod on 01/02/2018.
//

#include "Headers/rbfs.h"
#include <time.h>
#include <pthread.h>

struct Node *solution;
struct Node *pGoal;
float weight;
long nodesVisited;
int moves;
enum METHOD method = NORMAL;
char *subGoals;
int numberSubGoals;

// function. Starts the RBFS search process
struct Node* startRBFS(struct Node *root){

    solution = NULL;
    nodesVisited = 0;
    moves = 0;

    // initialise root node.
    root->f = get_h(root);
    root->F = root->f;

    searchRBSF(root, INF);

    // the number of moves = g of last node.
    moves = solution->g;

    printf("Moves taken = %d\n", moves);

    return solution;
}

struct Node *startMultiRBFS(struct Node *pRoot, struct Node *pG, float wgt){
    pGoal = pG;
    method = MULTI;
    weight = wgt;
    return startRBFS(pRoot);
}

struct Node *startSubRBFS(struct Node *pRoot, char *pSGoals, int noSG, char *pFutures, int noFutures){
    method = EDGE;
    char *q = malloc(noSG + noFutures);

    for (int i = 0; i < noSG; ++i) {
        q[i] = pSGoals[i];
    }
    for (int j = 0; j < noFutures; ++j) {
        q[j + noSG] = pFutures[j];
    }
    subGoals = q;
    numberSubGoals = noSG + noFutures;
    return startRBFS(pRoot);
}


float searchRBSF(struct Node *pNode, float bound) {

    nodesVisited++;


    // is this node goal?
    if(method == NORMAL) {
        if (goal(pNode->tiles)) {
            solution = pNode;
            return SUCCESS;
        }
    }  else if(method == EDGE){
        if(subGoal(pNode->tiles, subGoals, numberSubGoals)){
            solution = pNode;
            return  SUCCESS;
        }
    }

    // priority queue
    PQUEUE *pQueue = malloc(sizeof(*pQueue));

    // PQueueInitialise(pQueue, max number elements, lowest possible value, ascending order);
    PQueueInitialise(pQueue, 4, 0, 0);

    addChildren(pNode, pQueue);

    // if no children return infinity
    if(PQueueIsEmpty(pQueue)){
        return INF;
    }

    // getF is the function that decides what is the priority of queue. It says choose node.F
    struct Node *pBestChild = PQueuePop(pQueue, getF);
    struct Node *pSecondChild = PQueuePop(pQueue, getF);

    // this is used as value of second best child.F
    float F2;

    // loop until child.F > bound or its a dead end (F = INF)
    while(pBestChild->F <= bound && pBestChild->F < INF){

        // if only one child in queue.
        if(pSecondChild == NULL){
            F2 = INF;
        } else {
            F2 = pSecondChild->F;
        }
        // continue search with bestchild.
        float result = searchRBSF(pBestChild, minf(bound, F2));

        if(result == SUCCESS){
            // if solution found return success up to top of recursive chain.
            return SUCCESS;
        } else {
            pBestChild->F = result;
        }

        // update the queue with bestchild and its new F score.
        PQueuePush(pQueue, pBestChild, getF);

        if(pSecondChild != NULL) {
            // put second child back in queue.
            PQueuePush(pQueue, pSecondChild, getF);
        }

        // get new best child and second best child
        pBestChild = PQueuePop(pQueue, getF);
        pSecondChild = PQueuePop(pQueue, getF);
    }

    float F = pBestChild->F;

    // memory management. children explored and branch cut so dispose
    free(pBestChild);
    free(pSecondChild);
    while(!PQueueIsEmpty(pQueue)){
        free(PQueuePop(pQueue, getF));
    }
    PQueueFree(pQueue);
    free(pQueue);

    return F;
}

void addChildren(struct Node *pParent, PQUEUE *pQueue) {
    char blank = pParent->tiles[0];

    // move blank right
    if (blank % SIDE != 0) {
        struct Node *child = malloc(sizeof(*child));
        for (int i = 0; i < NUM_TILES; ++i) {
            child->tiles[i] = pParent->tiles[i];
            if (child->tiles[i] == blank + 1) {
                child->tiles[i] = blank;
            }
        }
        child->tiles[0] = blank + 1;
        child->parent = pParent;

        // add child to list of succssors if not already in path.
        if (notInPath(child)) {
            addChild(pParent, child, pQueue);
        } else {
            free(child);
        }
    }

    // move blank down
    if (blank <= SIDE * (SIDE - 1)) {
        struct Node *child = malloc(sizeof(*child));
        for (int i = 0; i < NUM_TILES; ++i) {
            child->tiles[i] = pParent->tiles[i];
            if (child->tiles[i] == blank + SIDE) {
                child->tiles[i] = blank;
            }
        }
        child->tiles[0] = blank + SIDE;
        child->parent = pParent;

        // add child to list of succssors if not already in path.
        if (notInPath(child)) {
            addChild(pParent, child, pQueue);
        } else {
            free(child);
        }
    }

    // move blank left
    if (blank % SIDE != 1) {
        struct Node *child = malloc(sizeof(*child));
        for (int i = 0; i < NUM_TILES; ++i) {
            child->tiles[i] = pParent->tiles[i];
            if (child->tiles[i] == blank - 1) {
                child->tiles[i] = blank;
            }
        }
        child->tiles[0] = blank - 1;
        child->parent = pParent;

        // add child to list of succssors if not already in path.
        if (notInPath(child)) {
            addChild(pParent, child, pQueue);
        } else {
            free(child);
        }
    }

    // move blank up
    if (blank > SIDE) {
        struct Node *child = malloc(sizeof(*child));
        for (int i = 0; i < NUM_TILES; ++i) {
            child->tiles[i] = pParent->tiles[i];
            if (child->tiles[i] == blank - SIDE) {
                child->tiles[i] = blank;
            }
        }
        child->tiles[0] = blank - SIDE;
        child->parent = pParent;
        // add child to list of succssors if not already in path.
        if (notInPath(child)) {
            addChild(pParent, child, pQueue);
        } else {
            free(child);
        }
    }
}

// add child to priority queue of children.
void addChild(struct Node *pParent, struct Node *pChild, PQUEUE *pQueue){

    // just used for test subGoal functions, when method = EDGE.
    char sg[3] = {1, 2, 3};

    pChild->g = pParent->g + 1;
    if(method == NORMAL) {
        pChild->f = get_h(pChild) + pChild->g;
    } else if (method == MULTI){
        // this is for when we have variable goal.
        pChild->f = get_double_h(pChild, pGoal, weight) + pChild->g;
    } else if (method == EDGE){
        // this is for when we just want to do edge.
        pChild->f =h_edge(pChild, subGoals, numberSubGoals) + pChild->g;
    }
    // if parent already expanded
    if (pParent->f < pParent->F) {
        pChild->F = maxf(pParent->F, pChild->f);
    } else {
        pChild->F = pChild->f;
    }
    PQueuePush(pQueue, pChild, getF);
}

// returns results of one search to program.c so that it can provide stats
struct Results testRBFS(struct Node *startingNode){
    clock_t startTime = clock();
    startRBFS(startingNode);
    clock_t endTime = clock();
    float totalTime = (float)(endTime - startTime) / 1000;
    struct Results results = {totalTime, nodesVisited, moves};

    //put solution in correct order
    solution = reverseSol(solution);

    // draw solution
    struct Node *temp = solution;
//    while(temp != NULL){
//        drawTiles(temp->tiles);
//        temp = temp->parent;
//    }

    return results;
}



