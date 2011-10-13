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

#ifndef POOL_HH
#define POOL_HH

#include "Util/Singleton.hh"
#include "Queue.hh"
#include "Task.hh"
#include "SafeObject.hh"
#include "Thread.hh"

#include <vector>

namespace Threads {

	/*! \brief Pool of threads.
	*/
	class Pool : public Util::Singleton<Pool> {
		public:

			/*! \brief Execute the given task.
			 *
			 * The task is executed in a thread from the pool.
			 * */
			void launchTask(Task& task);

		private:

			Pool();

			virtual ~Pool();

            class PoolThread : public Thread {
                public:
                    PoolThread(Pool& pool);
                private:
                    Pool& pool;
                    void run();
            };

            Condition cond;

            volatile int idle_count;

            volatile bool running;

            std::queue<Task*> tasks;

			SafeObject<std::vector<PoolThread*> > threads;

			friend class Util::Singleton<Pool>;
	};

}
#endif
