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

#ifndef TAGBASE_HH
#define TAGBASE_HH

#include "Util/utils.hh"
#include "View.hh"
#include "CastFunctor.hh"

#include "Item.hh"
#include "CData.hh"
#include "Xml.hh"

namespace XML {

	typedef boost::ptr_vector<Item> ItemList;
	typedef boost::ptr_vector<Item> TagList;

	class Tag;

	class TagBase {
		public:
			TagBase() :
				_tag_view(this->children()),
				_cdata_view(this->children()) { }

			TagBase(const Tag& tag) :
				_tag_view(this->children()),
				_cdata_view(this->children()) {
					foreach(child, tag.children()) {
						this->children().push_back(child->clone());
					}
				}

			TagBase(Tag* tag) :
				_tag_view(this->children()),
				_cdata_view(this->children()) {
					swap(this->children(), tag->children());
					delete tag;
				}

			TagBase(const ItemList& items) :
				_tag_view(this->children()),
				_cdata_view(this->children()) {
					foreach(child, items) {
						this->children().push_back(child->clone());
					}
				}


			void clearChildren();

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
