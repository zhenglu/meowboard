
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

#ifndef DATABASEEXCEPTION_HH
#define DATABASEEXCEPTION_HH

class database_exception : public std::runtime_error {
    public:
        explicit database_exception(const std::string& msg) : std::runtime_error(msg) { }
};

class game_not_found : public database_exception {
    public:
        explicit game_not_found(const std::string& msg) : database_exception(msg) { }
};

class user_not_found : public database_exception {
    public:
        explicit user_not_found(const std::string& msg) : database_exception(msg) { }
};

#endif
