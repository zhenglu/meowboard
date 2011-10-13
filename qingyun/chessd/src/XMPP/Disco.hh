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

#ifndef XMPPDISCO_HH
#define XMPPDISCO_HH

#include "Jid.hh"
#include "handlers.hh"

#include <set>
#include <map>
#include <functional>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace XMPP {

	typedef std::set<std::string> StringSet;

	/*! \brief A XMPP disco item. */
	struct DiscoItem {
		public:
			DiscoItem(const std::string& name, const Jid& jid);

			const std::string& name() const { return this->_name; }
			const Jid& jid() const { return this->_jid; }
		private:
			const std::string _name;
			const Jid _jid;
	};

	struct DiscoItemSet {
		private:
			typedef boost::ptr_map<Jid, DiscoItem> item_map;
			typedef boost::ptr_map<Jid, DiscoItem>::iterator iterator_map;
			typedef boost::ptr_map<Jid, DiscoItem>::const_iterator const_iterator_map;
			struct iterator_transformer :
				public std::unary_function<const item_map::value_type&, DiscoItem&> {
				DiscoItem& operator()(const item_map::value_type& v) const {
					return *v.second;
				}
			};
			struct const_iterator_transformer :
				public std::unary_function<const item_map::value_type&, const DiscoItem&> {
				const DiscoItem& operator()(const item_map::value_type& v) {
					return *v.second;
				}
			};
		public:
			DiscoItemSet() { }
			~DiscoItemSet() { }

			typedef boost::transform_iterator<DiscoItemSet::iterator_transformer, iterator_map> iterator;
			typedef boost::transform_iterator<DiscoItemSet::const_iterator_transformer, const_iterator_map> const_iterator;

			iterator begin() {
				return iterator(this->items.begin());
			}
			iterator end() {
				return iterator(this->items.end());
			}
			const_iterator begin() const {
				return const_iterator(this->items.begin());
			}
			const_iterator end() const {
				return const_iterator(this->items.end());
			}
			void insert(DiscoItem* item) {
				Jid jid = item->jid();
				this->items.insert(jid, item);
			}
			void erase(const Jid& jid) {
				this->items.erase(jid);
			}
		private:
			item_map items;
	};

	/*! \brief A implementation of the XMPP Disco extension. */
	class Disco {
		public:
			/*! \brief Costructor
			 *
			 * \param sender is a function used to send the stanzas
			 */
			Disco(const StanzaHandler& sender,
					const std::string& name = "",
					const std::string& category = "",
					const std::string& type = "");
			~Disco();

			/*! \brief Handler for the info
			 *
			 * The XMPP especifies that this iqs has 
			 * "http://jabber.org/protocol/disco#info" as xmlns
			 * */
			void handleIqInfo(const Stanza& stanza);
			/*! \brief Handler for the items
			 *
			 * The XMPP especifies that this iqs has 
			 * "http://jabber.org/protocol/disco#items" as xmlns
			 * */
			void handleIqItems(const Stanza& stanza);


			/*! \brief The node's category */
			const std::string& category() const { return this->_category; }
			//std::string& category() { return this->_category; }

			/*! \brief The node's type */
			const std::string& type() const { return this->_type; }
			//std::string& type() { return this->_type; }

			/*! \brief The node's name */
			const std::string& name() const { return this->_name; }
			//std::string& name() { return this->_name; }

			/*! \brief The node's features */
			const StringSet& features() const { return this->_features; }
			/*! \brief The node's features */
			StringSet& features() { return this->_features; }

			/*! \brief The node's items */
			const DiscoItemSet& items() const { return this->_items; }

			/*! \brief The node's items */
			DiscoItemSet& items() { return this->_items; }


            /*! \brief Set the disco extension */
            void setExtendedInfo(XML::Tag* info) { this->_ext_info = std::auto_ptr<XML::Tag>(info); }

            /*! \brief Get the disco extension */
            XML::Tag& getExtendedInfo() { return *this->_ext_info; }

		private:

			StanzaHandler stanza_sender;

			const std::string _name, _category, _type;

			StringSet _features;

			DiscoItemSet _items;

            std::auto_ptr<XML::Tag> _ext_info;

	};

}
#endif
