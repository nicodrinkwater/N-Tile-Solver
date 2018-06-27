//
// Created by nicod on 24/01/2018.
//

#include <math.h>
#include "Headers/heuristics.h"
#include "Headers/program.h"

char *pStore1;
char *pStore2;
char *pStore3;
char *pStore4;

char *pStore4_1;
char *pStore4_2;
char *pStore4_3;
char *pStore4_4;

char *pDisStore1;
char *pDisStore2;

// gets the h score depending on method chosen
float get_h(struct Node *node){
    if(heuristic == MANHATTAN){
        return h_funct(node);
    } else if(heuristic == SPECIAL_MAN){
        return h_special(node);
    } else if(heuristic == SUM_4s){
        return h_db16_4_Sum_Mirror(node);
    } else if(heuristic == MIRROR_8){
        return h_db16_Mirror8(node);
    } else if(heuristic == SUM_MIRROR_8){
        return h_db16_SumMirror8(node);
    } else if(heuristic == SUM_25_6){
        return h_25_Sum6(node);
    } else if(heuristic == WEIGHTED_25){
        return h_25_Weighted6(node);
    } else if(heuristic == SPECIAL_25_6){
        return h_25_Special6(node);
    } else if(heuristic == DISJOINT_16_8){
        return h_disjoint16_Sum8(node);
    } else if(heuristic == DISJOINT_16_8_MIRROR){
        return h_disjoint16_SumMirror8(node);
    } else if(heuristic == RR){
        return h_RR(node);
    }
}

// returns heuristic score for Node node
int h_funct(struct Node *node) {

    int score = 0;

    for (int i = 1; i < NUM_TILES; ++i) {
        score += getManDist(i, (int)node->tiles[i]);
    }
    return score;
}

// returns heuristic score for Node node
float h_special(struct Node *node) {

    float score = 0;

    for (int i = 1; i < NUM_TILES; ++i) {
        score += getManDist(i, (int)node->tiles[i]);
    }
    return score * 3;

}

// returns just left and top edge man dist (experimental)
float h_edge(struct Node *node, char *subGoals, int numberSubGoals){

    int score = 0;
    int inplace = 0;
    int distanceBlank;
    int manD;

    for (int i = 0; i < numberSubGoals; ++i) {
        score += getManDist(subGoals[i], node->tiles[subGoals[i]]);
        if(getManDist(subGoals[i], node->tiles[subGoals[i]]) == 0){
            inplace++;
        }
    }

    manD = getManDist(subGoals[numberSubGoals - 1], node->tiles[subGoals[numberSubGoals - 1]]);

    distanceBlank = getManDist(node->tiles[0], NUM_TILES - 1);


    return score + distanceBlank / 3;
}

// returns highest of two 8dbs for both original and its flip
int h_db16_4_Sum_Mirror(struct Node *node){
    char t1[4];
    char t2[4];
    char t3[4];
    char t4[4];

    char mt1[4];
    char mt2[4];
    char mt3[4];
    char mt4[4];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];

    t2[0] = node->tiles[5];
    t2[1] = node->tiles[6];
    t2[2] = node->tiles[7];
    t2[3] = node->tiles[8];

    t3[0] = node->tiles[9];
    t3[1] = node->tiles[10];
    t3[2] = node->tiles[11];
    t3[3] = node->tiles[12];

    t4[0] = node->tiles[13];
    t4[1] = node->tiles[14];
    t4[2] = node->tiles[15];
    t4[3] = node->tiles[0];


    mt1[0] = flip(node->tiles[1]);
    mt1[1] = flip(node->tiles[5]);
    mt1[2] = flip(node->tiles[9]);
    mt1[3] = flip(node->tiles[13]);

    mt2[0] = flip(node->tiles[2]);
    mt2[1] = flip(node->tiles[6]);
    mt2[2] = flip(node->tiles[10]);
    mt2[3] = flip(node->tiles[14]);

    mt3[0] = flip(node->tiles[3]);
    mt3[1] = flip(node->tiles[7]);
    mt3[2] = flip(node->tiles[11]);
    mt3[3] = flip(node->tiles[15]);

    mt4[0] = flip(node->tiles[4]);
    mt4[1] = flip(node->tiles[8]);
    mt4[2] = flip(node->tiles[12]);
    mt4[3] = flip(node->tiles[0]);


    unsigned long hash1 = getHash(t1, 4);
    unsigned long hash2 = getHash(t2, 4);
    unsigned long hash3 = getHash(t3, 4);
    unsigned long hash4 = getHash(t4, 4);

    int h1 = pStore4_1[hash1];
    int h2 = pStore4_2[hash2];
    int h3 = pStore4_3[hash3];
    int h4 = pStore4_4[hash4];

    hash1 = getHash(mt1, 4);
    hash2 = getHash(mt2, 4);
    hash3 = getHash(mt3, 4);
    hash4 = getHash(mt4, 4);

    int mh1 = pStore4_1[hash1];
    int mh2 = pStore4_2[hash2];
    int mh3 = pStore4_3[hash3];
    int mh4 = pStore4_4[hash4];

    // return biggest of tiles and it's mirror.

    return max(h1 + h2 + h3 + h4, mh1 + mh2 + mh3 + mh4);
}

