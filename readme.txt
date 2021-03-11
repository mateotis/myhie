===MYHIE WRITE-UP===
Máté Hekfusz

This is the write-up/readme for the myhie program. While I have gotten more comfortable with C/C++ since the first assignment, this one still had constant challenges for me to overcome and, requiring a great heap of creativity and tons of effort. As always, I have learned a lot on the journey, and I hope to share some of it here.
This readme will have a somewhat different structure than the one I wrote for mvote, to reflect the different nature of the two programs. For me, mvote was about the optimal design of data structures, while myhie is more about algorithms and the delicate art of inter-process communication.

So let's dive in!

==FILE STRUCTURE==
myhie is made up of five files in total: three cpp files, one header, and one makefile. Also included in my submission are the supplied test files, one of which the user has to provide the program with. Additionally, the user is given the option of writing the program's output to a file.

1. myhie.cpp is the main file and contains most of the program logic. It creates and manages the entire process hierarchy: starting with the root, then the coordinator, executing the worker nodes, then finally calling the merger.
2. worker.cpp contains everything the stand-alone worker node needs to be able to do its job and give its results back to the program. The main program exec's the executable created from this file.
3. sorters.cpp is the repository of sorting algorithms: two different ones for the workers, as required, and a third merging algorithm. Both the main and worker programs rely on its sorters to do, well, the sorting.
4. sorters.h is the header file for sorters.cpp. I created a header file for this because both other files include sorter functionality and it's better practice to include the header than the cpp file. That is also why I didn't create headers for the other two files, as they are just made up of their main() functions (and signal handlers, which I'll talk about later).
5. makefile links all the pieces together through separate compilation, creating two executables: myhie (which the user executes) and worker (which myhie executes)

==INTERPROCESS COMMUNICATION==
A key part of this assignment was managing multiple, concurrently running pieces: with two executables and a whole hierarchy of parents and children, I had to ensure all of them communicated smoothly, frequently, and that everyone did their job properly. Kind of like a real family, right?

I will now briefly outline the hierarchy of processes in myhie and how they communicate.

The first node, and the parent of all others, is root. The root creates the coordinator node, its first child, through a fork. The coordinator then creates as many worker nodes as the user specified with forks in a for loop, ensuring that they all share the coord as the one parent. These worker nodes spend their first few days (read: lines of code) in the comforting presence of their parent, before they execv() the aforementioned worker executable and begin working. They are still the children of coord, but they no longer share the same process space or memory. Its children now mature, the coordinator creates its last child through a fork: the merger node, and bestows upon it the array of PIDs corresponding to all the workers (and some extra information).

The merger node sets up a named pipe called intfifo through which it will read the results from each of the workers. Meanwhile, the workers are busy sorting their assigned entries. Once a worker finishes, it sets up the same named pipe and waits until it receives a signal from the merger node that it can start writing to it.

I make use of the fact that named pipes block until they have both ends open. The merger thus waits until the worker it wants to read from has also opened the pipe, which means that it finished with its sorting. Then, the merger sends a signal (an overloaded SIGCONT, to be specific) to that worker worker, telling it that it can start writing. The merger, thanks to all the data coord gathered on its worker children, also knows the exact ranges of each worker, and thus reads exactly that many entries from each worker. This ensures that both sides are on the same page and that every piece of data is received properly.

Once that worker has finished writing to the pipe, the merger closes and reopens its end of the pipe, ensuring that it "hangs" again until the next worker is ready. Then it signals it, receives what it reads into intfifo, moves onto the next one, etc etc until every worker has had their fair go at the pipe and merger has recovered all data.

Each parent process waits for all its children to finish before it exits. To me, that's just responsible parenting, but in a more pragmatic sense, it also allows for accurate time-keeping and synchronisity.

It took a lot of time and many, many attempts to get this sizeable family to communicate effectively and play nice with the finicky FIFO. But I managed!

As a final note, the naming of intfifo might make you wonder if there were any other-fifos as well. The answer is yes: there used to be a second named pipe, named charfifo, which originally handled the string parts of the entry (first name and last name). However, it turned out that pipes are finicky creatures indeed and have trouble handling anything other than the essential data types. The names of the entries are thus retrieved in a different way in the final version.

==ALGORITHMIC DESIGN==
myhie, a sorting program, is heavy on sorters. Here, I will detail what sorting algorithms I implemented and how they work.

Each worker has two sorting algorithms available to them, and they choose one based on their number: even-numbered workers use insertion sort, odd-numbered ones use bubble sort. These are both O(n^2) algorithms: not the most time-efficient, but both of them are relatively simple and intuitive, which were a big advantage since I implemented both from scratch.

Insertion sort works by comparing an element to all previous ones, one at a time, "moving" it backwards until it finds the right place for it, then inserting it there, hence the name. After any nth iteration, the array is sorted up to the nth element.

Bubble sort is a bit more unorthodox, in that its central loop is a do-while loop, not a for loop. It works by constantly comparing neighbouring elements and swapping them if they're out of order. This results in the highest/smallest (depending on how we're sorting) elements "bubbling" to the top of the array, hence the name. It repeats this procedure until the array is completely sorted.

