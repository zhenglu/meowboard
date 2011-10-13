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

#ifndef HISTORYPROCESS_HH
#define HISTORYPROCESS_HH

#include "XML/Tag.hh"
#include "libchess/xiangqi.hh"
#include <vector>

class HistoryProcess {
	public:
		HistoryProcess() { }

		static XML::Tag* generate(XML::Tag* history);
	private:
};

class ChessHistoryProcess : public HistoryProcess {
	public:
		ChessHistoryProcess() { }

		static XML::Tag* generate(XML::Tag* history);

		static XML::Tag* generateStateTag(const XiangqiState& state,int time, const std::string& mv, const std::string& short_mv);
	private:
};

#endif
