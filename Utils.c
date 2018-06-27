//
// Created by nicod on 23/01/2018.
//


#include "Headers/Utils.h"

// takes array of positions where layout[i] = tile number at position i on board.
// returns array where newLayout[i] = position on board of tile number i.
// blank is tile number 0, squares are 1 - NUM_TIlES inclusive
char* transform (char* layout){
    char *newLayout = malloc(NUM_TILES);
    for (int i = 0; i < NUM_TILES; ++i) {
        for (int j = 0; j < NUM_TILES; ++j) {
            if(layout[j] == i){
                newLayout[i] = j + 1;
            }
        }
    }
    return newLayout;
}

// reverses above transform function
char* revTransform (char* layout){
    char *newLayout = malloc(NUM_TILES);
    for (int i = 1; i < NUM_TILES + 1; ++i) {
        for (int j = 0; j < NUM_TILES; ++j) {
            if(layout[j] == i){
                newLayout[i - 1] = j;
            }
        }
    }
    return newLayout;
}

// draws the tiles
void drawTiles(char *layout){
    layout = revTransform(layout);
    for (int i = 0; i < NUM_TILES; ++i) {
        if(mod(i, SIDE) == 0){
            printf("\n");
        }
        char letter = getLetter(layout[i]);
        printf("%c", letter);

    }
    printf("\n");
}

// converts number into letter for drawing
char getLetter(char i) {
    switch(i){
        case 0: return ' ';
        case 1: return 'a';
        case 2: return 'b';
        case 3: return 'c';
        case 4: return 'd';
        case 5: return 'e';
        case 6: return 'f';
        case 7: return 'g';
        case 8: return 'h';
        case 9: return 'i';
        case 10: return 'j';
        case 11: return 'k';
        case 12: return 'l';
        case 13: return 'm';
        case 14: return 'n';
        case 15: return 'o';
        case 16: return 'p';
        case 17: return 'q';
        case 18: return 'r';
        case 19: return 's';
        case 20: return 't';
        case 21: return 'u';
        case 22: return 'v';
        case 23: return 'w';
        case 24: return 'x';
    }
    return '*';
}

// absolute  |a|
int abs(int a){
    if(a < 0){
        return -a;
    }
    else return a;
}

// returns a mod b
int mod(int a, int b){
    return a - (a / b) * b;
}

// returns max of a and b
int max(int a, int b){
    if(b > a){
        return  b;
    } else {
        return  a;
    }
}


// returns max of a and b
float maxf(float a, float b){
    if(b > a){
        return  b;
    } else {
        return  a;
    }
}


// returns min of a and b
float minf(float a, float b){
    if(b < a){
        return  b;
    } else {
        return  a;
    }
}

// returns min of a and b
float min(int a, int b){
    if(b < a){
        return  b;
    } else {
        return  a;
    }
}



// adds node to list
void addToList(struct Node *pChild, struct ListNode *pList) {
    struct ListNode *item = malloc(sizeof(struct ListNode));
    item->next = pList->next;
    item->node = pList->node;
    pList->node = pChild;
    pList->next = item;
}

int getListLength(struct ListNode *pItem) {
    int count = 0;

    struct ListNode *pTemp = pItem;
    while(pTemp != NULL){
        count++;
        pTemp = pTemp->next;
    }

    return count;
}

void addToEndOfList(struct ListNode *pNew, struct ListNode *pList){
    struct ListNode *pTemp = pList;
    while(pTemp->next != NULL){
        pTemp = pTemp->next;
    }

    pTemp->next = pNew;
}

// returns true if child not in path
int notInPath(struct Node *child) {
    struct Node*pTemp = child->parent->parent;
    while(pTemp != NULL){
        int equal = 1;
        for (int i = 0; i < NUM_TILES; ++i) {
            if(pTemp->tiles[i] != child->tiles[i]){
                equal = 0;
                break;
            }
        }
        if(equal == 1){
            return 0;
        }
        pTemp = pTemp->parent;
    }
    return 1;
}

int nodeNotInList(struct Node *child, struct ListNode *pList){
    struct ListNode *pTemp = pList;
    while(pTemp != NULL){
        int equal = 1;
        for (int i = 0; i < NUM_TILES; ++i) {
            if(pTemp->node->tiles[i] != child->tiles[i]){
                equal = 0;
                break;
            }
        }
        if(equal == 1){
            return 0;
        }
        pTemp = pTemp->next;
    }
    return 1;
}

// reverses solution so that starting position is head of list
// and final position (goal) is last member of list
struct Node * reverseSol(struct Node *cur) {
    struct Node *prev = NULL;
    while (cur) {
        struct Node *temp = cur;
        cur = cur->parent; // advance cur
        temp->parent = prev;
        prev = temp; // advance prev
    }
    return prev;
}

