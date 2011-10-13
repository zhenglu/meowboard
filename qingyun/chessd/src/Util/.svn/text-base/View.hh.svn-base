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

#ifndef VIEW_HH
#define VIEW_HH

#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <functional>


template <class T>
struct Identity : public std::unary_function<T, T> {
	const T& operator()(const T& t) const {
		return t;
	}
	T& operator()(T& t) const {
		return t;
	}
};

template <class CONTAINER, class FILTER, class TRANSFORM = Identity<typename CONTAINER::value_type> >
struct View  {
	private:
		template <class T> struct constify;
		template <class T> struct constify<T&> {
			typedef const T& type;
		};
		template <class T> struct constify<T*> {
			typedef const T* type;
		};
		template <class T> struct constify {
			typedef const T type;
		};

        template <class T> struct dereference;
        template <class T> struct dereference<T&> {
            typedef T type;
        };
        template <class T> struct dereference {
            typedef T type;
        };

		typedef typename CONTAINER::iterator iterator_base;
		typedef typename CONTAINER::const_iterator const_iterator_base;

		typedef boost::filter_iterator<FILTER, iterator_base> filter_iterator;
		typedef boost::filter_iterator<FILTER, const_iterator_base> const_filter_iterator;

	public:
		View(CONTAINER& container) : container(container) { }
		~View() { }

		typedef boost::transform_iterator<TRANSFORM, filter_iterator> iterator;
		typedef boost::transform_iterator<TRANSFORM, const_filter_iterator,
				typename constify<typename TRANSFORM::result_type>::type> const_iterator;

        typedef typename dereference<typename TRANSFORM::result_type>::type type;

		iterator begin() {
			return iterator(filter_iterator(this->container.begin(), this->container.end()));
		}
		iterator end() {
			return iterator(filter_iterator(this->container.end(), this->container.end()));
		}
		const_iterator begin() const {
			return const_iterator(const_filter_iterator(this->container.begin(), this->container.end()));
		}
		const_iterator end() const {
			return const_iterator(const_filter_iterator(this->container.end(), this->container.end()));
		}

        type& front() {
            return *this->begin();
        }
        const type& front() const {
            return *this->begin();
        }

		void erase(iterator user) {
			this->items.erase(user.base().base());
		}

	private:
		CONTAINER& container;
};

#endif
