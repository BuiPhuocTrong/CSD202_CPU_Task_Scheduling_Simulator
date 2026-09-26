#include "../header/validation.h"

bool isIntegerBelongRange(const std::string& s, int min, int max) {
    try {
        size_t pos;
        int num = stoi(s, &pos);

        // Check if the whole string is an integer
        if (pos != s.length())
            return false;

        return (num >= min && num <= max);
    }
    catch (...) {
        return false;
    }
}

bool isValidBurstTime(int burstTime) {
    return burstTime > 0;
}

bool isValidArrivalTime(int arrivalTime) {
    return arrivalTime >= 0;
}

bool isValidPriority(int priority) {
    return priority >= 1;
}

bool isValidTimeQuantum(int quantum) {
    return quantum > 0;
}
