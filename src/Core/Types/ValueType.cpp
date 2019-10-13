// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Types/ValueType.h>
#include <Core/Types/Float.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Boolean.h>
#include <Core/Log.h>
#include <Core/Types/String.h>
#include <Core/Types/Void.h>
#include <iostream>
#ifdef WIN32
	#include <tre.h>
	/* #define tre_regfree regfree
	#define tre_regcomp regcomp
	#define tre_regexec regexec
	#define REG_OK REG_NOERROR */
#else
	#include <tre/tre.h>
#endif
#include <sstream>
#include <Core/Peers/Base64.h>
#include <Core/Peers/OSSL.h>
#include <Core/Types/Void.h>
#include <cstdlib>




Meow::Core::Types::ValueType::ValueType( )
{
    SetMeowClassName("ValueType");
//  iGlyphs=new CoreArray();
//  iGlyphs->Set("&amp;",new String("&"));
//  iGlyphs->Set("&lt;",new String("<"));
//  iGlyphs->Set("&gt;",new String(">"));
//  iReverseGlyphs=new CoreArray();
//  iReverseGlyphs->Set("&",new String("&amp;"));
//  iReverseGlyphs->Set("<",new String("&lt;"));
//  iReverseGlyphs->Set(">",new String("&gt;"));
}

Meow::Core::Types::ValueType::~ValueType( )
{
}

bool Meow::Core::Types::ValueType::IsNumeric()
{
    return false;
}
bool Meow::Core::Types::ValueType::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Types::ValueType::CouldBeInteger()
{
    return false;
}
long Meow::Core::Types::ValueType::GetValueAsInteger()
{
    return 0;
}
double Meow::Core::Types::ValueType::GetValueAsFloat()
{
    return 0.0;
}
std::string Meow::Core::Types::ValueType::GetValueAsString()
{
    return "";
}
bool Meow::Core::Types::ValueType::GetValueAsBoolean()
{
    return false;
}
void * Meow::Core::Types::ValueType::GetValueAsVoid()
{
    return NULL;
}
long  Meow::Core::Types::ValueType::GetVoidLength()
{
    return 0;
}

void Meow::Core::Types::ValueType::Append( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Append",
                                  "sv",
                                  "Could not execute because it is not implemented for me to other object ",
                                  other.getPointerForceIncref() );
}
void Meow::Core::Types::ValueType::Increment( )
{
    Log::GetLog()->Warning( "ValueType::Increment",
                                  "s",
                                  "Could not execute because it is not implemented for me ");
}
void Meow::Core::Types::ValueType::Decrement( )
{
    Log::GetLog()->Warning( "ValueType::Decrement", "s", "Could not execute because it is not implemented for me ");
}
void Meow::Core::Types::ValueType::Let( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Let", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
}
void Meow::Core::Types::ValueType::Repeat( int times )
{
    Log::GetLog()->Warning( "ValueType::Repeat", "si", "Could not execute because it is not implemented for me argument ",times);
}
void Meow::Core::Types::ValueType::Add( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Add", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
}
void Meow::Core::Types::ValueType::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Subtract", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
}
void Meow::Core::Types::ValueType::Divide( Meow::Core::Types::ValueType_ptr other ) 
{
    Log::GetLog()->Warning( "ValueType::Divide", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());

}
void Meow::Core::Types::ValueType::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Multiply", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
}
void Meow::Core::Types::ValueType::Negate( )
{
    Log::GetLog()->Warning( "ValueType::Add", "s", "Could not execute because it is not implemented for me.");

}
void Meow::Core::Types::ValueType::Modulus( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Modulus", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
}
bool Meow::Core::Types::ValueType::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::GreaterThan", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
    return false;
}
bool Meow::Core::Types::ValueType::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::LessThan", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
    return false;
}
bool Meow::Core::Types::ValueType::Equals( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Equals", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
    return false;
}
bool Meow::Core::Types::ValueType::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::NotEquals", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
    return false;
}
void Meow::Core::Types::ValueType::And( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::And", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
}
void Meow::Core::Types::ValueType::Or( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Or", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
}
void Meow::Core::Types::ValueType::Xor( Meow::Core::Types::ValueType_ptr other )
{
    Log::GetLog()->Warning( "ValueType::Xor", "sv", "Could not execute because it is not implemented for me to other object ",other.getPointerForceIncref());
}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::ValueType::Left()
{
    Log::GetLog()->Warning( "ValueType::Left", "s", "Could not execute because it is not implemented for me.");
    Core::Types::ValueType_ptr retval;
    return retval;
}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::ValueType::Right( )
{
    Log::GetLog()->Warning( "ValueType::Right", "s", "Could not execute because it is not implemented for me");
    Core::Types::ValueType_ptr retval;
    return retval;
}
unsigned long Meow::Core::Types::ValueType::Count( )
{
    Log::GetLog()->Warning( "ValueType::Count", "s", "Could not execute because it is not implemented for me");
    return 0;
}

