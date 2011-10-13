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

#ifndef PROFILEMANAGER_HH
#define PROFILEMANAGER_HH

#include <map>
#include <set>
#include <memory>

#include "ServerModule.hh"
#include "DatabaseManager.hh"
#include "Util/Timer.hh"

#include "Threads/SafeObject.hh"

/*! \brief This is the component that handles user info requests */
class ProfileManager : public ServerModule {
	public:
		/*! \brief Constructor
		 *
		 * \param core_interface is the interface to the core.
		 * \param config is the configuration for this component.
		 */
		ProfileManager(
                DatabaseManager& database,
                const XMPP::StanzaHandler& send_stanze);

		/*! \brief Destructor
		 *
		 * Closes server connection if available
		 */
		virtual ~ProfileManager();

		/*! \brief Closes the conenction to the server */
		//void close();

        std::vector<std::string> namespaces() const;

	private:

        /*! \brief Handle an incoming stanza */
        void handleIq(const XMPP::Stanza&);

		/*! \brief Handle an incoming rating iq. */
        void handleRating(const XMPP::Stanza& stanza);

		/*! \brief Handle an incoming game search iq. */
		void handleSearchGame(const XMPP::Stanza& stanza);

		/*! \brief Handle an incoming game fetch iq. */
        void handleFetchGame(const XMPP::Stanza& stanza);

        /*! \brief Handle profile retrive or update */
        void handleProfile(const XMPP::Stanza& stanza);

        /*! \brief Handle search user request */
        void handleSearchUser(const XMPP::Stanza& stanza);

        /*! \brief This is a transaction that reads users ratings. */
		void fetchRating(const XMPP::Stanza& stanza,
                         DatabaseInterface& database);

        /*! \brief This is a transaction that search for games. */
        void searchGame(const XMPP::Stanza& stanza,
                        DatabaseInterface& database);

        /*! \brief This is a transaction that search for users. */
        void searchUser(const XMPP::Stanza& stanza,
                        DatabaseInterface& database);

        /*! \brief This is a transaction that fetch a requested game. */
        void fetchGame(const XMPP::Stanza& stanza,
                       DatabaseInterface& database);

        void fetchProfile(const XMPP::Stanza& stanza,
                          DatabaseInterface& database);

        /*! \brief Update the user's profile in the database */
        void updateProfile(const XMPP::Stanza& stanza,
                           DatabaseInterface& database);

        /*! \brief Handle an incoming presence */
        void handlePresence(const XMPP::Stanza& stanza);

        /*! \brief Update online time */
        void updateOnlineTime(const XMPP::PartialJid& user,
                              int increment,
                              DatabaseInterface& database);

        /*! \brief Receive notification of a change in the status of a user */
        void handleUserStatus(const XMPP::Jid& user_name,
                              const UserStatus& status);

        /*! \brief Receie an order to stop activities */
        void onStop();

        XMPP::ErrorHandler error_handler;

        DatabaseManager& database;

        Threads::SafeObject<std::map<XMPP::PartialJid, Util::Time> > last_logons;
};

#endif
