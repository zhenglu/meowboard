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

#ifndef XMPPSTREAM_HH
#define XMPPSTREAM_HH


#include "XML/Xml.hh"

#include "handlers.hh"

#include <boost/function.hpp>

#include <string>
#include <queue>

namespace XMPP {

	/*! \brief An implementation of a XMPP XML stream */
	class Stream {
		public:

			/*! \brief Constructor
			 *
			 * \param ns is the stream's namespace
			 */
			Stream(const std::string& ns);

            /*! \brief Create a stream with an already stabilished connection */
            Stream(int socket_fd);

			/*! \brief Destructor
			 *
			 * Close connection to the XMPP server if it is active.
			 */
			~Stream();

			/*! \brief Connect to the XMPP server.
			 *
			 * \param host is the server address.
			 * \param port is the port to connect.
			 * \return Returns true on success, false otherwise
			 */
			void connect(const std::string& host, int port);

			/*! \brief Close the connection to the server */
			void close();

			/*! \brief Send a tag through the stream
			 *
			 * The ownership of the Tag is passed to this class.
			 * \param tag is the Tag to be sent.
			 * \return Return true on success, false otherwise. If false is
			 * returned than the connection status should be checked with isActive
			 */
			void sendTag(XML::Tag* tag);

			/*! \brief Receive a tag from stream.
			 *
			 * This call block untill timeout elapses or a Tag is received.
			 * \param timeout is the maximun time to wait for a Tag. -1 means infinite.
			 * \return A pointer to the Tag received or NULL if timeout expires or if
			 * the connection was lost. The caller receives the Tag's ownership.
			 */
			XML::Tag* recvTag(int timeout);

			/*! \brief Get the connection status.
			 *
			 * \return Returns true if the connection is active, false otherwise.
			 */
			bool isActive() const { return this->active; }

            /*! \brief Returns the socket descriptor */
            int getFD() const;

			struct HookInfo;
		private:

			/*! \brief This struct is an opaque container for
			 * internal data */
			HookInfo* hinfo;

			/*! \brief The namespace in use. Storing it this way is a requirement
			 * of the iksemel library.
			 */
			char* ns;

			/*! \brief Hold connection status */
			bool active;
	};

}

#endif
