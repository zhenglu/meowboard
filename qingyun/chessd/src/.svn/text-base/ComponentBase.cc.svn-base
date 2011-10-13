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

#include "ComponentBase.hh"
#include "Util/utils.hh"

using namespace std;
using namespace XML;
using namespace XMPP;
using namespace Util;

ComponentBase::ComponentBase(const XML::Tag& config,
		const std::string& component_name) :
	component(config.getAttribute("component_name")),
	running(false),
	root_node(
            boost::bind(&ComponentBase::sendStanza, this, _1),
			XMPP::Jid(config.getAttribute("component_name")),
			component_name, "service", "game"),
    server_address(config.getAttribute("server_address")),
    server_port(parse_string<int>(config.getAttribute("component_port"))),
    server_password(config.getAttribute("component_password")),
	task_recv(boost::bind(&ComponentBase::run_recv, this)),
	task_send(boost::bind(&ComponentBase::run_send, this))
{
	this->dispatcher.start();
}

ComponentBase::~ComponentBase() {
	this->close();
	this->dispatcher.stop();
}

void ComponentBase::connect() {
    /* connect to the server */
    this->component.connect(
            this->server_address,
            this->server_port,
            this->server_password);

    /* start the threads */
    this->running = true;
    this->task_recv.start();
    this->task_send.start();

    /* Notify connection */
    this->dispatcher.queue(boost::bind(&ComponentBase::onConnect, this));
}

void ComponentBase::close() {
    this->dispatcher.exec(boost::bind(&ComponentBase::_close, this));
}

void ComponentBase::_close() {
	if(__sync_val_compare_and_swap(&this->running, true, false) == true) {

        /* stop receiving */
		this->task_recv.join();

        /* call handler */
        this->onClose();

        /* stop sending */
		this->stanza_queue.push(0);
		this->task_send.join();

        /* close connection */
		this->component.close();
	}
}

void ComponentBase::handleError(const std::string& error) {
    /* tunnel the call */
    this->dispatcher.queue(boost::bind(&ComponentBase::_handleError, this, error));
}

void ComponentBase::_handleError(const std::string& error) {
    /* call handler */
    this->onError(error);

    /* close connection */
    this->_close();
}

void ComponentBase::handleStanza(XMPP::Stanza* stanza) {
	this->dispatcher.queue(boost::bind(&XMPP::RootNode::handleStanza, &this->root_node, stanza));
}

void ComponentBase::run_recv() {
    XMPP::Stanza* stanza = 0;
	try {
		while(this->running) {
            /* receive stanzas */
			stanza = this->component.recvStanza(1);
            /* deliver the stanza */
            if(stanza != 0) {
                this->handleStanza(stanza);
            }
		}
	} catch (const runtime_error& error) {
        this->handleError(error.what());
	}
}

void ComponentBase::run_send() {
	XMPP::Stanza* stanza = 0;
    try {
        while(1) {
            /* take one stanza from the queue */
            stanza = this->stanza_queue.pop();

            /* we have to stop when we get a null pointer */
            if(stanza==0) {
                break;
            }

            /* deliver it */
            this->component.sendStanza(stanza);
        }
    } catch (const runtime_error& error) {
        this->handleError(error.what());
    }
}

void ComponentBase::sendStanza(XMPP::Stanza* stanza) {
    /* put in the queue*/
    this->stanza_queue.push(stanza);
}

void ComponentBase::onConnect() {
    /* Default action is to do nothing */
}
