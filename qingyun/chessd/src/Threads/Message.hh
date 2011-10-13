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

#ifndef MESSAGE_HH
#define MESSAGE_HH

#include "Semaphore.hh"
#include "Queue.hh"

#include <memory>

#include <boost/function.hpp>
#include <boost/utility/result_of.hpp>

namespace Threads {

	class Message{
		public:
			Message() { }
			virtual ~Message() { }
			virtual void send() = 0;
		protected:
	};

	template <class CALLABLE> class TypedMessage : public Message {
		public:
			typedef CALLABLE message_function;

			TypedMessage(const message_function& message)
				: message(message) { }

			virtual ~TypedMessage() { }

			virtual void send() {
				this->message();
			}

		private:
			message_function message;
	};


	template <class CALLABLE> Message* createMessage(const CALLABLE& callable) {
		return new TypedMessage<CALLABLE>(callable);
	}
}

#endif
