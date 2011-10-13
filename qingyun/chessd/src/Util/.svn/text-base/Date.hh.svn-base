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

#ifndef UTIL_DATE_HH
#define UTIL_DATE_HH

#include <boost/date_time/posix_time/posix_time.hpp>

#include <string>

#include "Util/Timer.hh"

namespace Util {

    time_t ptime_to_time_t(boost::posix_time::ptime t);

    std::string ptime_to_xmpp_date(boost::posix_time::ptime pt);

    std::string ptime_to_xmpp_time(boost::posix_time::ptime pt);

    std::string ptime_to_xmpp_date_time(boost::posix_time::ptime pt);

    time_t xmpp_date_time_to_time_t(const std::string& date);

    boost::posix_time::ptime xmpp_date_time_to_ptime(
            const std::string& xmpp_date_time);

    boost::posix_time::ptime ptime_local_time();

    Time ptime_to_systime(boost::posix_time::ptime pt);

    boost::posix_time::ptime systime_to_ptime(Time pt);

}

#endif
