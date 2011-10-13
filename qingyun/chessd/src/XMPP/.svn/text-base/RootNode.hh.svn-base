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

#ifndef XMPPROOTNODE_HH
#define XMPPROOTNODE_HH

#include <map>
#include "Node.hh"
#include "Jid.hh"


namespace XMPP {

	class RootNode : public Node {
		public:
			/*! \brief Constructor
			 *
			 * \param sender is a function to be used to send a stanza.
			 * \param category is the node category according to disco spec.
			 * \param type if the node type.
			 * \param name is the node name.
			 */
			RootNode(const StanzaHandler& send_stanza,
					const Jid& jid,
					const std::string& name,
					const std::string& category,
					const std::string& type);
			~RootNode();

			/*! \brief Send a stanza to this  node.
			 *
			 * \param stanza is the incoming stanza.
			 */
			void handleStanza(Stanza* stanza) throw();

			/*! \brief set a handler for a node
			 *
			 */
			void setNodeHandler(const std::string& node_name, const StanzaHandler& handler);

			void removeNodeHandler(const std::string& node_name);

		private:
			typedef std::map<std::string, StanzaHandler> HandlerMap;
			HandlerMap node_handlers;
	};
}

#endif
