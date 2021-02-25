#include <iostream>
#include <fstream>
#include <string> // Yay!
#include <cstring> // For strcmp

using namespace std;

int main(int argc, char* args[]) {

	string inputFile = "";
	for(int i = 0; i < argc; i++) {
		if (strcmp(args[i], "-i") == 0) { // Parameter parsing code adapted from my mvote program
			inputFile = args[i+1];
		}
	}

	if(inputFile == "") { // We need something to open!
		cerr << "Please specify an input file." << endl;
		return -1;
	}

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
