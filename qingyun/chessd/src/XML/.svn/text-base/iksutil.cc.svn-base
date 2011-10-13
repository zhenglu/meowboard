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

#include "iksutil.hh"

#include <cstdio>
#include <cstring>
#include <cerrno>

using namespace std;

namespace XML {

	static iks* _tag2iks(const Tag& tag, ikstack* stack) {
		iks* iktree = iks_new_within(tag.name().c_str(), stack);
		AttributeMap::const_iterator a_it;
		for(a_it = tag.attributes().begin(); a_it != tag.attributes().end(); ++a_it)
			iks_insert_attrib(iktree, a_it->first.c_str(), a_it->second.c_str());
		ItemList::const_iterator c_it;
		for(c_it = tag.children().begin(); c_it != tag.children().end(); ++c_it) {
			if(typeid(*c_it)==typeid(Tag)) {
				const Tag& tag_child = dynamic_cast<const Tag&>(*c_it);
				iks_insert_node(iktree, _tag2iks(tag_child,stack));
			} else if(typeid(*c_it)==typeid(CData)) {
				const CData& cdata_child = dynamic_cast<const CData&>(*c_it);
				iks_insert_cdata(iktree, cdata_child.data().c_str(), cdata_child.data().size());
			}
		}
		return iktree;
	}

	iks* tag2iks(const Tag& tag) {
		ikstack* stack = iks_stack_new(256, 256);
		return _tag2iks(tag, stack);
	}

	Tag* iks2tag(iks* tree) {
		Tag* tag = new Tag(iks_name(tree));
		for(iks* attrib = iks_attrib(tree); attrib; attrib  = iks_next(attrib))
			tag->attributes().insert(make_pair(string(iks_name(attrib)),
						string(iks_cdata(attrib))));
		for(iks* child = iks_child(tree); child; child = iks_next(child))
			if(iks_type(child) == IKS_TAG)
				tag->children().push_back(iks2tag(child));
			else
				tag->children().push_back(new CData(iks_cdata(child)));
		return tag;
	}

	static int readTagHook (void* user_data, char* name, char** atts, int type) {
		TagGenerator* generator = (TagGenerator*) user_data;
		if(type==IKS_OPEN||type==IKS_SINGLE) {
			generator->openTag(name);
			if(atts) 
				for(;*atts;atts+=2) {
					generator->addAttribute(*atts, *(atts+1));
				}
		}
		if(type==IKS_CLOSE||type==IKS_SINGLE) {
			generator->closeTag();
		}
		return IKS_OK;
	}

	static int readCDataHook (void* user_data, char* data, size_t len) {
		TagGenerator* generator = (TagGenerator*) user_data;
		if(not generator->empty())
			generator->addCData(string(data, len));
		return IKS_OK;
	}

#define BUFFER_SIZE 2048

	Tag* iksReadXMLFile(const std::string& filename) {
		TagGenerator generator;
		FILE* file;
		size_t len;
		file = fopen(filename.c_str(), "r");
		if(file==0) return 0;
		char* buffer = new char[BUFFER_SIZE];
		iksparser* parser = iks_sax_new((void*)&generator, readTagHook, readCDataHook);
		while((len=fread(buffer, 1, BUFFER_SIZE, file))) {
			if(iks_parse(parser, buffer, len, 0)!=IKS_OK) break;
		}
		if(len != 0 or iks_parse(parser, 0, 0, 1)!=IKS_OK or not generator.empty()) {
			iks_parser_delete(parser);
			delete[] buffer;
			fclose(file);
			return 0;
		}
		iks_parser_delete(parser);
		delete[] buffer;
		fclose(file);
		return generator.getTag();
	}

	Tag* iksReadXMLString(const std::string& xml) {
		TagGenerator generator;
		iksparser* parser = iks_sax_new((void*)&generator, readTagHook, readCDataHook);
		if(iks_parse(parser, xml.c_str(), xml.size(), 1)!=IKS_OK) {
			iks_parser_delete(parser);
			return 0;
		}
		iks_parser_delete(parser);
		return generator.getTag();
	}

}
