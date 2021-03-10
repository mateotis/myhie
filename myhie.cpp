#include <iostream>
#include <fstream>
#include <cstring> // For strcmp and strcpy
#include <cstdio>
#include <cstdlib>
#include <ctime> // For time-keeping (and random number generator seeding, believe it or not)
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <chrono>

#include "sorters.h"

using namespace std;

int sigusr1Num = 0; // Declaring the number of signals caught as global variables so we can increment them in the signal handler
int sigusr2Num = 0;

static void showReturnStatus(pid_t childpid, int status) // Diagnostic function taken from an OS lab
{
/*	if (WIFEXITED(status) && !WEXITSTATUS(status))
		printf("Child %ld terminated normally\n", (long)childpid);
	else if (WIFEXITED(status))
		printf("Child %ld terminated with return status %d\n", (long)childpid, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("Child %ld terminated due to uncaught signal %d\n", (long)childpid, WTERMSIG(status));
	else if (WIFSTOPPED(status))
		printf("Child %ld stopped due to signal %d\n", (long)childpid, WSTOPSIG(status));*/
}

void signalHandler(int signal)
{
	if(signal==SIGUSR1) {
		//cout << "Root received SIGUSR1 signal: a worker has finished its workload!" << endl;
		sigusr1Num++;
	}
	else if(signal==SIGUSR2) {
		cout << "Root received SIGUSR2 signal: execution is finished! Exiting..." << endl;
		sigusr2Num++;
	}
}