// returns man dist of state A to state B.
float get_double_h(struct Node *pA, struct Node *pB, float weight){
    int score = 0;
    for (int i = 1; i < NUM_TILES; ++i) {
        score += getManDist(pA->tiles[i], pB->tiles[i]);
    }
    return score * weight;
}

float getCornerH(struct Node *pNode, int weight){
    int score = 0;
    for (int i = 1; i <= SIDE; ++i) {
        score += getManDist(pNode->tiles[i], i);
    }

    for (int j = 1; j < SIDE; ++j) {
        score += getManDist(pNode->tiles[j * SIDE + 1], j * SIDE + 1);
    }

    return score * weight;
}

// returns manhattan distance of tile from it's destination
int getManDist(int tile, int position) {
    int x = mod(tile - 1, SIDE);
    int y = (tile - x) / SIDE;

    int posX = mod(position - 1, SIDE);
    int posY = (position - posX) / SIDE;

    return abs(posX - x) + abs(posY - y);
}

// this heuristic is ...
// the number of tiles not in place order STARTING from 1 and then 2 ...
// plus the distance of the first tile not in its correct place to its correct place
// plus the distance of blank to this tile.
float h_RR(struct Node *pNode){
    int tile = 1;
    int score = NUM_TILES - 1;
    for (int i = 1; i < NUM_TILES; ++i) {
        if(pNode->tiles[i] == i){
            score--;
            tile++;
        } else {
            break;
        }
    }
    score *= 2 * SIDE;
    if(score == 0){ // all tiles in correct place.
        return 0;
    }
    // tile is now the first tile not it's correct place.
    score += getManDist(tile, pNode->tiles[tile]) + getManDist(pNode->tiles[tile], pNode->tiles[0]);

    return score;
}

// weighted top left corners and edges
float getSpecialManDist(int tile, int position) {
    int x = mod(tile - 1, SIDE);
    int y = (tile - x) / SIDE;

    int posX = mod(position - 1, SIDE);
    int posY = (position - posX) / SIDE;

    float dist = abs(posX - x) + abs(posY - y);
    float factor = 1.1;
    if(tile == 1){
        dist *= factor * 6;
    } else if(tile == 2 || tile == 5){
        dist *= factor * 5;
    } else if(tile == 3 || tile == 9){
        dist *= factor * 4;
    } else if(tile == 4 || tile == 13){
        dist *= factor * 3;
    } else if(tile == 6){
        dist *= factor * 2;
    } else if(tile == 7 || tile == 10){
        dist *= factor * 1.5;
    } else if(tile == 8 || tile == 14){
        dist *= factor * 1.2;
    }

    return dist;
}

