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

#ifndef ANOUNCEMENTMANAGER_HH
#define ANOUNCEMENTMANAGER_HH

#include <map>
#include <set>
#include <memory>
#include <boost/ptr_container/ptr_map.hpp>

#include "XMPP/Roster.hh"

#include "MatchAnnouncement.hh"

#include "ServerModule.hh"

#include "ServerCore.hh"
#include "DatabaseManager.hh"

/*! \brief This class manages all the matchs int the server. */
class AnnouncementManager : public ServerModule {
    public:
        /*! \brief Constructor
         *
         * \param core_interface is the interface to the core.
         * \param config is the configuration for this component.
         */
        AnnouncementManager(
                     ServerCore& game_manager,
                     DatabaseManager& database_manager,
                     const XMPP::StanzaHandler& sendStanza);

        /*! \brief Destructor
         *
         * Closes server connection if available
         */
        ~AnnouncementManager();

        std::vector<std::string> namespaces() const;

    private:

        /*! \brief Store information about an annoucement */
        struct Announcement {
            Util::Time time;
            std::string category;
            int min_rating, max_rating;
            std::set<XMPP::Jid> players;
            std::auto_ptr<MatchAnnouncement> announcement;
        };

        /*! \brief Receive a notification of a status change */
        void handleUserStatus(const XMPP::Jid& user_name,
                              const UserStatus& status);

        /*! \brief Process an incoming iq */
        void handleIq(const XMPP::Stanza& stanza);

        /*! \brief Process a request to crete an annoucement */
        void handleCreate(const XMPP::Stanza& stanza);

        /*! \brief Process a request to delete an annoucement */
        void handleDelete(const XMPP::Stanza& stanza);

        /*! \brief Process a request to accept an annoucement */
        void handleAccept(const XMPP::Stanza& stanza);

        /*! \brief Process a request to search for annoucements */
        void handleSearch(const XMPP::Stanza& stanza);

        /*! \brief Send a notification to the users that a game has started */
        void notifyGame(const std::vector<GamePlayer>& players, const XMPP::Jid& room);

        /*! \brief Send a notification to the users that a game has started */
        void _notifyGame(const std::vector<GamePlayer>& players, const XMPP::Jid& room);

        /*! \brief perform a search for annoucements in the database */
        void searchAnnouncement(DatabaseInterface& database,
                const XMPP::Stanza& stanza);

        ServerCore& game_manager;

        DatabaseManager& database;

        uint64_t announcement_ids;

        boost::ptr_map<uint64_t, MatchAnnouncement> announcements;
};

#endif
