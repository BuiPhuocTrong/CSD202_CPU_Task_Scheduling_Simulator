# CPU Task Scheduling Simulator

A C++ console application that simulates CPU task scheduling using Queue and Priority Queue.

## Features

* **Normal Queue**: Round Robin scheduling with Time Quantum.
* **Priority Queue**: Handles System Tasks based on priority.
* Execution History and simple Gantt Chart.
* Scheduling statistics such as Waiting Time and Turnaround Time.

## Project Structure

```text
CPU-Task-Scheduling-Simulator/
│
├── header/
│   ├── constants.h
│   ├── process.h
│   ├── queue.h
│   ├── scheduler.h
│   ├── consoleInput.h
│   ├── validation.h
│   └── consoleOutput.h
│
├── src/
│   ├── process.cpp
│   ├── queue.cpp
│   ├── scheduler.cpp
│   ├── consoleInput.cpp
│   ├── validation.cpp
│   ├── consoleOutput.cpp
│   └── main.cpp
│
├── README.md
└── CPU-Task-Scheduling-Simulator.dev
```

## Scheduling Model

```text
System Tasks
     ↓
Priority Queue
     ↓
Highest Priority First
     ↓
    CPU

Normal Tasks
     ↓
Normal Queue
     ↓
Round Robin
     ↓
    CPU
```

Higher priority value means higher priority.

## Technologies

* C++
* Dev-C++ 6.3
* Console Application
* No GUI or external libraries
