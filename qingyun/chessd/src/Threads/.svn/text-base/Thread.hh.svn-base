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

#ifndef THREADS_HH
#define THREADS_HH

#include <pthread.h>

#include "Task.hh"

#include "Queue.hh"

#include <boost/function.hpp>

namespace Threads {

    class Thread {
        public:
            Thread();

            virtual ~Thread();

            bool start();

            void join();

        protected:
            virtual void run() = 0;

            static void* start_routine(void* thread);

        private:
            volatile bool running;

            pthread_t thread;
    };

}


#endif
