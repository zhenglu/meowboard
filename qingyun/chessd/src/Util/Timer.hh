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

#ifndef UTILTIMER_HH
#define UTILTIMER_HH

#include <string>
#include <time.h>

namespace Util {

	class Time;

	class Timer {
		public:

			/* returns current time */
			static Time now();

			/* block the given time */
			void block(Time tempo);
	};

	/* time units */
	struct _Microseconds { };
	struct _Miliseconds { };
	struct _Seconds { };
	struct _Minutes { };

	/* time facilities */
	extern const _Microseconds& Microseconds;
	extern const _Miliseconds& Miliseconds;
	extern const _Seconds& Seconds;
	extern const _Minutes& Minutes;

	class Time {
		private:

            long long _nanoseconds;

			Time(long long _nanoseconds) :
				_nanoseconds(_nanoseconds) { }

		public:

			Time() : _nanoseconds(0) { }

			Time(const Time& time) :
				_nanoseconds(time._nanoseconds) { }

            static Time Hours(long long);
            static Time Minutes(long long);
            static Time Seconds(long long);
            static Time Miliseconds(long long);
            static Time Microseconds(long long);
            static Time Nanoseconds(long long);

            static Time Seconds(const std::string&);
            static Time Miliseconds(const std::string&);
            static Time Microseconds(const std::string&);

			const Time& operator+=(Time time);
			const Time& operator-=(Time time);

			Time operator+(Time time) const;
			Time operator-(Time time) const;

			const Time& operator=(Time time);

			bool operator == (Time time) const;
			bool operator != (Time time) const;
			bool operator >= (Time time) const;
			bool operator <= (Time time) const;
			bool operator > (Time time) const;
			bool operator < (Time time) const;

			/* returns time */
			long long getSeconds() const;
			long long getMiliseconds() const;
			long long getMicroseconds() const;

            timespec getTimespec() const {
                timespec ret;
                ret.tv_sec = this->_nanoseconds/1000000000ll;
                ret.tv_nsec = this->_nanoseconds%1000000000ll;
                return ret;
            }
	};
}

#endif
