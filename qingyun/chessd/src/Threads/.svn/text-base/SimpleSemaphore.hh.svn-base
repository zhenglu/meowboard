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

#ifndef SIMPLESEMAPHORE_HH
#define SIMPLESEMAPHORE_HH

#include "Condition.hh"

namespace Threads {

	class SimpleSemaphore {
		public:
			SimpleSemaphore() : signaled(false) { }
			void post() {
                this->cond.lock();
                this->signaled = true;
                this->cond.signal();
                this->cond.unlock();
			}
			void wait() {
                this->cond.lock();
                if(not this->signaled) {
                    this->cond.wait();
                }
                this->signaled = false;
                this->cond.unlock();
			}
		private:
			Condition cond;
            volatile bool signaled;
	};

}

#endif
