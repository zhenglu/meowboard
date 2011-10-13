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

#ifndef XMPPNODE_HH
#define XMPPNODE_HH

#include <map>
#include "handlers.hh"
#include "Disco.hh"
#include "Util/IDSet.hh"


namespace XMPP {

	class Node {
		public:
			/*! \brief Constructor
			 *
			 * \param send_stanza is a function to be used to send a stanza.
			 * \param jid is the node's jid.
			 * \param name is the node name.
			 * \param category is the node category according to disco spec.
			 * \param type if the node type.
			 */
			Node(const StanzaHandler& send_stanza,
				 const Jid& jid,
				 const std::string& name,
				 const std::string& category,
				 const std::string& type);

			virtual ~Node();

			/*! \brief Set a handler for message stanzas.
			 *
			 * A message of type subtype is sent to the handler.
			 * \param handler is a function.
			 * \param subtype is the message's type.
			 */
			void setMessageHandler(const ConstStanzaHandler& handler,
					const std::string& subtype);

			/*! \brief Remove a message handler
			 *
			 * Removes the handler for the given type
			 * \param subtype is the message's type.
			 * */
			void removeMessageHandler(const std::string& subtype);

			/*! \brief Set a handler for iqs of the given xmlns.
			 *
			 * All the iq tanzas that has the given xmlns is forwarded to the handler.
			 * \param handler is a function.
			 * \param xmlns is the stanza's xmlns.
			 */
			void setIqHandler(const ConstStanzaHandler& handler, const std::string& xmlns);

			/*! \brief Remove a iq handler
			 *
			 * \param xmlns is the stanza's xmlns.
			 */
			void removeIqHandler(const std::string& xmlns);

			/*! \brief Set a handler for presence stanzas
			 *
			 * All presence stanzas are forwarded to the given handler.
			 * /param handler is a function.
			 */
			void setPresenceHandler(const ConstStanzaHandler& handler);

			/*! \brief Remove the presence handler. */
			void removePresenceHandler();

			/*! \brief Send a stanza to this  node.
			 *
			 * \param stanza is the incoming stanza.
			 */
			virtual void handleStanza(Stanza* stanza) throw();

			Disco& disco() { return this->_disco; }
			const Disco& disco() const { return this->_disco; }

			/*! \brief Send an Iq of type get or set.
			 *
			 * Choose an id for the iq and send it.
			 * When the result is received, it is passed to
			 * on_result. When timeout occurs on_timeout is called.
			 *
			 * \param stazna is the iq.
			 * \param on_result is the result callback.
			 * \param on_timeout is the timeout callback.
			 * */
			void sendIq(Stanza* stanza,
                    const ConstStanzaHandler& on_result = ConstStanzaHandler(),
					const TimeoutHandler& on_timeout = TimeoutHandler());

			void sendStanza(Stanza* stanza);

            const Jid& jid() { return this->_jid; }

		private:

			void handleIq(const Stanza& stanza);
			void handleMessage(const Stanza& stanza);
			void handlePresence(const Stanza& stanza);

			typedef std::map<std::string, ConstStanzaHandler> HandlerMap;
			HandlerMap message_handlers;
			HandlerMap iq_handlers;
			ConstStanzaHandler presence_handler;
			StanzaHandler send_stanza;

			Disco _disco;

			struct IQTrack;

			std::map<long long, IQTrack> iq_tracks;

			Jid _jid;

            long long iq_ids;
	};

}

#endif
