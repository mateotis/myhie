#include <string>

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