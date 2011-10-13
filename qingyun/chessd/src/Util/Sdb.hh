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

#ifndef UTILSDB_HH
#define UTILSDB_HH

#include <map>
#include <list>
#include <vector>
#include <iterator>
#include <functional>

#include "utils.hh"

/* XXX This code is currently unused */

namespace Util {

	template <class Data> class SimpleDatabase;

	template <class Data>
		class SDBIndexBase {
			protected:
				typedef std::list<Data> Table;

				virtual void insertEntry(typename Table::iterator entry) = 0;
				virtual void eraseEntry(typename Table::iterator entry) = 0;
				virtual ~SDBIndexBase() { }
				friend class SimpleDatabase<Data>;
		};

	/* Template for a key generator for containers.
	 * The keys are the container elements */
	template <class Container> struct KeyElements :
		public std::unary_function<Container, std::pair<typename Container::const_iterator, typename Container::const_iterator> > {
			std::pair<typename Container::const_iterator, typename Container::const_iterator> operator ()(const Container& container) {
				return make_pair(container.begin(), container.end());
			}
		};

	/* Generate a key which is a member variable */
	template <class Type, Type constant> struct _KeyMember;

	template <class Type, class MemberType, MemberType Type::*constant> struct _KeyMember<MemberType Type::*, constant > :
		public std::unary_function<MemberType,std::pair<const MemberType*, const MemberType*> >{
			std::pair<const MemberType*, const MemberType*> operator()(const Type& t) const {
				return std::make_pair(&(t.*constant),&(t.*constant)+1);
			}
		};

#define KeyMember(x) _KeyMember<typeof(x),x>

	template <class Iterator> struct cmp_iterator : public std::binary_function<bool, Iterator, Iterator> {
		bool operator()(Iterator it1, Iterator it2) const {
			return &*it1 < &*it2;
		}
	};

	template <class Data, class KeyFunctor> class SDBSingleIndex :
		public SDBIndexBase<Data> {
			private:
				typedef typename std::list<Data> Table;

			public:
				typedef typename std::iterator_traits<typename KeyFunctor::result_type::first_type>::value_type key_type;
				typedef typename std::iterator_traits<typename KeyFunctor::result_type::second_type>::value_type data_type;
				typedef std::pair<const key_type&, const data_type&> value_type;
				typedef std::less<key_type> key_compare;
			private:
				typedef std::map<key_type, typename Table::iterator> Index;

			public:
				typedef typename Index::iterator iterator;
				typedef typename Index::const_iterator const_iterator;

			private:

				typedef _GLIBCXX_STD::multimap<typename Table::iterator, typename Index::iterator, cmp_iterator<typename Table::iterator> > MetaIndex;

				KeyFunctor functor;

				Index index;
				MetaIndex meta_index;

			public:

				const_iterator begin() const {
					return this->index.begin();
				}
				iterator begin() {
					return this->index.begin();
				}

				iterator end() {
					return this->index.end();
				}
				const_iterator end() const {
					return this->index.end();
				}

				iterator find(const key_type& key) {
					return this->index.find(key);
				}
				const_iterator find(const key_type& key) const {
					return this->index.find(key);
				}

				iterator lower_bound(const key_type& key) {
					return this->index.lower_bound(key);
				}
				const_iterator lower_bound(const key_type& key) const {
					return this->index.lower_bound(key);
				}

				iterator upper_bound(const key_type& key) {
					return this->index.upper_bound(key);
				}
				const_iterator upper_bound(const key_type& key) const {
					return this->index.upper_bound(key);
				}

			protected:
				SDBSingleIndex(Table& table) {
					foreach(entry, table) {
						this->insertEntry(entry);
					}
				}

				virtual void insertEntry(typename Table::iterator entry) {
					foreach_pair(key, this->functor(*entry)) {
						typename Index::iterator tmp = this->index.insert(std::make_pair(*key,entry)).first;
						this->meta_index.insert(std::make_pair(entry, tmp));
					}
				}

				virtual void eraseEntry(typename Table::iterator entry) {
					foreach_pair(meta, this->meta_index.equal_range(entry)) {
						this->index.erase(meta->second);
					}
					this->meta_index.erase(entry);
				}
		};