// returns highest of two 8db h scores
int h_db16_8(struct Node *node){
    char t1[8];
    char t2[8];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];
    t1[4] = node->tiles[5];
    t1[5] = node->tiles[6];
    t1[6] = node->tiles[7];
    t1[7] = node->tiles[8];

    t2[0] = node->tiles[9];
    t2[1] = node->tiles[10];
    t2[2] = node->tiles[11];
    t2[3] = node->tiles[12];
    t2[4] = node->tiles[13];
    t2[5] = node->tiles[14];
    t2[6] = node->tiles[15];
    t2[7] = node->tiles[0];

    unsigned long hash1 = getHash(t1, 8);
    unsigned long hash2 = getHash(t2, 8);

    int h1 = pStore1[hash1];
    int h2 = pStore2[hash2];

    if(h2 > h1){
        return h2;
    }

    return h1;
}

int h_all9(struct Node *node){
    char t1[9];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];
    t1[4] = node->tiles[5];
    t1[5] = node->tiles[6];
    t1[6] = node->tiles[7];
    t1[7] = node->tiles[8];
    t1[8] = node->tiles[0];

    unsigned long hash1 = getHash(t1, 9);


    int h1 = pStore1[hash1];

    return h1;
}

// returns sum of two 8db h scores. WIll NOT give shortest path.
int h_db16_Sum8(struct Node *node){
    char t1[8];
    char t2[8];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];
    t1[4] = node->tiles[5];
    t1[5] = node->tiles[6];
    t1[6] = node->tiles[7];
    t1[7] = node->tiles[8];

    t2[0] = node->tiles[9];
    t2[1] = node->tiles[10];
    t2[2] = node->tiles[11];
    t2[3] = node->tiles[12];
    t2[4] = node->tiles[13];
    t2[5] = node->tiles[14];
    t2[6] = node->tiles[15];
    t2[7] = node->tiles[0];

    unsigned long hash1 = getHash(t1, 8);
    unsigned long hash2 = getHash(t2, 8);

    int h1 = pStore1[hash1];
    int h2 = pStore2[hash2];

    return h1 + h2;
}


// returns corner h. Admissible. gives shortest path.
int h_db16_Corner8(struct Node *node){
    char t1[8];
    char t2[8];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];
    t1[4] = node->tiles[5];
    t1[5] = node->tiles[6];
    t1[6] = node->tiles[9];
    t1[7] = node->tiles[13];

    t2[0] = node->tiles[7];
    t2[1] = node->tiles[8];
    t2[2] = node->tiles[10];
    t2[3] = node->tiles[11];
    t2[4] = node->tiles[12];
    t2[5] = node->tiles[14];
    t2[6] = node->tiles[15];
    t2[7] = node->tiles[0];

    unsigned long hash1 = getHash(t1, 8);
    unsigned long hash2 = getHash(t2, 8);

    int h1 = pStore3[hash1];
    int h2 = pStore4[hash2];

    return h1;
}

// returns sum of two disjoint 8db h scores. Admissible.
int h_disjoint16_Sum8(struct Node *node){
    char t1[8];
    char t2[7];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];
    t1[4] = node->tiles[5];
    t1[5] = node->tiles[6];
    t1[6] = node->tiles[7];
    t1[7] = node->tiles[8];

    t2[0] = node->tiles[9];
    t2[1] = node->tiles[10];
    t2[2] = node->tiles[11];
    t2[3] = node->tiles[12];
    t2[4] = node->tiles[13];
    t2[5] = node->tiles[14];
    t2[6] = node->tiles[15];
    //t2[7] = node->tiles[0];

    unsigned long hash1 = getHash(t1, 8);
    unsigned long hash2 = getHash(t2, 7);

    int h1 = pDisStore1[hash1];
    int h2 = pDisStore2[hash2];

    return h1 + h2;
}

