// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_VALUETYPE_H_
#define _CORE_VALUETYPE_H_
#include <Core/Object.h>
#include <Exceptions/DivideByZero.h>
#include <string>

namespace Meow {
	namespace Core {
		namespace Types {
			class ValueType;
			typedef Reference<ValueType> ValueType_ptr;
			class Void;
			typedef Reference<Void> Void_ptr;

			class ValueType : public Core::Object
			{
			public:
				ValueType();
				virtual ~ValueType();

				/* Enquiries */
				virtual bool                        IsNumeric();
				virtual bool                        CouldBeFloat();
				virtual bool                        CouldBeInteger();

				virtual long                        GetValueAsInteger();
				virtual double                      GetValueAsFloat();
				virtual std::string                 GetValueAsString();
				virtual bool                        GetValueAsBoolean();
				virtual void      *                 GetValueAsVoid();
				virtual long                        GetVoidLength();
				bool                      	    MatchesRegularExpression( std::string expression );
				virtual void                        Append( ValueType_ptr other );
				virtual void                        Increment( );
				virtual void                        Decrement( );
				virtual void                        Let( ValueType_ptr other );
				virtual void                        Repeat( int times );
				virtual void                        Add( ValueType_ptr other );
				virtual void                        Subtract( ValueType_ptr other );
				virtual void                        Divide( ValueType_ptr other );
				virtual void                        Multiply( ValueType_ptr other );
				virtual void                        Negate( );
				virtual void        		    Modulus( ValueType_ptr other );
				virtual bool       		    GreaterThan( ValueType_ptr other );
				virtual bool        		    LessThan( ValueType_ptr other );
				virtual bool       		    Equals( ValueType_ptr other );
				virtual bool        		    NotEquals( ValueType_ptr other );
				virtual void        		    And( ValueType_ptr other );
				virtual void        		    Or( ValueType_ptr other );
				virtual void        		    Xor( ValueType_ptr other );
				virtual ValueType_ptr               Left( );
				virtual ValueType_ptr               Right( );
				virtual unsigned long               Count( );
				virtual ValueType_ptr               Base64Encode( bool lengthTrim=false );
				virtual ValueType_ptr               Base64Decode(  bool safeIt=false );
				virtual ValueType_ptr               Replace( std::string find, 	std::string replace );
				virtual ValueType_ptr               Replace( ValueType_ptr ptrToArray );

				std::string                         MD5( );
				Void_ptr                            RawMD5( );
				std::string                         SHA256( );
				Void_ptr                            RawSHA256( );
				std::string                         HTMLEntities(  );
				std::string                         TranscodeDecode( );
				std::string                         TranscodeEncode( );
				std::string             	    StrongHTMLEntities(  );
				std::string                         URLEncode( bool strong );
				std::string             	    URLDecode( );
				std::string             	    XMLEncode();

				static long                         StringToInteger( std::string value );
				static double                       StringToFloat( std::string value );
				static bool                         StringToBoolean( std::string value );
				static std::string      	    IntegerToString( long value );
				static std::string     		    FloatToString( double value );
				static std::string      	    BooleanToString( bool value );

				static std::string		    Tail( std::string, unsigned long characters );
				static std::string		    PrintableOnly( std::string instring );
				static std::string		    Munch( std::string instring );
				static std::string			Encode64( std::string instring );
                std::string                 LeftPadString( char padwith, int length );
			};
		}
	}
} // End of namespace

#endif
