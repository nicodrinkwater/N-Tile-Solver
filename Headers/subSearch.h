//
// Created by nicod on 21/02/2018.
//

#ifndef SEARCH_SUBSEARCH_H
#define SEARCH_SUBSEARCH_H

#include <stdlib.h>
#include <stdio.h>
#include "jhjtypes.h"
#include "Utils.h"
#include "heuristics.h"
#include "rbfs.h"

struct Node* startSubSearch(struct Node *pRoot, int depth);
char* selectStartingOrder(int choice);
struct Node* subSearch(struct Node *pNode, char *pOrder, int *finished);
struct Node *solveSubGoal(struct Node *pNode, int depth, char *pOrder, char complete);
void diffOrders(struct Node *pRoot, int depth);

#endif //SEARCH_SUBSEARCH_H