// returns highest of two 8dbs for both original and its flip
int h_db16_Mirror8(struct Node *node){
    char t1[8];
    char t2[8];

    char t3[8];
    char t4[8];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];
    t1[4] = node->tiles[5];
    t1[5] = node->tiles[6];
    t1[6] = node->tiles[7];
    t1[7] = node->tiles[8];

    t2[0] = node->tiles[9];
    t2[1] = node->tiles[10];
    t2[2] = node->tiles[11];
    t2[3] = node->tiles[12];
    t2[4] = node->tiles[13];
    t2[5] = node->tiles[14];
    t2[6] = node->tiles[15];
    t2[7] = node->tiles[0];

    t3[0] = flip(node->tiles[1]);
    t3[1] = flip(node->tiles[5]);
    t3[2] = flip(node->tiles[9]);
    t3[3] = flip(node->tiles[13]);
    t3[4] = flip(node->tiles[2]);
    t3[5] = flip(node->tiles[6]);
    t3[6] = flip(node->tiles[10]);
    t3[7] = flip(node->tiles[14]);

    t4[0] = flip(node->tiles[3]);
    t4[1] = flip(node->tiles[7]);
    t4[2] = flip(node->tiles[11]);
    t4[3] = flip(node->tiles[15]);
    t4[4] = flip(node->tiles[4]);
    t4[5] = flip(node->tiles[8]);
    t4[6] = flip(node->tiles[12]);
    t4[7] = flip(node->tiles[0]);


    unsigned long hash1 = getHash(t1, 8);
    unsigned long hash2 = getHash(t2, 8);

    int h1 = pStore1[hash1];
    int h2 = pStore2[hash2];

    unsigned long hash3 = getHash(t3, 8);
    unsigned long hash4 = getHash(t4, 8);

    int h3 = pStore1[hash3];
    int h4 = pStore2[hash4];


    // return highest of four h's
    return max(max(h1,h2), max(h3,h4));
}

// returns max sum of two 8dbs for node and its flip. Admissible.
int h_disjoint16_SumMirror8(struct Node *node){
    char t1[8];
    char t2[7];

    char t3[8];
    char t4[7];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];
    t1[4] = node->tiles[5];
    t1[5] = node->tiles[6];
    t1[6] = node->tiles[7];
    t1[7] = node->tiles[8];

    t2[0] = node->tiles[9];
    t2[1] = node->tiles[10];
    t2[2] = node->tiles[11];
    t2[3] = node->tiles[12];
    t2[4] = node->tiles[13];
    t2[5] = node->tiles[14];
    t2[6] = node->tiles[15];

    t3[0] = flip(node->tiles[1]);
    t3[1] = flip(node->tiles[5]);
    t3[2] = flip(node->tiles[9]);
    t3[3] = flip(node->tiles[13]);
    t3[4] = flip(node->tiles[2]);
    t3[5] = flip(node->tiles[6]);
    t3[6] = flip(node->tiles[10]);
    t3[7] = flip(node->tiles[14]);

    t4[0] = flip(node->tiles[3]);
    t4[1] = flip(node->tiles[7]);
    t4[2] = flip(node->tiles[11]);
    t4[3] = flip(node->tiles[15]);
    t4[4] = flip(node->tiles[4]);
    t4[5] = flip(node->tiles[8]);
    t4[6] = flip(node->tiles[12]);


    unsigned long hash1 = getHash(t1, 8);
    unsigned long hash2 = getHash(t2, 7);

    int h1 = pDisStore1[hash1];
    int h2 = pDisStore2[hash2];

    unsigned long hash3 = getHash(t3, 8);
    unsigned long hash4 = getHash(t4, 7);

    int h3 = pDisStore1[hash3];
    int h4 = pDisStore2[hash4];

    // return highest of either regular or flip hs.
    return max(h1 + h2, h3 + h4);
}

