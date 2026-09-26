#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "process.h"
#include "queue.h"

struct HistoryRecord{
    Process process;
    int startTime;
    int endTime;
};

class Scheduler {
private:
    NormalQueue nq;
    PriorityQueue pq;
    std::vector<HistoryRecord> history;; //use vector for history storage then for DISPLAY
    
    int currentTime; //use to compare with arrival time
    int timeQuantum; //can use set feature to re-set

    void resetSimulation();
	void addArrivedNormalTasks(
        vector<int>& normalReady,
        vector<bool>& inNormalReady,
        const vector<Process>& processes
    );

public:
    Scheduler(int quantum = 2);

	//Setter,getter
    void setTimeQuantum(int quantum); //help for F6
    int getTimeQuantum() const;
    
    bool hasProcesses(const PriorityQueue& pq) const;
    bool hasProcesses(const NormalQueue& nq)const;
	int isExistedId(int id) const;
	bool isEmptyBothQueue();
	
    //CRUD feature
    void addProcess(); //F1
    void removeProcess(); //F2
    void updateProcess(); //F3
    void displayProcessById(); //F4
    void displayProcessByName(); //F5
    void setNewQuantumTime(); //F6

    void clearProcesses(); //F7
    void displayProcesses() const; //F8
	void runSimulation(); //F9
    void displayExecutionHistory() const; //F10
    void displayStatistics() const; //F11
};

#endif
