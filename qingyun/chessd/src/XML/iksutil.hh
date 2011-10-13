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

#ifndef IKSUTIL_HH
#define IKSUTIL_HH

#include "Xml.hh"

#include <iksemel.h>
#include <string>

namespace XML {
	/* Transform a Tag into a iksemel tree */
	iks* tag2iks(const XML::Tag& tag);

	/* Transform a iksemel tree into a Tag */
	XML::Tag* iks2tag(iks* tree); 

	/* Read an xml from a file */
	XML::Tag* iksReadXMLFile(const std::string& filename); 
	XML::Tag* iksReadXMLString(const std::string& xml); 

}

#endif
