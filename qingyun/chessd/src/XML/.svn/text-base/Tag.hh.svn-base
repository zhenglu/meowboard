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

#ifndef XMLTAG_HH
#define XMLTAG_HH

#include <vector>
#include <map>
#include <string>
#include <boost/ptr_container/ptr_vector.hpp>

#include "Item.hh"
#include "CData.hh"
#include "Exception.hh"

#include "Util/CastFunctor.hh"
#include "Util/View.hh"
#include "Util/utils.hh"

namespace XML {

	typedef std::map<std::string, std::string> AttributeMap;
	typedef boost::ptr_vector<Item> ItemList;

	class Tag : public Item {
		public:
			/* default constructor */
			Tag(const std::string& name) :
				_name(name),
				_tag_view(this->children()),
				_cdata_view(this->children()) { }

			/* copy contructor */
			Tag(const Tag&);

			/* move contructor */
			Tag(Moved<Tag> tag);

			/* destructor */
			virtual ~Tag() { }
			
			/* return its name */
			std::string& name() { return this->_name; }
			const std::string& name() const { return this->_name; }

			/* return tag attributes */
			AttributeMap& attributes() { return this->_attributes; }
			const AttributeMap& attributes() const { return this->_attributes; }

			/* clone the tag */
			virtual Tag* clone() const;

			/* swap content */
			virtual void swap(Tag& tag);

			/* generate xml string */
			std::string xml() const;

			bool hasAttribute(const std::string& name) const {
				return this->attributes().find(name) != this->attributes().end();
			}

			void setAttribute(const std::string& name, const std::string& value) {
				this->attributes().insert(std::pair<std::string, std::string>(name, value));
			}

			const std::string& getAttribute(const std::string& name) const {
                AttributeMap::const_iterator iterator = this->attributes().find(name);
                if(iterator == this->attributes().end())
                    throw(attribute_not_found("Attribute not found"));
                else
                    return this->attributes().find(name)->second;
            }

			std::string& getAttribute(const std::string& name) {
                AttributeMap::iterator iterator = this->attributes().find(name);
                if(iterator == this->attributes().end())
                    throw(attribute_not_found("Attribute not found"));
                else
                    return this->attributes().find(name)->second;
			}

            const std::string& getAttribute(const std::string& name,
                    const std::string& default_value) const {
                AttributeMap::const_iterator iterator = this->attributes().find(name);
                if(iterator == this->attributes().end())
                    return default_value;
                else
                    return this->attributes().find(name)->second;
            }

			Tag& findTag(const std::string& name) {
                foreach(tag, this->tags()) {
                    if(tag->name() == name)
                        return *tag;
                }
                throw (child_not_found("Tag not found"));
            }

			const Tag& findTag(const std::string& name) const {
                foreach(tag, this->tags()) {
                    if(tag->name() == name)
                        return *tag;
                }
                throw (child_not_found("Tag not found"));
            }

			CData& findCData() {
                foreach(child, this->children()) {
                    if(typeid(*child) == typeid(CData))
                        return static_cast<CData&>(*child);
                }
                throw (child_not_found("CData not found"));
            }

			const CData& findCData() const {
                foreach(child, this->children()) {
                    if(typeid(*child) == typeid(CData))
                        return static_cast<const CData&>(*child);
                }
                throw (child_not_found("CData not found"));
            }

            std::string getCData() const {
                std::string ret;
                foreach(child, this->children()) {
                    if(typeid(*child) == typeid(CData)) {
                        ret += static_cast<const CData&>(*child).data();
                    }
                }
                return ret;
            }

			/* generate xml string */
			virtual void xml(std::string&) const;

            Tag& operator=(const Tag& tag);

            const Tag& firstTag() const {
                if(this->tags().begin() == this->tags().end()) {
                    throw XML::child_not_found("Tag not found");
                }
                return *this->tags().begin();
            }

            Tag& firstTag() {
                if(this->tags().begin() == this->tags().end()) {
                    throw XML::child_not_found("Tag not found");
                }
                return *this->tags().begin();
            }

		private:


			struct is_tag : public std::unary_function<Item&, bool> {
				bool operator()(const Item& item) const {
					return typeid(item) == typeid(Tag);
				}
			};

			struct is_cdata : public std::unary_function<Item&, bool> {
				bool operator()(const Item& item) const {
					return typeid(item) == typeid(CData);
				}
			};

			typedef CastFunctor<Tag, Item> cast_tag;
			typedef CastFunctor<CData, Item> cast_cdata;


			typedef View<ItemList, is_tag, cast_tag> TagView;
			typedef View<ItemList, is_cdata, cast_cdata> CDataView;

			/* Attributes */
			std::string _name;
			AttributeMap _attributes;
			ItemList _children;
			TagView _tag_view;
			CDataView _cdata_view;
		public:
			const TagView& tags() const { return _tag_view; }
			TagView& tags() { return _tag_view; }

			const CDataView& cdatas() const { return _cdata_view; }
			CDataView& cdatas() { return _cdata_view; }

			ItemList& children() { return this->_children; }
			const ItemList& children() const { return this->_children; }
	};

}

#endif
