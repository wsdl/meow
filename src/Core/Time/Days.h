// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_DAYS_H_
#define _CORE_DAYS_H_
#include <Core/Types/ValueType.h>
// All our dates are in GMT offset.
//

namespace Meow {
	namespace Core {
		namespace Time {
			class Days;
			typedef Reference<Days> Days_ptr;

			class Days : public Core::Types::ValueType
			{
			public:
				Days( std::string daysasstring );
				Days( long days_as_long );
				Days( Meow::Core::Types::ValueType_ptr );

				Days();
				virtual ~Days();

				virtual bool                IsNumeric();
				virtual bool                CouldBeFloat();
				virtual bool                CouldBeInteger();
				virtual long                GetValueAsInteger();
				virtual double              GetValueAsFloat();
				virtual std::string         GetValueAsString();

				virtual void                Increment( );
				virtual void                Decrement( );
				virtual void                Let( Meow::Core::Types::ValueType_ptr other );
				virtual void                Add( Meow::Core::Types::ValueType_ptr other );
				virtual void                Subtract( Meow::Core::Types::ValueType_ptr other );
				virtual void        		Divide( Meow::Core::Types::ValueType_ptr other );
				virtual void                Multiply( Meow::Core::Types::ValueType_ptr other );

				virtual bool                GreaterThan( Meow::Core::Types::ValueType_ptr other );
				virtual bool                LessThan( Meow::Core::Types::ValueType_ptr other );
				virtual bool                Equals( Meow::Core::Types::ValueType_ptr other );
				virtual bool                NotEquals( Meow::Core::Types::ValueType_ptr other );

				private:
				unsigned long ts;
			};
		}
	}
} // End of namespace

#endif
