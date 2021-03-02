#include <iostream>
#include <cstdio> 
#include <string> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

using namespace std;

int main(int argc, char* args[]) {

	char* myfifo = "myfifo";
	if(mkfifo("myfifo", 0777) == -1) {
		if(errno != EEXIST) {
			cerr << "Error: couldn't create myfifo pipe" << endl;
			exit(-1);
		}
	}

	int workerNum;
	int fd1;
	fd1 = open(myfifo,O_RDONLY);
	read(fd1, &workerNum, sizeof(workerNum));
	cout << "Read from pipe: " << workerNum << endl;
	int rangeStart, rangeEnd;
	for(int i = 0; i < workerNum; i++) {
		read(fd1, &rangeStart, sizeof(rangeStart));
		read(fd1, &rangeEnd, sizeof(rangeEnd));
		cout << "Range start and end for worker #" << i << ": " << rangeStart << "-" << rangeEnd << endl;
	}	
	close(fd1);

	//string rangeStartStr, rangeEndStr, attrNumStr;

	// We know the exact order of arguments, so we don't have to look for them in a for loop
/*	rangeStartStr = args[1];
	int rangeStart = stoi(rangeStartStr);
	cout << rangeStart << endl;
	rangeEndStr = args[2];
	int rangeEnd = stoi(rangeEndStr);
	attrNumStr = args[3];
	int attrNum = stoi(attrNumStr);

	cout << "Range start: " << rangeStart << endl << "Range end: " << rangeEnd << endl << "Attr num: " << attrNum << endl;*/

	return 0;


}