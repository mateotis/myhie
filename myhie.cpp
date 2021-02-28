#include <iostream>
#include <fstream>
#include <string> // Yay!
#include <cstring> // For strcmp
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

static void showReturnStatus(pid_t childpid, int status)
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
	string outputFile = "";
	int workerNum, attrNum;
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
		else if(strcmp(args[i], "-r") == 0) {
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

	//execl("./execTest", "execTest", NULL);

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

	struct Taxpayer { // Struct that stores all our data - we don't need a class as you don't really modify this data, you just sort it
		int rid;
		string firstName;
		string lastName;
		int dep;
		float income;
		int zip;		
	};

	string line;
	int lineCount = 0;
	while(getline(fin, line)) { // Measure length of the file, essential to creating our data array
		lineCount++;
	}
	fin.close();
	cout << "File has " << lineCount << " lines." << endl;

	fin.open(inputFile); // Opening it again to reset the file reader to the top

	int i = 0;
	Taxpayer dataSet[lineCount]; // Array of Taxpayer structs, this is what we'll slice up and pass to all our sorters

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
		dataSet[j].rid = rid;
		dataSet[j].firstName = firstName;
		dataSet[j].lastName = lastName;
		dataSet[j].dep = dep;
		dataSet[j].income = income;
		dataSet[j].zip = zip;

		j++;
	}

	fin.close();

	for(unsigned int i = 0; i < sizeof(dataSet)/sizeof(dataSet[0]); i++) { // Print results of parsing
		cout << dataSet[i].rid << " " << dataSet[i].firstName << " " << dataSet[i].lastName << " " << dataSet[i].dep << " " << dataSet[i].income << " " << dataSet[i].zip << endl;
	}

	return 0;

}
