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

#ifndef XMLCDATA_HH
#define XMLCDATA_HH

#include <string>
#include "Move.hh"

namespace XML {


	class CData : public Item {
		public:
			/* default constructor */
			CData(const std::string& data = "") : _data(data) { }

			/* copy constructor */
			CData(const CData& cdata) : _data(cdata.data()) { }

			/* move constructor */
			CData(Moved<CData> cdata) { this->data().swap(cdata->data()); }

			virtual ~CData() { }

			/* return the char data */
			std::string& data() { return this->_data; }
			const std::string& data() const { return this->_data; }

			/* clone this object */
			virtual CData* clone() const;

			/* generate xml */
			virtual void xml(std::string&) const;

		private:
			std::string _data;
	};

}

#endif