// returns sum of two 8dbs and the sum of two 8dbs mirror imgae
int h_db16_SumMirror8(struct Node *node){
    char t1[8];
    char t2[8];

    char t3[8];
    char t4[8];

    // top left corner
    char t5[7];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[3];
    t1[3] = node->tiles[4];
    t1[4] = node->tiles[5];
    t1[5] = node->tiles[6];
    t1[6] = node->tiles[7];
    t1[7] = node->tiles[8];

    t2[0] = node->tiles[9];
    t2[1] = node->tiles[10];
    t2[2] = node->tiles[11];
    t2[3] = node->tiles[12];
    t2[4] = node->tiles[13];
    t2[5] = node->tiles[14];
    t2[6] = node->tiles[15];
    t2[7] = node->tiles[0];

    t3[0] = flip(node->tiles[1]);
    t3[1] = flip(node->tiles[5]);
    t3[2] = flip(node->tiles[9]);
    t3[3] = flip(node->tiles[13]);
    t3[4] = flip(node->tiles[2]);
    t3[5] = flip(node->tiles[6]);
    t3[6] = flip(node->tiles[10]);
    t3[7] = flip(node->tiles[14]);

    t4[0] = flip(node->tiles[3]);
    t4[1] = flip(node->tiles[7]);
    t4[2] = flip(node->tiles[11]);
    t4[3] = flip(node->tiles[15]);
    t4[4] = flip(node->tiles[4]);
    t4[5] = flip(node->tiles[8]);
    t4[6] = flip(node->tiles[12]);
    t4[7] = flip(node->tiles[0]);

    t5[0] = node->tiles[1];
    t5[1] = node->tiles[2];
    t5[2] = node->tiles[3];
    t5[3] = node->tiles[4];
    t5[4] = node->tiles[5];
    t5[5] = node->tiles[9];
    t5[6] = node->tiles[13];

    unsigned long hash1 = getHash(t1, 8);
    unsigned long hash2 = getHash(t2, 8);

    int h1 = pStore1[hash1];
    int h2 = pStore2[hash2];

    unsigned long hash3 = getHash(t3, 8);
    unsigned long hash4 = getHash(t4, 8);


    int h3 = pStore1[hash3];
    int h4 = pStore2[hash4];


    return max(h1 + h2, h3 + h4);
}

// returns sum of 4 6dbs for 25 puzzle. Not valid
int h_25_Sum6(struct Node *node){
    char t1[6];
    char t2[6];
    char t3[6];
    char t4[6];

    // mirror image
    char t5[6];
    char t6[6];
    char t7[6];
    char t8[6];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[6];
    t1[3] = node->tiles[7];
    t1[4] = node->tiles[11];
    t1[5] = node->tiles[12];

    t2[0] = node->tiles[3];
    t2[1] = node->tiles[4];
    t2[2] = node->tiles[5];
    t2[3] = node->tiles[8];
    t2[4] = node->tiles[9];
    t2[5] = node->tiles[10];

    t3[0] = node->tiles[13];
    t3[1] = node->tiles[14];
    t3[2] = node->tiles[15];
    t3[3] = node->tiles[18];
    t3[4] = node->tiles[19];
    t3[5] = node->tiles[20];

    t4[0] = node->tiles[16];
    t4[1] = node->tiles[17];
    t4[2] = node->tiles[21];
    t4[3] = node->tiles[22];
    t4[4] = node->tiles[23];
    t4[5] = node->tiles[24];

    // mirror image
    t5[0] = flip(node->tiles[1]);
    t5[1] = flip(node->tiles[6]);
    t5[2] = flip(node->tiles[2]);
    t5[3] = flip(node->tiles[7]);
    t5[4] = flip(node->tiles[3]);
    t5[5] = flip(node->tiles[8]);

    t6[0] = flip(node->tiles[11]);
    t6[1] = flip(node->tiles[16]);
    t6[2] = flip(node->tiles[21]);
    t6[3] = flip(node->tiles[12]);
    t6[4] = flip(node->tiles[17]);
    t6[5] = flip(node->tiles[22]);

    t7[0] = flip(node->tiles[13]);
    t7[1] = flip(node->tiles[18]);
    t7[2] = flip(node->tiles[23]);
    t7[3] = flip(node->tiles[14]);
    t7[4] = flip(node->tiles[19]);
    t7[5] = flip(node->tiles[24]);

    t8[0] = flip(node->tiles[4]);
    t8[1] = flip(node->tiles[9]);
    t8[2] = flip(node->tiles[5]);
    t8[3] = flip(node->tiles[10]);
    t8[4] = flip(node->tiles[15]);
    t8[5] = flip(node->tiles[20]);

    unsigned long hash1 = getHash(t1, 6);
    unsigned long hash2 = getHash(t2, 6);
    unsigned long hash3 = getHash(t3, 6);
    unsigned long hash4 = getHash(t4, 6);

    // mirror
    unsigned long hash5 = getHash(t5, 6);
    unsigned long hash6 = getHash(t6, 6);
    unsigned long hash7 = getHash(t7, 6);
    unsigned long hash8 = getHash(t8, 6);

    int h1 = pStore1[hash1];
    int h2 = pStore2[hash2];
    int h3 = pStore3[hash3];
    int h4 = pStore4[hash4];

    // mirror
    int h5 = pStore1[hash5];
    int h6 = pStore2[hash6];
    int h7 = pStore3[hash7];
    int h8 = pStore4[hash8];

    return max(h1 + h2 + h3 + h4, h5 + h6 + h7 + h8);
}

