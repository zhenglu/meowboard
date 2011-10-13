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

#ifndef CASTFUNCTOR_HH
#define CASTFUNCTOR_HH

#include <functional>

template <class T1, class T2>
struct CastFunctor : public std::unary_function<T2&, T1&> {
	T1& operator()(T2& item) const {
		return static_cast<T1&>(item);
	}
	const T1& operator()(const T2& item) const {
		return static_cast<const T1&>(item);
	}
};



#endif
