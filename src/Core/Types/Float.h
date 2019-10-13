// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_FLOAT_H_
#define _CORE_FLOAT_H_
#include <Core/Object.h>
#include <Core/Types/ValueType.h>
#include <string>

namespace Meow {
	namespace Core {
		namespace Types {
			class Float : public Core::Types::ValueType
			{
			public:
				Float();
				Float( float );
				Float( std::string );
				virtual ~Float();
				void SetValue( double item );
				double GetValue();
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
				virtual void        Let( Meow::Core::Types::ValueType_ptr other );
				virtual void        Add( Meow::Core::Types::ValueType_ptr other );
				virtual void        Subtract( Meow::Core::Types::ValueType_ptr other );
				virtual void        Divide( Meow::Core::Types::ValueType_ptr other );
				virtual void        Multiply( Meow::Core::Types::ValueType_ptr other );
				virtual void        Negate( );
				virtual bool        GreaterThan( Meow::Core::Types::ValueType_ptr other );
				virtual bool        LessThan( Meow::Core::Types::ValueType_ptr other );
				virtual bool        Equals( Meow::Core::Types::ValueType_ptr other );
				virtual bool        NotEquals( Meow::Core::Types::ValueType_ptr other );
				virtual unsigned long Count( );
				void    RoundUp( );
				void    RoundDown( );
			private:
				double Content;
			};
			typedef Reference<Float> Float_ptr;
		}
	}
} // End of namespace
#endif
