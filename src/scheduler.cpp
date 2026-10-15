#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <climits>

#include "../header/scheduler.h"
#include "../header/constants.h"
#include "../header/consoleOutput.h"
#include "../header/consoleInput.h"

#define NOT_IN_ANY_QUEUE 0
#define IN_NORMAL_QUEUE 1
#define IN_PRIORITY_QUEUE 2




using namespace std;

Scheduler::Scheduler(int quantum) : currentTime(0), timeQuantum(quantum) {}

void Scheduler::setTimeQuantum(int quantum) {
	if (quantum > 0) timeQuantum = quantum;
}

int Scheduler::getTimeQuantum() const {
	return timeQuantum;
}

bool Scheduler::hasProcesses(const NormalQueue& nq) const {
	return nq.isEmpty();
}
bool Scheduler::hasProcesses(const PriorityQueue& pq) const {
	return pq.isEmpty();
}

int Scheduler::isExistedId(int id) const {
	// Check NormalQueue
	const QueueNode* temp = nq.getFront();

	while (temp != nullptr) {
		if (id == temp->data.id) return IN_NORMAL_QUEUE;
		temp = temp->next;
	}

	// Check PriorityQueue
	temp = pq.getFront();

	while (temp != nullptr) {
		if (id == temp->data.id) return IN_PRIORITY_QUEUE;
		temp = temp->next;
	}
	return NOT_IN_ANY_QUEUE;
}

bool Scheduler::isEmptyBothQueue(){
	return nq.size()==0 && pq.size()==0;
}


#include <fstream>
using namespace std;

bool writeProcess(ofstream& file, const Process& p){
    if (p.id <= 0 || p.arrivalTime < 0 || p.burstTime <= 0 || p.priority < 0){
        return false;
    }

    file << p.id << ','
         << p.name << ','
         << p.arrivalTime << ','
         << p.burstTime << ','
         << p.priority << '\n';

    return file.good();
}


#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// Trả về:
//  1: Đọc thành công
//  0: Hết file
// -1: Dữ liệu không hợp lệ

int readProcess(ifstream& file, Process& p){
    string line;

    if (!getline(file, line)){
        if (file.eof())
            return 0;
        return -1;
    }

    istringstream iss(line);

    int id, arrival, burst, priority;
    string name;
    char sep1, sep2, sep3, sep4;

    if (!(iss >> id >> sep1) || sep1 != ',')
        return -1;

    if (!getline(iss, name, ','))
        return -1;

    if (!(iss >> arrival >> sep2
              >> burst >> sep3
              >> priority))
        return -1;

    // Kiểm tra các dấu phân cách còn lại
    if (sep2 != ',' || sep3 != ',')
        return -1;

    // Kiểm tra dữ liệu hợp lệ
    if (id <= 0 || arrival < 0 ||
        burst <= 0 || priority < 0)
        return -1;

    p = Process(id, name, arrival, burst, priority);
    p.remainingTime = p.burstTime;
    p.completionTime = 0;
    p.waitingTime = 0;

    return 1;
}


bool Scheduler::saveToFile() const{
    ofstream normalFile("data/normal.txt");
    ofstream priorityFile("data/priority.txt");

    if (!normalFile || !priorityFile){
        cout << "Cannot open files for saving.\n";
        return false;
    }

    // Ghi hang doi normal
    const QueueNode* temp = nq.getFront();
    while (temp != nullptr){
        if (temp->data.priority != 0){
            cout << "Invalid process in normal queue.\n";
            return false;
        }

        if (!writeProcess(normalFile, temp->data)){
            cout << "Error writing normal.txt\n";
            return false;
        }
        temp = temp->next;
    }

    // Ghi hang doi priority
    temp = pq.getFront();
    while (temp != nullptr){
        if (temp->data.priority <= 0){
            cout << "Invalid process in priority queue.\n";
            return false;
        }

        if (!writeProcess(priorityFile, temp->data))
        {
            cout << "Error writing priority.txt\n";
            return false;
        }

        temp = temp->next;
    }

    normalFile.close();
    priorityFile.close();

    if (!normalFile || !priorityFile){
        cout << "Error closing files.\n";
        return false;
    }
    return true;
}


