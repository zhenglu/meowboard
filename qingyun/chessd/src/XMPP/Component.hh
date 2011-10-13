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

#ifndef XMPPCOMPONENT_HH
#define XMPPCOMPONENT_HH

#include <string>
#include <map>
#include "Stream.hh"
#include "Stanza.hh"
#include "handlers.hh"

namespace XMPP {


	/*! \brief An implementation of a XMPP Component */
	class Component {
		public:
			/*! \brief Constructor 
             *
             * \param component_name is the jid of the component.
             * */
			Component(const std::string& component_name);

			/*! \brief Destructor */
			~Component();

            /* \brief Send a stanza to the jabber server. */
			void sendStanza(Stanza* stanza);

            /* \brief Receive an incoming stanza.
             *
             * \param max_time is the maximum time to wait. -1 is infinite
             * */
			Stanza* recvStanza(int max_time);

            /* \brief Connect to the server. */
            void connect(const std::string& host, int port, const std::string& password);

            /* \brief Close the connection to the server. */
            void close();

            /*! \brief Returns the socket descriptor */
            int getFD() const { return this->stream.getFD(); }

		private:

            Stream stream;

			std::string component_name;

            StanzaHandler handleStanza;

	};

}

#endif
