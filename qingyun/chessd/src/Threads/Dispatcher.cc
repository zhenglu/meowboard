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

#include "Dispatcher.hh"
#include "SimpleSemaphore.hh"

namespace Threads {

    Dispatcher::Dispatcher() : 
        task(boost::bind(&Dispatcher::run, this)),
        running(false) { }

    Dispatcher::~Dispatcher() {
    }

    void Dispatcher::start() {
        if(__sync_val_compare_and_swap(&this->running,false,true) == false) {
            this->task.start();
        }
    }

    void Dispatcher::run() {
        Message message;
        while(this->running) {
            if(this->agenda.empty()) {
                message = this->_queue.pop();
            } else {
                if(not this->_queue.try_pop(message, this->agenda.begin()->first)) {
                    message = this->agenda.begin()->second;
                    this->agenda.erase(this->agenda.begin());
                }
            }
            message();
        }
        /* call remaining messages */
        while(this->_queue.try_pop(message)) {
            message();
        }
    }

    static void func() { }

    void Dispatcher::stop() {
        /* A stop message is sent to the dispatcher instead of just
         * setting running to false. This way we make sure the dispatcher
         * will stop as soon as possible. */
        if(__sync_val_compare_and_swap(&this->running,true,false) == true) {
            /* wake the dispatcher if necessary */
            this->queue(func);
            /* wait for it to stop */
            this->join();
        }
    }

    void Dispatcher::join() {
        this->task.join();
    }

    void Dispatcher::_stop() {
        if(__sync_val_compare_and_swap(&this->running,true,false) == false) {
            /* was stopped already */
        }
    }

    void Dispatcher::queue(const Message& message) {
        this->_queue.push(message);
    }

    static void execMessage(const Message& message, SimpleSemaphore& sem) {
        message();
        sem.post();
    }

    void Dispatcher::exec(const Message& message) {
        /* if this is a different thread, put it inthe queue,
         * otherwise, just call the message */
        if(getCurrentThreadId() != this->task.getThreadId()) {
            SimpleSemaphore sem;
            this->_queue.push(boost::bind(execMessage, message, boost::ref(sem)));
            sem.wait();
        } else {
            message();
        }
    }

    void Dispatcher::schedule(const Message& message, Util::Time when) {
        this->queue(boost::bind(&Dispatcher::_schedule, this, message, when));
    }

    void Dispatcher::_schedule(const Message& message, Util::Time when) {
        this->agenda.insert(std::make_pair(when, message));
    }
}
