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

#ifndef COMPONENTBASE_HH
#define COMPONENTBASE_HH

#include "XML/Xml.hh"

#include "XMPP/RootNode.hh"
#include "XMPP/Component.hh"
#include "Threads/Dispatcher.hh"
#include "Threads/Task.hh"
#include "Threads/Queue.hh"


/*! \brief A base implementation for components. */
class ComponentBase {
    public:
        /*! \brief Constructor
         *
         * \param core_interface is the interface to the core.
         * \param config is the configuration for this component.
         */
        ComponentBase(const XML::Tag& config,
                      const std::string& component_name);

        /*! \brief Destructor
         *
         * Closes server connection if available
         */
        virtual ~ComponentBase();

        /*! \brief Connect to the server.
         *
         * \throw Throws an exception on error.
         */
        void connect();

        /*! \brief Closes the conxection to the server.
         *
         * This is a tunnel to the real one.*/
        void close();

        /*! \brief send s stanza to the server */
        void sendStanza(XMPP::Stanza* stanza);

    private:

        XMPP::Component component;

    protected:

        /*! \brief Real close */
        void _close();

        /*! \brief Close notification.
         *
         * Reimplement this method to receive notification that
         * the connection is about to close. */
        virtual void onClose() = 0;

        /*! \brief Error notification.
         *
         * Reimplement this method to receive notification that
         * the connection has failed. */
        virtual void onError(const std::string& msg) = 0;

        virtual void onConnect();

        /*! \brief Handle a connection error
         *
         * This is a tunnel to the real one.
         */
        void handleError(const std::string& error);

        /*! \brief Handle a connection error
         *
         * The real one.
         */
        void _handleError(const std::string& error);

        /*! \brief We run in a separated thread as a dispatcher */
        Threads::Dispatcher dispatcher;

        /*! \brief Is it running? */
        volatile bool running;

        /*! \brief A XMPP node*/
        XMPP::RootNode root_node;

    private:

        /*! \brief Receive messages from the server.
         *
         * This method is executed in a separated thread.
         */
        void run_recv();

        /*! \brief Send messages to the server.
         *
         * This method is executed in a separated thread.
         */
        void run_send();

        /*! \brief Handle an incoming stanza.
         *
         * This is a tunnel to RootNode::handleStanza
         * */
        void handleStanza(XMPP::Stanza* stanza);

        std::string server_address;
        int server_port;
        std::string server_password;

        Threads::Task task_recv;

        Threads::Task task_send;

        Threads::Queue<XMPP::Stanza*> stanza_queue;
};

#endif
