// main_first.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	Job null = { "null", 0, 0, 0, 0, 0, 0 };//������� ������
	int iter = 0;
	while (j[0].name == "null") {
		if (iter > N) break;//������� �� ��������� ������������
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
void doSomething(Event currentEvent, Event eventList[], Job input[], Job picking[], Job packing[], Job buf[], Job output[], int pickingSize, int packingSize, int bufSize, int countJob) {
	Job null = { "null", 0, 0, 0, 0, 0, 0, MAXTIME, false };//������� ������
	int indToDelete = findIndexByNameInEventList(eventList, countJob, currentEvent.name);//����� �������� ������� �����
	eventList[indToDelete].time = MAXTIME;
	eventList[indToDelete].name = "null";
	eventList[indToDelete].type = 7;
	int ind;
	int indPackingEmpty;
	int indBufEmpty;
	int packingInd;
	int extraTime;
	int bufInd;
	int outInd;
	switch (currentEvent.type)
	{
	case 1://picking
		ind = findIndexByName(picking, pickingSize, currentEvent.name);//���� ������ �� ������� ������ �� ������
		bufInd = FAI(buf, bufSize);//���� ��������� ����� � ������
		extraTime = currentEvent.time;//���������� ����� � �������� ������
		if (bufInd != -1) {//� ������ ���� �����
			packingInd = FAI(packing, packingSize);
			if (packingInd != -1) {//�� ������� ���� �����
				packing[packingInd] = picking[ind];//��������� ������ � ������� �� ������
				picking[ind] = null;
				shiftLeft(input, countJob);
				packing[packingInd].allTime += packing[packingInd].timeForPick;//�������� ������ ����� �� ������ � ������ �������
				int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				eventList[eventInd].name = packing[packingInd].name;//��������� ��� ������� � �������
				eventList[eventInd].type = 3;
				eventList[eventInd].time = extraTime + packing[packingInd].timeForPack;
				cout << packing[packingInd].name << " otpravilas na packing" << endl;
				if (input[0].name != "null") {
					picking[ind] = input[0];//�� �������������� ����� picking ����� ������
					input[0] = null;
					shiftLeft(input, countJob);//�������� �����	
					picking[ind].startTime = extraTime;//����� ������ - ����� �������� ������
					eventInd = FAIEvent(eventList, countJob);//���� ����� � ����������
					eventList[eventInd].time = picking[ind].timeForPick + extraTime;//������ �� ��� ����� ������ �� �������
					eventList[eventInd].type = 1;
					eventList[eventInd].name = picking[ind].name;
					cout << picking[ind].name << " prishla iz ocheredi na picking" << endl;
				}
				eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
			}
			else {
				buf[bufInd] = picking[ind];//� ����� ������� ������ �� ������
				int eventInd = FAIEvent(eventList, countJob);
				eventList[eventInd].time = extraTime;
				eventList[eventInd].type = 2;
				eventList[eventInd].name = picking[ind].name;
				cout << picking[ind].name << " otpravilas v bufer" << endl;
				if (input[0].name != "null") {
					picking[ind] = input[0];//�� �������������� ����� ����� ������
					shiftLeft(input, countJob);
					picking[ind].startTime = extraTime + picking[ind].timeForPick;
					picking[ind].allTime = extraTime;
					eventInd = FAIEvent(eventList, countJob);
					eventList[eventInd].time = picking[ind].timeForPick + extraTime;
					eventList[eventInd].type = 1;
					eventList[eventInd].name = picking[ind].name;
					cout << picking[ind].name << " prishla na piking" << endl;
				}
				eventListBubbleSort(eventList, countJob);
			}
		}
		else {//������������ �����
			currentEvent.type = 5;
			picking[ind].blocked = true;
			int eventInd = FAIEvent(eventList, countJob);
			eventList[eventInd] = currentEvent;//������� ����� ����� ����������� � ���������
			cout << picking[ind].name << " ne smogla poyti v bufer tak kak on polon" << endl;
		}
		//��� ����� �������� �� ���� ��������������� ������� � ���� �� ���
		indPackingEmpty = FAI(packing, packingSize);//������!!!!!
		while (indPackingEmpty != -1) {//��� ��� ������
			int helpTime = MAXTIME;//���� ����� ������� �� packing
			int helpInd = -1;
			for (int i = 0; i < bufSize; i++) {
				if (buf[i].blocked) {
					if (buf[i].timeForPack < helpTime) {
						helpTime = buf[i].timeForPack;
						helpInd = i;
					}
				}
			}
			if (helpInd != -1) {
				packing[indPackingEmpty] = buf[helpInd];
				buf[helpInd] = null;
				packing[indPackingEmpty].allTime += packing[indPackingEmpty].timeForPack;//�������� ������ ����� �� ������ � ������ �������
				int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				eventList[eventInd].name = packing[indPackingEmpty].name;//��������� ��� ������� � �������
				eventList[eventInd].type = 3;
				eventList[eventInd].time = packing[indPackingEmpty].allTime;
				cout << packing[indPackingEmpty].name << " poshla na packing iz bufera posle razblokirovki" << endl;
				indPackingEmpty = FAI(packing, packingSize);
			}
			else { break; }
		}
		//������ �� �� ����� ��� ������
		indBufEmpty = FAI(buf, bufSize);
		while (indBufEmpty != -1) {//���� ���� ����� � ������
			int helpTime = MAXTIME;//���� ����� ������� �� packing
			int helpInd = -1;
			for (int i = 0; i < pickingSize; i++) {
				if (picking[i].blocked) {
					if (picking[i].timeForPack < helpTime) {
						helpTime = picking[i].timeForPack;
						helpInd = i;
					}
				}
			}
			if (helpInd != -1) {
				buf[indBufEmpty] = picking[helpInd];
				picking[helpInd] = null;
				//��� ��� ����� ������!!! �� ���� ���� ���� ��������� allTIme �� �������� ������� �������� ������
				int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				eventList[eventInd].name = buf[indBufEmpty].name;//��������� ��� ������� � ������
				eventList[eventInd].type = 2;
				eventList[eventInd].time = buf[indBufEmpty].allTime;
				indBufEmpty = FAI(packing, packingSize);
				cout << buf[indBufEmpty].name << " poshla v bufer posle razblokirovki" << endl;
			}
			else { break; }
		}
		///��� �������� �������� ��������� ������� ������ �� ������, �� ������� � ���� ������ ������ �����
		eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
		break;
	case 2://�� ������
		ind = findIndexByName(buf, bufSize, currentEvent.name);//���� ������ �� ������� ������ �� ������
		packingInd = FAI(packing, packingSize);
		if (packingInd != -1) {//�� ������� ���� �����
			packing[packingInd] = buf[ind];//������ ������ �� ������
			packing[packingInd].allTime += packing[packingInd].timeForPack;
			int eventInd = FAIEvent(eventList, countJob);//����� ��������� ����� � ����������
			eventList[eventInd].name = packing[packingInd].name;//���� � ��������� ������
			eventList[eventInd].time = packing[packingInd].allTime;
			eventList[eventInd].type = 3;
			eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
			cout << packing[packingInd].name << " poshla na packing iz bufera" << endl;
		}
		else {//������ �����
			packing[packingInd].blocked = true;
			currentEvent.type = 5;
			int eventInd = FAIEvent(eventList, countJob);
			eventList[eventInd] = currentEvent;//������� ����� ����� ����������� � ���������
			eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
			cout << packing[packingInd].name << " bula zablokirovana v bufere" << endl;
		}
		//��� ����� �������� �� ���� ��������������� ������� � ���� �� ���
		indPackingEmpty = FAI(packing, packingSize);
		while (indPackingEmpty != -1) {//��� ��� ������
			int helpTime = MAXTIME;//���� ����� ������� �� packing
			int helpInd = -1;
			for (int i = 0; i < bufSize; i++) {
				if (buf[i].blocked) {
					if (buf[i].timeForPack < helpTime) {
						helpTime = buf[i].timeForPack;
						helpInd = i;
					}
				}
			}
			if (helpInd != -1) {
				packing[indPackingEmpty] = buf[helpInd];
				buf[helpInd] = null;
				packing[indPackingEmpty].allTime += packing[indPackingEmpty].timeForPack;//�������� ������ ����� �� ������ � ������ �������
				int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				eventList[eventInd].name = packing[indPackingEmpty].name;//��������� ��� ������� � �������
				eventList[eventInd].type = 3;
				eventList[eventInd].time = packing[indPackingEmpty].allTime;
				indPackingEmpty = FAI(packing, packingSize);
				cout << packing[indPackingEmpty].name << " ushla iz bufera na packing posle razblokirovki" << endl;
			}
			else { break; }
		}
		//������ �� �� ����� ��� ������
		indBufEmpty = FAI(buf, bufSize);
		while (indBufEmpty != -1) {//���� ���� ����� � ������
			int helpTime = MAXTIME;//���� ����� ������� �� packing
			int helpInd = -1;
			for (int i = 0; i < pickingSize; i++) {
				if (picking[i].blocked) {
					if (picking[i].timeForPack < helpTime) {
						helpTime = picking[i].timeForPack;
						helpInd = i;
					}
				}
			}
			if (helpInd != -1) {
				buf[indBufEmpty] = picking[helpInd];
				picking[helpInd] = null;
				//��� ��� ����� ������!!! �� ���� ���� ���� ��������� allTIme �� �������� ������� �������� ������
				int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				eventList[eventInd].name = buf[indBufEmpty].name;//��������� ��� ������� � ������
				eventList[eventInd].type = 2;
				eventList[eventInd].time = buf[indBufEmpty].allTime;
				indBufEmpty = FAI(packing, packingSize);
				cout << buf[indBufEmpty].name << " ushla iz piking v bufer posle razblokirovki" << endl;
			}
			else { break; }
		}
		eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
		break;
	case 3://���� ����� �� �������
		outInd = FAI(output, countJob);
		packingInd = findIndexByName(packing, packingSize, currentEvent.name);
		output[outInd] = packing[packingInd];//������ ������ � ����������
		output[outInd].allTime += output[outInd].timeForPack;
		expandBufTime(buf, bufSize, output[outInd].timeForPack);
		eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
		packing[packingInd] = null;
		//cout << "for test " << output[outInd].allTime << endl;
		cout << output[outInd].name << " ushla iz packing v output" << endl;
		break;
	default:
		cout << "something goes wrong!\n";
		break;
	}
}
void doEvent(Event currentEvent, Event eventList[], Job input[], Job picking[], Job packing[], Job buf[], Job output[], int pickingSize, int packingSize, int bufSize, int countJob) {
	Job null = { "null", 0, 0, 0, 0, 0, 0, MAXTIME, false };//������� ������
	int indToDelete = findIndexByNameInEventList(eventList, countJob, currentEvent.name);//����� �������� ������� �����
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
	switch (currentEvent.type)
	{
	case 1://picking
		ind = findIndexByName(picking, pickingSize, currentEvent.name);//���� ������ �� ������� ������ �� ������
		bufInd = FAI(buf, bufSize);//���� ��������� ����� � ������
		extraTime = currentEvent.time;//���������� ����� � �������� ������
		if (bufInd != -1) {//� ������ ���� �����
						   //packingInd = FAI(packing, packingSize);
						   ///*��� ��� �� �����*/if (packingInd != -1) {//�� ������� ���� �����
						   //	packing[packingInd] = picking[ind];//��������� ������ � ������� �� ������
						   //	picking[ind] = null;
						   //	shiftLeft(input, countJob);
						   //	packing[packingInd].allTime += packing[packingInd].timeForPick;//�������� ������ ����� �� ������ � ������ �������
						   //	int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
						   //	eventList[eventInd].name = packing[packingInd].name;//��������� ��� ������� � �������
						   //	eventList[eventInd].type = 3;
						   //	eventList[eventInd].time = extraTime + packing[packingInd].timeForPack;
						   //	cout << packing[packingInd].name << " otpravilas na packing" << endl;
						   //	if (input[0].name != "null") {
						   //		picking[ind] = input[0];//�� �������������� ����� picking ����� ������
						   //		input[0] = null;
						   //		shiftLeft(input, countJob);//�������� �����	
						   //		picking[ind].startTime = extraTime;//����� ������ - ����� �������� ������
						   //		eventInd = FAIEvent(eventList, countJob);//���� ����� � ����������
						   //		eventList[eventInd].time = picking[ind].timeForPick + extraTime;//������ �� ��� ����� ������ �� �������
						   //		eventList[eventInd].type = 1;
						   //		eventList[eventInd].name = picking[ind].name;
						   //		cout << picking[ind].name << " prishla iz ocheredi na picking" << endl;
						   //	}
						   //	eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
						   //}

			buf[bufInd] = picking[ind];//� ����� ������� ������ �� ������
			buf[bufInd].allTime = extraTime;///��� �������� ��������, �������� ����� �� ��
											/*int eventInd = FAIEvent(eventList, countJob);
											eventList[eventInd].time = extraTime;///��� �������� ��������, �������� ����� �� ��
											eventList[eventInd].type = 2;
											eventList[eventInd].name = picking[ind].name;*/
			cout << buf[bufInd].name << " otpravilas v bufer" << endl;
			picking[ind] = null;//���������� ����� �� �������
								///������ ��� ��� �� �����if (input[0].name != "null") {
								//	picking[ind] = input[0];//�� �������������� ����� ����� ������
								//	shiftLeft(input, countJob);
								//	picking[ind].startTime = extraTime + picking[ind].timeForPick;
								//	picking[ind].allTime = extraTime;
								//	eventInd = FAIEvent(eventList, countJob);
								//	eventList[eventInd].time = picking[ind].timeForPick + extraTime;
								//	eventList[eventInd].type = 1;
								//	eventList[eventInd].name = picking[ind].name;
								//	cout << picking[ind].name << " prishla na piking" << endl;
								//}
		}
		else {//������������ �����
			currentEvent.type = 5;
			picking[ind].blocked = true;
			//int eventInd = FAIEvent(eventList, countJob);
			//eventList[eventInd] = currentEvent;//������� ����� ����� ����������� � ���������
			cout << picking[ind].name << " ne smogla poyti v bufer tak kak on polon" << endl;
		}
		////��� ����� �������� �� ���� ��������������� ������� � ���� �� ���
		//indPackingEmpty = FAI(packing, packingSize);
		//while (indPackingEmpty != -1) {//��� ��� ������
		//	int helpTime = MAXTIME;//���� ����� ������� �� packing
		//	int helpInd = -1;
		//	for (int i = 0; i < bufSize; i++) {
		//		if (buf[i].blocked) {
		//			if (buf[i].timeForPack < helpTime) {
		//				helpTime = buf[i].timeForPack;
		//				helpInd = i;
		//			}
		//		}
		//	}
		//	if (helpInd != -1) {
		//		packing[indPackingEmpty] = buf[helpInd];
		//		buf[helpInd] = null;
		//		packing[indPackingEmpty].allTime += packing[indPackingEmpty].timeForPack;//�������� ������ ����� �� ������ � ������ �������
		//		int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
		//		eventList[eventInd].name = packing[indPackingEmpty].name;//��������� ��� ������� � �������
		//		eventList[eventInd].type = 3;
		//		eventList[eventInd].time = packing[indPackingEmpty].allTime;
		//		cout << packing[indPackingEmpty].name << " poshla na packing iz bufera posle razblokirovki" << endl;
		//		indPackingEmpty = FAI(packing, packingSize);
		//	}
		//	else { break; }
		//}
		////������ �� �� ����� ��� ������
		//indBufEmpty = FAI(buf, bufSize);
		//while (indBufEmpty != -1) {//���� ���� ����� � ������
		//	int helpTime = MAXTIME;//���� ����� ������� �� packing
		//	int helpInd = -1;
		//	for (int i = 0; i < pickingSize; i++) {
		//		if (picking[i].blocked) {
		//			if (picking[i].timeForPack < helpTime) {
		//				helpTime = picking[i].timeForPack;
		//				helpInd = i;
		//			}
		//		}
		//	}
		//	if (helpInd != -1) {
		//		buf[indBufEmpty] = picking[helpInd];
		//		picking[helpInd] = null;
		//		//��� ��� ����� ������!!! �� ���� ���� ���� ��������� allTIme �� �������� ������� �������� ������
		//		int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
		//		eventList[eventInd].name = buf[indBufEmpty].name;//��������� ��� ������� � ������
		//		eventList[eventInd].type = 2;
		//		eventList[eventInd].time = buf[indBufEmpty].allTime;
		//		indBufEmpty = FAI(packing, packingSize);
		//		cout << buf[indBufEmpty].name << " poshla v bufer posle razblokirovki" << endl;
		//	}
		//	else { break; }
		//}
		/////��� �������� �������� ��������� ������� ������ �� ������, �� ������� � ���� ������ ������ �����
		eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
												 //��� ��� ��������
												 //��� ������:
		availableBufJob = findAvailableJob(buf, bufSize);
		indPackingEmpty = FAI(packing, packingSize);

		while (indPackingEmpty != -1 && availableBufJob != -1) {//��� ��� ������
			int helpTime = MAXTIME;//���� ����� ������� �� packing
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
				int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				eventList[eventInd].name = packing[indPackingEmpty].name;//��������� ��� ������� � �������
				eventList[eventInd].type = 3;
				eventList[eventInd].time = packing[indPackingEmpty].timeForPack + extraTime;
				cout << packing[indPackingEmpty].name << " poshla na packing iz bufera posle ojidaniya" << endl;
				indPackingEmpty = FAI(packing, packingSize);
				availableBufJob = findAvailableJob(buf, bufSize);
				///� ��� ��� �� �������� blocked???
			}
			else { break; }
		}
		//��� ��� �������
		availablePickingJob = findAvailableJob(picking, pickingSize);
		indBufEmpty = FAI(buf, bufSize);
		while (availablePickingJob != -1 && indBufEmpty != -1) {//��� ��� �������
			int helpTime = MAXTIME;//���� ����� ������� �� packing
			int helpInd = -1;
			for (int i = 0; i < bufSize; i++) {
				if (picking[i].timeForPack < helpTime && buf[i].name != "null") {
					helpTime = picking[i].timeForPack;
					helpInd = i;
				}
			}
			if (helpInd != -1) {
				buf[indPackingEmpty] = picking[helpInd];
				picking[helpInd] = null;
				///��� ��� ����� �� �����int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				//eventList[eventInd].name = buf[indPackingEmpty].name;//��������� ��� ������� � �������
				//eventList[eventInd].type = 3;
				//eventList[eventInd].time = packing[indPackingEmpty].timeForPack + extraTime;
				cout << buf[indPackingEmpty].name << " poshla v bufer iz pickinga posle ojidaniya" << endl;
				availablePickingJob = findAvailableJob(picking, pickingSize);
				indBufEmpty = FAI(buf, bufSize);
				///� ��� ��� �� �������� blocked???
			}
			else { break; }
		}
		//����� ������ �� picking
		indPickingEmpty = FAI(picking, pickingSize);
		while (indPickingEmpty != -1 && input[0].name != "null") {
			picking[indPickingEmpty] = input[0];
			picking[indPickingEmpty].startTime = 0;
			picking[indPickingEmpty].allTime = 0;
			int eventInd = FAIEvent(eventList, countJob);
			eventList[eventInd].time = picking[indPickingEmpty].timeForPick;
			eventList[eventInd].type = 1;
			eventList[eventInd].name = picking[indPickingEmpty].name;
			input[indPickingEmpty] = null;
			indPickingEmpty = FAI(picking, pickingSize);
			shiftLeft(input, countJob);
			eventListBubbleSort(eventList, countJob);
		}
		break;
		///��� ������ ��� ������ case 2://�� ������
		//	ind = findIndexByName(buf, bufSize, currentEvent.name);//���� ������ � ������ ������ �� ������
		//	packingInd = FAI(packing, packingSize);
		//	if (packingInd != -1) {//�� ������� ���� �����
		//		packing[packingInd] = buf[ind];//������ ������ �� ������
		//		packing[packingInd].allTime += packing[packingInd].timeForPack;
		//		int eventInd = FAIEvent(eventList, countJob);//����� ��������� ����� � ����������
		//		eventList[eventInd].name = packing[packingInd].name;//���� � ��������� ������
		//		eventList[eventInd].time = packing[packingInd].allTime;
		//		eventList[eventInd].type = 3;
		//		eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
		//		cout << packing[packingInd].name << " poshla na packing iz bufera" << endl;
		//	}
		//	else {//������ �����
		//		packing[packingInd].blocked = true;
		//		currentEvent.type = 5;
		//		int eventInd = FAIEvent(eventList, countJob);
		//		eventList[eventInd] = currentEvent;//������� ����� ����� ����������� � ���������
		//		eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
		//		cout << packing[packingInd].name << " bula zablokirovana v bufere" << endl;
		//	}
		//	//��� ����� �������� �� ���� ��������������� ������� � ���� �� ���
		//	indPackingEmpty = FAI(packing, packingSize);
		//	while (indPackingEmpty != -1) {//��� ��� ������
		//		int helpTime = MAXTIME;//���� ����� ������� �� packing
		//		int helpInd = -1;
		//		for (int i = 0; i < bufSize; i++) {
		//			if (buf[i].blocked) {
		//				if (buf[i].timeForPack < helpTime) {
		//					helpTime = buf[i].timeForPack;
		//					helpInd = i;
		//				}
		//			}
		//		}
		//		if (helpInd != -1) {
		//			packing[indPackingEmpty] = buf[helpInd];
		//			buf[helpInd] = null;
		//			packing[indPackingEmpty].allTime += packing[indPackingEmpty].timeForPack;//�������� ������ ����� �� ������ � ������ �������
		//			int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
		//			eventList[eventInd].name = packing[indPackingEmpty].name;//��������� ��� ������� � �������
		//			eventList[eventInd].type = 3;
		//			eventList[eventInd].time = packing[indPackingEmpty].allTime;
		//			indPackingEmpty = FAI(packing, packingSize);
		//			cout << packing[indPackingEmpty].name << " ushla iz bufera na packing posle razblokirovki" << endl;
		//		}
		//		else { break; }
		//	}
		//	//������ �� �� ����� ��� ������
		//	indBufEmpty = FAI(buf, bufSize);
		//	while (indBufEmpty != -1) {//���� ���� ����� � ������
		//		int helpTime = MAXTIME;//���� ����� ������� �� packing
		//		int helpInd = -1;
		//		for (int i = 0; i < pickingSize; i++) {
		//			if (picking[i].blocked) {
		//				if (picking[i].timeForPack < helpTime) {
		//					helpTime = picking[i].timeForPack;
		//					helpInd = i;
		//				}
		//			}
		//		}
		//		if (helpInd != -1) {
		//			buf[indBufEmpty] = picking[helpInd];
		//			picking[helpInd] = null;
		//			//��� ��� ����� ������!!! �� ���� ���� ���� ��������� allTIme �� �������� ������� �������� ������
		//			int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
		//			eventList[eventInd].name = buf[indBufEmpty].name;//��������� ��� ������� � ������
		//			eventList[eventInd].type = 2;
		//			eventList[eventInd].time = buf[indBufEmpty].allTime;
		//			indBufEmpty = FAI(packing, packingSize);
		//			cout << buf[indBufEmpty].name << " ushla iz piking v bufer posle razblokirovki" << endl;
		//		}
		//		else { break; }
		//	}
		//	eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
		//	break;
	case 3://���� ����� �� �������
		extraTime = currentEvent.time;//���������� ����� � �������� ������
		outInd = FAI(output, countJob);
		packingInd = findIndexByName(packing, packingSize, currentEvent.name);
		output[outInd] = packing[packingInd];//������ ������ � ����������
		output[outInd].allTime = extraTime;
		//expandBufTime(buf, bufSize, output[outInd].timeForPack); ��� ������ ����� �������� ����
		eventListBubbleSort(eventList, countJob);//� ����� ��������� ���������
		packing[packingInd] = null;
		cout << output[outInd].name << " ushla iz packing v output" << endl;
		availableBufJob = findAvailableJob(buf, bufSize);
		indPackingEmpty = FAI(packing, packingSize);
		//cout << availableBufJob << " super test!! " << indPackingEmpty << endl;
		//printNames(packing, packingSize);

		while (indPackingEmpty != -1 && availableBufJob != -1) {//��� ��� ������
			printNames(buf, bufSize);
			int helpTime = MAXTIME;//���� ����� ������� �� packing
			int helpInd = -1;
			for (int i = 0; i < bufSize; i++) {
				if (buf[i].timeForPack < helpTime && buf[i].name != "null") {
					helpTime = buf[i].timeForPack;
					helpInd = i;
				}
			}
			cout << helpInd << " super test!! " << helpTime << endl;
			if (helpInd != -1) {
				packing[indPackingEmpty] = buf[helpInd];
				buf[helpInd] = null;
				int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				eventList[eventInd].name = packing[indPackingEmpty].name;//��������� ��� ������� � �������
				eventList[eventInd].type = 3;
				eventList[eventInd].time = packing[indPackingEmpty].timeForPack + extraTime;
				cout << packing[indPackingEmpty].name << " poshla na packing iz bufera posle ojidaniya" << endl;
				indPackingEmpty = FAI(packing, packingSize);
				availableBufJob = findAvailableJob(buf, bufSize);
				///� ��� ��� �� �������� blocked???
			}
			else { break; }
		}
		//��� ��� �������
		availablePickingJob = findAvailableJob(picking, pickingSize);
		indBufEmpty = FAI(buf, bufSize);
		while (availablePickingJob != -1 && indBufEmpty != -1) {//��� ��� �������
			int helpTime = MAXTIME;//���� ����� ������� �� packing
			int helpInd = -1;
			for (int i = 0; i < bufSize; i++) {
				if (picking[i].timeForPack < helpTime  && picking[i].name != "null") {
					helpTime = picking[i].timeForPack;
					helpInd = i;
				}
			}
			if (helpInd != -1) {
				buf[indPackingEmpty] = picking[helpInd];
				picking[helpInd] = null;
				///��� ��� ����� �� �����int eventInd = FAIEvent(eventList, countJob);//����� ������ ����� � �����
				//eventList[eventInd].name = buf[indPackingEmpty].name;//��������� ��� ������� � �������
				//eventList[eventInd].type = 3;
				//eventList[eventInd].time = packing[indPackingEmpty].timeForPack + extraTime;
				cout << buf[indPackingEmpty].name << " poshla v bufer iz pickinga posle ojidaniya" << endl;
				availablePickingJob = findAvailableJob(picking, pickingSize);
				indBufEmpty = FAI(buf, bufSize);
				///� ��� ��� �� �������� blocked???
			}
			else { break; }
		}
		//����� ������ �� picking
		indPickingEmpty = FAI(picking, pickingSize);
		while (indPickingEmpty != -1 && input[0].name != "null") {
			picking[indPickingEmpty] = input[0];
			picking[indPickingEmpty].startTime = 0;
			picking[indPickingEmpty].allTime = 0;
			int eventInd = FAIEvent(eventList, countJob);
			eventList[eventInd].time = picking[indPickingEmpty].timeForPick;
			eventList[eventInd].type = 1;
			eventList[eventInd].name = picking[indPickingEmpty].name;
			input[indPickingEmpty] = null;
			indPickingEmpty = FAI(picking, pickingSize);
			shiftLeft(input, countJob);
			eventListBubbleSort(eventList, countJob);
		}
		break;
	default:
		cout << "something goes wrong!\n";
		break;
	}
}
int calcFunc()
{
	int bufSize = 2;//������ ������		
	int pickingSize = 2;//���������� ����� �� �������
	int packingSize = 2;//���������� ����� �� �������
	int countJob = 3;//���������� �����
	int time = 0;//����� ����� ����������
	Event *eventList = new Event[countJob];//����� ����
	Job *input = new Job[countJob];//������� ������������������
	Job *picking = new Job[pickingSize];//����� ��� ������ �� �������	
	Job *packing = new Job[packingSize];//����� ��� ������ �� �������
										//Job j =  { "123", 4, 5, 6, 7, 8, 9 };
										//input[0] = j;
	Job *buf = new Job[bufSize];//����� ��� ������ � ������
	Job *output = new Job[countJob];//�������� ������������������
	Job null = { "null", 0, 0, 0, 0, 0, 0 };//������� ������
	for (int i = 0; i < pickingSize; i++) {
		picking[i] = null;
	}
	for (int i = 0; i < packingSize; i++) {
		packing[i] = null;
	}
	for (int i = 0; i < countJob; i++) {
		input[i] = null;
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
	Job j = { "123", 4, 1, 0, 6, 77, 0, 0, false };
	input[0] = j;
	j = { "234", 4, 1, 0, 6, 77, 0, 0, false };
	input[1] = j;
	j = { "345", 4, 1, 0, 6, 77, 0, 0, false };
	input[2] = j;
	/*j = { "234", 4, 5, 6, 7, 8, 9 };
	input[1] = j;
	j = { "345", 4, 5, 6, 777, 8, 9 };
	input[2] = j;*/
	//� ����� ����� � ����������� ������� �� ��������
	//� ��� ��������� ��������� �����

	//������ ���������� ���������
	for (int i = 0; i < pickingSize && i < countJob; i++) {
		picking[i] = input[i];
		picking[i].startTime = 0;
		picking[i].allTime = 0;
		eventList[i].time = picking[i].timeForPick;
		eventList[i].type = 1;
		eventList[i].name = picking[i].name;
		input[i] = null;
	}
	shiftLeft(input, countJob);
	eventListBubbleSort(eventList, countJob);
	printNamesEventList(eventList, countJob);
	while (output[countJob - 1].name == "null") {
		Event currentEvent = eventList[0];
		doEvent(currentEvent, eventList, input, picking, packing, buf, output, pickingSize, packingSize, bufSize, countJob);
	}
	//cout << output[countJob - 1].allTime;
	//system("pause");
	return output[countJob - 1].allTime;
}

