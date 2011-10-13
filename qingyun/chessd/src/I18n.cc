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

#include "I18n.hh"

#include "XML/Xml.hh"
#include "Util/utils.hh"

/* XXX This is currently unused code */

I18n i18n;

uint32_t I18n::getTextCode(const std::string& text_name) const {
    /* find the string code */
    std::map<std::string, uint32_t>::const_iterator it = this->code_map.find(text_name);

    /* check if the code was found */
    if(it == this->code_map.end()) {
        return 0xffffffff;
    } else {
        return it->second;
    }
}

const std::string& I18n::getText(const std::string& text_name, const std::string& lang) const {
    std::map<std::string, std::map<std::string, std::string> >::const_iterator it1;
    std::map<std::string, std::string>::const_iterator it2;

    /* find the lang */
    it1 = this->langs.find(lang);
    if(it1 == this->langs.end())
        return text_name;

    /* find the string name */
    it2 = it1->second.find(text_name);
    if(it2 == it1->second.end())
        return text_name;

    return it2->second;
}

void I18n::loadCodeTable(const std::string& filename) {
    /* parse XML file */
    std::auto_ptr<XML::Tag> xml(XML::parseXmlFile(filename));

    /* read codes */
    foreach(text, xml->tags()) {
        uint32_t code = Util::parse_string<uint32_t>(text->getAttribute("code"));
        const std::string& text_name = text->findCData().data();
        this->code_map[text_name] = code;
    }
}

void I18n::loadLang(const std::string& filename) {
    /* parse file */
    std::auto_ptr<XML::Tag> xml(XML::parseXmlFile(filename));

    /* get lang name */
    std::string lang = xml->getAttribute("lang");

    /* get strings */
    foreach(child, xml->tags()) {
        const std::string& text_name = child->findTag("name").findCData().data();
        const std::string& text = child->findTag("text").findCData().data();
        this->langs[lang][text_name] = text;
    }
}

void I18n::loadLangs(const std::string& locales_path) {
    /* load code */
    this->loadCodeTable(locales_path + "/texts_code.xml");

    /* load langs */
    this->loadLang(locales_path + "/pt-br.xml");
    this->loadLang(locales_path + "/en.xml");
}
