//
// Created by nicod on 24/01/2018.
//

#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include "Utils.h"
#include "ida.h"
#include "rbfs.h"
#include "subSearch.h"

enum SEARCH_TYPE {IDA_STAR, RBFS};

static enum SEARCH_TYPE searchType;

void beginProgram();

int getInput();

void testSet(int number);

struct Results searchOneRandom(double);

struct Results searchOneSet(int number);

void testRandom(int number);

void printH();

void createTrainingSet();

void createStartingTrainingSet();


#endif //PROGRAM_H
