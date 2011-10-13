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

#ifndef RATING_HH
#define RATING_HH

class Rating {
	public:
		Rating() : _rating(0), _volatility(0.0), _wins(0), _losses(0), _draws(0), _last_game(0) { }
		Rating(int rating, double volatility) : _rating(rating), _volatility(volatility), _wins(0), _losses(0), _draws(0), _last_game(0) { }

		int rating() const { return this->_rating; }
		int& rating() { return this->_rating; }

		double volatility() const { return this->_volatility; }
		double& volatility() { return this->_volatility; }

		int wins() const { return this->_wins; }
		int& wins() { return this->_wins; }

		int losses() const { return this->_losses; }
		int& losses() { return this->_losses; }

		int draws() const { return this->_draws; }
		int& draws() { return this->_draws; }

		int last_game() const { return this->_last_game; }
		int& last_game() { return this->_last_game; }

		int countGames() const { return this->_wins + this->_losses + this->_draws; }

	private:
		int _rating;
		double _volatility;
		int _wins, _losses, _draws;
        int _last_game;
};

#endif