bool Meow::Core::Types::ValueType::MatchesRegularExpression( std::string expression )
{
    regex_t pattern;
    bool returnvalue=false;

    int rc=tre_regcomp(&pattern,expression.c_str(),REG_EXTENDED);
    if ( rc==REG_OK )
    {
        std::string value=GetValueAsString();
        rc=tre_regexec(&pattern,value.c_str(),0,NULL,REG_NOTEOL);
        if ( rc==REG_OK )
            returnvalue=true;
        tre_regfree(&pattern);
    }
    else
    {
        switch ( rc )
        {
        case REG_NOMATCH:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "No match found.");
            break;
        case REG_BADPAT:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Invalid regular expression.");
            break;
        case REG_ECOLLATE:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Unknown collecting element.");
            break;
        case REG_ECTYPE:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Unknown character class name.");
            break;
        case REG_EESCAPE:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Trailing backslash in expression.");
            break;
        case REG_ESUBREG:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Invalid back reference.");
            break;
        case REG_EBRACK:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Imbalance of [ and ] in expression.");
            break;
        case REG_EPAREN:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Impalance of ( and ) in expression.");
            break;
        case REG_EBRACE:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Impalance of { and } in expression.");
            break;
            /*case REG_EBADBR:
            	Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
            		"s",
            		"Invalid content in { and }.");
            	break; */
        case REG_ESPACE:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Out of memory.");
            break;
        case REG_BADRPT:
            Log::GetLog()->Warning("ValueType::MatchesRegularExpression",
                                         "s",
                                         "Bad use of repetition operators.");
            break;
        }

    }
    return returnvalue;
}


Meow::Core::Types::ValueType_ptr Meow::Core::Types::ValueType::Base64Encode( bool lengthTrim )
{
    Meow::Core::Types::ValueType_ptr retval;
    unsigned long voidLength=GetVoidLength();
    if ( lengthTrim )
		voidLength--;
//    retval=new Meow::Core::Types::Void(Meow::Core::Peers::Base64::encodeBase64(voidLength,(unsigned char*)GetValueAsVoid()),voidLength);
	retval=new Meow::Core::Types::String(Meow::Core::Peers::Base64::encodeBase64(voidLength,(unsigned char*)GetValueAsString().c_str()));
    return retval;
}

Meow::Core::Types::ValueType_ptr Meow::Core::Types::ValueType::Base64Decode( bool safeIt )
{
    Meow::Core::Types::ValueType_ptr retval;
    std::string value=GetValueAsString();
    int dlen=value.length()*2;
//    int dlen=0;
//    unsigned char * target_buffer=(unsigned char*)calloc(value.length()+1,1);
    // TODO - Use own base64 decoder
    unsigned char * target_buffer=(unsigned char*)calloc(dlen,1);
    Meow::Core::Peers::Base64::decodeBase64(target_buffer,value.c_str());
    retval=new Void((void *)target_buffer,dlen);
    free(target_buffer);
    return retval;
}
std::string Meow::Core::Types::ValueType::MD5( )
{

	// TODO - use own md5
    return Meow::Core::Peers::OSSL::MD5(GetValueAsString());
}
std::string Meow::Core::Types::ValueType::SHA256( )
{
	// TODO - use own sha256
	return Meow::Core::Peers::OSSL::_SHA256(GetValueAsString());
}
Meow::Core::Types::Void_ptr Meow::Core::Types::ValueType::RawMD5( )
{
	// TODO - use own md5 raw
    void * data=Meow::Core::Peers::OSSL::RawMD5(GetValueAsString());
    Meow::Core::Types::Void_ptr retval=new Void(data,16);
    free(data);
    return retval;
}
Meow::Core::Types::Void_ptr Meow::Core::Types::ValueType::RawSHA256( )
{
	void * data=Meow::Core::Peers::OSSL::RawSHA256(GetValueAsString());
    Meow::Core::Types::Void_ptr retval=new Void(data,32);
    free(data);
    return retval;
}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::ValueType::Replace( std::string search, std::string replace )
{
    Meow::Core::Types::ValueType_ptr retval;
    std::string strd=GetValueAsString();
    size_t pos;
    do
    {
        pos = strd.find(search);
        if ( pos!=string::npos )
            strd.replace(pos,search.length(),replace);	
    } while ( pos!=string::npos );
    retval=new String(strd);
    return retval;
}
std::string Meow::Core::Types::ValueType::StrongHTMLEntities( )
{
    std::string in=GetValueAsString();
    std::string out="";
    for ( int i=0; i<in.length(); i++ )
    {
        switch (in[i])
        {
		case '&':
			out.append("&amp;");
			break;
        case '<':
            out.append("&lt;");
            break;
        case '>':
            out.append("&gt;");
            break;
        case '\n' :
        case '\r' :
            out.append("<br/>");
            break;
        case '\'':
            out.append("&quot;");
            break;
        case '"':
            out.append("&quot;");
            break;
        case '`':
            out.append("&prime;");
            break;
        default:
            char pszBuf[2];
            sprintf(pszBuf,"%c",in[i]);
            out.append(pszBuf);
            break;
        }
    }
    return out;
}

