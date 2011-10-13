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

#ifndef I18N_HH
#define I18N_HH

#include <map>
#include <string>
#include <stdint.h>

/* XXX This is currently unused code */

class I18n {
    public:

        /*! \brief Return the code of a text */
        uint32_t getTextCode(const std::string& text_name) const;

        /*! \brief Return the tranlation of a text */
        const std::string& getText(const std::string& text_name, const std::string& lang) const;

        /*! \brief Load langs files in the given path */
        void loadLangs(const std::string& locales_path);

    private:

        void loadCodeTable(const std::string& file_name);

        void loadLang(const std::string& file_name);

        
        std::map<std::string, uint32_t> code_map;

        std::map<std::string, std::map<std::string, std::string> > langs;
};

extern I18n i18n;

#endif
