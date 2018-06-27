//
// Created by nicod on 08/02/2018.
//

#include "Headers/multiStart.h"

// heads of open anc closed lists.
struct ListNode *headClosed;
struct ListNode *headOpen;

// returns a list of the first nodes found by breadth first search from the root.
// Length of list = sizeLimit.
struct ListNode* multiStart(struct Node *pRoot, int sizeLimit){

    struct ListNode *pStart = malloc(sizeof(struct ListNode));
    struct ListNode *pClosed = malloc(sizeof(struct ListNode));

    pStart->node = pRoot;

    pStart->next = NULL;

    pClosed->node = pRoot;

    pClosed->next = NULL;

    headClosed = pClosed;
    headOpen = pStart;


    return createOpenNodes(sizeLimit);
}

// returns list of nodes found by breadth first search.
struct ListNode* createOpenNodes(int sizeLimit) {
    int openSize = 1;
    int g = 0;

    while(getListLength(headOpen) < sizeLimit){
        addToOpenList();
    }
    // this is the list of nodes found by breadth first search.
    return headOpen;
}

// adds to new nodes to open list.
void addToOpenList() {
    struct Node *pParent = headOpen->node;
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
        if(nodeNotInList(child, headClosed)){
            child->g = pParent->g + 1;
            addChildToOpenList(child);
            // add to closed so that it is not added again later
            addChildToClosed(child);
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
        if(nodeNotInList(child, headClosed)){
            child->g = pParent->g + 1;
            addChildToOpenList(child);
            addChildToClosed(child);
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
        if(nodeNotInList(child, headClosed)){
            child->g = pParent->g + 1;
            addChildToOpenList(child);
            addChildToClosed(child);
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
        if(nodeNotInList(child, headClosed)){
            child->g = pParent->g + 1;
            addChildToOpenList(child);
            addChildToClosed(child);
        } else {
            free(child);
        }
    }

    struct ListNode *temp = headOpen;
    headOpen = headOpen->next;

    free(temp);
}


void addChildToClosed(struct Node *pChild) {
    struct ListNode *pTemp = malloc(sizeof(struct ListNode));
    pTemp->node = pChild;
    pTemp->next = headClosed;
    headClosed = pTemp;
}

void addChildToOpenList(struct Node *pNode) {
    struct ListNode *pNew= malloc(sizeof(struct ListNode));
    pNew->node = pNode;
    pNew->next = NULL;

    addToEndOfList(pNew, headOpen);
}



