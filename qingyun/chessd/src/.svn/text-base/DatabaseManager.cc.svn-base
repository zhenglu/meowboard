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

#include "DatabaseManager.hh"

#include "Util/utils.hh"

#include <boost/bind.hpp>

#include "Util/Log.hh"

using namespace Util;
using namespace std;

struct pqxx_transaction : public pqxx::transactor<>
{
    public:

        pqxx_transaction(const Transactor& transactor) : transactor(transactor) { }

        void operator()(pqxx::work& work)
        {
            DatabaseInterface interface(work);
            this->transactor(interface);
        }

        void on_abort(const char* msg) throw()
        {
            Util::log.log(string("Transaction aborted: ") + msg);
        }

    private:

        Transactor transactor;
};

TransactorObject::TransactorObject(const Transactor& transactor) :
    transactor(transactor),
    finished(false) { }

void TransactorObject::wait() { 
    this->condition.lock();
    if(not this->finished) {
        condition.wait();
    }
    this->condition.unlock();
}

void TransactorObject::operator()(DatabaseInterface& interface) {
    this->transactor(interface);

    this->condition.lock();
    finished = true;
    this->condition.signal();
    this->condition.unlock();
}

DatabaseManager::DatabaseManager(const XML::Tag& config) :
    colector_task(boost::bind(&DatabaseManager::colector, this))
{
    /* prepare the connetion string */
    this->connection_string =
        " host=" + config.getAttribute("host") +
        " port=" + config.getAttribute("port") +
        " dbname=" + config.getAttribute("db_name") +
        " user=" + config.getAttribute("user") +
        " password=" + config.getAttribute("password") +
        " sslmode=disable";

    this->connections_left = parse_string<int>(config.getAttribute("max_connections"));

    /* Check if we can connect to the database */
    pqxx::connection* conn = this->getConnection();

    /* put the unused connection on the queue */
    this->free_connections.push(conn);

    /* start the tasks colector */
    this->colector_task.start();
}

DatabaseManager::~DatabaseManager()
{
    /* stop threads */
    this->running_tasks.push(0);
    this->colector_task.join();

    /* close connections */
    pqxx::connection* connection = 0;
    while(this->free_connections.try_pop(connection)) {
        delete connection;
    }
}

void DatabaseManager::execTransaction(const Transactor& transactor)
{
    pqxx_transaction t(transactor);

    pqxx::connection* conn = this->getConnection();;

    /* execute it */
    conn->perform(t);

    /* return the connection to the queue */
    this->free_connections.push(conn);
}

void DatabaseManager::queueTransaction(const Transactor& transaction)
{
    /* run the transaction as a task */
    Threads::Task* task =
        new Threads::Task(boost::bind(
                    &DatabaseManager::execTransaction,
                    this,
                    transaction));

    task->start();

    this->running_tasks.push(task);
}

void DatabaseManager::queueTransaction(TransactorObject& transaction)
{
    this->queueTransaction(Transactor(boost::ref(transaction)));
}

void DatabaseManager::colector()
{
    Threads::Task* task = 0;
    while((task = this->running_tasks.pop()) != 0) {
        task->join();
        delete task;
    }
}

pqxx::connection* DatabaseManager::getConnection() {
    pqxx::connection* connection = 0;

    /* try to get a connection from the queue */
    if(this->free_connections.try_pop(connection)) {
        return connection;
    }
    /* if there is no connection in the queue, check if we can create more connections */
    if(this->connections_left > 0 and __sync_fetch_and_sub(&this->connections_left, 1) > 0) {
        return new pqxx::connection(this->connection_string);
    } else {
        /* if we have reached the connections limit, wait in the queue */
        return this->free_connections.pop();
    }

}
