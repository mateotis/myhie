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

// SORTING ALGORITHMS
// All three of these sorting algorithms have several variations with minimal changes to cover every order-attribute combination - for the first two, comments are repeated in each variation; for the third, I decided it was tidier to just comment on the first occurences of each algorithm

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
				insertPos = i;
				currentEntry = dataSet[i];

				while(insertPos > 0 && dataSet[insertPos-1].dep > currentEntry.dep) {
					dataSet[insertPos] = dataSet[insertPos-1];
					insertPos--;
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

void merge(Taxpayer partSortedData[], int workerRangeStarts[], int workerNum, int lineCount, int attrNum, string sortOrder) {
	cout << "In merger!" << endl;

	Taxpayer finalSortedData[lineCount];
	int workerIterators[workerNum] = {0}; // Iterators used by the merging algorithm

	// The merger checks the first elements of every sub-array, looking for the smallest/largest value, then it advances the iterator of that sub-array before moving onto the next value, steadily filling up the final sorted array as such

	// As with the other sorting algos in this file, we need to have a version for every potential order-attribute combination

	if(attrNum == 0) { // Sorting by RID
		if(sortOrder == "ascending") {
			int currentMin = 9999999;
			int currentMinWorker = 0;
			Taxpayer currentTaxpayer;
			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < workerNum; j++) {
					if(j == workerNum - 1 && workerIterators[j] > lineCount - workerRangeStarts[j] - 1) { // Special case for the last worker, as the workerRangeStarts[j+1] condition in the proceeding condition would be out of bounds - instead, we can simply get the range of the last worker by subtracting its starting point from the total number of entries 
						continue;
					}
					if(workerIterators[j] > workerRangeStarts[j+1] - workerRangeStarts[j] - 1) { // If the iterator for any sub-array reaches its range (given by range end - range start - 1), skip as we are done with that sub-array
						continue;
					}
			
					if(partSortedData[workerRangeStarts[j] + workerIterators[j]].rid < currentMin) { // If you remember, we did the same indexing when we first assembled partSortedData - adding the starting point of the worker to the current position of the iterator - except back there the iterator was just i, here it's a whole array of them!
						currentTaxpayer = partSortedData[workerRangeStarts[j] + workerIterators[j]];
						currentMin = partSortedData[workerRangeStarts[j] + workerIterators[j]].rid;
						currentMinWorker = j;
					}
				}

				finalSortedData[i] = currentTaxpayer;
				workerIterators[currentMinWorker]++; // Advance the iterator of the sub-array where we found the right element, to ensure that we don't consider it again

				currentMin = 9999999;
				currentMinWorker = 0;

			}
		}
		else { // When we're sorting by descending order, we flip min to max and also the relation sign, otherwise the algorithm is the same
			int currentMax = 0;
			int currentMaxWorker = 0;
			Taxpayer currentTaxpayer;
			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < workerNum; j++) {
					if(j == workerNum - 1 && workerIterators[j] > lineCount - workerRangeStarts[j] - 1) {
						continue;
					}
					if(workerIterators[j] > workerRangeStarts[j+1] - workerRangeStarts[j] - 1) {
						continue;
					}

					if(partSortedData[workerRangeStarts[j] + workerIterators[j]].rid > currentMax) { // Flipping things here specifically
						currentTaxpayer = partSortedData[workerRangeStarts[j] + workerIterators[j]];
						currentMax = partSortedData[workerRangeStarts[j] + workerIterators[j]].rid;
						currentMaxWorker = j;
					}
				}

				finalSortedData[i] = currentTaxpayer;
				workerIterators[currentMaxWorker]++;

				currentMax = 0;
				currentMaxWorker = 0;

			}
		}
	}
	else if(attrNum == 3) { // By number of dependents
		if(sortOrder == "ascending") {
			int currentMin = 9999999;
			int currentMinWorker = 0;
			Taxpayer currentTaxpayer;
			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < workerNum; j++) {
					if(j == workerNum - 1 && workerIterators[j] > lineCount - workerRangeStarts[j] - 1) {
						continue;
					}
					if(workerIterators[j] > workerRangeStarts[j+1] - workerRangeStarts[j] - 1) {
						continue;
					}
			
					if(partSortedData[workerRangeStarts[j] + workerIterators[j]].dep < currentMin) {
						currentTaxpayer = partSortedData[workerRangeStarts[j] + workerIterators[j]];
						currentMin = partSortedData[workerRangeStarts[j] + workerIterators[j]].dep;
						currentMinWorker = j;
					}
				}

				finalSortedData[i] = currentTaxpayer;
				workerIterators[currentMinWorker]++;

				currentMin = 9999999;
				currentMinWorker = 0;

			}
		}
		else {
			int currentMax = -1; // Number of dependents can legitimately be 0, so currentMax is one less here to avoid issues 
			int currentMaxWorker = 0;
			Taxpayer currentTaxpayer;
			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < workerNum; j++) {
					if(j == workerNum - 1 && workerIterators[j] > lineCount - workerRangeStarts[j] - 1) {
						continue;
					}
					if(workerIterators[j] > workerRangeStarts[j+1] - workerRangeStarts[j] - 1) {
						continue;
					}

					if(partSortedData[workerRangeStarts[j] + workerIterators[j]].dep > currentMax) {
						currentTaxpayer = partSortedData[workerRangeStarts[j] + workerIterators[j]];
						currentMax = partSortedData[workerRangeStarts[j] + workerIterators[j]].dep;
						currentMaxWorker = j;
					}
				}

				finalSortedData[i] = currentTaxpayer;
				workerIterators[currentMaxWorker]++;

				currentMax = -1;
				currentMaxWorker = 0;

			}
		}
	}
	else if(attrNum == 4) { // By income
		if(sortOrder == "ascending") {
			int currentMin = 9999999;
			int currentMinWorker = 0;
			Taxpayer currentTaxpayer;
			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < workerNum; j++) {
					if(j == workerNum - 1 && workerIterators[j] > lineCount - workerRangeStarts[j] - 1) { // Special case for the last worker, as the workerRangeStarts[j+1] condition in the proceeding condition would be out of bounds - instead, we can simply get the range of the last worker by subtracting its starting point from the total number of entries 
						continue;
					}
					if(workerIterators[j] > workerRangeStarts[j+1] - workerRangeStarts[j] - 1) { // If the iterator for any sub-array reaches its range (given by range end - range start - 1), skip as we are done with that sub-array
						continue;
					}
			
					if(partSortedData[workerRangeStarts[j] + workerIterators[j]].income < currentMin) { // If you remember, we did the same indexing when we first assembled partSortedData - adding the starting point of the worker to the current position of the iterator - except back there the iterator was just i, here it's a whole array of them!
						currentTaxpayer = partSortedData[workerRangeStarts[j] + workerIterators[j]];
						currentMin = partSortedData[workerRangeStarts[j] + workerIterators[j]].income;
						currentMinWorker = j;
					}
				}

				finalSortedData[i] = currentTaxpayer;
				workerIterators[currentMinWorker]++; // Advance the iterator of the sub-array where we found the right element, to ensure that we don't consider it again

				currentMin = 9999999;
				currentMinWorker = 0;

			}
		}
		else { // When we're sorting by descending order, we flip min to max and also the relation sign, otherwise the algorithm is the same
			int currentMax = 0;
			int currentMaxWorker = 0;
			Taxpayer currentTaxpayer;
			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < workerNum; j++) {
					if(j == workerNum - 1 && workerIterators[j] > lineCount - workerRangeStarts[j] - 1) {
						continue;
					}
					if(workerIterators[j] > workerRangeStarts[j+1] - workerRangeStarts[j] - 1) {
						continue;
					}

					if(partSortedData[workerRangeStarts[j] + workerIterators[j]].income > currentMax) { // Flipping things here specifically
						currentTaxpayer = partSortedData[workerRangeStarts[j] + workerIterators[j]];
						currentMax = partSortedData[workerRangeStarts[j] + workerIterators[j]].income;
						currentMaxWorker = j;
					}
				}

				finalSortedData[i] = currentTaxpayer;
				workerIterators[currentMaxWorker]++;

				currentMax = 0;
				currentMaxWorker = 0;

			}
		}
	}
	else if(attrNum == 5) { // By zip code
		if(sortOrder == "ascending") {
			int currentMin = 9999999;
			int currentMinWorker = 0;
			Taxpayer currentTaxpayer;
			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < workerNum; j++) {
					if(j == workerNum - 1 && workerIterators[j] > lineCount - workerRangeStarts[j] - 1) { // Special case for the last worker, as the workerRangeStarts[j+1] condition in the proceeding condition would be out of bounds - instead, we can simply get the range of the last worker by subtracting its starting point from the total number of entries 
						continue;
					}
					if(workerIterators[j] > workerRangeStarts[j+1] - workerRangeStarts[j] - 1) { // If the iterator for any sub-array reaches its range (given by range end - range start - 1), skip as we are done with that sub-array
						continue;
					}
			
					if(partSortedData[workerRangeStarts[j] + workerIterators[j]].zip < currentMin) { // If you remember, we did the same indexing when we first assembled partSortedData - adding the starting point of the worker to the current position of the iterator - except back there the iterator was just i, here it's a whole array of them!
						currentTaxpayer = partSortedData[workerRangeStarts[j] + workerIterators[j]];
						currentMin = partSortedData[workerRangeStarts[j] + workerIterators[j]].zip;
						currentMinWorker = j;
					}
				}

				finalSortedData[i] = currentTaxpayer;
				workerIterators[currentMinWorker]++; // Advance the iterator of the sub-array where we found the right element, to ensure that we don't consider it again

				currentMin = 9999999;
				currentMinWorker = 0;

			}
		}
		else { // When we're sorting by descending order, we flip min to max and also the relation sign, otherwise the algorithm is the same
			int currentMax = 0;
			int currentMaxWorker = 0;
			Taxpayer currentTaxpayer;
			for(int i = 0; i < lineCount; i++) {
				for(int j = 0; j < workerNum; j++) {
					if(j == workerNum - 1 && workerIterators[j] > lineCount - workerRangeStarts[j] - 1) {
						continue;
					}
					if(workerIterators[j] > workerRangeStarts[j+1] - workerRangeStarts[j] - 1) {
						continue;
					}

					if(partSortedData[workerRangeStarts[j] + workerIterators[j]].zip > currentMax) { // Flipping things here specifically
						currentTaxpayer = partSortedData[workerRangeStarts[j] + workerIterators[j]];
						currentMax = partSortedData[workerRangeStarts[j] + workerIterators[j]].zip;
						currentMaxWorker = j;
					}
				}

				finalSortedData[i] = currentTaxpayer;
				workerIterators[currentMaxWorker]++;

				currentMax = 0;
				currentMaxWorker = 0;

			}
		}
	}



	cout << "FINAL SORTED DATA" << endl;
	for(int i = 0; i < lineCount; i++) {
		cout << finalSortedData[i].rid << " " << finalSortedData[i].firstName << " " << finalSortedData[i].lastName << " " << finalSortedData[i].dep << " " << finalSortedData[i].income << " " << finalSortedData[i].zip << endl;		
	}

}