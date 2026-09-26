#ifndef CONSOLE_INPUT_H
#define CONSOLE_INPUT_H

#include "process.h"
#include <string>

int readIntegerInRange(int min, int max, const std::string& prompt);
int readIntegerFromOne(const std::string& prompt);
int readIntegerFromZero(const std::string& prompt);
std::string readString(const std::string& prompt);
Process readProcessFromConsole(int id);
void waitForEnter();

#endif
