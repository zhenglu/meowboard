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

#ifndef CHILDRENLIST_HH
#define CHILDRENLIST_HH

#include "XML/Xml.hh"

#include "Util/CastFunctor.hh"
#include "Util/View.hh"

#include "Util/utils.hh"

#include <boost/ptr_container/ptr_vector.hpp>

namespace XMPP {

    class ChildrenList : public XML::ItemList {
        public:
            ChildrenList() : _tag_view(*this), _cdata_view(*this) { }

            ChildrenList(const ChildrenList& cl) : _tag_view(*this), _cdata_view(*this) {
                foreach(child, cl) {
                    this->push_back(child->clone());
                }
            }

            ~ChildrenList() { }

            void swap(ChildrenList& cl) {
                XML::ItemList::swap(cl);
            }

        private:

            struct is_tag : public std::unary_function<XML::Item&, bool> {
                bool operator()(const XML::Item& item) const {
                    return typeid(item) == typeid(XML::Tag);
                }
            };

            struct is_cdata : public std::unary_function<XML::Item&, bool> {
                bool operator()(const XML::Item& item) const {
                    return typeid(item) == typeid(XML::CData);
                }
            };

            typedef CastFunctor<XML::Tag, XML::Item> cast_tag;
            typedef CastFunctor<XML::CData, XML::Item> cast_cdata;


            typedef View<XML::ItemList, is_tag, cast_tag> TagView;
            typedef View<XML::ItemList, is_cdata, cast_cdata> CDataView;

            /* Attributes */
            TagView _tag_view;
            CDataView _cdata_view;
        public:
            const TagView& tags() const { return _tag_view; }
            TagView& tags() { return _tag_view; }

            const CDataView& cdatas() const { return _cdata_view; }
            CDataView& cdatas() { return _cdata_view; }
    };

}

namespace std {
    inline void swap(XMPP::ChildrenList& cl1, XMPP::ChildrenList& cl2) {
        cl1.swap(cl2);
    }
}


#endif
