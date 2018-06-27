//
// Created by nicod on 21/01/2018.
//

#ifndef UTILS
#define UTILS

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define INF 99999
#define SUCCESS -1

#define NUM_TILES 9
#define SIDE 3

// ---------------------------------------------------------------------------
// Node used in search.
// ---------------------------------------------------------------------------

struct Node {
    float f;
    int g;
    float F;
    char tiles[NUM_TILES];
    struct Node *parent;
    int direction;
};

struct Data {
    char length;
    char *moves;
};

struct ListNode {
    struct Node *node;
    struct ListNode *next;
};

struct Results {
    float time;
    long nodes;
    int moves;
};

enum HEUR {
    MANHATTAN,
    SUM_4s,
    BEST_8,
    MIRROR_8,
    SUM_8,
    SPECIAL_MAN,
    SUM_MIRROR_8,
    SUM_25_6,
    WEIGHTED_25,
    SPECIAL_25_6,
    DISJOINT_16_8,
    DISJOINT_16_8_MIRROR,
    CORNER_8,
    RR
};

enum HEUR heuristic;

enum SEARCHING {FOUND, NOT_FOUND, IN_PROGRESS};

int abs(int);

int mod(int, int);

int max(int, int);

float maxf(float a, float b);

float min(int a, int b);

float minf(float a, float b);

char getLetter(char i);

char* transform (char* layout);

char* revTransform (char* layout);

void drawTiles(char *layout);

int goal(char *tiles);

int multiGoal(char *tiles, char *goal);

int edgeGoal(char *tiles);

int subGoal(char *tiles, char *subGoals, int numberSubGoals);

void addToList(struct Node *pChild, struct ListNode *pList);

char getLetter(char i);

int getListLength(struct ListNode *pItem);

void addToEndOfList(struct ListNode *pNew, struct ListNode *pList);

int notInPath(struct Node *child);

int nodeNotInList(struct Node *child, struct ListNode *pList);

struct Node * reverseSol(struct Node *cur);

void clearSolution(struct Node *pSol);

char* getRandom(int seed);

char* getRandomLayout();

int checkAdmissible(char *tiles);

struct Node* getStartNode(int seed);

struct Node* getSetNode(int number);

int doEasyMove(char *layout, int direction);

struct Node* getEasyNode(int total);

#endif //UTILS