The third sorting algorithm is used in the merging process. It's the most complex one, and the one I'm proudest of. I don't think it has a specific name; it's kind of like a variant of selection sort, but not exactly. Let me describe how it works!

The merger has a set of k sorted subarrays available to it: the fruits of each worker's labour. It also keeps a set of k iterators (literally just an array of ints), one for each subarray.
What it does is that it compares the first element in each such subarray, finds the smallest/largest, and inserts it to the first place of a newly-constructed array. Then, and here's the critical part, it increments the iterator of *that specific subarray* to ensure that it doesn't evaluate the same element twice.

It repeats this procedure n times, n being the total number of entries, filling up the final sorted array one element at a time, always selecting the best fit from k choices.

By creating such an algorithm, I was able to assemble the fully sorted array in O(nk) time, with k being the number of workers. As this number is generally way smaller than n, this runs much faster than a naive O(n^2) sorting algorithm would, *while* also making good use of the partial work done by the workers.

Final note: To account for all the possible order (asc/desc) and attribute (RID, dependent number, income, zip code) combinations, each sorter has 8 variants separated by simple if/else statements. That's why sorters.cpp is so long.


==INTERESTING PARTS==
As this readme is already getting quite long, instead of writing a full program flow description, I will just mention what I think are some of the more interesting parts of my implementation. Some of these are just creative solutions to requirements, some are (I believe) places where I went beyond them.

- As I mentioned earlier, the workers only pass the numerical values of an entry through the pipe back to the merger. After all such data is received, the merger matches the string parts to the partially sorted data with the help of a plain, unsorted array with all the information that root read into memory at the start of the program. By the time this partially sorted data is passed to the merging algorithm, it has all the information.

- Talking about strings, I came up with what I'd say is quite a creative solution to input file parsing. The input files we were provided with had variable amounts of whitespace between the tags in each line which meant that I couldn't just tokenise the line easily. Instead, I wrote a "smart" parser that keeps track of what variable it's currently reading and can detect when one variable ends and another begins.

- While myhie is capable of handling all specified parameters, it can actually run with just one of them! If you're lazy or short on time, you can just give it an input file and it will execute with default settings no problem. These default settings are: 5 workers, sorting in ascending order based on RID, with uniform ranges and no writing to any output file.

- While we're on user input: if the user chooses to have workers sort random ranges, the generation code is quite robust in ensuring there are no invalid range values or repetitions. I wrote an algorithm that checks each generated number against *all other* generated numbers to ensure there are no repetitions. If any are found, the entire loop is re-run from the start until every value is unique. This can add to the execution time in the most extreme cases (when there are lots of workers for few entries) but it 100% ensures our ranges are valid.

- The list containing the partially sorted (read: pre-merge) data that we got from all workers is actually just a 1D array. I use some clever indexing (based on the known ranges of each worker and the fact that we process them in order) to achieve this and create the "illusion" that these sub-arrays are separated.

- All throughout the program, I do my best to keep you, the user, informed of what the program is doing. There are live updating counters for how many entries merger has read through the pipe, how the string matcher is progressing, or how far along merging is.

- The program dynamically allocates most of its data arrays to ensure there's always enough space even with the largest datasets. It is also very diligent about freeing up all this memory before it exits - I used the popular memory check tool Valgrind to test myhie and ensure that there are always zero memory leaks.

==FINAL NOTES==
Thanks for sticking with me throughout this entire document! I just have a few final notes to impart.

Firstly, performance. The individual algorithms in myhie aren't the most efficient, generally running in O(n^2) time. Its efficiency is given by its ability to divide this work between any number of workers. But since the number of workers is entirely dependent on the user, the individual sorting work can take some time if there are only a few workers specified for a massive data set. In any case, please be patient while the program executes, even if it seems to hang - trust me, that just means the workers are really hard at work! As mentioned earlier, I did my best to keep you, the user, abreast of what the program is doing at any given moment.

I also recommend scaling the amount of workers provided to the data size if you want the fastest times: the 100,000 entry set sorts well with 50 workers, and you can give the 1,000,000 set 500 workers - no sweat, myhie will give you results!

That said, there is one bottleneck that I want to mention: the string matcher. It goes through the entire data set in O(n^2) time, making it by far the slowest part of myhie. This is really only noticeable on the two largest datasets. But regardless, to make testing easier, I included an extra parameter: if you add "-ws" when running myhie, the program will skip the string matcher entirely. Your output will, of course, lack the names of the entries because of this, but it will otherwise be perfectly sorted and formatted as usual. Use it as you wish!

Finally, I wanted to mention that there was a single issue that eluded my grasp all throughout. In *very rare* cases when there are only two workers given, the program gives strange, repeated output and doesn't exit properly. I tried for hours, but I wasn't able to consistently reproduce this bug, and you may never encounter it either. But I'm warning you, just in case.
If you do encounter it, just terminate the program and re-run it with simple parameters (a small dataset with one worker will do the job) to make sure the pipe and memory are cleared out properly. After you've done so, you can carry on as normal!

Thank you, once again, for reading my readme to the end. I hope I could take you on an enjoyable journey, because that's what's making this assignment was for me.

Enjoy myhie!
--Máté
