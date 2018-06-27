//
// Created by nicod on 01/02/2018.
//

#ifndef SEARCH_RBFS_H
#define SEARCH_RBFS_H

#include <stdlib.h>
#include <stdio.h>
#include "jhjtypes.h"
#include "Utils.h"
#include "heuristics.h"
#include "pqueue.h"

enum METHOD {NORMAL, MULTI, EDGE};

struct Node* startRBFS(struct Node *root);

struct Node *startEdgeRBFS(struct Node *pRoot, float wgt);

struct Node *startSubRBFS(struct Node *pRoot, char *pSGoals, int noSG, char *pFutures, int noFutures);

float searchRBSF(struct Node *pNode, float bound);

void addChildren(struct Node *pParent, PQUEUE *pQueue);

struct Results testRBFS(struct Node *startingNode);

void addChild(struct Node *pParent, struct Node *pChild, PQUEUE *pQueue);

#endif //SEARCH_RBFS_H
