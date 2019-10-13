// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetClassName and GetClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#ifndef _CORE_STRING_H_
#define _CORE_STRING_H_
#include <Core/Object.h>
#include <Core/Types/ValueType.h>
#include <Core/Types/Array.h>
#include <string>

namespace Meow {
	namespace Core {
		namespace Types {
			class Array;
			typedef Reference<Array> Array_ptr;

			class String;
			typedef Reference<String> String_ptr;

			class Integer;
			typedef Reference<Integer> Integer_ptr;

			class ValueType;

			class String : public ValueType
			{
			public:
				String();
				String( std::string initialiser );
				virtual ~String();
				void SetValue( std::string item );
				std::string GetValue();
				String_ptr  Reverse();
				String_ptr  UpperCaseFirstChar();
				String_ptr  SubString(int offset, int length );
				String_ptr ToLowerCase();
				String_ptr ToUpperCase();
				Array_ptr Explode( std::string delimiter );
				Array_ptr CSVExplode( char delimiter );
				String_ptr GetNextToken(int offset, char delimiter);
				String_ptr GetAfterToken(int offset, char delimiter);
				char       FirstChar();
				char       LastChar();
				String_ptr ShrinkBy(int amount);
				String_ptr SkipLeadingTabs( );
				virtual unsigned long       Count();
				virtual bool        IsNumeric();
				virtual bool        CouldBeFloat();
				virtual bool        CouldBeInteger();
				virtual long        GetValueAsInteger();
				virtual double      GetValueAsFloat();
				virtual std::string GetValueAsString();
				virtual bool        GetValueAsBoolean();
				virtual void      * GetValueAsVoid();
				virtual long        GetVoidLength();
				void       			Append( String_ptr otherstr );
				void       			Append( std::string other );
				virtual void        Append( ValueType_ptr other );
				virtual void        Increment( );
				virtual void        Decrement( );
				virtual void        Let( ValueType_ptr other );
				virtual void        Repeat( int times );
				virtual void        Add( ValueType_ptr other );
				virtual void        Subtract( ValueType_ptr other );
				virtual void        Divide( ValueType_ptr other );
				virtual void        Multiply( ValueType_ptr other );
				virtual void        Negate( );
				virtual void        Modulus( ValueType_ptr other );
				virtual bool        GreaterThan( ValueType_ptr other );
				virtual bool        LessThan( ValueType_ptr other );
				virtual bool        Equals( ValueType_ptr other );
				virtual bool        NotEquals( ValueType_ptr other );
				virtual void        And( ValueType_ptr other );
				virtual void        Or( ValueType_ptr other );
				virtual void        Xor( ValueType_ptr other );
				virtual ValueType_ptr               Left( );
				virtual ValueType_ptr               Right( );
				bool Contains( char character);
				String_ptr Between( std::string starts, std::string ends);
				Integer_ptr IndexOf( std::string needle );
				Array_ptr Chunk( int stringlength=76 );

				static std::string Trim( std::string otherString );
				Core::Types::String_ptr SubstituteVariables( Core::Types::Array_ptr properties );
				
				
			private:
				string Content;
			};
		}
	}
} // End of namespace

#endif
