#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using namespace std;

struct Process {
    int id;
    string name;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority; //0 is for normal process, 1 or above is for system task
    int completionTime;
    int waitingTime;

    Process();
    Process(int processId, const string& processName, int arrival, int burst, int processPriority = 0);

    bool isCompleted() const;
};

#endif
