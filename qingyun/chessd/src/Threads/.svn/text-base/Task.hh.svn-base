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

#ifndef TASK_HH
#define TASK_HH

#include "Mutex.hh"
#include "Condition.hh"

#include <boost/function.hpp>

namespace Threads {

	enum TaskStatus {
		/* The task is ready to run */
		TaskReady,
		/* The task is waiting to be executed */
		TaskWaiting,
		/* The task is running */
		TaskRunning
	};

    inline pthread_t getCurrentThreadId() {
        return pthread_self();
    }


	/*! \brief Executes stuff in parallel
	 */
	class Task {
		public:
			/*! \brief Constructor
			 *
			 * \param function to be called in a separate thread
			 */
			Task(const boost::function<void ()>& function);

			/*! \brief Destructor
			 * 
			 * If the task is still running, it will joi it.
			 * */
			~Task();

			/*! \brief Launch the task.
			 *
			 * The given function in the constructor will be
			 * called in a separated thread
			 */
			void start();

			/*! \brief Join the task.
			 *
			 * It will block until the task finishes.
			 */
			bool join();

			/*! \brief Get task status.
			 *
			 * \return Return the current task status
			 */
			TaskStatus getStatus() const;

			/*! \brief Run the task */
			void run();

            /*! \brief The the id of the thread that this task is running */
            pthread_t getThreadId() const {
                return thread_id;
            }

		private:
			Condition condition;
			TaskStatus status;
			bool waiting;
			boost::function<void ()> function;
            pthread_t thread_id;
	};

}

#endif
