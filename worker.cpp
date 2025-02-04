#include <iostream> // Libraries included for the same functions as detailed in myhie.cpp
#include <fstream>
#include <string>
#include <cstring>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/wait.h>
#include <unistd.h>
#include <chrono>

#include "sorters.h"

using namespace std;

bool readyToWrite = false; // Define this as a global variable as we change it from the signal handler which doesn't accept extra parametres

void signalHandler(int signal)
{
	if (signal==SIGCONT) { // Getting SIGCONT from merger means the worker is given the go-ahead to write to the pipe
		readyToWrite = true;
	}
}

int main(int argc, char* args[]) {

	signal(SIGCONT, signalHandler);

	string inputFile, rangeStartStr, rangeEndStr, attrNumStr, workerNumStr, sortOrder, rootPIDStr;
	
	auto workerTimeStart = chrono::system_clock::now();

	// We know the exact order of arguments, so we don't have to look for them in a for loop
	inputFile = args[1];
	rangeStartStr = args[2];
	int rangeStart = stoi(rangeStartStr); // Thankfully stoi() also works on char arrays so no need for extra magic here
	rangeEndStr = args[3];
	int rangeEnd = stoi(rangeEndStr);
	attrNumStr = args[4];
	int attrNum = stoi(attrNumStr);
	workerNumStr = args[5];
	int workerNum = stoi(workerNumStr);
	sortOrder = args[6];
	rootPIDStr = args[7];
	int rootPID = stoi(rootPIDStr);

	cout << "Worker #" << workerNum << " started." << endl;

	ifstream fin;
	fin.open(inputFile);

	Taxpayer* dataSet = new Taxpayer[rangeEnd - rangeStart + 1]; // Array of Taxpayer structs, specifically the size of the worker range

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

	int n = rangeEnd - rangeStart + 1; // Calculate array size

	// Invoke one of two sorters based on worker number - even-numbered ones use insertion, odd-numbered ones use bubble
	if(workerNum % 2 == 0) {
		insertionSort(dataSet, n, attrNum, sortOrder, workerNum);
	}
	else {
		bubbleSort(dataSet, n, attrNum, sortOrder, workerNum);	
	}

	if(mkfifo("intfifo" , 0777) == -1) { // Create pipe for our numerical variables
		if(errno != EEXIST) {
			cerr << "Error: couldn't create intfifo pipe" << endl;
			exit(-1);
		}
	}

	int fd1;
	fd1 = open("intfifo", O_WRONLY); // Open pipe to signal merger that it can start reading

	while(readyToWrite == false) { // Worker waits until it receives the signal from the merger that it can write into the pipe
		sleep(1);
	}

	for(int i = 0; i < n; i++) { // Only write as much as there are entries

		//cout << i << " Writing RID: " << dataSet[i].rid << endl;
		write(fd1, &dataSet[i].rid, sizeof(dataSet[i].rid));
		write(fd1, &dataSet[i].dep, sizeof(dataSet[i].dep));
		write(fd1, &dataSet[i].income, sizeof(dataSet[i].income));
		write(fd1, &dataSet[i].zip, sizeof(dataSet[i].zip));
	}

	auto workerTimeEnd = chrono::system_clock::now();
	chrono::duration<double> workerExecTimeChrono = workerTimeEnd - workerTimeStart;
	double workerExecTime = workerExecTimeChrono.count(); // Using an extra variable to simplify writing it to the pipe

	write(fd1, &workerExecTime, sizeof(workerExecTime));

	close(fd1);

	kill(rootPID, SIGUSR1); // Sending SIGUSR1 to root to notify it that this worker has finished

	return 0;


}