// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_DATE_H_
#define _CORE_DATE_H_
#include <Core/Types/ValueType.h>
#include <Core/Types/Array.h>
// All our dates are in GMT offset.
//

namespace Meow {
	namespace Core {
		namespace Time {
			class Date;
			typedef Reference<Date> Date_ptr;

			class Date : public Core::Types::ValueType
			{
			public:
				Date( std::string date_as_string );
				Date( std::string date_as_string, std::string format );
				Date( long date_as_long );
				Date(  Meow::Core::Types::ValueType_ptr );
				Date(
						short hour,
						short minute,
						short second,
						short day,
						short month,
						short year );
				Date();
				virtual ~Date();

				static Meow::Core::Time::Date_ptr 	GetCurrentTime();
				short 				GetHourOfDay( bool usegmt=false);
				short 				GetMinuteOfHour( bool usegmt=false );
				short 				GetSecondOfMinute( bool usegmt=false );
				short 				GetDayOfMonth( bool usegmt=false);
				short 				GetMonthOfYear( bool usegmt=false);
				short 				GetYear( bool usegmt=false);
				short 				GetDayOfWeek( bool usegmt=false );
				void 				AddHour( short no_hours );
				void 				AddMinute( short no_mins );
				void 				AddSecond( short no_seconds );
				void 				AddDay( short days );
				void 				AddMonth( short months );
				void 				AddYear( short years );
				void 				SubtractHour( short no_hours );
				void 				SubtractMinute( short no_mins );
				void 				SubtractSecond( short no_seconds );
				void 				SubtractDay( short days );
				void 				SubtractMonth( short months );
				void 				SetToStartOfDay( );
				void 				SetToEndOfDay( );
				void 				SetSecondOfMinute( int second );
				void 				SetMinuteOfHour( int minute );
				void 				SetHourOfDay( int hour );
				void 				SetDayOfMonth( int day );
				void 				SetMonthOfYear( int month );
				void 				SetYear( int year );
				bool 				Equals( Date_ptr another );
				void 				Add( Date_ptr other );
				virtual bool        IsNumeric();
				virtual bool        CouldBeFloat();
				virtual bool        CouldBeInteger();
				virtual long        GetValueAsInteger();
				virtual double      GetValueAsFloat();
				virtual std::string GetValueAsString();
				std::string         Format( std::string format, bool usegmt=false );
				virtual void        Increment( );
				virtual void        Decrement( );
				virtual void        Let( Meow::Core::Types::ValueType_ptr other );
				virtual void        Add( Meow::Core::Types::ValueType_ptr other );
				virtual void        Subtract( Meow::Core::Types::ValueType_ptr other );
				virtual void        Divide( Meow::Core::Types::ValueType_ptr other ) ;
				virtual void        Multiply( Meow::Core::Types::ValueType_ptr other );
				virtual bool        GreaterThan( Meow::Core::Types::ValueType_ptr other );
				virtual bool        LessThan( Meow::Core::Types::ValueType_ptr other );
				virtual bool        Equals( Meow::Core::Types::ValueType_ptr other );
				virtual bool        NotEquals( Meow::Core::Types::ValueType_ptr other );
				
				void 				FromStandardString( std::string timeString );
				Date_ptr			NextWorkingDay( int number =1);
			private:
				unsigned long ts;
			};
		}
	}
} // End of namespace

#endif