// returns best of 6dbs for 25 puzzle. Valid. Gets shortest path
float h_25_Weighted6(struct Node *node){
    char t1[6];
    char t2[6];
    char t3[6];
    char t4[6];

    // mirror image
    char t5[6];
    char t6[6];
    char t7[6];
    char t8[6];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[6];
    t1[3] = node->tiles[7];
    t1[4] = node->tiles[11];
    t1[5] = node->tiles[12];

    t2[0] = node->tiles[3];
    t2[1] = node->tiles[4];
    t2[2] = node->tiles[5];
    t2[3] = node->tiles[8];
    t2[4] = node->tiles[9];
    t2[5] = node->tiles[10];

    t3[0] = node->tiles[13];
    t3[1] = node->tiles[14];
    t3[2] = node->tiles[15];
    t3[3] = node->tiles[18];
    t3[4] = node->tiles[19];
    t3[5] = node->tiles[20];

    t4[0] = node->tiles[16];
    t4[1] = node->tiles[17];
    t4[2] = node->tiles[21];
    t4[3] = node->tiles[22];
    t4[4] = node->tiles[23];
    t4[5] = node->tiles[24];

    // mirror image
    t5[0] = flip(node->tiles[1]);
    t5[1] = flip(node->tiles[6]);
    t5[2] = flip(node->tiles[2]);
    t5[3] = flip(node->tiles[7]);
    t5[4] = flip(node->tiles[3]);
    t5[5] = flip(node->tiles[8]);

    t6[0] = flip(node->tiles[11]);
    t6[1] = flip(node->tiles[16]);
    t6[2] = flip(node->tiles[21]);
    t6[3] = flip(node->tiles[12]);
    t6[4] = flip(node->tiles[17]);
    t6[5] = flip(node->tiles[22]);

    t7[0] = flip(node->tiles[13]);
    t7[1] = flip(node->tiles[18]);
    t7[2] = flip(node->tiles[23]);
    t7[3] = flip(node->tiles[14]);
    t7[4] = flip(node->tiles[19]);
    t7[5] = flip(node->tiles[24]);

    t8[0] = flip(node->tiles[4]);
    t8[1] = flip(node->tiles[9]);
    t8[2] = flip(node->tiles[5]);
    t8[3] = flip(node->tiles[10]);
    t8[4] = flip(node->tiles[15]);
    t8[5] = flip(node->tiles[20]);

    unsigned long hash1 = getHash(t1, 6);
    unsigned long hash2 = getHash(t2, 6);
    unsigned long hash3 = getHash(t3, 6);
    unsigned long hash4 = getHash(t4, 6);

    // mirror
    unsigned long hash5 = getHash(t5, 6);
    unsigned long hash6 = getHash(t6, 6);
    unsigned long hash7 = getHash(t7, 6);
    unsigned long hash8 = getHash(t8, 6);

    int h1 = pStore1[hash1];
    int h2 = pStore2[hash2];
    int h3 = pStore3[hash3];
    int h4 = pStore4[hash4];

    // mirror
    int h5 = pStore1[hash5];
    int h6 = pStore2[hash6];
    int h7 = pStore3[hash7];
    int h8 = pStore4[hash8];

    float best = max(h1 + h2 + h3 + h4, h5 + h6 + h7 + h8);

    return best * 2;
}

