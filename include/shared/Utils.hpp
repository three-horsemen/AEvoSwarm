//
// Created by reubenjohn on 9/11/16.
//

#ifndef AEVO_UTILS_HPP
#define AEVO_UTILS_HPP

#include <string>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#include <iostream>

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

	static int modulo(int, int);

};

template<class T>
inline ostream &operator<=(ostream &stream, const T &t) {
	return stream.write((const char *) &t, sizeof(t));
}

template<class T>
inline istream &operator>=(istream &stream, T &t) {
	return stream.read((char *) &t, sizeof(t));
}

#endif //AEVO_UTILS_HPP
