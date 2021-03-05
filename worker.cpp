#include <iostream>
#include <fstream>
#include <cstdio> 
#include <string>
#include <cstring>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <ctime>

#include "sorters.cpp"

using namespace std;

int main(int argc, char* args[]) {

	string inputFile, rangeStartStr, rangeEndStr, attrNumStr, workerNumStr, sortOrder;
	clock_t timeStart, timeEnd; // We'll measure the execution time of each worker
	double execTime;

	timeStart = clock();

	// We know the exact order of arguments, so we don't have to look for them in a for loop
	inputFile = args[1];
	rangeStartStr = args[2];
	int rangeStart = stoi(rangeStartStr); // Thankfully stoi() also works on char arrays so no need for extra magic here
	cout << rangeStart << endl;
	rangeEndStr = args[3];
	int rangeEnd = stoi(rangeEndStr);
	attrNumStr = args[4];
	int attrNum = stoi(attrNumStr);
	workerNumStr = args[5];
	int workerNum = stoi(workerNumStr);
	sortOrder = args[6];

	cout << "Worker #" << workerNum << " started." << endl;

	ifstream fin;
	fin.open(inputFile);

	int i = 0;
	Taxpayer dataSet[rangeEnd - rangeStart + 1]; // Array of Taxpayer structs, this is what we'll slice up and pass to all our sorters

	string line;
	int readVar = 0; // Which variable we're currently trying to assemble
	int j = 0; // Used to iterate over the Taxpayer array since we're using a while loop
	int k = 0; // Used to check if we have reached into the worker's range yet
	while(getline(fin, line)) { // The random number of spaces between variables is very problematic in trying to parse file input, so we have to construct our strings carefully

		if(k >= rangeStart && k <= rangeEnd) { // To my knowledge there is no neat way of starting getline() from a specific line, so we just iterate from the start until we reach the start of our range, and stop when we're done with our range
			// Initialising everything to strings initially to enable the upcoming heuristic parsing; will convert into proper types afterwards
			string ridStr = "";
			string firstName = "";
			string lastName = "";
			string depStr = "";
			string incomeStr = "";
			string zipStr = "";
			for(int i = 0; i < line.length(); i++) {
				if(i != 0 && line[i] != ' ' && line[i-1] == ' ') { // We detect when a new variable arrives by looking ahead: if the current character is whitespace and the next one isn't, then we know that the next one is the start of the new variable, since all variables are separated by at least one whitespace
					readVar++;
				}
				if(line[i] != ' ') { // Adding the actual (read: non whitespace) characters to the appropriate strings
					if(readVar == 0) {
						ridStr += line[i];
					}
					else if(readVar == 1) {
						firstName += line[i];
					}
					else if(readVar == 2) {
						lastName += line[i];
					}
					else if(readVar == 3) {
						depStr += line[i];
					}
					else if(readVar == 4) {
						incomeStr += line[i];
					}
					else if(readVar == 5) {
						zipStr += line[i];
					}
				}
				else { // Else, it's a useless space, so we can just ignore it
					continue;
				}
			}
			readVar = 0;

			// Converting to appropriate types
			int rid = stoi(ridStr); // A lovely C++11 function that replaces the less stable atoi()
			int dep = stoi(depStr);
			int zip = stoi(zipStr);
			float income = stof(incomeStr); // Ditto, for floats

			// Finally filling up the appropriate struct in the array
			dataSet[j].rid = rid;
			dataSet[j].firstName = firstName;
			dataSet[j].lastName = lastName;
			dataSet[j].dep = dep;
			dataSet[j].income = income;
			dataSet[j].zip = zip;

			j++;
		}
		else if (k > rangeEnd) { // If we're done, we're done - no need to iterate over the rest of the file
			break;
		}
		k++;
	}

	fin.close();

	cout << "\n" << "Worker #" << workerNum << " output in range " << rangeStart << " - " << rangeEnd << endl;
	if(workerNum % 2 == 0) {
		cout << "Sorting with insertion sort." << endl;
	}
	else {
		cout << "Sorting with bubble sort." << endl;
	}	

	int n = sizeof(dataSet)/sizeof(dataSet[0]); // Calculate array size

	// Invoke one of two sorters based on worker number - even-numbered ones use insertion, odd-numbered ones use bubble
	if(workerNum % 2 == 0) {
		insertionSort(dataSet, n, attrNum, sortOrder);
	}
	else {
		bubbleSort(dataSet, n, attrNum, sortOrder);	
	}
	
/*	for(unsigned int i = 0; i < sizeof(dataSet)/sizeof(dataSet[0]); i++) { // Print results of parsing
		cout << dataSet[i].rid << " " << dataSet[i].firstName << " " << dataSet[i].lastName << " " << dataSet[i].dep << " " << dataSet[i].income << " " << dataSet[i].zip << endl;
	}*/

	cout << "Sending data back to main through the pipe..." << endl;

	char* intfifo = "intfifo";
	if(mkfifo("intfifo" , 0777) == -1) {
		if(errno != EEXIST) {
			cerr << "Error: couldn't create intfifo pipe" << endl;
			exit(-1);
		}
	}
	char* charfifo = "charfifo";
	if(mkfifo("charfifo" , 0777) == -1) {
		if(errno != EEXIST) {
			cerr << "Error: couldn't create charfifo pipe" << endl;
			exit(-1);
		}
	}

	int fd1, fd2;
	fd1 = open(intfifo, O_WRONLY);
	fd2 = open(charfifo, O_WRONLY);

	write(fd1, &n, sizeof(n));

	// A lesson learned the hard way: do NOT mix two pipes with different type variables in them. You'll only get sad and frustrated
	for(int i = 0; i < n; i++) {

		write(fd2, (dataSet[i].firstName).c_str(), (dataSet[i].firstName).length()+1);
		write(fd2, (dataSet[i].lastName).c_str(), (dataSet[i].lastName).length()+1);
	}

	close(fd2);

	for(int i = 0; i < n; i++) {

		write(fd1, &dataSet[i].rid, sizeof(dataSet[i].rid));
		write(fd1, &dataSet[i].dep, sizeof(dataSet[i].dep));
		write(fd1, &dataSet[i].income, sizeof(dataSet[i].income));
		write(fd1, &dataSet[i].zip, sizeof(dataSet[i].zip));
	}

	close(fd1);

	timeEnd = clock();
	execTime = (double(timeEnd) - double(timeStart)) / CLOCKS_PER_SEC; // CLOCKS_PER_SEC is a constant defined in ctime

	cout << "Execution time for worker #" << workerNum << ": " << execTime << " seconds." << endl; 

	return 0;


}