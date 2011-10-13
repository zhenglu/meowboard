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

#ifndef SEMAPHORE_HH
#define SEMAPHORE_HH

#include <semaphore.h>

namespace Threads {

	class Semaphore {
		public:
			Semaphore(int value = 0) {
				sem_init(&this->semaphore, 0, value);
			}
			~Semaphore() {
				sem_destroy(&this->semaphore);
			}
			void post() {
				sem_post(&this->semaphore);
			}
			void wait() {
				sem_wait(&this->semaphore);
			}
		private:
			sem_t semaphore;
	};

}

#endif
