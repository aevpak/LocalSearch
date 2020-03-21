// main_first.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <list>
#include <iterator>
#include "header.h"
using namespace std;
#define PICKING 0
#define PACKING 1
#define BUF 2
#define OUT 3
#define IN 4
#define MAXTIME 1000000
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
typedef struct Event {
	int type;//0 input 1 picking 2 buffer 3 packing 4 output 5 blockedAtPicking 6 blockedAtBuf 7 null
	string name;
	int time;
} Event;
int FAI(Job j[], int N) {//First available index
	int ind = -1;
	for (int i = 0; i < N; i++) {
		if (j[i].name == "null") {
			ind = i;
			break;
		}
	}
	return ind;
}
int FAIEvent(Event j[], int N) {//First available index in EventList
	int ind = -1;
	for (int i = 0; i < N; i++) {
		if (j[i].type == 7) {
			ind = i;
			break;
		}
	}
	return ind;
}
void shiftLeft(Job j[], int N) {
	Job null = { "null", 0, 0, 0, 0, 0, 0 };//нулевая работа
	int iter = 0;
	while (j[0].name == "null") {
		if (iter > N) break;//костыль на избежание зацикливания
		for (int i = 1; i < N; i++) {
			j[i - 1] = j[i];
		}
		j[N - 1] = null;
		iter++;
	}
}
void printNames(Job j[], int N) {
	for (int i = 0; i < N; i++) {
		cout << j[i].name << ' ';
	}
	cout << endl;
}
void printNamesEventList(Event j[], int N) {
	for (int i = 0; i < N; i++) {
		cout << j[i].name << ' ';
	}
	cout << endl;
}
void eventListBubbleSort(Event eventList[], int N) {
	for (int i = N - 1; i >= 0; i--) {
		for (int j = 0; j < i; j++) {
			if (eventList[j].time > eventList[j + 1].time) {
				Event e = eventList[j];
				eventList[j] = eventList[j + 1];
				eventList[j + 1] = e;
			}
		}
	}
}
int findIndexByName(Job j[], int N, string id) {
	int ind = -1;
	for (int i = 0; i < N; i++) {
		if (j[i].name == id) {
			ind = i;
			break;
		}
	}
	return ind;
}
int findIndexByNameInEventList(Event j[], int N, string id) {
	int ind = -1;
	for (int i = 0; i < N; i++) {
		if (j[i].name == id) {
			ind = i;
			break;
		}
	}
	return ind;
}
void expandBufTime(Job j[], int N, int t) {
	for (int i = 0; i < N; i++) {
		if (j[i].name != "null") {
			j[i].allTime += t;
		}
	}
}
int findAvailableJob(Job j[], int N) {
	int ind = -1;
	for (int i = 0; i < N; i++) {
		if (j[i].name != "null") {
			ind = i;
			break;
		}
	}
	return ind;
}

