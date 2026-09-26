#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

struct QueueNode {
    Process data;
    QueueNode* next;

    QueueNode(const Process& process);
    ~QueueNode() = default;    
};

class Queue {
protected:
    QueueNode* front;
    QueueNode* rear;
    int count;

    // Outside can not call
    Queue();

public:
    virtual ~Queue();

    bool isEmpty() const;
    void clear();
    int size() const;
    QueueNode* getFront() const;
    QueueNode* getRear() const;
    
    bool removeById(int id);
    const Process* findById(int id) const;
    const Process* findArrivedProcess(int currentTime) const;
};

class NormalQueue : public Queue {
public:
    NormalQueue() = default;
    ~NormalQueue() override = default;

    void enqueue(const Process& process);
    Process dequeue();
};


class PriorityQueue : public Queue {
public:
    PriorityQueue() = default;
    ~PriorityQueue() override = default;

    void insert(const Process& process);
    Process dequeue();
};


#endif
