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

#ifndef SAFEOBJECT_HH
#define SAFEOBJECT_HH

#include "RwLock.hh"

namespace Threads {

    template <class T> class SafeObject {
        public:

            SafeObject() { }
            SafeObject(const T& object) : object(object) { }
            ~SafeObject() { }

            T& getWriteLock() {
                this->rwlock.wrlock();
                return object;
            }
            const T& getReadLock() {
                this->rwlock.rdlock();
                return object;
            }
            void releaseLock() {
                this->rwlock.unlock();
            }

        private:
            T object;
            RwLock rwlock;
    };

	template <class T> class ReadLock {
		public:
			ReadLock(SafeObject<T>& safe_object) :
				safe_object(safe_object),
				object(&safe_object.getReadLock()) { }

			~ReadLock() {
                if(this->object != 0) {
                    safe_object.releaseLock();
                }
			}

			const T* operator->() const {
				return this->object;
			}

            const T& operator*() const {
                return *this->object;
            }

            void release() {
                this->safe_object.releaseLock();
                this->object = 0;
            }

		private:
			SafeObject<T>& safe_object;
			const T* object;
	};

	template <class T> class WriteLock {
		public:
			WriteLock(SafeObject<T>& safe_object) :
				safe_object(safe_object),
				object(&safe_object.getWriteLock()) { }

			~WriteLock() {
                if(this->object != 0) {
                    safe_object.releaseLock();
                }
			}

			T* operator->() {
				return this->object;
			}

			const T* operator->() const {
				return this->object;
			}

            T& operator*() {
                return *this->object;
            }

            const T& operator*() const {
                return *this->object;
            }

            void release() {
                this->safe_object.releaseLock();
                this->object = 0;
            }

		private:
			SafeObject<T>& safe_object;
			T* object;
	};
}
#endif
