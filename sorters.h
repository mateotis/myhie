#ifndef SORTERS_H
#define SORTERS_H

#include <string>

struct Taxpayer { // Struct that stores all our data - we don't need a class as we don't really modify this data, we just sort it
	int rid;
	std::string firstName;
	std::string lastName;
	int dep;
	float income;
	int zip;		
};

void insertionSort(Taxpayer dataSet[], int n, int attrNum, std::string sortOrder, int workerNum);
void bubbleSort(Taxpayer dataSet[], int n, int attrNum, std::string sortOrder, int workerNum);
void merge(Taxpayer partSortedData[], int workerRangeStarts[], int workerNum, int lineCount, int attrNum, std::string sortOrder, std::string outputFile);

#endif