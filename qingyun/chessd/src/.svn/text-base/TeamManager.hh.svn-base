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

#ifndef MATCHMANAGER_HH
#define MATCHMANAGER_HH

#include <map>
#include <set>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>
#include "CoreInterface.hh"
#include "XMPP/Component.hh"
#include "XMPP/RootNode.hh"
#include "XMPP/Disco.hh"
#include "XMPP/Roster.hh"
#include "Threads/Dispatcher.hh"
#include "ComponentWrapper.hh"
#include "Util/Timer.hh"

#include "Match.hh"
#include "MatchRule.hh"
#include "MatchProtocol.hh"
#include "MatchDatabase.hh"


class MatchManager {
	public:
		/*! \brief Constructor
		 *
		 * \param core_interface is the interface to the core.
		 * \param config is the configuration for this component.
		 */
		MatchManager(const XML::Tag& config, const XMPP::ErrorHandler& handleError);

		/*! \brief Destructor
		 *
		 * Closes server connection if available
		 */
		~MatchManager();

		/*! \brief Connect to the server.
		 *
		 * \throw Throws an exception on error.
		 */
		void connect();

		/*! \brief Insert a match rule
		 *
		 * The rule's ownership is passed to this class.
		 * This must be called befre connect.
		 * \param rule is the MatchRule to be inserted.
		 */
		void insertMatchRule(MatchRule* rule);

		/*! \brief Closes the conenction to the server */
		void close();


	private:

		/* several handlers for the incoming events */

		/*! \brief handle an incoming match offer */
		void handleMatchOffer(XMPP::Stanza* stanza);
		/*! \brief handle an incoming match acceptance */
		void handleMatchAccept(XMPP::Stanza* stanza);
		/*! \brief handle an incoming match declinance */
		void handleMatchDecline(XMPP::Stanza* stanza);
		/*! \brief handle an incoming match iq */
		void handleMatch(XMPP::Stanza* stanza);

		void notifyMatchOffer(int id);

		void notifyMatchResult(int id, bool accepted);

		void notifyUserStatus(const XMPP::Jid jid, bool available);

		void _handleError(const std::string& error);

		void handleStanza(XMPP::Stanza* stanza);

		/*! \brief We run in a separated thread as a dispatcher */
		Threads::Dispatcher dispatcher;

		/*! \brief Is it running? */
		bool running;

		/*! \brief Interface to the core */
		CoreInterface core_interface;

		/*! \brief A component wrapper */
		ComponentWrapper component;

		/*! \brief A XMPP node*/
		XMPP::RootNode root_node;

		/*! \brief A XMPP roster */
		XMPP::Roster roster;

		/*! \brief Team database */
		TeamDatabase teams;

		typedef boost::ptr_map<std::string, MatchRule>  RuleMap;
		/*! \brief Registered rules */
		RuleMap rules;

		/*! \brief Pending offers */
		MatchDatabase match_db;

		Util::IDSet match_ids;

		/*! \brief The XMPP server address */
		std::string server_address;

		/*! \brief The server port */
		int server_port;

		/*! \brief The server password */
		std::string server_password;

		XMPP::ErrorHandler handleError;
};

#endif
