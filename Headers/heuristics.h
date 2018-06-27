//
// Created by nicod on 24/01/2018.
//

#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "Utils.h"

float get_h(struct Node *node);

int h_funct(struct Node *node);

float h_special(struct Node *node);

int getManDist(int tile, int position);

float getSpecialManDist(int tile, int position);

float get_double_h(struct Node *pA, struct Node *pB, float weight);

float getCornerH(struct Node *pNode, int weight);

float h_edge(struct Node *node, char *subGoals, int numberSubGoals);

int h_db16_4_Sum_Mirror(struct Node *node);

float h_RR(struct Node *pNode);

void loadDB(int number);

int h_db16_8(struct Node *node);

int h_all9(struct Node *node);

int h_db16_Sum8(struct Node *node);

int h_db16_Mirror8(struct Node *node);

int h_db16_Corner8(struct Node *node);

int h_db16_SumMirror8(struct Node *node);

int h_disjoint16_Sum8(struct Node *node);

int h_disjoint16_SumMirror8(struct Node *node);

int h_25_Sum6(struct Node *node);

float h_25_Weighted6(struct Node *node);

int h_25_Special6(struct Node *node);

void loadData(char *filename, char *pStore);

unsigned long getHash(char *state, int numberTiles);

char flip(int pos);

#endif // HEURISTICS_H