std::string Meow::Core::Types::ValueType::HTMLEntities(  )
{
    return TranscodeEncode();
}
std::string Meow::Core::Types::ValueType::TranscodeDecode(  )
{
    std::string outdata;
    std::string indata=GetValueAsString();
    for ( int i=0; i<indata.length(); i++ )
    {
        switch(indata[i])
        {
        case '&':
			{
				// hunt forward for a ;

				size_t next=indata.find_first_of(';',i);
				if ( next!=string::npos )
				{
					std::string glyph=indata.substr(i,next-i+1);
					if ( glyph[1]=='#' && glyph.length()==6 )
					{
						std::string numberAsString=glyph.substr(2,3);
						int number=atoi(numberAsString.c_str());
						char pszBuf[2];
						pszBuf[0]=number;
						pszBuf[1]='\0';
						outdata.append(pszBuf);
					}
					else
					{
						if ( !glyph.compare("&lt;") )
							outdata.append("<");
						else if ( !glyph.compare("&gt;") )
							outdata.append(">");
						else if ( !glyph.compare("&amp;") )
							outdata.append("&");
						else
						{
							next=i;
						}
					}
					//      i=next+1;
					i=next;
				}
				else
					outdata.append(indata.substr(i,1));

			}
			break;
        default:
            outdata.append(indata.substr(i,1));
            break;
        }
    }
    return outdata;
}

std::string Meow::Core::Types::ValueType::TranscodeEncode(  )
{
    std::string outdata="";
    std::string indata=GetValueAsString();

    for ( int i=0; i<indata.length(); i++ )
    {
        switch (indata[i])
        {
        case '<':
            outdata.append(std::string("&lt;"));
            break;
        case '>':
            outdata.append(std::string("&gt;"));
            break;
        case '&':
        {
            // hunt forward for a ;
            size_t next=indata.find_first_of(';',i);
            if ( next==string::npos )
            {
                outdata.append("&amp;");
            }
            else
            {
                std::string glyph=indata.substr(i,next-i+1);
                if ( glyph.length()==6 && glyph[1]=='#' )
                {
                    // Numeric transformation detected, do we want to handle this differently?
                    outdata.append(indata.substr(i,1));
                }
                else if ( !glyph.compare("&lt;") ||
                          !glyph.compare("&amp;") ||
                          !glyph.compare("&gt;") )
                {
                    outdata.append(indata.substr(i,1));
                }
                else
                    outdata.append("&amp;");
            }
        }
        break;
        default:
            if ( indata[i]=='\n' || indata[i]=='\t' || ( indata[i]>31 && indata[i] <127 ) )
                outdata.append(indata.substr(i,1));
            else
            {
                outdata.append(" ");
            }
            break;
        }
    }
    return outdata;
}
Meow::Core::Types::ValueType_ptr  Meow::Core::Types::ValueType::Replace( Meow::Core::Types::ValueType_ptr ptrToArray )
{
    String_ptr retval;
    // We expect this to be a kernel array
    if ( ptrToArray->IsInstanceOf("Array") )
    {
        Array_ptr array=(Array_ptr)ptrToArray;
        Stack_ptr keys=array->GetKeys();
        std::string strd=GetValueAsString();
        while ( !keys->IsEmpty() )
        {
            String_ptr key=(String_ptr)keys->Pop();
            ValueType_ptr value=(ValueType_ptr)array->Get(key->GetValueAsString());
            std::string search=key->GetValueAsString();
            std::string replace=value->GetValueAsString();
            int pos;
            do
            {
                pos = strd.find(search);
                if ( pos!=string::npos )
                    strd.replace(pos,search.length(),replace);
            } while ( pos!=string::npos );
        }
        retval=new String(strd);
    }
    else
        retval=new String(GetValueAsString());
    return (Meow::Core::Types::ValueType_ptr)retval;
}

