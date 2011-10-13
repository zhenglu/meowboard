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

#ifndef GAMEEXCEPTIONS_HH
#define GAMEEXCEPTIONS_HH

#include <stdexcept>

class game_exception : public std::runtime_error {
	public:
		game_exception(const std::string &msg) : runtime_error(msg) { }
};

class missing_information : public game_exception {
	public:
		missing_information(const std::string &msg) : game_exception(msg) { }
};

class bad_information : public game_exception {
	public:
		bad_information(const std::string &msg) : game_exception(msg) { }
};

class wrong_turn : public game_exception {
	public:
		wrong_turn(const std::string &msg) : game_exception(msg) { }
};

class invalid_move : public game_exception {
	public:
		invalid_move(const std::string &msg) : game_exception(msg) { }
};

class time_over : public game_exception {
	public:
		time_over(const std::string &msg) : game_exception(msg) { }
};

class game_over : public game_exception {
	public:
		game_over(const std::string &msg) : game_exception(msg) { }
};

#endif