void doEvent(Event currentEvent, Event eventList[], Job input[], Job picking[], Job packing[], Job buf[], Job output[], int pickingSize, int packingSize, int bufSize, int countJob) {
	Job null = { "null", 0, 0, 0, 0, 0, 0, MAXTIME, false };//нулевая работа
	int indToDelete = findIndexByNameInEventList(eventList, countJob, currentEvent.name);//хотим занулить текущий эвент

	eventList[indToDelete].time = MAXTIME;
	eventList[indToDelete].name = "null";
	eventList[indToDelete].type = 7;
	int ind;
	int indPackingEmpty;
	int indBufEmpty;
	int indPickingEmpty;
	int packingInd;
	int extraTime;
	int bufInd;
	int outInd;
	int availableBufJob;
	int availablePickingJob;
	bool otladka = false;
	switch (currentEvent.type)
	{
	case 1://picking
		ind = findIndexByName(picking, pickingSize, currentEvent.name);//ищем индекс на пикинге задачи из эвента
		//cout << "OTLADKA PICKINGA  " << currentEvent.name << " " <<  picking[0].name << " " << picking[1].name << endl;
		bufInd = FAI(buf, bufSize);//ищем свободное место в буфере
		extraTime = currentEvent.time;//добавочное время с текущего эвента
		if (bufInd != -1) {//в буфере есть место
						 
			/*тут был маркер*/buf[bufInd] = picking[ind];//в буфер сложить задачу из эвента
			buf[bufInd].allTime = extraTime;///тут обратить внимание, возможно время не то
			if (otladka) cout << buf[bufInd].name << " otpravilas v bufer" << endl;
			picking[ind] = null;//освободили место на пикинге///!!!!!!!ТУТ ОШИБКАААА??????????????????????
		}
		else {//заблокирован буфер
			currentEvent.type = 5;
			picking[ind].blocked = true;
			if (otladka) cout << picking[ind].name << " ne smogla poyti v bufer tak kak on polon" << endl;
		}
		eventListBubbleSort(eventList, countJob);//в конце сортируем эвентлист
												 //это ряд действий
												 //для буфера:
		availableBufJob = findAvailableJob(buf, bufSize);
		indPackingEmpty = FAI(packing, packingSize);

		while (indPackingEmpty != -1 && availableBufJob != -1) {//это для буфера
			int helpTime = MAXTIME;//ищем самую быструю на packing
			int helpInd = -1;
			for (int i = 0; i < bufSize; i++) {
				//cout << buf[i].timeForPack << " TEST!!! " << buf[i].name << endl;
				if (buf[i].timeForPack < helpTime && buf[i].name != "null") {
					helpTime = buf[i].timeForPack;
					helpInd = i;
				}
			}
			if (helpInd != -1) {
				packing[indPackingEmpty] = buf[helpInd];
				buf[helpInd] = null;
				int eventInd = FAIEvent(eventList, countJob);//найти пустой эвент в листе
				eventList[eventInd].name = packing[indPackingEmpty].name;//заполнить его задачей с пэкинга
				eventList[eventInd].type = 3;
				eventList[eventInd].time = packing[indPackingEmpty].timeForPack + extraTime;
				if (otladka) cout << packing[indPackingEmpty].name << " poshla na packing iz bufera posle ojidaniya" << endl;
				indPackingEmpty = FAI(packing, packingSize);
				availableBufJob = findAvailableJob(buf, bufSize);
				///а что тут со статусом blocked???
			}
			else { break; }
		}
		//это для пикинга
		availablePickingJob = findAvailableJob(picking, pickingSize);
		indBufEmpty = FAI(buf, bufSize);
		
		while (availablePickingJob != -1 && indBufEmpty != -1) {//это для пикинга
			int helpTime = MAXTIME;//ищем самую быструю на packing
			int helpInd = -1;
			for (int i = 0; i < pickingSize/*bufSize*/; i++) {
				if (picking[i].timeForPack < helpTime && picking[i].name != "null" && picking[i].blocked == true /*&& picking[i].startTime + picking[i].timeForPick <= extraTime*/) {
					helpTime = picking[i].timeForPack;
					helpInd = i;
				}
			}
			if (helpInd != -1) {
				//buf[indPackingEmpty] = picking[helpInd];
				buf[indBufEmpty] = picking[helpInd];
				picking[helpInd] = null;
				///это тут вроде не нужноint eventInd = FAIEvent(eventList, countJob);//найти пустой эвент в листе
				//eventList[eventInd].name = buf[indPackingEmpty].name;//заполнить его задачей с пэкинга
				//eventList[eventInd].type = 3;
				//eventList[eventInd].time = packing[indPackingEmpty].timeForPack + extraTime;
				if (otladka) cout << buf[indBufEmpty].name << " poshla v bufer iz pickinga posle ojidaniya" << endl;
				availablePickingJob = findAvailableJob(picking, pickingSize);
				indBufEmpty = FAI(buf, bufSize);
				///а что тут со статусом blocked???
			}
			else { break; }
		}
		//новые задачи на picking
		indPickingEmpty = FAI(picking, pickingSize);
		while (indPickingEmpty != -1 && input[0].name != "null") {
			//cout << input[0].name << " OTLADKA" << endl;
			picking[indPickingEmpty] = input[0];
			picking[indPickingEmpty].startTime = extraTime;
			picking[indPickingEmpty].allTime = 0;
			if (otladka) cout << input[0].name << " prishla iz input na picking" << endl;
			int eventInd = FAIEvent(eventList, countJob);
			eventList[eventInd].time = picking[indPickingEmpty].timeForPick;
			eventList[eventInd].type = 1;
			eventList[eventInd].name = picking[indPickingEmpty].name;
			//input[indPickingEmpty] = null;
			input[0] = null;
			indPickingEmpty = FAI(picking, pickingSize);
			shiftLeft(input, countJob);
			//cout << input[0].name << " OTLADKA POSLE SDVIGA" << endl;
			eventListBubbleSort(eventList, countJob);
		}
		break;
		
	case 3://если эвент из пэкинга
		extraTime = currentEvent.time;//добавочное время с текущего эвента
		outInd = FAI(output, countJob);
		packingInd = findIndexByName(packing, packingSize, currentEvent.name);
		output[outInd] = packing[packingInd];//пихаем работу в выполненые
		output[outInd].allTime = extraTime;
		//expandBufTime(buf, bufSize, output[outInd].timeForPack); вот насчет этого подумать надо
		eventListBubbleSort(eventList, countJob);//в конце сортируем эвентлист
		packing[packingInd] = null;
		if (otladka) cout << output[outInd].name << " ushla iz packing v output" << endl;
		availableBufJob = findAvailableJob(buf, bufSize);
		indPackingEmpty = FAI(packing, packingSize);
		//cout << availableBufJob << " super test!! " << indPackingEmpty << endl;
		//printNames(packing, packingSize);

		while (indPackingEmpty != -1 && availableBufJob != -1) {//это для буфера
			//printNames(buf, bufSize);
			int helpTime = MAXTIME;//ищем самую быструю на packing
			int helpInd = -1;
			for (int i = 0; i < bufSize; i++) {
				if (buf[i].timeForPack < helpTime && buf[i].name != "null") {
					helpTime = buf[i].timeForPack;
					helpInd = i;
				}
			}
			//cout << helpInd << " super test!! " << helpTime << endl;
			if (helpInd != -1) {
				packing[indPackingEmpty] = buf[helpInd];
				buf[helpInd] = null;
				int eventInd = FAIEvent(eventList, countJob);//найти пустой эвент в листе
				eventList[eventInd].name = packing[indPackingEmpty].name;//заполнить его задачей с пэкинга
				eventList[eventInd].type = 3;
				eventList[eventInd].time = packing[indPackingEmpty].timeForPack + extraTime;
				if (otladka) cout << packing[indPackingEmpty].name << " poshla na packing iz bufera posle ojidaniya" << endl;
				indPackingEmpty = FAI(packing, packingSize);
				availableBufJob = findAvailableJob(buf, bufSize);
				///а что тут со статусом blocked???
			}
			else { break; }
		}
		//это для пикинга
		availablePickingJob = findAvailableJob(picking, pickingSize);
		indBufEmpty = FAI(buf, bufSize);
		while (availablePickingJob != -1 && indBufEmpty != -1) {//это для пикинга
			int helpTime = MAXTIME;//ищем самую быструю на packing
			int helpInd = -1;
			for (int i = 0; i < pickingSize/*bufSize*/; i++) { 
				if (picking[i].timeForPack < helpTime  && picking[i].name != "null" && picking[i].blocked == true /*&& picking[i].startTime + picking[i].timeForPick <= extraTime*/) {
					helpTime = picking[i].timeForPack;
					helpInd = i;
				}
			}
			if (helpInd != -1) {
				//buf[indPackingEmpty] = picking[helpInd];
				buf[indBufEmpty] = picking[helpInd];
				picking[helpInd] = null;
				///это тут вроде не нужноint eventInd = FAIEvent(eventList, countJob);//найти пустой эвент в листе
				//eventList[eventInd].name = buf[indPackingEmpty].name;//заполнить его задачей с пэкинга
				//eventList[eventInd].type = 3;
				//eventList[eventInd].time = packing[indPackingEmpty].timeForPack + extraTime;
				if (otladka) cout << buf[indBufEmpty].name << " poshla v bufer iz pickinga posle ojidaniya" << endl;
				availablePickingJob = findAvailableJob(picking, pickingSize);
				indBufEmpty = FAI(buf, bufSize);
				///а что тут со статусом blocked???
			}
			else { break; }
		}
		//новые задачи на picking
		indPickingEmpty = FAI(picking, pickingSize);
		while (indPickingEmpty != -1 && input[0].name != "null") {
			//cout << input[0].name << " OTLADKA" << endl;
			picking[indPickingEmpty] = input[0];
			picking[indPickingEmpty].startTime = 0;
			picking[indPickingEmpty].allTime = 0;
			if (otladka) cout << input[0].name << " prishla iz input na picking" << endl;
			int eventInd = FAIEvent(eventList, countJob);
			eventList[eventInd].time = picking[indPickingEmpty].timeForPick;
			eventList[eventInd].type = 1;
			eventList[eventInd].name = picking[indPickingEmpty].name;
			input[0] = null;
			indPickingEmpty = FAI(picking, pickingSize);
			shiftLeft(input, countJob);
			//cout << input[0].name << " OTLADKA POSLE SDVIGA" << endl;
			eventListBubbleSort(eventList, countJob);
		}
		break;
	default:
		cout << currentEvent.type << " something goes wrong!\n";
		if (otladka) printNamesEventList(eventList, countJob);
		if (otladka) printNames(picking, pickingSize);
		if (otladka) printNames(buf, bufSize);
		break;
	}
}
int calcFunc(Job input1[], int bufSize, int pickingSize, int packingSize, int countJob)
{
	Job * input = new Job[countJob];
	for (int i = 0; i < countJob; i++) {
		input[i] = input1[i];
	}
	//cout << endl << endl;
	//printNames(input, countJob);
	//cout << endl << endl;
	//cout << bufSize << ' ' << pickingSize << ' ' << packingSize << ' ' << countJob << endl;
	//int bufSize = 2;//размер буфера		
	//int pickingSize = 2;//количество машин на пикинге
	//int packingSize = 2;//количество машин на пэкинге
	//int countJob = 3;//количество работ
	int time = 0;//общее время выполнения
	Event *eventList = new Event[countJob];//эвент лист
	//Job *input = new Job[countJob];//входная последовательность
	Job *picking = new Job[pickingSize];//места под работы на пикинге	
	Job *packing = new Job[packingSize];//места под работы на пэкинге
										//Job j =  { "123", 4, 5, 6, 7, 8, 9 };
										//input[0] = j;
	Job *buf = new Job[bufSize];//места под работы в буфере
	Job *output = new Job[countJob];//выходная последовательность
	Job null = { "null", 0, 0, 0, 0, 0, 0 };//нулевая работа
	for (int i = 0; i < pickingSize; i++) {
		picking[i] = null;
	}
	for (int i = 0; i < packingSize; i++) {
		packing[i] = null;
	}
	for (int i = 0; i < countJob; i++) {
		output[i] = null;
		eventList[i].name = "null";
		eventList[i].time = MAXTIME;
		eventList[i].type = 7;
	}
	for (int i = 0; i < bufSize; i++) {
		buf[i] = null;
	}
	/*
	string name;
	int type;
	int count;
	int timeInBuf;
	int timeForPick;
	int timeForPack;
	int startTime;
	int allTime;
	bool blocked; */
	//Job j = { "123", 4, 1, 0, 6, 77, 0, 0, false };
	//input[0] = j;
	//j = { "234", 4, 1, 0, 6, 77, 0, 0, false };
	//input[1] = j;
	//j = { "345", 4, 1, 0, 6, 77, 0, 0, false };
	//input[2] = j;
	/*j = { "234", 4, 5, 6, 7, 8, 9 };
	input[1] = j;
	j = { "345", 4, 5, 6, 777, 8, 9 };
	input[2] = j;*/
	//в проге косяк с присвоением времени на событиях
	//и нет обработки состояний блокд

	//отсюда начинается программа
	for (int i = 0; i < pickingSize && i < countJob; i++) {
		picking[i] = input[i];
		picking[i].startTime = 0;
		picking[i].allTime = 0;
		eventList[i].time = picking[i].timeForPick;
		eventList[i].type = 1;
		eventList[i].name = picking[i].name;
		input[i] = null;
	}
	//cout << endl << endl;
	//printNames(input, countJob);
	//cout << endl << endl;
	shiftLeft(input, countJob);
	//printNames(input, countJob);
	eventListBubbleSort(eventList, countJob);
	//printNamesEventList(eventList, countJob);
	while (output[countJob - 1].name == "null") {
		Event currentEvent = eventList[0];
		doEvent(currentEvent, eventList, input, picking, packing, buf, output, pickingSize, packingSize, bufSize, countJob);
	}
	//cout << output[countJob - 1].allTime;
	//system("pause");
	return output[countJob - 1].allTime;
}

