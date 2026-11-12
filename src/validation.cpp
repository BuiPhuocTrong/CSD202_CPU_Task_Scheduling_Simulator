#include "../header/validation.h"

bool isIntegerBelongRange(const std::string& s, int min, int max) {
    if (s.empty()) return false;
    size_t i = 0;
    bool negative = false;
    if (s[0] == '-' || s[0] == '+') {
        negative = (s[0] == '-');
        i++;
    }
    if (i == s.length()) return false;
    long long num = 0;
    for (; i < s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
        num = num * 10 + (s[i] - '0');
        long long value = negative ? -num : num;
        if (value < min || value > max) return false;
    }
    return true;
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
