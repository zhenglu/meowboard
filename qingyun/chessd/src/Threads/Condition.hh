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

#ifndef CONDITION_HH
#define CONDITION_HH

#include <pthread.h>

#include "Util/Timer.hh"
#include "Mutex.hh"

#include <stdexcept>

namespace Threads {

	class Condition {
		private:
			pthread_cond_t condition;
            Mutex mutex;
		public:
			Condition() {
                //pthread_condattr_t cond_attribute;

                /* init the condition attribute object */
                /* TODO, the attribute could be cached so
                 * we don't have to init it everytime */
                //pthread_condattr_init(&cond_attribute);

                /* set the condition clock to the monotonic clock,
                 * so that we don't run into trubles if someone
                 * modify the system clock */
                //pthread_condattr_setclock(&cond_attribute, CLOCK_MONOTONIC);

                /* init the condition object */
				//pthread_cond_init(&condition, &cond_attribute);
				pthread_cond_init(&condition, 0);

                /* destroy our condition attribute */
                //pthread_condattr_destroy(&cond_attribute);
			}
			~Condition() {
				pthread_cond_destroy(&condition);
			}
			void signal() {
				pthread_cond_signal(&condition);
			}
            void lock() {
                this->mutex.lock();
            }
            void unlock() {
                this->mutex.unlock();
            }
            void tryLock() {
                this->mutex.tryLock();
            }
			void wait() {
				pthread_cond_wait(&condition, &this->mutex.mutex);
			}
            bool wait(Util::Time time) {
                timespec t = time.getTimespec();
                return pthread_cond_timedwait(&condition, &this->mutex.mutex, &t) == 0;
            }

			void broadcast() {
				pthread_cond_broadcast(&condition);
			}
	};

}

#endif
