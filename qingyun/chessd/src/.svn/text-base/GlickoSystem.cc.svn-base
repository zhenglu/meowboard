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

#include "GlickoSystem.hh"
#include <cmath>
#include <algorithm>
#include <time.h>
#include "Util/Date.hh"

double GlickoSystem::c() {
	return 1.483; //got from previous chessd server
}
double GlickoSystem::InitRD(const Rating &r) {
	//logaritmal formula
    /*FIXME: current time should all use the boost library, MUST not use the standard time()*/
    int current_time = Util::ptime_to_time_t(Util::ptime_local_time());
	return std::min(sqrt(r.volatility()*r.volatility()+c()*c()*log(1.0+double(current_time-r.last_game())/60.0)),350.0);
	//linear formula
//	return std::min(sqrt(r.volatility()*r.volatility()+square(0.027777777777777)*double(time(NULL)-r.last_game())),350.0);
}
double GlickoSystem::gRD(const Rating &r) {
	return 1.0/(sqrt(1.0 + 3.0*q()*q()*square(r.volatility())/square(M_PI)));
}
double GlickoSystem::gBRD(const Rating &r1, const Rating &r2, const Rating &r3) {
	return 1.0/sqrt(1.0 + 9.0*square(Bq())/square(M_PI) * (square(r1.volatility()) + square(r2.volatility()) + square(r3.volatility()) ) ) ;
}
double GlickoSystem::E(const Rating &r, const Rating &r2) {
	return 1.0/(1.0+pow(10.0,gRD(r2)*double(r2.rating()-r.rating())/400.0 ));
}
double GlickoSystem::BE(const Rating &r, const Rating &r1, const Rating &r2, const Rating &r3) {
	return 1.0/(1.0+pow(10.0,double(r2.rating()+r3.rating()-r.rating()-r1.rating())*(gBRD(r1,r2,r3))/800.0 ));
}
double GlickoSystem::q() {
	return 0.00575646273248511421;
}
double GlickoSystem::Bq() {
	return 0.00287823136624255710;
}
double GlickoSystem::dsquare(const Rating &r, const Rating &r2) {
	return 1.0/(square(q()) *square(gRD(r2)) * E(r,r2) * (1.0 - E(r,r2)));
}
int GlickoSystem::round(double n) {
	if(n>0)
		return (int)(n+0.5);
	return (int)(n-0.5);
}