	template <class Data, class KeyFunctor> class SDBMultiIndex : public SDBIndexBase<Data> {
		public:
			typedef typename std::iterator_traits<typename KeyFunctor::result_type::first_type>::value_type key_type;
			typedef typename std::iterator_traits<typename KeyFunctor::result_type::second_type>::value_type data_type;
			typedef typename std::pair<const key_type&, const data_type&> value_type;
			typedef typename std::less<key_type> key_compare;
		private:
			typedef std::list<Data> Table;
			typedef _GLIBCXX_STD::multimap<key_type, typename Table::iterator> Index;
		public:
			typedef typename Index::const_iterator const_iterator;
			typedef typename Index::iterator iterator;

		private:
			KeyFunctor functor;

			typedef _GLIBCXX_STD::multimap<typename Table::iterator, typename Index::iterator, cmp_iterator<typename Table::iterator> > MetaIndex;

			Index index;
			MetaIndex meta_index;

		public:

			const_iterator begin() const {
				return this->index.begin();
			}
			iterator begin() {
				return this->index.begin();
			}

			const_iterator end() const {
				return this->index.end();
			}
			iterator end() {
				return this->index.end();
			}

			const_iterator find(const key_type& key) const {
				return this->index.find(key);
			}
			iterator find(const key_type& key) {
				return this->index.find(key);
			}

			const_iterator lower_bound(const key_type& key) const {
				return this->index.lower_bound(key);
			}
			iterator lower_bound(const key_type& key) {
				return this->index.lower_bound(key);
			}

			const_iterator upper_bound(const key_type& key) const {
				return this->index.upper_bound(key);
			}
			iterator upper_bound(const key_type& key) {
				return this->index.upper_bound(key);
			}

			std::pair<const_iterator,const_iterator> equal_range(const key_type& key) const {
				return this->index.equal_range(key);
			}
			std::pair<iterator,iterator> equal_range(const key_type& key) {
				return this->index.equal_range(key);
			}

		protected:
			SDBMultiIndex(Table& table) {
				foreach(entry, table) {
					this->insertEntry(entry);
				}
			}

			virtual void insertEntry(typename Table::iterator entry) {
				foreach_pair(key, this->functor(*entry)) {
					typename Index::iterator tmp = this->index.insert(std::make_pair(*key,entry));
					this->meta_index.insert(std::make_pair(entry, tmp));
				}
			}

			virtual void eraseEntry(typename Table::iterator entry) {
				foreach_pair(meta, this->meta_index.equal_range(entry)) {
					this->index.erase(meta->second);
				}
				this->meta_index.erase(entry);
			}
	};

	template <class Data, class KeyFunctor, bool MultiIndex=false> struct SDBIndex;

	template <class Data, class KeyFunctor> struct SDBIndex<Data, KeyFunctor, true> :
		public SDBMultiIndex<Data, KeyFunctor> {
			protected:
				SDBIndex(std::list<Data>& table) : SDBMultiIndex<Data, KeyFunctor>(table) { }
				friend class SimpleDatabase<Data>;
		};

	template <class Data, class KeyFunctor> struct SDBIndex<Data, KeyFunctor, false> :
		public SDBSingleIndex<Data, KeyFunctor> {
			protected:
				SDBIndex(std::list<Data>& table) : SDBSingleIndex<Data, KeyFunctor>(table) { }
				friend class SimpleDatabase<Data>;
		};

	template <class Data> class SimpleDatabase {
		private:
			/* it has to be a list, not a vector */
			typedef std::list<Data> ItemList;

		public:
			typedef typename ItemList::iterator iterator;
			typedef typename ItemList::const_iterator const_iterator;

			SimpleDatabase() { }
			~SimpleDatabase() {
				foreach(index, this->indices) {
					delete *index;
				}
			}

			template <class KeyFunctor, bool MultiIndex> SDBIndex<Data, KeyFunctor,MultiIndex>* createIndex() {
				this->indices.push_back(new SDBIndex<Data, KeyFunctor, MultiIndex>(this->items));
				return static_cast<SDBIndex<Data, KeyFunctor, MultiIndex>*>(this->indices.back());
			}

			template <class KeyFunctor, bool MultiIndex> const SDBIndex<Data, KeyFunctor,MultiIndex>* getIndex() const {
				foreach(index, this->indices) {
					const SDBIndex<Data, KeyFunctor, MultiIndex>* tmp =
						dynamic_cast<const SDBIndex<Data, KeyFunctor, MultiIndex>*>(*index);
					if(tmp)
						return tmp;
				}
				return 0;
			}

			template <class KeyFunctor, bool MultiIndex> SDBIndex<Data, KeyFunctor,MultiIndex>* getIndex() {
				foreach(index, this->indices) {
					SDBIndex<Data, KeyFunctor, MultiIndex>* tmp =
						dynamic_cast<SDBIndex<Data, KeyFunctor, MultiIndex>*>(*index);
					if(tmp)
						return tmp;
				}
				return 0;
			}

			void insert(const Data& item) {
				typename std::list<Data>::iterator it = this->items.insert(this->items.end(), item);
				foreach(index, this->indices) {
					(*index)->insertEntry(it);
				}
			}

			void erase(typename std::list<Data>::iterator item) {
				this->items.erase(item);
				foreach(index, this->indices) {
					(*index)->eraseEntry(item);
				}
			}

			template <class KeyFunctor, bool MultiIndex> void erase(const typename SDBIndex<Data, KeyFunctor, MultiIndex>::key_type& key, SDBIndex<Data, KeyFunctor, MultiIndex>* index) {
				std::vector<typename std::list<Data>::iterator> vec;
				typeof(index->equal_range(key)) pair = index->equal_range(key);
				foreach_pair(it, pair) {
					vec.push_back(it->second);
				}
				foreach(it, vec) {
					this->erase(*it);
				}
			}

			template <class KeyFunctor, bool MultiIndex> void erase(typename SDBIndex<Data, KeyFunctor, MultiIndex>::iterator it) {
				this->erase(it->second);
			}

			void update(typename std::list<Data>::iterator item) {
				foreach(index, this->indices) {
					(*index)->eraseEntry(item);
					(*index)->insertEntry(item);
				}
			}

			iterator begin() {
				return this->items.begin();
			}

			iterator end() {
				return this->items.end();
			}

			const_iterator begin() const {
				return this->items.begin();
			}

			const_iterator end() const {
				return this->items.end();
			}

			typename ItemList::size_type size() const {
				return this->items.size();
			}

		private:
			ItemList items;
			std::vector<SDBIndexBase<Data>*> indices;
	};

}
#endif
