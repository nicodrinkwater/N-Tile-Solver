//
// Created by nicod on 22/01/2018.
//

#include <time.h>
#include "Headers/ida.h"


struct Node *solution = NULL;
long nodesVisited;
int moves;
int firstMove;

// function. Starts the ida* search process
struct Node* ida_star(struct Node *root){

    solution = NULL;
    nodesVisited = 0;
    moves = 0;
    firstMove = 0;

    float bound;

    root->f = get_h(root);
    bound = root->f;
    root->g = 0;
    root->parent = NULL;

    if(root->f == 0){
        firstMove = 0;
        solution = root;
        return root;
    }

    enum SEARCHING search_status = IN_PROGRESS;
    int result = 0;

    while(search_status == IN_PROGRESS){

        result = search(root, bound);
        if(result == SUCCESS){
            search_status = FOUND;
        }
        if(result == INF){
            printf("No solution found");
            search_status = NOT_FOUND;
        }
        bound = result;
    }

    // the number of moves = g of last node.
    moves = solution->g;

    //put solution in correct order
    solution = reverseSol(solution);


    firstMove = solution->parent->direction;

    return solution;
}

float search(struct Node *node, float bound) {

    nodesVisited++;

    int min = INF;
    int result;

    if(node->f > bound){
        return node->f;
    }

    if(goal(node->tiles)){
        return SUCCESS;
    }

    min = INF;

    struct ListNode *pChildren = malloc(8);
    pChildren->node = NULL;
    pChildren->next = NULL;

    // add children of node to list pChildren
    addSuccessors(node, pChildren);

    while(pChildren->node != NULL) {

        struct Node *child = pChildren->node;

        // continue search with child until f > bound or goal reached
        result = search(child, bound);
        if(result == SUCCESS){
            if(solution == NULL) {
                // child is the goal node. the childs' ancestors form solution
                solution = child;
            }
            return SUCCESS;
        }
        if(result < min){
            min = result;
        }
        // clear memory.
        free(child);
        struct ListNode *temp = pChildren;

        // try next child in list of successors
        pChildren = pChildren->next;
        free(temp);
    }
    // free memory of successors as they have all been searched
    free(pChildren);
    return min;
}

// create list of successors from children of current node.
void addSuccessors(struct Node *pParent, struct ListNode *childList) {

    char blank = pParent->tiles[0];

    char state[NUM_TILES];

    // move blank right
    if(blank % SIDE != 0){
        struct Node *child = malloc(sizeof(*child));
        for (int i = 0; i < NUM_TILES; ++i) {
            child->tiles[i] = pParent->tiles[i];
            if(child->tiles[i] == blank + 1){
                child->tiles[i] = blank;
            }
        }
        child->tiles[0] = blank + 1;
        child->parent = pParent;

        // add child to list of succssors if not already in path.
        if(notInPath(child)){
            child->g = pParent->g + 1;
            child->f = get_h(child) + child->g;
            child->direction = 2;
            addToList(child, childList);

        } else {
            free(child);
        }
    }

    // move blank down
    if(blank <=  SIDE * (SIDE - 1)){
        struct Node *child = malloc(sizeof(*child));
        for (int i = 0; i < NUM_TILES; ++i) {
            child->tiles[i] = pParent->tiles[i];
            if(child->tiles[i] == blank + SIDE){
                child->tiles[i] = blank;
            }
        }
        child->tiles[0] = blank + SIDE;
        child->parent = pParent;

        // add child to list of succssors if not already in path.
        if(notInPath(child)){
            child->g = pParent->g + 1;
            child->f = get_h(child) + child->g;
            child->direction = 3;
            addToList(child, childList);
        } else {
            free(child);
        }
    }

    // move blank left
    if(blank % SIDE != 1){
        struct Node *child = malloc(sizeof(*child));
        for (int i = 0; i < NUM_TILES; ++i) {
            child->tiles[i] = pParent->tiles[i];
            if(child->tiles[i] == blank - 1){
                child->tiles[i] = blank;
            }
        }
        child->tiles[0] = blank - 1;
        child->parent = pParent;

        // add child to list of succssors if not already in path.
        if(notInPath(child)){
            child->g = pParent->g + 1;
            child->f = get_h(child) + child->g;
            child->direction = 4;
            addToList(child, childList);
        } else {
            free(child);
        }
    }

    // move blank up
    if(blank >  SIDE){
        struct Node *child = malloc(sizeof(*child));
        for (int i = 0; i < NUM_TILES; ++i) {
            child->tiles[i] = pParent->tiles[i];
            if(child->tiles[i] == blank - SIDE){
                child->tiles[i] = blank;
            }
        }
        child->tiles[0] = blank - SIDE;
        child->parent = pParent;

        // add child to list of succssors if not already in path.
        if(notInPath(child)){
            child->g = pParent->g + 1;
            child->f = get_h(child) + child->g;
            child->direction = 1;
            addToList(child, childList);
        } else {
            free(child);
        }
    }
}

// returns results of one search to program.c so that it can provide stats
struct Results testIda(struct Node *startingNode){
    clock_t startTime = clock();
    ida_star(startingNode);
    clock_t endTime = clock();
    float totalTime = (float)(endTime - startTime) / 1000;
    struct Results results = {totalTime, nodesVisited, moves};

    //writeLength();
    //clearSolution(solution);

    return results;
}

void writeLength(){
    FILE *pFile;
    char *startPos = revTransform(solution->tiles);
    pFile = fopen("results.txt", "a");
    if(pFile == NULL){
        pFile = fopen("results.txt", "w");
    }
    for (int i = 0; i < NUM_TILES; ++i) {
        fprintf(pFile, "%d,", startPos[i]);
    }
    fprintf(pFile, "%d\n", moves);
    fclose(pFile);
}


void writeFirstMove(struct Node *startingNode){
    ida_star(startingNode);
    FILE *pFile;
    char *startPos = revTransform(solution->tiles);
    pFile = fopen("firstMoves.txt", "a");
    if(pFile == NULL){
        pFile = fopen("results.txt", "w");
    }
    for (int i = 0; i < NUM_TILES; ++i) {
        fprintf(pFile, "%d,", startPos[i]);

    }
    fprintf(pFile, "%d,", firstMove);
    fprintf(pFile, "%d\n", moves);
    fclose(pFile);
}




