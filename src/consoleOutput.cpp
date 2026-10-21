#include <iostream>
#include "../header/consoleOutput.h"

using namespace std;
void showMenu() {
    cout << "\n========== CPU Task Scheduling Simulator ==========\n";
    cout << "1.  Add process                                   =\n";
    cout << "2.  Remove process                                =\n";
    cout << "3.  Update process                                =\n";
    cout << "4.  Display process by ID                         =\n";
    cout << "5.  Display process by name                       =\n";
    cout << "6.  Set time quantum                              =\n";
    cout << "7.  Clear all processes                           =\n";
    cout << "8.  Display all processes after sim               =\n";
    cout << "9.  Run simulation                                =\n";
    cout << "10. Display execution history                     =\n";
    cout << "11. Display statistics                            =\n";
    cout << "0. Exit                                           =";
    cout << "\n===================================================\n";
}
