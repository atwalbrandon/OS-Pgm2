# OS-Pgm2
This is a program I made for CS 4348 Operating System Concepts.

Instructions: 
Translate Figure 5.14 into a Pthreads implementation (sample code available at: https://linux.die.net/man/3/pthread_create). Use semaphores as indicated in the figure (see https://linux.die.net/man/7/sem_overview). Rather than a single consumer, implement five. Let produce simply supply the next long long value: originally initialized to zero. Let the main terminate all threads after five minute. Create a chart which depicts how many  times each consumer consume a value from the buffer.
Test in your Ubuntu environment.
Submit your code and report (with chart) here.
Bonus points (10): Create a chart (also included in your report) which shows which values were consumed by which consumer.


Feedback to Learner 4/4/22 2:46 PM
{Total: 87; Build: 9.5, Warning messages from compiler.; Code: 36, Storage space too small: may result in lost production.; Comments: 13, File header incorrect. (Procedures should be listed in the same order that they are found in the file.) Insufficient inline comments. Inline comments should be lined up vertically to make reading easier. Procedure header incomplete. (Argument lists missing.); Report: 28.5, Includes grammar or spelling errors. (I.e., ptheads versus pthreads.)}