// frees memory holding solution.
void clearSolution(struct Node *pSol){
    struct Node *temp1;
    struct Node *temp2 = pSol;

    while(temp2->parent != NULL){
        temp1 = temp2;
        temp2 = temp1->parent;
        free(temp1);
    }
}

// returns a random layout of the tiles.
char * getRandomLayout(){

    char *tiles = malloc(NUM_TILES);
    for (int j = 0; j < NUM_TILES; ++j) {
        tiles[j] = -1;
    }
    for (int i = 0; i < NUM_TILES; ++i) {
        int a = random() % NUM_TILES;
        while(tiles[a] != -1){
            a = random() % NUM_TILES;
        }
        tiles[a] = i;
    }
    return tiles;
}

// returns a random, solvable layout of tiles
char * getRandom(int seed){
    int a = 0;
    char *tiles;
    while(a != 1){
        srandom(seed);
        seed++;
        tiles = getRandomLayout();
        a = checkAdmissible(tiles);
    }
    tiles = transform(tiles);
    return tiles;
}

// checks the inversions of the layout. returns 1 if solvable, 0 otherwise.
int checkAdmissible(char *tiles){

    int inversions = 0;
    int blankPos;
    for (int i = 0; i < NUM_TILES; ++i) {
        if(tiles[i] == 0){
            blankPos = i;
        }
        for (int j = i + 1; j < NUM_TILES; ++j) {
            if(tiles[j] < tiles[i] && tiles[j] != 0){
                inversions++;
            }
        }
    }

    int blankRow = (blankPos - mod(blankPos, SIDE)) / SIDE;

    // if even number of rows/columns:
    if(mod(SIDE, 2) == 0){
        // if blank on odd row (from top) we need even number of inversions
        if(mod(blankRow, 2) == 1 && mod(inversions, 2) == 0) {
            return 1;
        }
        // else if blank on even row we need odd number of inversions.
        else if(mod(blankRow, 2) == 0 && mod(inversions, 2) == 1){
            return 1;
        }
    } else {   // if odd number of rows/columns we need even number of inversions
        if(mod(inversions, 2) == 0){
            return 1;
        }
    }
    return 0;  // default: not admissible
}

// returns a random starting node for search
struct Node* getStartNode(int seed){
    char *tiles = getRandom(seed);
    struct Node *node = malloc(sizeof(struct Node));
    for (int i = 0; i < NUM_TILES; ++i) {
        node->tiles[i] = tiles[i];
    }
    node->parent = NULL;
    node->g = 0;
    return node;
}

// returns 1 if tiles in goal state. 0 otherwise
int goal(char *tiles){
    for (int i = 1; i < NUM_TILES; ++i) {
        if(tiles[i] != i){
            return 0;
        }
    }
    return 1;
}


// returns 1 if tiles in goal state. 0 otherwise
int multiGoal(char *tiles, char *goal){
    for (int i = 1; i < NUM_TILES; ++i) {
        if(tiles[i] != goal[i]){
            return 0;
        }
    }
    return 1;
}


// returns 1 if tiles in goal state. 0 otherwise
int edgeGoal(char *tiles){
    for (int i = 1; i <= SIDE; ++i) {
        if(tiles[i] != i){
            return 0;
        }
        if(tiles[(i - 1) * SIDE + 1] != (i - 1) * SIDE + 1){
            return 0;
        }
    }

    return 1;
}



// returns 1 if all subgoals (individual tile placement) including present subgoal are in goal state. 0 otherwise
int subGoal(char *tiles, char *subGoals, int numberSubGoals){

    for (int i = 0; i < numberSubGoals; ++i) {
        if(tiles[subGoals[i]] != subGoals[i]){
            return 0;
        }
    }
    return 1;
}

// returns a set starting node identified by 'number'
struct Node* getSetNode(int number) {
    struct Node *node = malloc(sizeof(struct Node));
    char *tiles;

