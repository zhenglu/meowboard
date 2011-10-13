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

#include "Pool.hh"
#include "Util/utils.hh"

#include <algorithm>

#include <boost/bind.hpp>

#include <cassert>

using namespace std;

namespace Threads {

    /* Is 256Kb enough? */
	const size_t StackSize = 1 << 18;

	Pool::Pool() :
        idle_count(0),
        running(true) { }

	Pool::~Pool() {
        /* acquire lock */
        this->cond.lock();

        /* signal all threads to stop*/
        this->running = false;
        this->cond.broadcast();

        /* release the lock */
        this->cond.unlock();

        /* wait then to stop completely */
        WriteLock<vector<PoolThread*> > threads(this->threads);
		foreach(thread, *threads) {
            (*thread)->join();
		}

        /* free resources */
		foreach(thread, *threads) {
            delete *thread;
		}
	}

    Pool::PoolThread::PoolThread(Pool& pool) :
        pool(pool) { }

    void Pool::PoolThread::run() {
        Task* task;
        while(1) {
            /* acquire lock */
            pool.cond.lock();

            /* if not running anymore, abort */
            if(not pool.running) {
                pool.cond.unlock();
                break;
            }
            
            /* if queue is empty, wait a task */
            while(pool.tasks.empty()) {
                pool.idle_count++;
                pool.cond.wait();

                /* if the thread was awakened to stop, then stop */
                if(not pool.running) {
                    pool.cond.unlock();
                    return;
                }
            }

            assert(not pool.tasks.empty());

            /* if not, there is a task in the queue */
            task = pool.tasks.front();
            pool.tasks.pop();

            /* release the lock */
            pool.cond.unlock();

            /* run the task */
            task->run();
        }
    }

	void Pool::launchTask(Task& task) {
        /* acquire lock first */
        this->cond.lock();

        /* put the task in the queue */
        this->tasks.push(&task);

        if(this->idle_count > 0) {
            /* if there is a thread wating, send a signal */
            this->idle_count--;
            this->cond.signal();
        } else {
            /* if not, create a new thread */
            PoolThread* thread = new PoolThread(*this);
            if(not thread->start()) {
                /* if the thread has failed to start, the task
                 * will have to wait for a thread to be done */
                delete thread;
            } else {
                /* put the thread in the thread list */
                WriteLock<vector<PoolThread*> > threads(this->threads);
                threads->push_back(thread);
            }
        }
        this->cond.unlock();
	}
}
