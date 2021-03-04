#include <iostream>
#include <fstream>
#include <cstdio> 
#include <string> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

using namespace std;


struct Taxpayer { // Struct that stores all our data - we don't need a class as you don't really modify this data, you just sort it
	int rid;
	string firstName;
	string lastName;
	int dep;
	float income;
	int zip;		
};

void insertionSort(Taxpayer dataSet[], int n, int attrNum, string sortOrder) { // Insertion sort algorithm written from scratch - although it also has O(n^2) time complexity, to my mind it's intuitive and mostly easy to understand 
	int insertPos;
	Taxpayer currentEntry;

	if(attrNum == 0) { // Sorting by RID
		if(sortOrder == "ascending") {
			cout << "in here" << endl;

			for(int i = 1; i < n; i++) {
				insertPos = i; // Where we're going to insert
				currentEntry = dataSet[i]; // What we're inserting

				while(insertPos > 0 && dataSet[insertPos-1].rid > currentEntry.rid) { // Move entry back as long as its RID is smaller than the previous entry's 
					dataSet[insertPos] = dataSet[insertPos-1]; // Moving our entry back actually means moving what was previously there, forward - tricky, right?
					insertPos--; // Keep going backwards until we find the spot

				}

				if(insertPos != i) { // If we're not already there, then finally insert the entry to its proper place in the array
					dataSet[insertPos] = currentEntry; // This moves the whole struct, without having to move members separately, quite efficient
				}
				
			}		
		}
		else { // Sort in descending order - essentially the same algorithm, just with a flipped relation check

			for(int i = 1; i < n; i++) {
				insertPos = i; // Where we're going to insert
				currentEntry = dataSet[i]; // What we're inserting

				while(insertPos > 0 && dataSet[insertPos-1].rid < currentEntry.rid) { // Move entry back as long as its RID is larger than the previous entry's 
					dataSet[insertPos] = dataSet[insertPos-1];
					insertPos--;

				}

				if(insertPos != i) {
					dataSet[insertPos] = currentEntry;
				}
				
			}		
		}		
	}
	else if(attrNum == 3) { // By number of dependents
		if(sortOrder == "ascending") {

			for(int i = 1; i < n; i++) {
				insertPos = i; // Where we're going to insert
				currentEntry = dataSet[i]; // What we're inserting

				while(insertPos > 0 && dataSet[insertPos-1].dep > currentEntry.dep) { // Move entry back as long as its number of dependents is smaller than the previous entry's 
					dataSet[insertPos] = dataSet[insertPos-1]; // Moving our entry back actually means moving what was previously there, forward - tricky, right?
					insertPos--; // Keep going backwards until we find the spot

				}

				if(insertPos != i) { // If we're not already there, then finally insert the entry to its proper place in the array
					dataSet[insertPos] = currentEntry;
				}
				
			}		
		}
		else { // Sort in descending order - essentially the same algorithm, just with a flipped relation check

			for(int i = 1; i < n; i++) {
				insertPos = i; // Where we're going to insert
				currentEntry = dataSet[i]; // What we're inserting

				while(insertPos > 0 && dataSet[insertPos-1].dep < currentEntry.dep) { // Move entry back as long as its number of dependents is larger than the previous entry's 
					dataSet[insertPos] = dataSet[insertPos-1];
					insertPos--;

				}

				if(insertPos != i) {
					dataSet[insertPos] = currentEntry;
				}
				
			}		
		}		
	}
	else if(attrNum == 4) { // By income
		if(sortOrder == "ascending") {

			for(int i = 1; i < n; i++) {
				insertPos = i; // Where we're going to insert
				currentEntry = dataSet[i]; // What we're inserting

				while(insertPos > 0 && dataSet[insertPos-1].income > currentEntry.income) { // Move entry back as long as its income is smaller than the previous entry's 
					dataSet[insertPos] = dataSet[insertPos-1]; // Moving our entry back actually means moving what was previously there, forward - tricky, right?
					insertPos--; // Keep going backwards until we find the spot

				}

				if(insertPos != i) { // If we're not already there, then finally insert the entry to its proper place in the array
					dataSet[insertPos] = currentEntry;
				}
				
			}		
		}
		else { // Sort in descending order - essentially the same algorithm, just with a flipped relation check

			for(int i = 1; i < n; i++) {
				insertPos = i; // Where we're going to insert
				currentEntry = dataSet[i]; // What we're inserting

				while(insertPos > 0 && dataSet[insertPos-1].income < currentEntry.income) { // Move entry back as long as its income is larger than the previous entry's 
					dataSet[insertPos] = dataSet[insertPos-1];
					insertPos--;

				}

				if(insertPos != i) {
					dataSet[insertPos] = currentEntry;
				}
				
			}		
		}		
	}
	else if(attrNum == 5) { // By zip code
		if(sortOrder == "ascending") {

			for(int i = 1; i < n; i++) {
				insertPos = i; // Where we're going to insert
				currentEntry = dataSet[i]; // What we're inserting

				while(insertPos > 0 && dataSet[insertPos-1].zip > currentEntry.zip) { // Move entry back as long as its zip code is smaller than the previous entry's 
					dataSet[insertPos] = dataSet[insertPos-1]; // Moving our entry back actually means moving what was previously there, forward - tricky, right?
					insertPos--; // Keep going backwards until we find the spot

				}

				if(insertPos != i) { // If we're not already there, then finally insert the entry to its proper place in the array
					dataSet[insertPos] = currentEntry;
				}
				
			}		
		}
		else { // Sort in descending order - essentially the same algorithm, just with a flipped relation check

			for(int i = 1; i < n; i++) {
				insertPos = i; // Where we're going to insert
				currentEntry = dataSet[i]; // What we're inserting

				while(insertPos > 0 && dataSet[insertPos-1].zip < currentEntry.zip) { // Move entry back as long as its zip code is larger than the previous entry's 
					dataSet[insertPos] = dataSet[insertPos-1];
					insertPos--;

				}

				if(insertPos != i) {
					dataSet[insertPos] = currentEntry;
				}
				
			}		
		}		
	}
}

