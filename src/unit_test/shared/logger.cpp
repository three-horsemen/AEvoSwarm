#include "shared/logger.hpp"
#include <iostream>
using namespace std;

int main() {
	cout << "Initiating logger test.\n";
	LOG_TRACE << "this is a trace message";
	LOG_DEBUG << "this is a debug message";
	LOG_INFO << "this is an info message";
	LOG_WARNING << "this is a warning message";
	LOG_ERROR << "this is an error message";
	LOG_FATAL << "this is a fatal error message";
	cout << "Logger completed.\n";
	return 0;
}
