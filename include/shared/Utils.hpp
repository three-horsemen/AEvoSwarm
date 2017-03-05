//
// Created by reubenjohn on 9/11/16.
//

#ifndef SFTP_UTILS_HPP
#define SFTP_UTILS_HPP

#include <string>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

class Utils {
public:
	static long getCurrentTimeMillis();

	static string getFormattedEpochTime(long);

	static bool isNumber(const std::string &s);

	static void displayInBoldAndRed(std::string message);

	static void displayInBoldAndViolet(std::string message);

	static std::string getPassword();

};


#endif //SFTP_UTILS_HPP
