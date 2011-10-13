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

#ifndef GLICKOSYSTEM_HH
#define GLICKOSYSTEM_HH
#include "Rating.hh"
/*
 * Glicko System number 1
*/
class GlickoSystem {
	public:
		//Step 1
		/*! \brief returns the c constant for calcuting the new RD based on time*/
		static double c();
		/*! \brief Calculate the new Rating Deviation*/
		static double InitRD(const Rating& r);

		//Step 2
		/*! \brief function g(RD)
		 * 1/sqrt(1+3q^2*RD^2/PI^2)
		 */
		static double gRD(const Rating &r);

		static double gBRD(const Rating &r1, const Rating &r2, const Rating &r3);

		/*! \brief Function E*/
		static double E(const Rating &r1, const Rating &r2);

		/*! \brief Function E for Bughouse*/
		static double BE(const Rating &r, const Rating &r1, const Rating &r2, const Rating &r3);

		/*! \brief ln(10)/400 */
		static double q();

		/*! \brief ln(10)/800 */
		static double Bq();

		/*! \brief function d^2*/
		static double dsquare(const Rating &r1, const Rating &r2);

		static int round(double a);

		template<class T>
		static T square(T x) {
			return x*x;
		}
	private:
};
#endif
