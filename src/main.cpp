#include <iostream>
#include <cstdlib>
#include "../header/consoleInput.h"
#include "../header/consoleOutput.h"
#include "../header/scheduler.h"
#include "../header/validation.h"

using namespace std;

int main() {
    Scheduler scheduler;
    
    //Load file
    if (!scheduler.loadFromFile()){
        cout << "Failed to load data.\n";
		waitForEnter();
    }

	int choice;
    do {
    	system("cls");
        showMenu();
        choice = readIntegerInRange(0, 11, ">>> Enter your choice: ");

		system("cls");
        switch (choice) {
            case 1:
                scheduler.addProcess();
                break;

            case 2:
                scheduler.removeProcess();
                break;

            case 3:
                scheduler.updateProcess();
                break;

            case 4:
                scheduler.displayProcessById();
                break;

            case 5:
                scheduler.displayProcessByName();
                break;

            case 6:
                scheduler.setNewQuantumTime();
                break;

            case 7:
                scheduler.clearProcesses();
                break;
                
            case 8:
                scheduler.displayProcesses();
                break;
                
            case 9:
                scheduler.runSimulation();
                break;
                
            case 10:
                scheduler.displayExecutionHistory();
                break;    
            case 11:
                scheduler.displayStatistics();
                break;      

            case 0:
                cout <<"Exiting program...";
                break;

            default:
                cout <<"Invalid menu choice.";
        }
    } while (choice != 0);

    return 0;
}
