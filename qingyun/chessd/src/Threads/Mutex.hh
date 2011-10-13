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

#ifndef MUTEX_HH
#define MUTEX_HH

#include <pthread.h>

namespace Threads {

	class Mutex {
		private:
			pthread_mutex_t mutex;
			friend class Condition;
		public:
			Mutex() {
				pthread_mutex_init(&this->mutex, 0);
			}

			~Mutex() {
				pthread_mutex_destroy(&mutex);
			}

			void lock() {
				pthread_mutex_lock(&mutex);
			}

			void unlock() {
				pthread_mutex_unlock(&mutex);
			}
			bool tryLock() {
				return pthread_mutex_trylock(&mutex) == 0;
			}
	};

}

#endif