bool Scheduler::loadFromFile(){
    NormalQueue tempNormal;
    PriorityQueue tempPriority;
    Process p;
    int status;

    ifstream normalFile("data/normal.txt");
    ifstream priorityFile("data/priority.txt");

    // Kiem tra ca hai file
    if (!normalFile || !priorityFile){
        cout << "Cannot open files for loading.\n";
        return false;
    }

    // Doc normal.txt
    while (true){
        status = readProcess(normalFile, p);

        if (status == 0)
            break;

        if (status == -1){
            cout << "Error reading normal.txt\n";
            return false;
        }

        if (p.priority != 0){
            cout << "Invalid data in normal.txt\n";
            return false;
        }
        tempNormal.enqueue(p);
    }

    // Doc priority.txt
    while (true){
        status = readProcess(priorityFile, p);

        if (status == 0)
            break;

        if (status == -1){
            cout << "Error reading priority.txt\n";
            return false;
        }

        if (p.priority <= 0){
            cout << "Invalid data in priority.txt\n";
            return false;
        }
        tempPriority.insert(p);
    }

    normalFile.close();
    priorityFile.close();

    // Cap nhat hang doi chinh sau khi doc thanh cong
    nq.clear();
    pq.clear();

    while (!tempNormal.isEmpty())
        nq.enqueue(tempNormal.dequeue());

    while (!tempPriority.isEmpty())
        pq.insert(tempPriority.dequeue());

    history.clear();
    currentTime = 0;

    return true;
}

//Feature 1
void Scheduler::addProcess() {
	cout << "===== ADD PROCESS =====\n";
	int n = readIntegerFromZero("Input the number of process need to add (Enter 0 to back): ");

	if (n == 0) {
		waitForEnter();
		return;
	}

	for (int i = 1; i <= n; i++) {
		cout << "Input process" << i << ": \n";
		int id, checkId;
		do {
			id = readIntegerFromOne("Process ID: ");
			checkId = isExistedId(id);
			if (checkId == IN_NORMAL_QUEUE || checkId == IN_PRIORITY_QUEUE)
				cout << "ID of this process already existed";
		} while (checkId == IN_NORMAL_QUEUE || checkId == IN_PRIORITY_QUEUE);

		Process p = readProcessFromConsole(id);

		if (p.priority == 0) {
			nq.enqueue(p);
		} else if (p.priority > 0) {
			pq.insert(p);
		}
	}
	
	if (!saveToFile()){
		cout << "Error while saving";
		waitForEnter();
		return;
	}
	
	cout << "Process added successfully.\n";
    waitForEnter();
}

//Feature 2
void Scheduler::removeProcess(){
	cout << "===== REMOVE PROCESS =====\n";
    int id;
    int checkId;

    do{
        id = readIntegerFromZero("Input the ID of process need to remove (Enter 0 to back): ");
        // 0 = return back
        if (id == 0){
            waitForEnter();
            return;
        }
        checkId = isExistedId(id);
        if (checkId == NOT_IN_ANY_QUEUE) 
            cout << "ID of this process does not exist. Please enter another ID.\n";
    } while (checkId == NOT_IN_ANY_QUEUE);


    if (checkId == IN_NORMAL_QUEUE) 
		nq.removeById(id);
    else if (checkId == IN_PRIORITY_QUEUE)
        pq.removeById(id);

	if (!saveToFile()){
		cout << "Error while saving";
		waitForEnter();
		return;
	}

    cout << "Process removed successfully.\n";
    waitForEnter();
}

