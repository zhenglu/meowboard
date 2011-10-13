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

#ifndef UTILIDSET_HH
#define UTILIDSET_HH

#include <stack>

namespace Util {
	class IDSet {
		public:
			/*! \brief Constructor */
			IDSet();

			/*! \brief Destructor */
			~IDSet();

			/*! \brief Acquire an unused ID.
			 *
			 * Get an unused ID and set it as in use.
			 * \return An unused ID.
			 */
			int acquireID();

			/*! \brief Release an in usedID.
			 *
			 * Release an in use ID so it can be
			 * acquired latter.
			 */
			void releaseID(int ID);
		private:
			std::stack<int> unused_ids;
			int used_id;
	};
}

#endif
