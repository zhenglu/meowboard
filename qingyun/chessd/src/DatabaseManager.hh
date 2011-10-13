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

#ifndef DATABASE_HH
#define DATABASE_HH

#include <string>

#include <pqxx/pqxx>

#include <boost/function.hpp>

#include "Threads/Task.hh"

#include "Threads/Queue.hh"

#include "Threads/Condition.hh"

#include "XML/Xml.hh"

#include "DatabaseInterface.hh"

typedef boost::function<void (DatabaseInterface&)> Transactor;

/*! \brief A more robust transactor
 *
 * This class has he same purpose of the transacotr type
 * but it has aditional functionality.
 */
class TransactorObject {
    public:

        /*! \brief Constructor.
         * \param transactor is the function that
         *        will execute the transaction
         */
        TransactorObject(const Transactor& transactor);

        /*! \brief Wait for the transaction to complete */
        void wait();

        /*! \brief Execute the transaction */
        void operator()(DatabaseInterface&);

    private:

        Transactor transactor;

        Threads::Condition condition;

        volatile bool finished;
};

/*! \brief Controls access to the database. 
 *
 * Every transaction is exected in a separated thread
 * */
class DatabaseManager {
    public:

        /*! \brief Constructor. */
        DatabaseManager(const XML::Tag& config);

        /*! \brief Destructor. */
        ~DatabaseManager();

        /*! \brief Execute the given transaction immediately.
         *
         * The transaction is executed in the current thread,
         * and this method returs only when the transaction
         * uis complete
         * */
        void execTransaction(const Transactor& transactor);

        /*! \brief Queue a transaction.
         *
         * The transaction is executed in a separated thread.
         * The transation may be called more than once in case
         * of a a rollback.
         * \param transaction is a function that performs the transation.
         * */
        void queueTransaction(const Transactor& transaction);

        /*! \brief Queue a transaction
         *
         * Same as the other form, but take a TransactorObject 
         * as parameter, it does not copy it and the insance passed
         * must exist until the transaction is over */
        void queueTransaction(TransactorObject& transaction);

    private:

        /*! \brief Collects unused resources. */
        void colector();

        pqxx::connection* getConnection();

        std::string connection_string;
        
        Threads::Queue<pqxx::connection*> free_connections;

        Threads::Queue<Threads::Task*> running_tasks;

        Threads::Task colector_task;

        volatile int connections_left;
};

#endif
