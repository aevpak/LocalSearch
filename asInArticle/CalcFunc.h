#pragma second
#ifndef CalcFunc
#define CalcFunc
#include "stdafx.h"
#include <string>
using namespace std;
typedef struct Job {
	string name;
	int type;
	int count;
	int timeInBuf;
	int timeForPick;
	int timeForPack;
	/*int timeAtPick;
	int timeAtPack;*/
	int startTime;
	int allTime;
	bool blocked;
} Job;
int calcFunc(Job input[], int bufSize, int pickingSize, int packingSize, int countJob);
void printNames(Job j[], int N);
#endif