// returns sum of highest two of four 6dbs for 25 puzzle. Not valid.
int h_25_Special6(struct Node *node){
    char t1[6];
    char t2[6];
    char t3[6];
    char t4[6];

    // mirror image
    char t5[6];
    char t6[6];
    char t7[6];
    char t8[6];

    t1[0] = node->tiles[1];
    t1[1] = node->tiles[2];
    t1[2] = node->tiles[6];
    t1[3] = node->tiles[7];
    t1[4] = node->tiles[11];
    t1[5] = node->tiles[12];

    t2[0] = node->tiles[3];
    t2[1] = node->tiles[4];
    t2[2] = node->tiles[5];
    t2[3] = node->tiles[8];
    t2[4] = node->tiles[9];
    t2[5] = node->tiles[10];

    t3[0] = node->tiles[13];
    t3[1] = node->tiles[14];
    t3[2] = node->tiles[15];
    t3[3] = node->tiles[18];
    t3[4] = node->tiles[19];
    t3[5] = node->tiles[20];

    t4[0] = node->tiles[16];
    t4[1] = node->tiles[17];
    t4[2] = node->tiles[21];
    t4[3] = node->tiles[22];
    t4[4] = node->tiles[23];
    t4[5] = node->tiles[24];

    // mirror image
    t5[0] = flip(node->tiles[1]);
    t5[1] = flip(node->tiles[6]);
    t5[2] = flip(node->tiles[2]);
    t5[3] = flip(node->tiles[7]);
    t5[4] = flip(node->tiles[3]);
    t5[5] = flip(node->tiles[8]);

    t6[0] = flip(node->tiles[11]);
    t6[1] = flip(node->tiles[16]);
    t6[2] = flip(node->tiles[21]);
    t6[3] = flip(node->tiles[12]);
    t6[4] = flip(node->tiles[17]);
    t6[5] = flip(node->tiles[22]);

    t7[0] = flip(node->tiles[13]);
    t7[1] = flip(node->tiles[18]);
    t7[2] = flip(node->tiles[23]);
    t7[3] = flip(node->tiles[14]);
    t7[4] = flip(node->tiles[19]);
    t7[5] = flip(node->tiles[24]);

    t8[0] = flip(node->tiles[4]);
    t8[1] = flip(node->tiles[9]);
    t8[2] = flip(node->tiles[5]);
    t8[3] = flip(node->tiles[10]);
    t8[4] = flip(node->tiles[15]);
    t8[5] = flip(node->tiles[20]);

    unsigned long hash1 = getHash(t1, 6);
    unsigned long hash2 = getHash(t2, 6);
    unsigned long hash3 = getHash(t3, 6);
    unsigned long hash4 = getHash(t4, 6);

    // mirror
    unsigned long hash5 = getHash(t5, 6);
    unsigned long hash6 = getHash(t6, 6);
    unsigned long hash7 = getHash(t7, 6);
    unsigned long hash8 = getHash(t8, 6);

    int h1 = pStore1[hash1];
    int h2 = pStore2[hash2];
    int h3 = pStore3[hash3];
    int h4 = pStore4[hash4];

    // mirror
    int h5 = pStore1[hash5];
    int h6 = pStore2[hash6];
    int h7 = pStore3[hash7];
    int h8 = pStore4[hash8];

    int regularH = h1 + h2 + h3 + h4;
    int mirrorH = h5 + h6 + h7 + h8;

    if(regularH > mirrorH){
        return regularH + mirrorH / 2;
    } else {
        return mirrorH + regularH / 2;
    }
}

