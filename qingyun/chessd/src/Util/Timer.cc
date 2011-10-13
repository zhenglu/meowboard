/*
 *   Copyright (c) 2007-2008 C3SL.
 *
 *   This file is part of Chessd.
 *
 *   Chessd is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Chessd is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 */

#include "Timer.hh"

#include "utils.hh"

#include <sys/time.h>

#include <ctime>
#include <cmath>

namespace Util {

	static const long long one_nanosecond = 1ll;
    static const long long one_microsecond = one_nanosecond * 1000ll;
	static const long long one_milisecond = one_microsecond * 1000ll;
	static const long long one_second = one_milisecond * 1000ll;
	static const long long one_minute = one_second * 60ll;
	static const long long one_hour = one_minute * 60ll;
	static const long long one_day = one_hour * 24ll;

	Time Timer::now() {
		/*timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		return Time(ts.tv_sec, ts.tv_nsec);*/
		timeval tv;
		gettimeofday(&tv, 0);
		return Time::Nanoseconds(tv.tv_sec * one_second + tv.tv_usec*one_microsecond);
	}
	
	const Time& Time::operator += (Time time) {
		this->_nanoseconds += time._nanoseconds;
		return *this;
	}

	const Time& Time::operator -= (Time time) {
		this->_nanoseconds -= time._nanoseconds;
		return *this;
	}


	Time Time::operator+(Time time) const {
        return Time(this->_nanoseconds + time._nanoseconds);
	}

	Time Time::operator-(Time time) const {
        return Time(this->_nanoseconds - time._nanoseconds);
	}


	const Time& Time::operator=(Time time) {
		this->_nanoseconds = time._nanoseconds;
		return *this;
	}


	bool Time::operator==(Time time) const {
		return this->_nanoseconds == time._nanoseconds;
	}

	bool Time::operator!=(Time time) const {
		return this->_nanoseconds != time._nanoseconds;
	}

	bool  Time::operator>=(Time time) const {
		return this->_nanoseconds >= time._nanoseconds;
	}

	bool  Time::operator<=(Time time) const {
		return this->_nanoseconds <= time._nanoseconds;
	}

	bool  Time::operator>(Time time) const {
		return this->_nanoseconds > time._nanoseconds;
	}

	bool  Time::operator<(Time time) const {
		return this->_nanoseconds < time._nanoseconds;
	}

	long long Time::getSeconds() const {
		return this->_nanoseconds / one_second;
	}

	long long Time::getMiliseconds() const {
		return this->_nanoseconds / one_milisecond;
	}

	long long Time::getMicroseconds() const {
		return this->_nanoseconds / one_microsecond;
	}

	Time Time::Hours(long long constant) {
        return Time(constant * one_hour);
	}

	Time Time::Minutes(long long constant) {
        return Time(constant * one_minute);
	}

	Time Time::Seconds(long long constant) {
        return Time(constant * one_second);
	}

	Time Time::Miliseconds(long long constant) {
        return Time(constant * one_milisecond);
	}

	Time Time::Microseconds (long long constant) {
        return Time(constant * one_microsecond);
	}

	Time Time::Nanoseconds (long long constant) {
        return Time(constant);
	}

    Time Time::Seconds(const std::string& constant) {
        return Time(parse_string<long long>(constant) * one_second);
	}

	Time Time::Miliseconds(const std::string& constant) {
        return Time(parse_string<long long>(constant) * one_milisecond);
	}

	Time Time::Microseconds(const std::string& constant) {
        return Time(parse_string<long long>(constant) * one_microsecond);
	}
}
