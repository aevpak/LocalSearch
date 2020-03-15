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
	int bufSize = 2;//размер буфера		
	int pickingSize = 2;//количество машин на пикинге
	int packingSize = 2;//количество машин на пэкинге
	int countJob = 3;//количество работ
	int i = 0;
	Job *input = new Job[countJob];//входная последовательность
	string line;
	ifstream in("hello.txt"); // окрываем файл для чтения
	if (in.is_open())
	{
		while (getline(in, line))
		{				
			istringstream ist(line);
			int j = 0;
			string * inLine = new string[5];
			for (std::string word; ist >> word; j++) { inLine[j] = word; };
			input[i].name = inLine[0];
			input[i].type = stoi(inLine[1]);
			input[i].count = stoi(inLine[2]);
			input[i].timeForPick = stoi(inLine[3]);
			input[i].timeForPack = stoi(inLine[4]);
			input[i].timeInBuf = 0;
			input[i].allTime = 0;
			input[i].blocked = false;
			input[i].startTime = 0;
		}
	}
	in.close();     // закрываем файл
	cout << "End of program" << std::endl;
	//cout << calcFunc();
	system("pause");
	return 0;
}

