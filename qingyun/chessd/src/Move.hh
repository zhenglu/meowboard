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

#ifndef MOVE_HH
#define MOVE_HH

/*! \brief This class implments the move pattern
 *
 * The move pattern is used to pass an object to a function
 * meaning that the object is not going to be used by the caller anymore.
 * So, he function can safely modify the object without any visible effect
 * by the caller.
 */
template <class T>
class Moved {
	public:
		explicit Moved(T& ref) : _ref(ref) { }

		~Moved() { }

		T& operator*() { return _ref; }
		const T& operator*() const { return _ref; }

		T* operator->() { return &_ref; }
		const T* operator->() const { return &_ref; }


	private:
		T& _ref;
};

template <class T, class U>
Moved<T> move(Moved<U> ref) {
	return Moved<T>(*ref);
}

template <class T>
Moved<T> move(T& ref) {
	return Moved<T>(ref);
}


#endif
