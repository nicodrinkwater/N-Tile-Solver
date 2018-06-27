//
// Created by nicod on 08/02/2018.
//

#ifndef MULTISTART_H
#define MULTISTART_H

#include <stdlib.h>
#include <stdio.h>
#include "Utils.h"


struct ListNode* createOpenNodes(int sizeLimit);

void addToOpenList();

void addChildToOpenList(struct Node *pNode);

struct ListNode* multiStart(struct Node *pRoot, int sizeLimit);

void addChildToClosed(struct Node *pChild);

#endif //MULTISTART_H