//Feature 3
void Scheduler::updateProcess(){
	cout << "===== UPDATE PROCESS =====\n";
    int id;
    int checkId;

    do{
        id = readIntegerFromZero("Input the ID of process need to update (Enter 0 to back): ");
        if (id == 0){
            waitForEnter();
            return;
        }
        checkId = isExistedId(id);
        if (checkId == NOT_IN_ANY_QUEUE) cout << "ID of this process does not exist. Please enter another ID.\n";
    } while (checkId == NOT_IN_ANY_QUEUE);

    // Find current process
    const Process* currentProcess = nullptr;

    if (checkId == IN_NORMAL_QUEUE) currentProcess = nq.findById(id);
    else if (checkId == IN_PRIORITY_QUEUE) currentProcess = pq.findById(id);

    // Safety check
    if (currentProcess == nullptr){
        cout << "Process could not be found.\n";
        waitForEnter();
        return;
    }

    // Make a copy for updating
    Process updatedProcess = *currentProcess;

    // Display current information
    cout << "\nCurrent process information:\n";
    cout << "ID: " << updatedProcess.id << '\n';
    cout << "Name: " << updatedProcess.name << '\n';
    cout << "Arrival Time: " << updatedProcess.arrivalTime << '\n';
    cout << "Burst Time: " << updatedProcess.burstTime << '\n';
    cout << "Priority: " << updatedProcess.priority << '\n';


    // Input new information
    cout << "\nInput new process information:\n";
	int option = readIntegerInRange (1, 5, 
		"Choose option to update: \n 1.Name\n2.Arrival Time\n3.Burst Time\n4.Priority (0 is normal, 1 and above is priority)\n5.Update again all info\n");
	
	switch (option){
		case 1:
			updatedProcess.name = readString("Process Name: ");
		case 2:
			updatedProcess.arrivalTime = readIntegerFromZero("Arrival Time: ");
		case 3: 
			updatedProcess.burstTime = readIntegerFromOne("Burst Time: ");
		case 4:
			updatedProcess.priority = readIntegerFromZero("Priority: ");
		case 5:
			updatedProcess = readProcessFromConsole(updatedProcess.id); 
		default:
			cout << "error";
	}

    // Remove old process
    if (checkId == IN_NORMAL_QUEUE) nq.removeById(id);
    else if (checkId == IN_PRIORITY_QUEUE) pq.removeById(id);

    // Insert updated process into the correct queue
    if (updatedProcess.priority == 0) nq.enqueue(updatedProcess);
    else pq.insert(updatedProcess);

	if (!saveToFile()){
		cout << "Error while saving";
		waitForEnter();
		return;
	}

    cout << "\nProcess updated successfully.\n";
    waitForEnter();
}

//Feature 4
void Scheduler::displayProcessById(){
	cout << "===== DISPLAY PROCESS BY ID =====\n";
    int id = readIntegerFromZero("Input the ID of process need to display (Enter 0 to back): ");

    if (id == 0){
        waitForEnter();
        return;
    }

    const Process* process = nullptr;

    // Search in Normal Queue
    process = nq.findById(id);

    // If not found, search in Priority Queue
    if (process == nullptr) process = pq.findById(id);

    // Not found in both queues
    if (process == nullptr){
        cout << "\nProcess with ID " << id << " does not exist.\n";
        waitForEnter();
        return;
    }

    // Display process information
    cout << "\n========== PROCESS INFORMATION ==========\n";
    cout << "ID:              " << process->id << '\n';
    cout << "Name:            " << process->name << '\n';
    cout << "Arrival Time:    " << process->arrivalTime << '\n';
    cout << "Burst Time:      " << process->burstTime << '\n';
    cout << "Remaining Time:  " << process->remainingTime << '\n';
    cout << "Priority:        " << process->priority << '\n';
    cout << "Completion Time: " << process->completionTime << '\n';
    cout << "Waiting Time:    " << process->waitingTime << '\n';

    if (process->priority == 0)
        cout << "Process type:           Normal Process\n";
    else
        cout << "Process type:           Priority Process(System task)\n";

    cout << "=========================================\n";

    waitForEnter();
}

