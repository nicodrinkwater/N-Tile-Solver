//
// Created by nicod on 06/02/2018.
//

#ifndef MULTISEARCH_H
#define MULTISEARCH_H

#include <stdlib.h>
#include <stdio.h>
#include "Utils.h"
#include "heuristics.h"
#include "rbfs.h"


void startMulti(struct Node *pRoot, int divisions, float weight);

void twoSearch(float weight);

struct Node *getNthNode(struct Node *pPath, int placeInPath);

struct Node *getHalfwayNode(struct Node *pPath);

void setFinalGoal();

void setRoot();

#endif //MULTISEARCH_H
