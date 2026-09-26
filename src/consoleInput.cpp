#include <iostream>
#include <limits>
#include "../header/consoleInput.h"
#include "../header/validation.h"
#include "../header/constants.h"

#include <string>

using namespace std;

//Input integer in range
int readIntegerInRange(int min, int max, const string& prompt) {
    string input;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (!isIntegerBelongRange(input, min, max)) {
            cout << "Only enter a number from "
                 << min << " to " << max << "." << endl;
            continue;
        }

        return stoi(input);
    }
}

int readIntegerFromOne(const string& prompt){
	return readIntegerInRange(1, MAX_INT_RANGE, prompt);
}
int readIntegerFromZero(const string& prompt){
	return readIntegerInRange(0, MAX_INT_RANGE, prompt);
}

string readString(const string& prompt) {
    string input;

    while (true)
    {
        cout << prompt;
        getline(cin, input);

        if (input.empty())
        {
            cout << "Input cannot be empty.\n";
            continue;
        }

        return input;
    }
}

Process readProcessFromConsole(int id) { // not including input ID, bcs we need to check existed ID
    string name = readString("Process name: ");

    int typeChoice, arrival, burst;

    do {
        arrival = readIntegerFromZero("Arrival time: ");
    } while (!isValidArrivalTime(arrival));

    do {
        burst = readIntegerFromOne("Burst time: ");
    } while (!isValidBurstTime(burst));
    
    do {
        typeChoice = readIntegerInRange(1, 2, "Process type (1 = Normal, 2 = System[Priority]): ");
    } while (typeChoice != 1 && typeChoice != 2);

    int priority = 0;
    if (typeChoice == 2) {
        do {
            priority = readIntegerFromOne("Priority (1 and above, higher value = higher priority): ");
        } while (!isValidPriority(priority));
    }
    else {
    	priority = 0;
	}

    return Process(id, name, arrival, burst, priority);
}

void waitForEnter(){
    cout << "\nPress Enter to return...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}