    char t1[16] =  {2,1,3,4,5,6,7,8,9,10,11,12,13,15,14,0}; // 28 moves
    char t2[16] =  {6,12,2,10,9,13,1,7,0,3,4,8,11,5,14,15}; // 46
    char t3[16] =  {9,0,4,10,1,13,12,5,11,3,7,2,15,6,14,8}; // 49
    char t4[16] =  {14,13,10,4,11,2,12,9,8,0,5,1,6,15,3,7}; // 55
    char t5[16] =  {7,9,0,1,14,15,3,4,5,8,13,12,10,6,11,2}; // 50
    char t6[16] =  {11,8,4,1,6,14,10,7,13,3,2,12,0,9,5,15}; // 47
    char t7[16] =  {14,9,12,10,7,11,15,13,8,6,0,2,4,5,1,3}; // 66
    char t8[16] =  {3,4,9,0,2,12,7,13,1,5,11,10,15,6,8,14}; // 51
    char t9[16] =  {3,15,13,11,0,6,7,9,2,5,12,4,8,1,14,10}; // 55
    char t10[16] = {15,12,0,1,10,13,14,3,9,11,8,5,6,7,2,4}; // 60

    char a1[25] =  {1,2,8,5,9,6,7,4,3,0,11,12,14,18,10,16,17,13,20,15,21,22,23,19,24}; //  moves
    char a2[25] =  {1,2,3,4,5,6,8,12,9,10,11,7,13,14,15,16,17,18,19,0,21,22,23,24,20}; //
    char a3[25] =  {6,1,2,5,0,7,12,3,4,10,16,13,9,8,14,21,17,18,23,15,11,22,19,20,24}; //
    char a4[25] =  {1,7,2,4,9,6,8,13,3,5,11,12,15,19,0,16,17,18,10,14,21,22,23,24,20}; //
    char a5[25] =  {1,7,2,4,9,6,12,8,3,5,11,0,13,19,14,16,22,15,18,20,21,23,17,10,24}; // extension of a4
    char a6[25] =  {7,2,8,19,9,1,6,13,3,5,11,12,14,18,0,16,22,15,20,4,21,23,17,10,24}; // 48. extension of a5/a4


    if(NUM_TILES == 16){
        switch(number){
            case 1: tiles = transform(t1);
                break;
            case 2: tiles = transform(t2);
                break;
            case 3: tiles = transform(t3);
                break;
            case 4: tiles = transform(t4);
                break;
            case 5: tiles = transform(t5);
                break;
            case 6: tiles = transform(t6);
                break;
            case 7: tiles = transform(t7);
                break;
            case 8: tiles = transform(t8);
                break;
            case 9: tiles = transform(t9);
                break;
            case 10: tiles = transform(t10);
                break;
        }
    } else if(NUM_TILES == 25){
        switch(number){
            case 1: tiles = transform(a1);
                break;
            case 2: tiles = transform(a2);
                break;
            case 3: tiles = transform(a3);
                break;
            case 4: tiles = transform(a4);
                break;
            case 5: tiles = transform(a5);
                break;
            case 6: tiles = transform(a6);
                break;
        }
    }

    for (int i = 0; i < NUM_TILES; ++i) {
        node->tiles[i] = tiles[i];
    }
    node->parent = NULL;
    node->g = 0;

    return node;

}

struct Node *getHalfway(struct Node *root){

}


struct Node* getEasyNode(int total){
    int numberMoves = total / 300 + 1;
    char *layout = malloc(sizeof(NUM_TILES));

    for (int i = 0; i < NUM_TILES; ++i) {
        layout[i] = i + 1;
    }

    layout[NUM_TILES - 1] = 0;


    for (int j = 0; j < numberMoves; ++j) {
        int dir = rand() % 4;
        doEasyMove(layout, dir);
    }

    char *finishedLayout = transform(layout);

    struct Node *node = malloc(sizeof(struct Node));
    for (int i = 0; i < NUM_TILES; ++i) {
        node->tiles[i] = finishedLayout[i];
    }
    node->parent = NULL;
    node->g = 0;
    node->direction = 0;
    return node;
}

int doEasyMove(char *layout, int direction){
    int blankPos = 0;
    for (char i = 0; i < NUM_TILES; ++i) {
        if(layout[i] == 0){
            blankPos = i;
        }
    }

    // move blank up
    if(direction == 0 && blankPos >= SIDE) {
        char tile = layout[blankPos - SIDE];
        layout[blankPos] = tile;
        layout[blankPos - SIDE] = 0;
    }

    // move blank right
    if(direction == 1 && blankPos % SIDE != SIDE - 1){
        char tile = layout[blankPos + 1];
        layout[blankPos] = tile;
        layout[blankPos + 1] = 0;
    }

    // move blank down
    if(direction == 2 && blankPos < SIDE * (SIDE - 1)) {
        char tile = layout[blankPos + SIDE];
        layout[blankPos] = tile;
        layout[blankPos + SIDE] = 0;
    }

    // move blank left
    if(direction == 3 && blankPos % SIDE != 0) {
        char tile = layout[blankPos - 1];
        layout[blankPos] = tile;
        layout[blankPos - 1] = 0;
    }
}