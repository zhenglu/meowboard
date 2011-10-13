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

#ifndef RWLOCK_HH
#define RWLOCK_HH

#include <pthread.h>

namespace Threads {

	class RwLock {
		public:
			RwLock() {
				pthread_rwlock_init(&this->lock, 0);
			}
			~RwLock() {
				pthread_rwlock_destroy(&this->lock);
			}
			void rdlock() {
				pthread_rwlock_rdlock(&this->lock);
			}
			void wrlock() {
				pthread_rwlock_wrlock(&this->lock);
			}
			void unlock() {
				pthread_rwlock_unlock(&this->lock);
			}
		private:
			pthread_rwlock_t lock;
	};

}
#endif
