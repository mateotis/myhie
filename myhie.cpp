#include <iostream>
#include <fstream>
#include <string> // Yay!
#include <cstring> // For strcmp and strcpy
#include <cstdio>
#include <cstdlib>
#include <ctime> // For time-keeping (and random number generator seeding, believe it or not)
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <sys/types.h> 

using namespace std;

static void showReturnStatus(pid_t childpid, int status) // Diagnostic function taken from an OS lab
{
	if (WIFEXITED(status) && !WEXITSTATUS(status))
		printf("Child %ld terminated normally\n", (long)childpid);
	else if (WIFEXITED(status))
		printf("Child %ld terminated with return status %d\n", (long)childpid, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("Child %ld terminated due to uncaught signal %d\n", (long)childpid, WTERMSIG(status));
	else if (WIFSTOPPED(status))
		printf("Child %ld stopped due to signal %d\n", (long)childpid, WSTOPSIG(status));
}

int main(int argc, char* args[]) {

	string inputFile = "";
	string workerNumStr = "";
	string attrNumStr = "";
	string outputFile = "output.csv";
	int workerNum = 5; // We set up some default values so that the program could technically run with only the input file as parameter
	int attrNum = 0;
	bool randomRanges = false;
	bool ascendingOrder = true; // If false, then we'll sort in a descending order, obviously

	for(int i = 0; i < argc; i++) {
		if(strcmp(args[i], "-i") == 0) { // Parameter parsing code adapted from my mvote program, except we have many more params here
			inputFile = args[i+1];
		}
		else if(strcmp(args[i], "-k") == 0) {
			workerNumStr = args[i+1];
			workerNum = stoi(workerNumStr); // We take the variable outside the loop with this
		}
		else if(strcmp(args[i], "-r") == 0) { // A simple flag with no parameter after it
			randomRanges = true;
		}
		else if(strcmp(args[i], "-a") == 0) {
			attrNumStr = args[i+1];
			attrNum = stoi(attrNumStr);
			if(attrNum != 0 && attrNum != 3 && attrNum != 4 && attrNum != 5) { // Catch any invalid (read: non-numerical) input right here
				cerr << "Error: invalid sorting attribute. Please specify a numerical column." << endl;
				return -1;
			}
		}
		else if(strcmp(args[i], "-o") == 0) {
			if(strcmp(args[i+1], "d") == 0) { // That's all we need to check as we set the boolean to true by default
				ascendingOrder = false;
			}
		}
		else if(strcmp(args[i], "-s") == 0) {
			outputFile = args[i+1];
		}
	}


	if(inputFile == "") { // We need something to open!
		cerr << "Please specify an input file." << endl;
		return -1;
	}

	// Construct and print a neat little program configuration based on the command line parametres
	cout << "PROGRAM SETTINGS" << endl << "Input file: " << inputFile << endl << "Output file: " << outputFile << endl << "Number of sorters: " << workerNum << endl << "Attribute to sort by: ";
	if(attrNum == 0) {
		cout << "Resident ID" << endl;
	}
	else if(attrNum == 3) {
		cout << "Number of dependents" << endl;
	}
	else if(attrNum == 4) {
		cout << "Income" << endl;
	}
	else {
		cout << "Zip code" << endl;
	}

	if(randomRanges == true) {
		cout << "Random ranges: Yes" << endl;
	}
	else {
		cout << "Random ranges: No" << endl;
	}

	if(ascendingOrder == false) {
		cout << "Sorting order: Descending" << endl;
	}
	else {
		cout << "Sorting order: Ascending" << endl;
	}
	cout << "\n" << endl;

	ifstream fin;
	fin.open(inputFile);
	if(!fin) { // Fallback in case we can't open the file for whatever reason
		cerr << "Error: file could not be opened." << endl;
		return -1;
	}

	string line;
	int lineCount = 0;
	while(getline(fin, line)) { // Measure length of the file, essential to creating our data array
		lineCount++;
	}
	fin.close();
	cout << "File has " << lineCount << " lines." << endl;

	// Creating the coordinator node and sending it info
	int fd[2];
	pid_t pid;
	pid = fork(); // First we fork it
	if(pid < 0) { // Error handling
		cerr << "Error: could not start coordinator child process." << endl;
		return -1;
	} 
	else if(pid == 0) { // PID == 0 means we're always in the child, here known as the coordinator node
		int workerRanges[workerNum][2]; // 2D array with each entry corresponding to a worker with two ints in it: range start and range end
		workerRanges[0][0] = 0;

		if(randomRanges == true) { // Generating random ranges
			int randNums[workerNum-1]; // We need one less number than the # of workers because the first worker always starts at 0
			srand(time(0)); // Seed the generator based on current time; ensures that we always get new values

			for(int i = 0; i < workerNum - 1; i++) { // Generate the numbers
				randNums[i] = rand() % lineCount;
			}

			for(int i = 0; i < workerNum - 1; i++) { // After we have our random numbers, we have to do some sanity checking
				srand(time(0));
				while(randNums[i] == 0) { // No number can be 0, since 0 is already the start of worker #1
					randNums[i] = rand() % lineCount;
				}
			}

			cout << "Our random numbers are: ";
			for(int i = 0; i < workerNum - 1; i++) {
				cout << randNums[i] << " ";
			}
			cout << endl;

			int currentMin = lineCount; // One higher than any of the possible random values we generated
			int currentMinLoc = 0;
			for(int i = 1; i < workerNum; i++) { // My trusty selection sort algorithm is deployed once again, this time to fill up a sorted array in ascending order!
				for(int j = 0; j < workerNum -1; j++) {
					if(randNums[j] < currentMin) {
						currentMin = randNums[j];
						currentMinLoc = j; // We save where we found the max value, so we can set it to 0 at the end of one iteration
					}
				}

				workerRanges[i][0] = currentMin; // We start filling up the starting points of each worker node with the random variables in ascending order
				randNums[currentMinLoc] = lineCount; // Makes sure we don't find it again
				currentMin = lineCount;
			}

			srand(time(0)); // TO-DO: sanity check to ensure that no starting points match
			for(int i = 0; i < workerNum; i++) { // Now that we have the starting points, we get the end points to ensure we cover all ground
				if(i < workerNum - 1) { // Boundary check
					if(workerRanges[i+1][0] - workerRanges[i][0] == 0) {
						cerr << "Error: worker starting points match!" << endl;
						return -1;
					}
					workerRanges[i][1] = workerRanges[i+1][0] - 1;
				}
				else { // The end point of the last worker is the end of the data array
					workerRanges[i][1] = lineCount - 1;
				}
			}
		}
		else { // Splitting up the ranges equally
			// As it turns out, it doesn't matter if the range splits nicely (meaning lineCount % workerNum == 0) or not - we can just add whatever remainder there is to the last worker's lot in a special case
			int range = lineCount / workerNum; // This is how many entries each worker will work on
			for(int i = 0; i < workerNum; i++) {
				workerRanges[i][0] = i*range; // The starting point is always just current worker number times the range

				if(i == workerNum - 1) {  // The last worker range is handled specially, the rest have a simple formula
					workerRanges[i][1] = lineCount - 1; // Its end is always set to the very last entry, meaning that it takes any extra remainders - what a hard worker!
				}
				else {
					workerRanges[i][1] = (i+1)*range - 1; // The end point of a worker is the next worker's starting point minus one
				}

			}
		}

		for(int i = 0; i < workerNum; i++) {
			cout << "Worker #" << i << " range is: " << workerRanges[i][0] << " - " << workerRanges[i][1] << endl;
		}

		// Creating worker child

		pid_t pid;
		pid = fork(); // First we fork it
		if(pid < 0) { // Error handling
			cerr << "Error: could not start worker child process." << endl;
			return -1;
		} 
		else if(pid == 0) { // Worker child, we pass to it the file it needs to read and the specific ranges
			cout << "Starting worker process" << endl;
			string rangeStartStr = to_string(workerRanges[1][0]);
			string rangeEndStr = to_string(workerRanges[1][1]);
			string attrNumStr = to_string(attrNum);

			// Sadly, we're not free of char arrays in this program either - the exec command can't handle strings as parameters, so we have to go the old-fashioned way
			char* inputFileChar = new char[30];
			char* rangeStartChar = new char[30];
			char* rangeEndChar = new char[30];
			char* attrNumChar = new char[30];

			// As hacky as this seems, this is genuinely the best way I found to convert an int to a char array (by first converting it to a string)
			strcpy(inputFileChar, inputFile.c_str());
			strcpy(rangeStartChar, rangeStartStr.c_str());
			strcpy(rangeEndChar, rangeEndStr.c_str());
			strcpy(attrNumChar, attrNumStr.c_str());

			char *arg[] = {"worker",inputFileChar,rangeStartChar, rangeEndChar, attrNumChar, NULL}; 
			execv("./worker", arg);			
		}
		else { // Parent will wait for worker to finish

			wait(NULL);
			cout << "Worker child finished running." << endl;

		}



	} 
	else { // Parents waits for child to complete
		wait(NULL);
		cout << "Coord child finished running." << endl;
	}

/*	for(unsigned int i = 0; i < sizeof(dataSet)/sizeof(dataSet[0]); i++) { // Print results of parsing
		cout << dataSet[i].rid << " " << dataSet[i].firstName << " " << dataSet[i].lastName << " " << dataSet[i].dep << " " << dataSet[i].income << " " << dataSet[i].zip << endl;
	}*/

	return 0;

}
