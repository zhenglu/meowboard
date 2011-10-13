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

#ifndef DISPATCH_HH
#define DISPATCH_HH

//#include "Message.hh"
#include "Queue.hh"
#include "Task.hh"
#include "Util/Timer.hh"

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_map.hpp>

typedef boost::function<void()> Message;

namespace Threads {

	class Dispatcher {
		public:
			Dispatcher();

			~Dispatcher();

            /*! \brief Start to run the dispatcher */
			void start();

            /*! \brief Stop the dispatcher */
			void stop();

            /*! \brief Join the dispatcher's thread */
            void join();

            /*! \brief Put a message in the queue to be executed and returns immediately */
            void queue(const Message& message);

            /*! \brief Put a message in the queue to be executed and wait for it to be executed */
            void exec(const Message& message);

            /*! \brief Schedule a message to be executed in the given time */
            void schedule(const Message& message, Util::Time when);

		private:

			void run();

            void _schedule(const Message& message, Util::Time when);

			Task task;

			Queue<Message> _queue;

            std::map<Util::Time, Message> agenda;

			volatile bool running;

			void _stop();

	};

}
#endif
