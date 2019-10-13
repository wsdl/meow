// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_BOOLEAN_H_
#define _CORE_BOOLEAN_H_
#include <Core/Object.h>
#include <Core/Types/ValueType.h>
#include <string>

namespace Meow {
	namespace Core {
		namespace Types {
			class Boolean : public ValueType
			{
			public:
				Boolean();
				Boolean(std::string);
				Boolean(bool);
				virtual ~Boolean();

				void SetValue( bool item );
				bool GetValue();
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
				virtual void        Increment( );
				virtual void        Decrement( );
				virtual void        Let( Core::Types::ValueType_ptr other );
				virtual void        Add( Core::Types::ValueType_ptr other );
				virtual void        Subtract( Core::Types::ValueType_ptr other );
				virtual void        Negate( );
				virtual bool        GreaterThan( Core::Types::ValueType_ptr other );
				virtual bool        LessThan( Core::Types::ValueType_ptr other );
				virtual bool        Equals( Core::Types::ValueType_ptr other );
				virtual bool        NotEquals( Core::Types::ValueType_ptr other );
				virtual void        And( Core::Types::ValueType_ptr other );
				virtual void        Or( Core::Types::ValueType_ptr other );
				virtual void        Xor( Core::Types::ValueType_ptr other );
			private:
				bool Content;
			};
			typedef Reference<Boolean> Boolean_ptr;
		}
	}
} // End of namespace

#endif
