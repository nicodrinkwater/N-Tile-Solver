//
// Created by Nico on 22/01/2018.
//

#ifndef IDA
#define IDA

#include <stdlib.h>
#include <stdio.h>
#include "jhjtypes.h"
#include "Utils.h"
#include "heuristics.h"


#define MAX 10000

struct Results testIda(struct Node *startingNode);

struct Node* ida_star(struct Node *root);

float search(struct Node *node, float bound);

void addSuccessors(struct Node *pParent, struct ListNode *childList);

void writeLength();


void writeFirstMove(struct Node *startingNode);


#endif //IDA
