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

#ifndef LOG_HH
#define LOG_HH

#include <ostream>
#include <string>

#include "Threads/Dispatcher.hh"

namespace Util {
    class Log {
        public:

            Log();

            ~Log();

            void setOutput(std::ostream& output);

            void log(const std::string& msg);
        private:

            void _log(const std::string& msg);

            std::ostream* output;

            Threads::Dispatcher dispatcher;
    };

    extern Log log;
}


#endif
