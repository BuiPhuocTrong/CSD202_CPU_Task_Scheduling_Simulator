# CPU Task Scheduling Simulator

A C++ console program that simulates CPU scheduling for two kinds of processes (course CSD202).

| Queue | Processes | Rule |
|---|---|---|
| **NormalQueue** | priority = 0 | Round Robin, time quantum `q` (default 2) |
| **PriorityQueue** | priority >= 1 | Highest priority first, **non-preemptive** (tie: smaller BT, then smaller ID) |

Both queues are hand-written sorted linked lists (no `std::queue`). The two queues are simulated separately, both starting at time 0.

## Build and run

- **Dev-C++ 6.3:** open `CPU_Task_Scheduling_Simulator.dev`, then **Execute > Rebuild All**.
- **g++:** `g++ -std=c++17 src/*.cpp -o sim`
- Run from the project folder (the program reads `data/`). Windows only (`system("cls")`).
- After editing any `.h` file, always **Rebuild All** (old `.o` files can crash the program).

## Menu

| # | Function | # | Function |
|---|---|---|---|
| 1 | Add process | 7 | Clear all (makes a backup) |
| 2 | Remove process | 8 | Display all processes |
| 3 | Update process | 9 | **Run simulation** |
| 4 | Show process by ID | 10 | Execution history (Gantt chart) |
| 5 | Search by name | 11 | Statistics (WT, TAT) |
| 6 | Set time quantum | 12 | Recover from backup |
| 0 | Exit | | |

Use **9 → 10 → 11** to run, see the Gantt chart, then see the statistics.

## Data files

`data/normal.txt` and `data/priority.txt`, one process per line:

```
id,name,arrival,burst,priority
1,A,0,4,0
```

- ID unique and >= 1, arrival >= 0, burst >= 1.
- `normal.txt` needs priority 0; `priority.txt` needs priority >= 1.
- Files are loaded at start and saved after every change.
- Option 7 writes `deleted_file_backup_<time>.txt`; option 12 restores it.

## Terms

`AT` arrival time, `BT` burst time, `RT` remaining time, `CT` completion time,
`TAT = CT - AT`, `WT = TAT - BT`.

## Sample result

With the data in `data/` and q = 2: average WT = 3.40, average TAT = 8.60.
Normal Gantt: `A 0-2, C 2-4, B 4-6, A 6-8, B 8-10, B 10-11`. Priority Gantt: `D 2-12, E 12-17`.
A `NO PROCESS ARRIVAL` block marks time when the CPU is idle.

## Project layout

```
header/   .h files        src/   .cpp files        data/   input files
queue     Queue, NormalQueue, PriorityQueue (linked lists)
scheduler Scheduler: the 12 functions, simulation, history, statistics
process, validation, consoleInput, consoleOutput, constants, main
```

## Limits

- No GUI, no preemptive priority, no aging, no I/O.
- If input ends (EOF), the program exits.
