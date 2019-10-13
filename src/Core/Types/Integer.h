// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_INTEGER_H_
#define _CORE_INTEGER_H_
#include <Core/Object.h>
#include <Core/Types/ValueType.h>
#include <string>

namespace Meow {
	namespace Core {
		namespace Types {
			class Integer;
			typedef Reference<Integer> Integer_ptr;
			class Integer : public Core::Types::ValueType
			{
			public:
				static Integer_ptr GetRandomNumber( );

				Integer();
				Integer(std::string);
				Integer(int);
				virtual ~Integer();

				void SetValue( long item );
				long GetValue();
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

				virtual void                Append( ValueType_ptr other );
				virtual void                Increment( );
				virtual void                Decrement( );
				virtual void                Let( ValueType_ptr other );
				virtual void                Repeat( int times );
				virtual void                Add( ValueType_ptr other );
				virtual void                Subtract( ValueType_ptr other );
				virtual void                Divide( ValueType_ptr other );
				virtual void                Multiply( ValueType_ptr other );
				virtual void                Negate( );
				virtual void                Modulus( ValueType_ptr other );
				virtual bool                GreaterThan( ValueType_ptr other );
				virtual bool                LessThan( ValueType_ptr other );
				virtual bool                Equals( ValueType_ptr other );
				virtual bool                NotEquals( ValueType_ptr other );
				virtual unsigned long Count( );
			private:
				long Content;
			};
		}
	}

} // End of namespace

#endif
