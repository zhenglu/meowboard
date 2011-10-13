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

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <signal.h>

#include "Util/Log.hh"

#include "Threads/Condition.hh"

#include "XML/Xml.hh"

#include "DatabaseManager.hh"
#include "ServerCore.hh"

using namespace std;
using namespace XML;

volatile bool running = true;
pthread_t main_thread;

void handleError(const string& error) {
    cerr << error << endl;
    if(__sync_val_compare_and_swap(&running, true, false) == true) {
        pthread_kill(main_thread, SIGINT);
    }
}

int main(int argc, char** argv) {
    fstream log_file;

    /* prepare signal handler */
    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIGINT);
    sigaddset(&signal_set, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &signal_set, NULL);

    /* get the main thread id */
    main_thread = pthread_self();

    try {
        /* Load config file */
        std::string file_name = (argc>=2) ? argv[1] : "config.xml";
        std::auto_ptr<XML::Tag> config(XML::parseXmlFile(file_name));

        if(config.get() == 0) {
            throw "Unable to load config file";
        }

        /* Init database manager */
        DatabaseManager database_manager(config->findTag("database"));

        /* Init the game server */
        ServerCore server(config->findTag("xmpp-component"),
                database_manager,
                boost::bind(handleError, _1));

        /* Set log output */
        try {
            log_file.open(config->findTag("log").getAttribute("filename").c_str(), iostream::out);
            if(not log_file.fail()) {
                Util::log.setOutput(log_file);
            } else {
                cerr << "Failed to open log file" << endl;
            }
        } catch (XML::xml_error) {
            /* Log file is not set in the config */
        }

        /* Start the server */
        server.connect();

        /* wait for a SIGINT or SIGTERM */
        int sig_number;
        sigwait(&signal_set, &sig_number);

        /* Close the server */
        cerr << "Exiting..." << endl;
        server.close();
    } catch (const char* msg) {
        cerr << "Error: " << msg << endl;
        return 1;
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << endl;
        return 1;
    }
    return 0;
}
