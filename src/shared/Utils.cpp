//
// Created by reubenjohn on 9/11/16.
//

#include "shared/Utils.hpp"

long Utils::getCurrentTimeMillis() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	return ms;
}

string Utils::getFormattedEpochTime(long millisFromEpoch) {
	std::time_t tt = static_cast<time_t>(millisFromEpoch / 1000);
	boost::posix_time::ptime sentAt = boost::posix_time::from_time_t(tt);
	return boost::posix_time::to_iso_string(sentAt);
}

bool Utils::isNumber(const std::string &s) {
	return !s.empty() && std::find_if(s.begin(),
									  s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

void Utils::displayInBoldAndRed(std::string message) {
	string displayMessage = string("\033[1;31m") + message + string("\033[0m");
	cout << displayMessage;
}

void Utils::displayInBoldAndViolet(std::string message) {
	string displayMessage = string("\033[1;36m") + message + string("\033[0m");
	cout << displayMessage;
}

std::string Utils::getPassword() {
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	cout<<"Password: ";
	std::string password;
	getline(cin, password);
	(void) tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
	return password;
}
