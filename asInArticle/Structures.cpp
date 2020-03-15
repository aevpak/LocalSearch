#include "stdafx.h"
#include <string>
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