// chooses which dbs to load into memory
void loadDB(int number){

    if(NUM_TILES == 9){
        char *pDBStore = malloc(200000000);
        pStore1 = pDBStore;
        loadData("all9.txt", pStore1);
    }
    if(NUM_TILES == 16){
        if(number == 4){
            char *pDBStore4 = malloc(180000);
            pStore4_1 = pDBStore4;
            pStore4_2 = pDBStore4 + 45000;
            pStore4_3 = pDBStore4 + 2 * 45000;
            pStore4_4 = pDBStore4 + 3 * 45000;
            loadData("db4by4_4_1.txt", pStore4_1);
            loadData("db4by4_4_2.txt", pStore4_2);
            loadData("db4by4_4_3.txt", pStore4_3);
            loadData("db4by4_4_4.txt", pStore4_4);
        } else if(number == 8){
//            char *pDBStore = malloc(1100000000);
//            pStore1 = pDBStore;
//            pStore2 = pDBStore + 550000000;
//            loadData("db4by4_8_top.txt", pStore1);
//            loadData("db4by4_8_bottom.txt", pStore2);
//

            char *pDisjintStore = malloc(700000000);
            pDisStore1 = pDisjintStore;
            pDisStore2 = pDisjintStore + 550000000;
            loadData("db16_disjoint_8_top.txt", pDisStore1);
            loadData("db16_disjoint_8_bottom.txt", pDisStore2);
        }
    } else if(NUM_TILES == 25){
        if(number == 6){
            char *pDBStore = malloc(600000000);
            pStore1 = pDBStore;
            pStore2 = pDBStore + 150000000;
            pStore3 = pDBStore + 2 * 150000000;
            pStore4 = pDBStore + 3 * 150000000;
            loadData("dj_25_6_1", pStore1);
            loadData("dj_25_6_2", pStore2);
            loadData("dj_25_6_3", pStore3);
            loadData("dj_25_6_4", pStore4);
        }
    }
}

// loads dbs into memory
void loadData(char *filename, char *pStore) {
    FILE *file;
    file = fopen(filename, "r");

    int i;
    long place = 0;
    fscanf(file, "%d", &i);
    while (!feof (file))
    {
        pStore[place] = (char)i;
        place++;
        fscanf(file, "%d", &i);
    }
    fclose(file);
}

// one-to-one hash of state
unsigned long getHash(char *state, int numberTiles){
    unsigned long hash = 0;

    for (int i = 0; i < numberTiles; ++i) {
        unsigned long number = state[i] - 1;
        for (int j = 0; j < i; ++j) {
            if(state[i] > state[j]){
                number--;
            }
        }
        for (int k = i; k < numberTiles - 1; ++k) {
            number *= (NUM_TILES - k - 1);
        }
        hash += number;
    }
    return hash;
}

// returns mirror image of position on x = -y axis
char flip(int pos){
    if(NUM_TILES == 16){
        switch(pos){
            case 1: return 1;
            case 2: return 5;
            case 3: return 9;
            case 4: return 13;
            case 5: return 2;
            case 6: return 6;
            case 7: return 10;
            case 8: return 14;
            case 9: return 3;
            case 10: return 7;
            case 11: return 11;
            case 12: return 15;
            case 13: return 4;
            case 14: return 8;
            case 15: return 12;
            case 16: return 16;
        }
    }

    if(NUM_TILES == 25){
        switch(pos){
            case 1: return 1;
            case 2: return 6;
            case 3: return 11;
            case 4: return 16;
            case 5: return 21;
            case 6: return 2;
            case 7: return 7;
            case 8: return 12;
            case 9: return 17;
            case 10: return 22;
            case 11: return 3;
            case 12: return 8;
            case 13: return 13;
            case 14: return 18;
            case 15: return 23;
            case 16: return 4;
            case 17: return 9;
            case 18: return 14;
            case 19: return 19;
            case 20: return 24;
            case 21: return 5;
            case 22: return 10;
            case 23: return 15;
            case 24: return 20;
            case 25: return 25;
        }
    }
}

