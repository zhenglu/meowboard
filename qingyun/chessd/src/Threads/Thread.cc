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

#include "Thread.hh"

namespace Threads {

    /* Is 256Kb enough? */
    const size_t StackSize = 1 << 18;

    Thread::Thread() :
        running(false) { }

    Thread::~Thread() {
        if(this->running) {
            this->join();
        }
    }

    void* Thread::start_routine(void* _thread) {
        Thread* thread = static_cast<Thread*>(_thread);
        thread->run();
        return 0;
    }

    bool Thread::start() {
        pthread_attr_t thread_attr;
        pthread_attr_init(&thread_attr);
        pthread_attr_setstacksize(&thread_attr, StackSize);
        if(pthread_create(&this->thread, &thread_attr, Thread::start_routine, static_cast<void*>(this))==0) {
            this->running = true;
        } else {
            this->running = false;
        }
        return this->running;
    }

    void Thread::join() {
        pthread_join(this->thread, 0);
    }
}