std::string Meow::Core::Types::ValueType::URLEncode( bool strong )
{
    std::string c=GetValueAsString();
    string escaped="";
    int max = c.length();
    for(int i=0; i<max; i++)
    {
        if ( (48 <= c[i] && c[i] <= 57) ||//0-9
                (65 <= c[i] && c[i] <= 90) ||//abc...xyz
                (97 <= c[i] && c[i] <= 122) || //ABC...XYZ
                (c[i]=='~' || c[i]=='!' || c[i]=='*' || c[i]=='(' || c[i]==')' || c[i]=='\'') ||	
				(!strong && ( c[i] == '=' || c[i] == '-' || c[i] == '.' || c[i] == '_' || c[i] == '%' )) 
           )
        {
            escaped.append( &c[i], 1);
        }
        else
        {
            escaped.append("%");
			Core::Types::String_ptr hexified=new Core::Types::String(Meow::Core::Peers::Base64::charToHex(c[i]));
			
            escaped.append(hexified->ToUpperCase()->GetValueAsString() );//converts char 255 to string "ff"
        }
    }
    return escaped;
}

bool IsHex( char inp )
{
    char logIt[2];
    logIt[0]=inp;
    logIt[1]='\0';
    return inp=='0' || inp=='1' || inp=='2' || inp=='3' || inp=='4' || inp=='5' || inp=='6' || inp=='7' || inp=='8' || inp=='9' || inp=='A' || inp=='B' || inp=='C' || inp=='D' || inp=='E' || inp=='F';
}

char CharFromHex (std::string a)
{
    std::istringstream Blat (a);
    int Z;
    Blat >> std::hex >> Z;
    return char (Z); // cast to char and return
}
std::string Meow::Core::Types::ValueType::URLDecode( )
{
    std::string Text=GetValueAsString();
    std::string::size_type Pos;
    std::string Hex;
    size_t position=Text.find('%');
    while (std::string::npos != position )
    {
        // if ( (Pos+2) < Text.length() )
        // {
        Pos=position;
        Hex = Text.substr(Pos + 1, 2);
        if ( Hex.length() > 0 )
        {   if ( IsHex(Hex[0]) && IsHex(Hex[1]) ) Text.replace(Pos, 3, 1, CharFromHex(Hex));
        }
        // }
        position=Text.find('%',position+1);
    }

    return Text;
}
std::string Meow::Core::Types::ValueType::XMLEncode()
{
	// TODO xmlencode
	return "";
}

long Meow::Core::Types::ValueType::StringToInteger( std::string value )
{
	String_ptr val=new String(value);
	return val->GetValueAsInteger();
}
double Meow::Core::Types::ValueType::StringToFloat( std::string value )
{
	String_ptr val=new String(value);
	return val->GetValueAsFloat();
}
bool Meow::Core::Types::ValueType::StringToBoolean( std::string value )
{
  String_ptr val=new String(value);
	return val->GetValueAsBoolean();
}
std::string Meow::Core::Types::ValueType::IntegerToString( long value )
{
	Integer_ptr intValue=new Integer(value);
	return intValue->GetValueAsString();
}
std::string Meow::Core::Types::ValueType::FloatToString( double value )
{
    Float_ptr floatValue=new Float(value);
	return floatValue->GetValueAsString();
}
std::string Meow::Core::Types::ValueType::BooleanToString( bool value )
{
    Boolean_ptr boolValue=new Boolean(value);
	return boolValue->GetValueAsString();
}
std::string Meow::Core::Types::ValueType::Tail( std::string input, unsigned long bumps )
{
	 // Beautiful solution credit http://stackoverflow.com/users/147192/matthieu-m
	 if (bumps >= input.size()) { return input; }
	  return input.substr(input.size() - bumps);
}
std::string	Meow::Core::Types::ValueType::PrintableOnly( std::string instring )
{
	ValueType_ptr data=new String(instring);
	std::string retval=data->TranscodeEncode();
	for ( int i=0; i<retval.length(); i++ )
	{
		if ( ( retval[i]<32 || retval[i]>128 ) && retval[i]!='\n' && retval[i]!='\t' && retval[i]!='\r' ) 
			retval[i]=' ';
	}
	return retval;
}
std::string	Meow::Core::Types::ValueType::Munch( std::string instring )
{
	std::string retval=instring;
	for ( int i=0; i<retval.length(); i++ )
	{
		if ( ( retval[i]<32 || retval[i]>128 ) && retval[i]!='\n' && retval[i]!='\t' && retval[i]!='\r' )
			retval[i]=' ';
	}
	return retval;
}
std::string	Meow::Core::Types::ValueType::Encode64( std::string instring )
{
	Core::Types::String_ptr str=new Core::Types::String(instring);
	str=(Core::Types::String_ptr)str->Base64Encode(true);
	return str->GetValueAsString();
}
std::string Meow::Core::Types::ValueType::LeftPadString( char padwith, int length )
{
    std::string old_string=GetValueAsString();
    std::string retval = std::string(length - old_string.length(), padwith) + old_string;
    return retval;
}