void bubbleSort(Taxpayer dataSet[], int n, int attrNum, string sortOrder) { // Bubble sort from scratch - another O(n^2) sorting algorithm that employs constant pair comparisons to sort
	Taxpayer tempEntry;
	bool swapped = false;

	if(attrNum == 0) { // Sorting by RID

		if(sortOrder == "ascending") {

			do { // Probably the first time I've ever written a do-while loop in an actual program - but it's necessary here, as bubble sort always requires one full loop to tell if the array is sorted
				swapped = false; // If we go through an entire for loop without this changing to true, that means the array is sorted and we're done
				for(int i = 1; i < n; i++) {
					if(dataSet[i-1].rid > dataSet[i].rid) { // We always compare neighbouring entries: if they're out of order, swap them
						tempEntry = dataSet[i];
						dataSet[i] = dataSet[i-1]; // We moved the value originally at dataSet[i-1] forward by one place - on next iteration, we'll compare it again, and move it again if necessary; this way the highest/lowest entry "bubbles to the top"
						dataSet[i-1] = tempEntry;

						swapped = true; // We've done at least one swap, so the array is not yet sorted - repeat the while loop
					}
				}
			}
			while(swapped == true);
		}
		else {

			do {
				swapped = false;
				for(int i = 1; i < n; i++) {
					if(dataSet[i-1].rid < dataSet[i].rid) { // As with insertion sort, all we have to do to achieve descending sorting is flip the comparison sign
						tempEntry = dataSet[i];
						dataSet[i] = dataSet[i-1];
						dataSet[i-1] = tempEntry;

						swapped = true;
					}
				}
			}
			while(swapped == true);		
		}
	}
	else if(attrNum == 3) { // By number of dependents

		if(sortOrder == "ascending") {

			do { // Probably the first time I've ever written a do-while loop in an actual program - but it's necessary here, as bubble sort always requires one full loop to tell if the array is sorted
				swapped = false; // If we go through an entire for loop without this changing to true, that means the array is sorted and we're done
				for(int i = 1; i < n; i++) {
					if(dataSet[i-1].dep > dataSet[i].dep) { // We always compare neighbouring entries: if they're out of order, swap them
						tempEntry = dataSet[i];
						dataSet[i] = dataSet[i-1]; // We moved the value originally at dataSet[i-1] forward by one place - on next iteration, we'll compare it again, and move it again if necessary; this way the highest/lowest entry "bubbles to the top"
						dataSet[i-1] = tempEntry;

						swapped = true; // We've done at least one swap, so the array is not yet sorted - repeat the while loop
					}
				}
			}
			while(swapped == true);
		}
		else {

			do {
				swapped = false;
				for(int i = 1; i < n; i++) {
					if(dataSet[i-1].dep < dataSet[i].dep) { // As with insertion sort, all we have to do to achieve descending sorting is flip the comparison sign
						tempEntry = dataSet[i];
						dataSet[i] = dataSet[i-1];
						dataSet[i-1] = tempEntry;

						swapped = true;
					}
				}
			}
			while(swapped == true);		
		}
	}
	else if(attrNum == 4) { // By income

		if(sortOrder == "ascending") {

			do { // Probably the first time I've ever written a do-while loop in an actual program - but it's necessary here, as bubble sort always requires one full loop to tell if the array is sorted
				swapped = false; // If we go through an entire for loop without this changing to true, that means the array is sorted and we're done
				for(int i = 1; i < n; i++) {
					if(dataSet[i-1].income > dataSet[i].income) { // We always compare neighbouring entries: if they're out of order, swap them
						tempEntry = dataSet[i];
						dataSet[i] = dataSet[i-1]; // We moved the value originally at dataSet[i-1] forward by one place - on next iteration, we'll compare it again, and move it again if necessary; this way the highest/lowest entry "bubbles to the top"
						dataSet[i-1] = tempEntry;

						swapped = true; // We've done at least one swap, so the array is not yet sorted - repeat the while loop
					}
				}
			}
			while(swapped == true);
		}
		else {

			do {
				swapped = false;
				for(int i = 1; i < n; i++) {
					if(dataSet[i-1].income < dataSet[i].income) { // As with insertion sort, all we have to do to achieve descending sorting is flip the comparison sign
						tempEntry = dataSet[i];
						dataSet[i] = dataSet[i-1];
						dataSet[i-1] = tempEntry;

						swapped = true;
					}
				}
			}
			while(swapped == true);		
		}
	}
	else if(attrNum == 5) { // By zip code

		if(sortOrder == "ascending") {

			do { // Probably the first time I've ever written a do-while loop in an actual program - but it's necessary here, as bubble sort always requires one full loop to tell if the array is sorted
				swapped = false; // If we go through an entire for loop without this changing to true, that means the array is sorted and we're done
				for(int i = 1; i < n; i++) {
					if(dataSet[i-1].zip > dataSet[i].zip) { // We always compare neighbouring entries: if they're out of order, swap them
						tempEntry = dataSet[i];
						dataSet[i] = dataSet[i-1]; // We moved the value originally at dataSet[i-1] forward by one place - on next iteration, we'll compare it again, and move it again if necessary; this way the highest/lowest entry "bubbles to the top"
						dataSet[i-1] = tempEntry;

						swapped = true; // We've done at least one swap, so the array is not yet sorted - repeat the while loop
					}
				}
			}
			while(swapped == true);
		}
		else {

			do {
				swapped = false;
				for(int i = 1; i < n; i++) {
					if(dataSet[i-1].zip < dataSet[i].zip) { // As with insertion sort, all we have to do to achieve descending sorting is flip the comparison sign
						tempEntry = dataSet[i];
						dataSet[i] = dataSet[i-1];
						dataSet[i-1] = tempEntry;
						
						swapped = true;
					}
				}
			}
			while(swapped == true);		
		}
	}
}

int main(int argc, char* args[]) {

	string inputFile, rangeStartStr, rangeEndStr, attrNumStr, workerNumStr, sortOrder;

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

	int n = sizeof(dataSet)/sizeof(dataSet[0]);
	//insertionSort(dataSet, n, attrNum, sortOrder);
	bubbleSort(dataSet, n, attrNum, sortOrder);
	for(unsigned int i = 0; i < sizeof(dataSet)/sizeof(dataSet[0]); i++) { // Print results of parsing
		cout << dataSet[i].rid << " " << dataSet[i].firstName << " " << dataSet[i].lastName << " " << dataSet[i].dep << " " << dataSet[i].income << " " << dataSet[i].zip << endl;
	}


	return 0;


}