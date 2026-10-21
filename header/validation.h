#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

bool isIntegerBelongRange(const std::string& s, int min, int max);
bool isValidBurstTime(int burstTime);
bool isValidArrivalTime(int arrivalTime);
bool isValidPriority(int priority);
bool isValidTimeQuantum(int quantum);

#endif