int main(int argc, char* args[]) {

	string inputFile = "";
	string workerNumStr = "";
	string attrNumStr = "";
	// We set up some default values so that the program could technically run with only the input file as parameter
	string outputFile = "";
	int workerNum = 5;
	int attrNum = 0;
	bool randomRanges = false;
	bool ascendingOrder = true; // If false, then we'll sort in a descending order, obviously

	//chrono::time_point<chrono::system_clock> start, end; 

	auto rootTimeStart = chrono::system_clock::now(); // Using the auto type to avoid having to type long, library specific types

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

	int lineCount = 0;
	string line;
	while(getline(fin, line)) { // Measuring the number of entries in the input file, needed for our data structures
		lineCount++;
	}
	fin.close();

	Taxpayer* initialDataSet = new Taxpayer[lineCount]; // Array of Taxpayer structs, stored for now; will be used to cross-check and add the strings to the structs during the merge process. Needs to be dynamically allocated as processing large inputs would take up too much space on the stack!

	// Open file again, this time to read all the data into the array
	fin.open(inputFile);

	int readVar = 0; // Which variable we're currently trying to assemble
	int j = 0; // Used to iterate over the Taxpayer array since we're using a while loop
	while(getline(fin, line)) { // The random number of spaces between variables is very problematic in trying to parse file input, so we have to construct our strings carefully

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
		initialDataSet[j].rid = rid;
		initialDataSet[j].firstName = firstName;
		initialDataSet[j].lastName = lastName;
		initialDataSet[j].dep = dep;
		initialDataSet[j].income = income;
		initialDataSet[j].zip = zip;

		j++;
	}

	fin.close();
	cout << "File has " << lineCount << " lines." << endl;

	signal(SIGUSR1, signalHandler); // Setting up the signal handler to catch the program signals
	signal(SIGUSR2, signalHandler);

	// Creating the coordinator node and sending it info
	int fd[2];
	pid_t pid;
	pid = fork(); // First we fork it
	if(pid < 0) { // Error handling
		cerr << "Error: could not start coordinator child process." << endl;
		return -1;
	} 
	else if(pid == 0) { // PID == 0 means we're always in the child, here known as the coordinator node

		auto coordTimeStart = chrono::system_clock::now();

		// ASSIGNING AND GENERATING WORKER RANGES
		int workerRanges[workerNum][2]; // 2D array with each entry corresponding to a worker with two ints in it: range start and range end
		workerRanges[0][0] = 0;

		if(randomRanges == true) { // Generating random ranges
			int randNums[workerNum-1]; // We need one less number than the # of workers because the first worker always starts at 0
			srand(time(0)); // Seed the generator based on current time; ensures that we always get new values

			for(int i = 0; i < workerNum - 1; i++) { // Generate the numbers
				randNums[i] = rand() % lineCount;
			}

			// After we have our random numbers, we have to do some sanity checking
			for(int i = 0; i < workerNum - 1; i++) {

				while(randNums[i] == 0) { // No number can be 0, since 0 is already the start of worker #0
					srand(time(0));
					randNums[i] = rand() % lineCount;
				}
			}

			bool anyMatches = false;
			// This set of loops checks and rectifies any matching numbers very exhaustively. If a number matches *any* of the *other* numbers in our list, it rerolls that number and sets the do-while condition to true, which means that the whole procedure repeats at least once more. The loop ends when every number is unique. While this might seem overkill, it's honestly the simplest way I found that 100% ensures no repetition.

			// That said, it can drastically increase run time if the user picks an absurdly high number of workers when compared to the input size like say, 50 workers for 100 entries.
			do {
				anyMatches = false;
				for(int i = 0; i < workerNum - 1; i++) {
					for(int j = 0; j < workerNum - 1; j++) {
						if(i != j) {
							if(randNums[i] == randNums[j]) {
								anyMatches = true; // We'll repeat the while loop to make sure nothing gets past
								srand(time(0));
								randNums[i] = rand() % lineCount; // Reroll matching number
							}
						}
					}
				}
			}
			while(anyMatches == true);

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

			srand(time(0));
			for(int i = 0; i < workerNum; i++) { // Now that we have the starting points, we get the end points to ensure we cover all ground
				if(i < workerNum - 1) { // Boundary check
					if(workerRanges[i+1][0] - workerRanges[i][0] == 0) { // This should never occur as we ensure there are no repetitions earlier - but hey, crazier things have happened, and it's better to be safe than sorry
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

		cout << "Worker ranges generated." << endl;
		for(int i = 0; i < workerNum; i++) {
			cout << "Worker #" << i << " range is: " << workerRanges[i][0] << " - " << workerRanges[i][1] << endl;
		}

		// Creating worker children

		pid_t pidArray[workerNum]; // Array to save the child PIDs in, will be useful for signal sending/catching

		pid_t rootPID = getppid(); // Saving the PID of root (the parent of coord) to pass to the workers so the workers can later send signals straight to root

		for(int childNum = 0; childNum < workerNum; childNum++) {

			pid_t pid;
			pid = fork(); // First we fork it
			pidArray[childNum] = pid;

			if(pid < 0) { // Error handling
				cerr << "Error: could not start worker child process." << endl;
				return -1;
			} 
			else if(pid == 0) { // Worker child, we pass to it the file it needs to read, the specific ranges, the attribute it needs to sort by, and also its assigned worker number
				string rangeStartStr = to_string(workerRanges[childNum][0]);
				string rangeEndStr = to_string(workerRanges[childNum][1]);
				string attrNumStr = to_string(attrNum);
				string childNumStr = to_string(childNum);
				string rootPIDStr = to_string(rootPID);
				string workerName = "worker";
				string sortOrder = "";
				if(ascendingOrder == true) {
					sortOrder = "ascending";
				}
				else {
					sortOrder = "descending";
				}

				// Sadly, we're not free of char arrays in this program either - the exec() command can't handle strings as parameters, so we have to go the old-fashioned way
				char* inputFileChar = new char[30];
				char* rangeStartChar = new char[30];
				char* rangeEndChar = new char[30];
				char* attrNumChar = new char[30];
				char* childNumChar = new char[30];
				char* sortOrderChar = new char[30];
				char* rootPIDChar = new char[30];
				char* workerNameChar = new char[30];

				// As hacky as this seems, this is genuinely the best way I found to convert an int to a char array (by first converting it to a string)
				strcpy(inputFileChar, inputFile.c_str());
				strcpy(rangeStartChar, rangeStartStr.c_str());
				strcpy(rangeEndChar, rangeEndStr.c_str());
				strcpy(attrNumChar, attrNumStr.c_str());
				strcpy(childNumChar, childNumStr.c_str());
				strcpy(sortOrderChar, sortOrder.c_str());
				strcpy(rootPIDChar, rootPIDStr.c_str());
				strcpy(workerNameChar, workerName.c_str()); // Put the "worker" string into a char array to stop the compiler from complaining that we're using a string in an array of char pointers

				char* arg[] = {workerNameChar,inputFileChar,rangeStartChar, rangeEndChar, attrNumChar, childNumChar, sortOrderChar, rootPIDChar, NULL}; 
				execv("./worker", arg);	
			}
		
		}

		pid_t pid;
		pid = fork(); // Forking the merger node
		if(pid < 0) { // Error handling
			cerr << "Error: could not start merger child process." << endl;
			return -1;
		} 
		else if(pid == 0) { // In merger node

			auto mergerTimeStart = chrono::system_clock::now();

			if(mkfifo("intfifo" , 0777) == -1) { // Create pipe for our numerical variables
				if(errno != EEXIST) {
					cerr << "Error: couldn't create intfifo pipe" << endl;
					exit(-1);
				}
			}

			// This was originally a 2D array, which was a simple and intuitive solution - BUT, C++ doesn't allow you to pass arrays with variable sizes to functions, which caused trouble later on in the merging process. So instead I rewrote it to be a regular array and use some clever indexing to fit everything in there in a partially sorted manner.
			Taxpayer* partSortedData = new Taxpayer[lineCount];
			double workerExecTimes[workerNum]; // Array to store worker execution times we get from the pipe
			int fd1;

			int rid, dep, zip;
			float income;
			double execTime;
			fd1 = open("intfifo",O_RDONLY);

			for(int j = 0; j < workerNum; j++) { // Letting each worker have their pass at the pipe
				sleep(0.1);
				kill(pidArray[j], SIGCONT); // We overload the SIGCONT signal to do what we want as SIGUSR1 and 2 are in use for other, required reasons - besides, it makes sense that we'd use SIGCONT to tell the worker that it can proceed 

				for(int i = 0; i < workerRanges[j][1] - workerRanges[j][0]+1; i++) { // For each worker, we *strictly* only read as much as it can write to us; that is, its range
					read(fd1, &rid, sizeof(rid));
					read(fd1, &dep, sizeof(dep));
					read(fd1, &income, sizeof(income));
					read(fd1, &zip, sizeof(zip));

					// Basically, each element is inserted at i + the starting point of the current worker. This ensures that the data is still 'separated' (as long as you know these starting values for every worker) in a simple one-dimensional array.
					partSortedData[i+workerRanges[j][0]].rid = rid;
					partSortedData[i+workerRanges[j][0]].dep = dep;
					partSortedData[i+workerRanges[j][0]].income = income;
					partSortedData[i+workerRanges[j][0]].zip = zip;

					cout << "\rSorted entries read from workers through pipe: " << i+workerRanges[j][0]+1;
				}
				read(fd1, &execTime, sizeof(execTime)); // Get the worker's execution time as its last read and store it
				workerExecTimes[j] = execTime;
			}
			cout << endl;
			close(fd1);

			// Delete the named pipe - if you don't do this, there might still be data left in it when you next start the program! Definitely caused me a few strange bugs
			unlink("intfifo");

			// Because the strings/char arrays really did not want to work well with the pipe, no matter how I sliced it, I decided to take a different approach - with the piping done and partSortedData assembled, we can cross-check the unique RIDs with the initialDataSet we parsed earlier to assign the missing string members before we start the merging process 

			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < lineCount; j++) {
					if(partSortedData[i].rid == initialDataSet[j].rid) { // Simple O(n^2) matching - it makes for a  bottleneck in program execution time, but it ensures stable and accurate data output
						partSortedData[i].firstName = initialDataSet[j].firstName;
						partSortedData[i].lastName = initialDataSet[j].lastName;
					}
				}
				cout << "Matching a name to each entry through the RID. Entries processed: " << i+1;
				cout << "\r";
			}
			cout << endl;

			// The below code prints the sorted sub-arrays received from each worker; uncomment if you're interested in that sort of thing
/*			for(int i = 0; i < lineCount; i++) {
				if(i == workerRanges[j][0]) { // Whenever we reach the starting index of a worker, we know that the entries afterwards (and until the next starting index) were sorted by that worker
					cout << "Sorted array received from worker #" << j << endl;
					j++;
				}
				cout << partSortedData[i].rid << " " << partSortedData[i].firstName << " " << partSortedData[i].lastName << " " << partSortedData[i].dep << " " << partSortedData[i].income << " " << partSortedData[i].zip << endl;	
			}*/

			cout << "All workers finished sorting." << endl;

			int workerRangeStarts[workerNum]; // An array with just the starting points of each worker; will be sent to the merger
			for(int i = 0; i < workerNum; i++) {
				workerRangeStarts[i] = workerRanges[i][0];
			}

			string sortOrder = "";
			if(ascendingOrder == true) {
				sortOrder = "ascending";
			}
			else {
				sortOrder = "descending";
			}
			merge(partSortedData, workerRangeStarts, workerNum, lineCount, attrNum, sortOrder, outputFile);

			// Displaying execution times for workers and the merger itself
			for(int i = 0; i < workerNum; i++) {
				cout << "Execution time for worker #" << i << ": " << workerExecTimes[i] << endl;
			}
			auto mergerTimeEnd = chrono::system_clock::now();
			chrono::duration<double> mergerExecTime = mergerTimeEnd - mergerTimeStart;
			cout << "Execution time for merger node: " << mergerExecTime.count() << endl;

			kill(rootPID, SIGUSR2); // After the merging is done, we're done properly - send signal to root accordingly
		}

		int status = 0;

		while ((pid = wait(&status)) != -1) // Coord waits for all its children to finish before it finishes
		{

			//call function to display return status of child with this pid
			showReturnStatus(pid, status);			
		}
		delete[] initialDataSet; // Deleting this in the coord space

		auto coordTimeEnd = chrono::system_clock::now();
		chrono::duration<double> coordExecTime = coordTimeEnd - coordTimeStart;
		cout << "Execution time for coordinator node: " << coordExecTime.count() << endl;
	} 
	else { // Parents waits for coord to finish
		wait(NULL);

		delete[] initialDataSet; // Deleting this in the root space

		cout << "SIGUSR1 signals caught by root: " << sigusr1Num << endl << "SIGUSR2 signals caught by root: " << sigusr2Num << endl;

		auto rootTimeEnd = chrono::system_clock::now(); 
		chrono::duration<double> rootExecTime = rootTimeEnd - rootTimeStart; 

		cout << "Execution time for myhie: " << rootExecTime.count() << " seconds." << endl; 
	}

	return 0;

}
