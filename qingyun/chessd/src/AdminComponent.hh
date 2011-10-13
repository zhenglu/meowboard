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

#ifndef ADMINCOMPONENT_HH
#define ADMINCOMPONENT_HH

#include <map>
#include <set>
#include <memory>

#include "ServerModule.hh"
#include "DatabaseManager.hh"

/*! \brief This is the component that handles user info requests */
class AdminComponent : public ServerModule {
    public:
        /*! \brief Constructor
         *
         * \param core_interface is the interface to the core.
         * \param config is the configuration for this component.
         */
        AdminComponent(
                const std::string& server_name,
                DatabaseManager& database,
                const XMPP::StanzaHandler& send_stanza);

        /*! \brief Destructor
         *
         * Closes server connection if available
         */
        virtual ~AdminComponent();


        virtual std::vector<std::string> namespaces() const ;

    private:

        void onStart();

        /*! \brief handle an incoming iq */
        void handleIq(const XMPP::Stanza& iq);

        /*! \brief Handle an incoming admin iq. */
        void handleAdmin(const XMPP::Stanza& stanza);

        /*! \brief Handle a request for the banned user list */
        void handleBannedList(const XMPP::Stanza& stanza);

        /*! \brief handle kick request */
        void handleKick(const XMPP::Stanza& stanza);

        /*! \brief handle ban request */
        void handleBan(const XMPP::Stanza& stanza);

        /*! \brief handle unban request */
        void handleUnban(const XMPP::Stanza& stanza);

        /*! \brief handle list banned words */
        void handleBannedWordsList(const XMPP::Stanza& stanza);

        /*! \brief handle ban word request */
        void handleBanWord(const XMPP::Stanza& stanza);

        /*! \brief handle unban word request */
        void handleUnbanWord(const XMPP::Stanza& stanza);

        /*! \brief This is a transaction that reads user type. */
        void fetchUserType(const XMPP::Stanza& stanza, DatabaseInterface& database);

        /*! \brief Load admins names from the database */
        void loadAcl(DatabaseInterface& database);

        /*! \brief Set the admin list */
        void setAdmins(const std::set<XMPP::PartialJid>& admins);

        //void setAccessRules();

        void updateAcl();

        void execAdminCommand(const XMPP::Stanza& stanza, const std::string& user_type);

        void kickUser(const XMPP::PartialJid& user);

        void banUser(const XMPP::PartialJid& user, const std::string& reason);

        void unbanUser(const XMPP::PartialJid& user);

        DatabaseManager& database;

        XMPP::Jid server_name;

        //std::set<XMPP::PartialJid> admins;

        std::map<XMPP::PartialJid, std::string> banneds;

        std::set<std::string> banned_words;
};

#endif