//Feature 5
void Scheduler::displayProcessByName(){
	cout << "===== DISPLAY PROCESS BY NAME =====\n";
    string keyword;

    cout << "Input the process name to search (press ENTER to back main menu): ";
    getline(cin >> ws, keyword); //getline help read hole line, but "ws" is to skip whitespace when read

    if (keyword.empty()){ //empty is method off string
        cout << "\nSearch keyword cannot be empty.\n";
        waitForEnter();
        return;
    }

    bool found = false;

    cout << "\n========== SEARCH RESULT ==========\n";

    // Search Normal Queue
    const QueueNode* temp = nq.getFront();

    while (temp != nullptr){
        if (temp->data.name.find(keyword) != string::npos){
            const Process& process = temp->data;
            
            cout << "\nID:            " << process.id << '\n';
            cout << "Name:            " << process.name << '\n';
            cout << "Arrival Time:    " << process.arrivalTime << '\n';
            cout << "Burst Time:      " << process.burstTime << '\n';
            cout << "Remaining Time:  " << process.remainingTime << '\n';
            cout << "Priority:        " << process.priority << '\n';
            cout << "Completion Time: " << process.completionTime << '\n';
            cout << "Waiting Time:    " << process.waitingTime << '\n';
            cout << "Process Type:     Normal Process\n";
            
			found = true;
        }
        temp = temp->next;
    }

    // Search Priority Queue
    temp = pq.getFront();

    while (temp != nullptr){
        if (temp->data.name.find(keyword) != string::npos){
            const Process& process = temp->data;

            cout << "\nID:              " << process.id << '\n';
            cout << "Name:            " << process.name << '\n';
            cout << "Arrival Time:    " << process.arrivalTime << '\n';
            cout << "Burst Time:      " << process.burstTime << '\n';
            cout << "Remaining Time:  " << process.remainingTime << '\n';
            cout << "Priority:        " << process.priority << '\n';
            cout << "Completion Time: " << process.completionTime << '\n';
            cout << "Waiting Time:    " << process.waitingTime << '\n';
            cout << "Process Type:           Priority Process\n";

            found = true;
        }
        temp = temp->next;
    }

    cout << "\n===================================\n";

    if (!found) cout << "No process found with name containing \"" << keyword << "\".\n";
    waitForEnter();
}

//Feature 6
void Scheduler::setNewQuantumTime(){
	cout << "===== SET NEW QUANTUM TIME =====\n";
	cout << "Old quantum time: q = " << getTimeQuantum();
	int quantum = readIntegerFromOne("\n==>Enter new quantum time for Round-Robin simulation with normal processes: ");
	setTimeQuantum (quantum);
	cout << "New time quantum updated successfully (q new = "<< quantum <<")!";
	waitForEnter();
}

//Feature 7
void Scheduler::clearProcesses(){
	cout << "===== CLEAR ALL PROCESSES =====\n";
    if (nq.isEmpty() && pq.isEmpty() && history.empty()){
        cout << "\nThere are no processes to clear.\n";
        waitForEnter();
        return;
    }

    nq.clear();
    pq.clear();
    history.clear();
    
    if (!saveToFile()){
		cout << "Error while saving";
		waitForEnter();
		return;
	}

    cout << "\nAll processes and execution history have been cleared successfully.\n";
    waitForEnter();
}

//Feature 8
void Scheduler::displayProcesses() const{
    cout << "\n========== DISPLAY ALL PROCESSES ==========\n";
    
    cout << "\n========== NORMAL PROCESSES ==========\n";
    if (nq.isEmpty())
        cout << "***Normal process Queue is empty.\n";
    else{
        const QueueNode* temp = nq.getFront();
        while (temp != nullptr){
            const Process& process = temp->data;

            cout << "\nID:              " << process.id << '\n';
            cout << "Name:            " << process.name << '\n';
            cout << "Arrival Time:    " << process.arrivalTime << '\n';
            cout << "Burst Time:      " << process.burstTime << '\n';
            cout << "Remaining Time:  " << process.remainingTime << '\n';
            cout << "Priority:        " << process.priority << '\n';
            cout << "Completion Time: " << process.completionTime << '\n';
            cout << "Waiting Time:    " << process.waitingTime << '\n';

            temp = temp->next;
        }
    }

    cout << "\n========== PRIORITY PROCESS ==========\n";
    if (pq.isEmpty())
        cout << "***Priority process Queue is empty.\n";
    else{
        const QueueNode* temp = pq.getFront();

        while (temp != nullptr)
        {
            const Process& process = temp->data;

            cout << "\nID:              " << process.id << '\n';
            cout << "Name:            " << process.name << '\n';
            cout << "Arrival Time:    " << process.arrivalTime << '\n';
            cout << "Burst Time:      " << process.burstTime << '\n';
            cout << "Remaining Time:  " << process.remainingTime << '\n';
            cout << "Priority:        " << process.priority << '\n';
            cout << "Completion Time: " << process.completionTime << '\n';
            cout << "Waiting Time:    " << process.waitingTime << '\n';

            temp = temp->next;
        }
    }
    cout << "\n====================================\n";
    waitForEnter();
}

