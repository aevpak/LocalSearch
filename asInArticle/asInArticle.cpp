// main_first.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <list>
#include "header.h"
#include "CalcFunc.h"
#include "FileRead.h"
#include <fstream>
#include <sstream>
using namespace std;
int main()
{
	int bufSize = 3;//размер буфера		
	int pickingSize = 7;//количество машин на пикинге
	int packingSize = 7;//количество машин на пэкинге
	int countJob = 30;//количество работ
	int i = 0;
	Job *input = new Job[countJob];//входная последовательность
	string line;
	ifstream in("hello.txt"); // окрываем файл для чтения
	if (in.is_open())
	{
		while (getline(in, line))
		{	
			if (line[0] != '/') {
				istringstream ist(line);
				int j = 0;
				string * inLine = new string[5];
				for (std::string word; ist >> word; j++) {
					inLine[j] = word;
				};
				input[i].name = inLine[0];
				input[i].type = stoi(inLine[1]);
				input[i].count = stoi(inLine[2]);
				input[i].timeForPick = stoi(inLine[3]);
				input[i].timeForPack = stoi(inLine[4]);
				input[i].timeInBuf = 0;
				input[i].allTime = 0;
				input[i].blocked = false;
				input[i].startTime = 0;
				i++;
			}
		}
	}
	in.close();     // закрываем файл
	cout << "End of program" << std::endl;
	//printNames(input, countJob);
	Job * start = new Job[countJob];//стартовая последовательность для поиска
	Job * usingInput = new Job[countJob];
	
	for (int i = 0; i < countJob; i++) {
		for (int j = 0; j < countJob; j++) {
			if (i != j) {
				for (int k = 0; k < countJob; k++) {
					usingInput[k] = input[k];
				}
				Job swap = usingInput[i];
				usingInput[i] = usingInput[j];
				usingInput[j] = swap;
				printNames(usingInput, countJob);
				cout << calcFunc(usingInput, bufSize, pickingSize, packingSize, countJob) << endl << endl;
			}
		}
	}
	//cout << calcFunc(input, bufSize, pickingSize, packingSize, countJob);
	system("pause");
	return 0;
}

