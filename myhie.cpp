#include <iostream>
#include <fstream>
#include <string> // Yay!
#include <cstring> // For strcmp

using namespace std;

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

	string line;
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
		int income;
		int zip;		
	};

	int rid, dep, income, zip;
	string firstName, lastName;

	int lineCount = 0;
	while (fin >> rid >> firstName >> lastName >> dep >> income >> zip) { // Measure length of the file, essential to creating our data array
		lineCount++;
	}
	fin.close();

	fin.open(inputFile); // Opening it again to reset the file reader to the top

	int i = 0;
	Taxpayer dataSet[lineCount]; // Array of Taxpayer structs, this is what we'll slice up and pass to all our sorters

	while (fin >> rid >> firstName >> lastName >> dep >> income >> zip) { // As with the previous assignment, this syntax makes parsing easy - even easier, in fact, as we have strings instead of char arrays and don't have to bother with memory allocation
		if(i > lineCount-1) { // Makes sure we don't go out of bounds
			break;
		}
		dataSet[i].rid = rid; // The struct at each entry of the array is filled up with the data
		dataSet[i].firstName = firstName;
		dataSet[i].lastName = lastName;
		dataSet[i].dep = dep;
		dataSet[i].income = income;
		dataSet[i].zip = zip;
		i++;

	}
	fin.close();

	for(unsigned int i = 0; i < sizeof(dataSet)/sizeof(dataSet[0]); i++) { // Print results of parsing
		cout << dataSet[i].rid << " " << dataSet[i].firstName << " " << dataSet[i].lastName << " " << dataSet[i].dep << " " << dataSet[i].income << " " << dataSet[i].zip << endl;
	}

	return 0;

}