//Feature 9
void Scheduler::runSimulation(){
	cout << "\n========== RUN SIMULATION ==========\n";

    // 1. Check whether there are any processes
    if (nq.isEmpty() && pq.isEmpty()){
        cout << "\nThere are no processes to simulate.\n";
        waitForEnter();
        return;
    }

    // 2. Reset simulation
    currentTime = 0;
    history.clear();

    // 3. Copy all processes from Normal Queue and Priority Queue
    // IMPORTANT:
    // We only COPY the processes.
    // The original nq and pq will NOT be changed.
    vector<Process> processes;

    // Copy Normal Queue
    const QueueNode* temp = nq.getFront();
    while (temp != nullptr){
        Process process = temp->data;

        // Reset simulation information
        process.remainingTime = process.burstTime;
        process.completionTime = 0;
        process.waitingTime = 0;
        
        processes.push_back(process);
        temp = temp->next;
    }

    // Copy Priority Queue
    temp = pq.getFront();
    while (temp != nullptr){
        Process process = temp->data;

        // Reset simulation information
        process.remainingTime = process.burstTime;
        process.completionTime = 0;
        process.waitingTime = 0;

        processes.push_back(process);
        temp = temp->next;
    }

    // 4. Prepare simulation data
    int totalProcesses = static_cast<int>(processes.size());
    int completedProcesses = 0;

    /*
     * normalReady stores indexes of Normal Processes
     * that are currently ready to run.
     *
     * Example:
     *
     * processes:
     * index 0 -> P1
     * index 1 -> P2
     * index 2 -> P3
     *
     * normalReady:
     * [1, 0]
     *
     * means P2 runs first, then P1.
     */

    vector<int> normalReady;

    /*
     * inNormalReady[i] tells us whether process i
     * is currently inside normalReady.
     *
     * This prevents duplicate entries.
     */

    vector<bool> inNormalReady(
        totalProcesses,
        false
    );

    cout << "\n========== START SIMULATION ==========\n";
    // =========================================================
    // 5. Main simulation loop
    // =========================================================

    while (completedProcesses < totalProcesses){
        // =====================================================
        // 5.1 Add newly arrived Normal Processes
        // =====================================================

        for (int i = 0; i < totalProcesses; i++){
            // Only Normal Processes
            if (processes[i].priority != 0)
                continue;

            // Already completed
            if (processes[i].remainingTime <= 0)
                continue;

            // Process has not arrived yet
            if (processes[i].arrivalTime > currentTime)
                continue;

            // Avoid adding the same process twice
            if (!inNormalReady[i])
            {
                normalReady.push_back(i);
                inNormalReady[i] = true;
            }
        }

        // =====================================================
        // 5.2 Find the highest-priority arrived process
        // =====================================================

        int priorityIndex = -1;

        for (int i = 0; i < totalProcesses; i++){
            // Only Priority Processes
            if (processes[i].priority <= 0)
                continue;

            // Already completed
            if (processes[i].remainingTime <= 0)
                continue;

            // Has not arrived yet
            if (processes[i].arrivalTime > currentTime)
                continue;

            // First suitable Priority Process
            if (priorityIndex == -1)
            {
                priorityIndex = i;
                continue;
            }

            // Higher priority comes first
            if (processes[i].priority >
                processes[priorityIndex].priority)
            {
                priorityIndex = i;
            }
            // Same priority -> earlier arrival first
            else if (
                processes[i].priority ==
                processes[priorityIndex].priority &&
                processes[i].arrivalTime <
                processes[priorityIndex].arrivalTime)
            {
                priorityIndex = i;
            }
        }

        // =====================================================
        // 5.3 Run Priority Process
        //
        // Priority Process:
        // - Does NOT use timeQuantum
        // - Runs until completion
        // =====================================================

        if (priorityIndex != -1){
            Process& process = processes[priorityIndex];

            int startTime = currentTime;

            // Run the entire remaining time
            currentTime += process.remainingTime;

            process.remainingTime = 0;

            // Process has completed
            process.completionTime = currentTime;

            process.waitingTime =
                process.completionTime
                - process.arrivalTime
                - process.burstTime;

            // Save execution history
            history.push_back({
                process,
                startTime,
                currentTime
            });

            completedProcesses++;

            cout << "\nProcess "
                 << process.id
                 << " (" << process.name << ")"
                 << " [Priority = "
                 << process.priority
                 << "]"
                 << " runs from "
                 << startTime
                 << " to "
                 << currentTime
                 << " and completes.";

            /*
             * Priority Process is completed.
             *
             * Go back to the beginning of the loop
             * and check for another Priority Process.
             */

            continue;
        }

        // =====================================================
        // 5.4 No Priority Process is ready
        //
        // Run Normal Process using Round Robin
        // =====================================================

        if (!normalReady.empty()){
            // Get the first process in Ready Queue
            int normalIndex = normalReady.front();

            // Remove from front
            normalReady.erase(normalReady.begin());

            // It is no longer inside normalReady
            inNormalReady[normalIndex] = false;

            Process& process = processes[normalIndex];

            int startTime = currentTime;

            // =================================================
            // Run at most one time quantum
            // =================================================

            int runTime = min(
                timeQuantum,
                process.remainingTime
            );

            currentTime += runTime;

            process.remainingTime -= runTime;

            // =================================================
            // Save this CPU execution segment
            // =================================================

            history.push_back({ process, startTime, currentTime});

            cout << "\nProcess " << process.id << " (" << process.name << ")"
                 << " [Normal]" << " runs from " << startTime << " to " << currentTime;

            // =================================================
            // Process completed
            // =================================================

            if (process.remainingTime == 0){
                process.completionTime = currentTime;

                process.waitingTime =
                    process.completionTime
                    - process.arrivalTime
                    - process.burstTime;

                completedProcesses++;

                cout << " and completes.";
            }

            // =================================================
            // Process is not completed
            // =================================================

            else
            {
                /*
                 * Put the process at the END
                 * of the Round Robin Ready Queue.
                 */

                normalReady.push_back(normalIndex);

                inNormalReady[normalIndex] = true;

                cout << ". Remaining time: "
                     << process.remainingTime;
            }

            /*
             * Go back to the beginning.
             *
             * This is IMPORTANT because a Priority Process
             * may have arrived during this quantum.
             */

            continue;
        }

        // =====================================================
        // 5.5 No process is ready
        //
        // CPU is idle.
        //
        // Increase currentTime until another process arrives.
        // =====================================================

        currentTime++;
    }

    // =========================================================
    // 6. Simulation completed
    // =========================================================

    cout << "\n\n========== SIMULATION COMPLETED ==========\n";

    cout << "Total execution time: " << currentTime << '\n';

    waitForEnter();
}

