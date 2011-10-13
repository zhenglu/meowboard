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

#ifndef TOURNEYEXCEPTION_HH
#define TOURNEYEXCEPTION_HH

#include <stdexcept>

class tourney_exception : public std::runtime_error {
	public:
		tourney_exception(const std::string& msg) : runtime_error(msg) { }
};

class result_error : public tourney_exception {
	public:
		result_error(const std::string& msg) : tourney_exception(msg) { }
};

class tourney_over : public tourney_exception {
	public:
		tourney_over(const std::string& msg) : tourney_exception(msg) { }
};

#endif
