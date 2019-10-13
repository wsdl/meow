// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_VOID_H_
#define _CORE_VOID_H_
#include <Core/Object.h>
#include <Core/Types/ValueType.h>
#include <string>

namespace Meow {
	namespace Core {
		namespace Types {
			class Void : public ValueType
			{
			public:
				Void();
				Void(std::string);
				Void( void * dataptr, unsigned long length );
				~Void();
				void SetValue( void * dataptr, unsigned long length );
				void * GetValue();
				std::string ToString();
				virtual bool        IsNumeric();
				virtual bool        CouldBeFloat();
				virtual bool        CouldBeInteger();
				virtual long        GetValueAsInteger();
				virtual double      GetValueAsFloat();
				virtual std::string GetValueAsString();
				virtual bool        GetValueAsBoolean();
				virtual void      * GetValueAsVoid();
				virtual long        GetVoidLength();
				virtual unsigned long Count( );
				virtual void                Append( ValueType_ptr other );
				virtual void                Let( ValueType_ptr other );
				virtual void                Repeat( int times );
				virtual ValueType_ptr               Left( );
				virtual ValueType_ptr               Right( );
			private:
				char * Content;
				unsigned long ContentLength;
			};
			typedef Reference<Void> Void_ptr;
		}
	}
} // End of namespace

#endif