//Feature 10
void Scheduler::displayExecutionHistory() const{
	cout << "\n===== DISPLAY EXECUTION HISTORY =====\n";
    if (history.empty()){
        cout << "\nExecution history is empty.\n";
        waitForEnter();
        return;
    }

    cout << "\\nn========== EXECUTION HISTORY ==========\n";
    for (const HistoryRecord& record : history){
        cout << "Process " << record.process.id << " (" << record.process.name << ")"
             << ": " << record.startTime << " -> " << record.endTime << '\n';
    }

    cout << "\n\n=============== GANTT CHART ===============\n|";
    for (const HistoryRecord& record : history){
        cout << " P" << record.process.id << " |";
    }

    cout << '\n';
    cout << history[0].startTime;

    for (const HistoryRecord& record : history){
        cout << "    " << record.endTime;
    }
    cout << "\n=============================================\n";
    waitForEnter();
}

//Feature 11
void Scheduler::displayStatistics() const{
	cout << "\n===== DISPLAY STATISTICS =====\n";
    if (history.empty()){
        cout << "\nThere is no simulation data to display statistics.\n";
        waitForEnter();
        return;
    }

    // Lưu process duy nhất dựa trên ID
    vector<Process> completedProcesses;

    for (const HistoryRecord& record : history){
        bool existed = false;

        for (Process& process : completedProcesses){
            if (process.id == record.process.id){
                // Record xuất hiện sau có endTime lớn hơn,
                // nên đây là completion time mới nhất.
                process.completionTime = record.endTime;
                existed = true;
                break;
            }
        }

        if (!existed){
            Process process = record.process;
            process.completionTime = record.endTime;
            completedProcesses.push_back(process);
        }
    }

    int totalProcesses = static_cast<int>(completedProcesses.size());

    int normalCount = 0;
    int priorityCount = 0;

    double totalWaitingTime = 0.0;
    double totalTurnaroundTime = 0.0;

    int minWaitingTime = completedProcesses[0].completionTime
                       - completedProcesses[0].arrivalTime - completedProcesses[0].burstTime;

    int maxWaitingTime = minWaitingTime;

    int minTurnaroundTime =
        completedProcesses[0].completionTime - completedProcesses[0].arrivalTime;

    int maxTurnaroundTime = minTurnaroundTime;

    double normalWaitingTime = 0.0;
    double priorityWaitingTime = 0.0;

    for (const Process& process : completedProcesses){
        int waitingTime = process.completionTime - process.arrivalTime - process.burstTime;

        int turnaroundTime = process.completionTime - process.arrivalTime;

        if (process.priority == 0){
            normalCount++;
            normalWaitingTime += waitingTime;
        }
        else{
            priorityCount++;
            priorityWaitingTime += waitingTime;
        }

        totalWaitingTime += waitingTime;
        totalTurnaroundTime += turnaroundTime;

        if (waitingTime < minWaitingTime)
            minWaitingTime = waitingTime;

        if (waitingTime > maxWaitingTime)
            maxWaitingTime = waitingTime;

        if (turnaroundTime < minTurnaroundTime)
            minTurnaroundTime = turnaroundTime;

        if (turnaroundTime > maxTurnaroundTime)
            maxTurnaroundTime = turnaroundTime;
    }

    double averageWaitingTime =
        totalWaitingTime / totalProcesses;

    double averageTurnaroundTime =
        totalTurnaroundTime / totalProcesses;

    cout << "\n========== STATISTICS ==========\n";

    cout << "\nTotal processes:             " << totalProcesses << '\n';
    cout << "Normal processes:            " << normalCount << '\n';
    cout << "Priority processes:          " << priorityCount << '\n';

    cout << "\n------ WAITING TIME ------\n";

    cout << "Average waiting time:        " << fixed << setprecision(2) << averageWaitingTime << '\n';
    cout << "Minimum waiting time:        " << minWaitingTime << '\n';
    cout << "Maximum waiting time:        " << maxWaitingTime << '\n';

    cout << "\n------ TURNAROUND TIME ------\n";

    cout << "Average turnaround time:     " << averageTurnaroundTime << '\n';
    cout << "Minimum turnaround time:     " << minTurnaroundTime << '\n';
    cout << "Maximum turnaround time:     " << maxTurnaroundTime << '\n';

    cout << "\n------ BY QUEUE ------\n";

    cout << "Normal processes:\n";
    cout << "    Count:                   " << normalCount << '\n';
    if (normalCount > 0){
        cout << "    Average waiting time:    " << normalWaitingTime / normalCount << '\n';
    }
    else{
        cout << "    Average waiting time:    N/A\n";
    }

    cout << "\nPriority processes:\n";
    cout << "    Count:                   "<< priorityCount << '\n';
    if (priorityCount > 0){
        cout << "    Average waiting time:    "
             << priorityWaitingTime / priorityCount
             << '\n';
    }
    else{
        cout << "    Average waiting time:    N/A\n";
    }
    cout << "\n================================\n";
    waitForEnter();
}