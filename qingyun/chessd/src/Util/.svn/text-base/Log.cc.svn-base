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

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include "Log.hh"

using namespace std;
using namespace Util;

Log Util::log;

Log::Log() : output(&cout) {
    this->dispatcher.start();
}

Log::~Log() {
    this->dispatcher.stop();
}

void Log::setOutput(ostream& output) {
    this->output = &output;
}

void Log::log(const std::string& msg) {
    this->dispatcher.queue(boost::bind(&Log::_log, this, msg));
}

void Log::_log(const std::string& msg) {
    *(this->output) << boost::posix_time::second_clock::local_time() << ": " << msg << endl;
}
