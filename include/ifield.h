/*
Copyright (C) 2008 by Charles Solar
charlessolar@gmail.com

This file is part of bafprp.

bafprp is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bafprp is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with bafprp.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BAFPRPIFIELD_H
#define BAFPRPIFIELD_H

#include <string>
#include <map>

#include "bafdefines.h"


namespace bafprp
{
	class IField
	{
		friend class FieldMaker;
	public:
		virtual bool getBool() const { return 0; }
		virtual int getInt() const { return 0; }
		virtual long getLong() const { return 0; }
		virtual float getFloat() const { return 0.0f; }
		virtual std::string getString() const { return ""; }

		std::string getError() const { return _lastError; }
		DWORD getUID() const { return _uid; }
		
		virtual bool convert ( const BYTE* data ) = 0;
		virtual int getSize() const = 0;
		// returns a generic type that would most easily be asociated with the type of data
		// useful for sql output
		virtual std::string getType() const = 0;
		virtual std::string getName() const = 0;

		~IField() {}
	protected:
		IField() : _converted(false), _return(""), _lastError("") 
		{
			// each field needs a unique id so we can safely list them with a 'getNext' in the record class.
			static DWORD uid_counter = 0;
			_uid = uid_counter++;
		}

		bool _converted;
		std::string _return;
		std::string _lastError;
	private:
		DWORD _uid;
	};

	class FieldMaker
	{
	private:
		typedef std::map<std::string, FieldMaker*> maker_map;

		static maker_map& getReg()
		{
			static maker_map registry;
			return registry;
		}
		FieldMaker() {}
	public:
		static IField* newField( const std::string type );
	protected:
		FieldMaker( std::string type )
		{
			getReg().insert( std::make_pair ( type, this ) );
		}
		virtual IField* make() const = 0;
	};
}

#endif