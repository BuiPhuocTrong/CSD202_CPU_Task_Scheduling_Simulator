# CPU Task Scheduling Simulator

A C++ console program that simulates how a CPU schedules processes (course CSD202).
It uses two hand-written queues (linked lists, no `std::queue`):

| Queue | Holds | Scheduling rule |
|---|---|---|
| **NormalQueue** | normal processes (priority = 0) | **Round Robin**, time quantum `q` (default 2) |
| **PriorityQueue** | system tasks (priority >= 1) | **Highest priority first, non-preemptive** (tie: smaller BT, then smaller ID) |

The two queues are simulated separately and both start at time 0. Windows only (`system("cls")`).

## Folder structure

```
Project/
├── CPU_Task_Scheduling_Simulator.dev   Dev-C++ project file (open this)
├── CPU_Task_Scheduling_Simulator.exe   compiled program (made by the build)
├── Makefile.win, *.layout              Dev-C++ build / layout files
├── README.md
│
├── data/                               input files, read at start, saved after every change
│   ├── normal.txt                      normal processes (priority 0)
│   └── priority.txt                    system tasks (priority >= 1)
│
├── header/                             declarations (.h)
│   ├── process.h                       struct Process (id, name, AT, BT, RT, priority, CT, WT)
│   ├── queue.h                         QueueNode, Queue (base), NormalQueue, PriorityQueue
│   ├── scheduler.h                     class Scheduler, struct HistoryRecord
│   ├── consoleInput.h                  keyboard input functions
│   ├── consoleOutput.h                 menu printing
│   ├── validation.h                    input checks
│   └── constants.h                     shared constants
│
└── src/                                implementations (.cpp)
    ├── main.cpp                        menu loop: calls Scheduler functions F1-F12
    ├── scheduler.cpp                   all 12 features, simulation, history, statistics, file I/O
    ├── queue.cpp                       sorted insert, dequeue, find/remove by ID, clear
    ├── process.cpp                     Process constructors
    ├── consoleInput.cpp                read numbers/text with error checking, build a Process
    ├── consoleOutput.cpp               showMenu()
    └── validation.cpp                  isIntegerBelongRange() and value checks
```

## Terms

`ID` process number | `AT` arrival time | `BT` burst time (CPU time needed) | `RT` remaining time |
`CT` completion time | `TAT = CT - AT` | `WT = TAT - BT` (time spent waiting).

## Features (menu)

Typed numbers must be whole numbers up to 99,999,999. Wrong input is rejected and asked again.

| # | Feature | What it does |
|---|---|---|
| 1 | **Add process** | Asks how many to add (0 = back). For each: ID (>= 1, must be unused), name, AT (>= 0), BT (>= 1), type (1 = normal, 2 = system), and priority (>= 1) for system tasks. The process goes to the right queue in sorted place. Files are saved at the end. |
| 2 | **Remove process** | Asks the ID (0 = back), removes it from whichever queue holds it, saves. |
| 3 | **Update process** | Asks the ID (0 = back), then what to change: 1 name, 2 AT, 3 BT, 4 priority, 5 everything. Changing priority between 0 and >= 1 moves the process to the other queue. Saves. |
| 4 | **Show by ID** | Shows one process (name, type, priority, AT, BT). After a simulation it also shows RT, CT, WT. |
| 5 | **Search by name** | Finds all processes whose name **contains** the text (case-sensitive). Press Enter with no text to go back. |
| 6 | **Set time quantum** | Shows the old `q` and asks for a new one (>= 1). Used the next time you run feature 9. |
| 7 | **Clear all** | Deletes all processes and the history **at once (no confirmation)**, but first writes a backup file `deleted_file_backup_<sec>_<min>_<hour>_<day>_<month>_<year>.txt` in the program folder. Both data files become empty. |
| 8 | **Display all** | Lists Normal then Priority processes. Before a simulation it says "not run simulation yet"; after it shows RT, CT and WT of each process. |
| 9 | **Run simulation** | Simulates both queues from time 0 (see rules below). Can be run again and again with the same result. It only stores the results; use 10 and 11 to see them. |
| 10 | **Execution history** | Vertical Gantt chart for each queue: time, then a block per time slice (ID, name, AT, BT, RT after the slice), then the end time. A block **NO PROCESS ARRIVAL** appears when the CPU was idle (at the start or between two slices). Run 9 first. |
| 11 | **Statistics** | Number of processes; average, minimum and maximum of WT and TAT; average WT of each queue (N/A if a queue is empty). Run 9 first. |
| 12 | **Recover backup** | Asks the backup file path. Checks the format, duplicate IDs, and that Normal has priority 0 and Priority has priority >= 1. If valid, replaces the current data and saves; if not, shows an error and changes nothing. |
| 0 | **Exit** | Quits. Data is already saved after each change. |

Typical use: `9` (run) → `10` (Gantt chart) → `11` (statistics).

## Scheduling rules used by feature 9

- **Normal (Round Robin):** processes wait in arrival order; the first one runs for at most `q` time units, then goes to the back if not finished. Processes that arrive during a slice join the line **before** the one that was just paused.
- **Priority (non-preemptive):** among processes that have arrived, pick the largest priority (tie: smaller BT, then smaller ID) and run it to the end. A higher-priority process that arrives later must wait.
- If nobody has arrived yet, the CPU idles until the next arrival.
- Sort order inside the queues: Normal by AT, BT, ID; Priority by AT, higher priority, BT, ID.

## Data files

One process per line: `id,name,arrival,burst,priority`

```
1,A,0,4,0      <- normal.txt (priority must be 0)
4,D,2,10,4     <- priority.txt (priority must be >= 1)
```

If a file is missing or has a bad line, the program shows an error and starts empty (the files are not changed until you add data).

## Sample result

With the sample data and q = 2: average WT = 3.40, average TAT = 8.60.

| Queue | Gantt chart |
|---|---|
| Normal | A 0-2, C 2-4, B 4-6, A 6-8, B 8-10, B 10-11 |
| Priority | idle 0-2, D 2-12, E 12-17 |

## Limits

- No GUI, no preemptive priority, no aging, no I/O time.
- The two queues do not compete for one CPU (they are simulated independently).
- If the input stream ends (EOF), the program exits.
