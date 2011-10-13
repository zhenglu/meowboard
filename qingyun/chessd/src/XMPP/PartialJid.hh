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

#ifndef XMPPPARTIALJID_HH
#define XMPPPARTIALJID_HH

#include <string>

namespace XMPP {

    class PartialJid {
        public:
            PartialJid(const PartialJid& jid);

            explicit PartialJid(const std::string& jid);

            explicit PartialJid(const std::string& node,
                                const std::string& domain);

            PartialJid();

            ~PartialJid();

            const std::string& node() const { return this->_node; }
            std::string& node() { return this->_node; }

            const std::string& domain() const { return this->_domain; }
            std::string& domain() { return this->_domain; }

            /* \brief Returns the jid representation */
            std::string full() const;

            bool empty() const {
                return this->node().empty() and this->domain().empty();
            }

            void swap(PartialJid& jid) {
                this->node().swap(jid.node());
                this->domain().swap(jid.domain());
            }

            bool operator==(const PartialJid& jid) const {
                return this->node() == jid.node() and
                       this->domain() == jid.domain();
            }
            bool operator!=(const PartialJid& jid) const {
                return this->node() != jid.node() or
                       this->domain() != jid.domain();
            }

            bool operator<(const PartialJid& jid) const {
                if(this->node() < jid.node())
                    return true;
                else if(this->node() > jid.node())
                    return false;
                else if(this->domain() < jid.domain())
                    return true;
                else
                    return false;
            }

            bool operator>(const PartialJid& jid) const {
                if(this->node() > jid.node())
                    return true;
                else if(this->node() < jid.node())
                    return false;
                else if(this->domain() > jid.domain())
                    return true;
                else
                    return false;
            }

        protected:
            std::string _node, _domain;
    };

}

/* Overloaded swap function for the PartialJid class */
namespace std {
    inline void swap(XMPP::PartialJid& j1, XMPP::PartialJid& j2) {
        j1.swap(j2);
    }
}

#endif
