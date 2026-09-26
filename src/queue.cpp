#include "../header/queue.h"

//Queue Node
QueueNode::QueueNode(const Process& process) : data(process), next(nullptr) {}
//Queue class
Queue::Queue() : front(nullptr), rear(nullptr), count(0) {}

Queue::~Queue() {
    clear();
}

bool Queue::isEmpty() const {
    return front == nullptr && count == 0;
}

int Queue::size() const {
    return count;
}

void Queue::clear()
{
    QueueNode* current = front;

    while (current != nullptr){
        QueueNode* temp = current;
        current = current->next;
        delete temp;
    }
    
	//restart
    front = nullptr;
    rear = nullptr;
    count = 0;
}

QueueNode* Queue::getFront() const{
    return front;
}

QueueNode* Queue::getRear() const{
    return rear;
}

bool Queue::removeById(int id)
{
    if (isEmpty())
        return false;

    // Xóa node đầu
    if (front->data.id == id)
    {
        QueueNode* temp = front;

        front = front->next;

        delete temp;
        count--;

        if (count == 0)
            rear = nullptr;

        return true;
    }

    // Tìm node cần xóa
    QueueNode* temp = front;

    while (temp->next != nullptr)
    {
        if (temp->next->data.id == id)
        {
            QueueNode* cur = temp->next;

            temp->next = cur->next;

            // Nếu xóa node cuối
            if (cur == rear)
                rear = temp;

            delete cur;
            count--;

            return true;
        }

        temp = temp->next;
    }

    return false;
}

const Process* Queue::findById(int id) const{
    const QueueNode* temp = front;

    while (temp != nullptr)
    {
        if (temp->data.id == id)
        {
            return &temp->data;
        }

        temp = temp->next;
    }

    return nullptr;
}

const Process* Queue::findArrivedProcess(int currentTime) const{
    const QueueNode* temp = front;

    while (temp != nullptr){
        if (temp->data.arrivalTime <= currentTime)
            return &temp->data;

        temp = temp->next;
    }
    return nullptr;
}

//NORMAL QUEUE
void NormalQueue::enqueue(const Process& process){
    QueueNode* newNode = new QueueNode(process);

    if (isEmpty()){
        front = rear = newNode;
    }
    else{
    	//Add last SLL
        rear->next = newNode;
        rear = newNode;
    }

    count++;
}

Process NormalQueue::dequeue(){
    QueueNode* temp = front;

	//Save data to return removed object information
    Process process = temp->data;

    front = front->next;
    delete temp;
    count--;

    if (count == 0){
        rear = nullptr;
    }

    return process;
}

void PriorityQueue::insert(const Process& process)
{
    QueueNode* newNode = new QueueNode(process);

    // Queue đang rỗng
    if (isEmpty())
    {
        front = newNode;
        rear = newNode;
        count++;
        return;
    }

    /*
        newNode phải đứng trước front nếu:

        1. priority cao hơn
        hoặc
        2. cùng priority nhưng arrivalTime nhỏ hơn
    */
    if (process.priority > front->data.priority ||
        (process.priority == front->data.priority &&
         process.arrivalTime < front->data.arrivalTime))
    {
        newNode->next = front;
        front = newNode;
        count++;
        return;
    }

    // Tìm vị trí thích hợp
    QueueNode* current = front;

    while (current->next != nullptr)
    {
        QueueNode* nextNode = current->next;

        if (process.priority > nextNode->data.priority ||
            (process.priority == nextNode->data.priority &&
             process.arrivalTime < nextNode->data.arrivalTime))
        {
            break;
        }

        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    // Nếu thêm vào cuối thì cập nhật rear
    if (newNode->next == nullptr)
    {
        rear = newNode;
    }

    count++;
}

Process PriorityQueue::dequeue(){
    QueueNode* temp = front;

    Process process = temp->data;

    front = front->next;

    delete temp;

    count--;

    if (count == 0)
    {
        rear = nullptr;
    }

    return process;
}

