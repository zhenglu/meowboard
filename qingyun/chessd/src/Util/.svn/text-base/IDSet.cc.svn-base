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

#include "IDSet.hh"

namespace Util {

	IDSet::IDSet() : used_id(0) {
	}

	IDSet::~IDSet() {
	}

	int IDSet::acquireID() {
		if(this->unused_ids.empty()) {
			return this->used_id++;
		} else {
			int id = this->unused_ids.top();
			this->unused_ids.pop();
			return id;
		}
	}

	void IDSet::releaseID(int id) {
		this->unused_ids.push(id);
	}

}
