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

#ifndef FILA_HH
#define FILA_HH 

#include "Mutex.hh"
#include "Condition.hh"

#include <queue>
#include <semaphore.h>

namespace Threads {

    template <class T>
        class Queue {
            private:
                std::queue<T> queue;
                Condition condition;
            public:
                Queue() { }
                ~Queue() { }
                T pop() {
                    condition.lock();
                    while(queue.empty()) {
                        condition.wait();
                    }
                    T tmp = queue.front();
                    queue.pop();
                    condition.unlock();
                    return tmp;
                }
                bool try_pop(T& item, Util::Time time) {
                    bool ret;

                    condition.lock();
                    if(queue.empty() and not condition.wait(time)) {
                        ret = false;
                    } else {
                        item = queue.front();
                        queue.pop();
                        ret = true;
                    }
                    condition.unlock();
                    return ret;
                }
                bool try_pop(T& item) {
                    bool ret;

                    condition.lock();
                    if(not queue.empty() > 0) {
                        ret = true;
                        item = queue.front();
                        queue.pop();
                    } else {
                        ret = false;
                    }
                    condition.unlock();
                    return ret;
                }
                void push(const T& t) {
                    condition.lock();
                    queue.push(t);
                    condition.signal();
                    condition.unlock();
                }
        };
}

#endif
