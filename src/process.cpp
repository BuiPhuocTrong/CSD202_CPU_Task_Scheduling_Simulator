#include "../header/process.h"

Process::Process(): 
    id(0), 
	name(""), 
	arrivalTime(0), 
	burstTime(0),
    remainingTime(0), 
	priority(0), 
	completionTime(-1),
    waitingTime(0) {}

Process::Process(int processId, 
				 const string& processName,
                 int arrival, 
				 int burst,
                 int processPriority): 
	id(processId), 
	name(processName), 
    arrivalTime(arrival), 
	burstTime(burst), 
	remainingTime(burst),
    priority(processPriority), 
	completionTime(-1),
    waitingTime(0){}

bool Process::isCompleted() const {
    return remainingTime <= 0;
}

