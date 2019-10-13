// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_HOURS_H_
#define _CORE_HOURS_H_
#include <Core/Types/ValueType.h>
// All our dates are in GMT offset.
//

namespace Meow {
	namespace Core {
		namespace Time {
			class Hours;
			typedef Reference<Hours> Hours_ptr;

			class Hours : public Core::Types::ValueType
			{
			public:
				Hours( std::string daysasstring );
				Hours( long days_as_long );
				Hours( Meow::Core::Types::ValueType_ptr );

				Hours();
				virtual ~Hours();


				virtual bool                IsNumeric();
				virtual bool                CouldBeFloat();
				virtual bool                CouldBeInteger();
				virtual long                GetValueAsInteger();
				virtual double              GetValueAsFloat();
				virtual std::string         GetValueAsString();

				virtual void                Increment( );
				virtual void                Decrement( );
				virtual void                Let( Core::Types::ValueType_ptr other );
				virtual void                Add( Core::Types::ValueType_ptr other );
				virtual void                Subtract( Core::Types::ValueType_ptr other );
				virtual void        		Divide( Meow::Core::Types::ValueType_ptr other );
				virtual void                Multiply( Core::Types::ValueType_ptr other );

				virtual bool                GreaterThan( Core::Types::ValueType_ptr other );
				virtual bool                LessThan( Core::Types::ValueType_ptr other );
				virtual bool                Equals( Core::Types::ValueType_ptr other );
				virtual bool                NotEquals( Core::Types::ValueType_ptr other );

				private:
				unsigned long ts;
			};
		}
	}

} // End of namespace

#endif
