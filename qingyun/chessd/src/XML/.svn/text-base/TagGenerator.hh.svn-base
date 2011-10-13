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

#ifndef XMLTAGGENERATOR_HH
#define XMLTAGGENERATOR_HH

#include <string>
#include <stack>
#include "Tag.hh"
#include "CData.hh"

namespace XML {

	class TagGenerator {
		public:
			TagGenerator();
			~TagGenerator();

			void openTag(const std::string& name);

			void closeTag();

			void addAttribute(const std::string& name, const std::string& value);

			void addCData(const std::string& data);

			void addChild(Tag* tag);

			Tag* getTag();

			bool empty() const;

		private:
			std::stack<Tag*> tag_stack;
			Tag* tag;
	};
}

#endif
