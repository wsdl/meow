// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetMeowClassName and GetMeowClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#include <Core/Types/String.h>
#include <Core/Types/Integer.h>
#include <Object.h> // Only for the array substitution stuff
#include <Core/Log.h>
#include <stdio.h>
#include <iostream>

Meow::Core::Types::String::String( std::string initialiser )
{
    Content=initialiser;
    SetMeowClassName("String");
}

Meow::Core::Types::String::String( )
{
    Content="";
    SetMeowClassName("String");
}

Meow::Core::Types::String::~String( )
{
}

void Meow::Core::Types::String::SetValue( std::string item )
{
    Content=item;
}

std::string Meow::Core::Types::String::GetValue( )
{
    return Content;
}

Meow::Core::Types::String_ptr  Meow::Core::Types::String::Reverse()
{
    Meow::Core::Types::String_ptr retval=new Meow::Core::Types::String();
    for ( int i=Content.length(); i>-1; i--)
    {
        retval->Content=retval->Content + Content[i];
    }
    return retval;

}
Meow::Core::Types::String_ptr  Meow::Core::Types::String::UpperCaseFirstChar()
{
    Meow::Core::Types::String_ptr retval=new Meow::Core::Types::String();
    retval->Content=Content;
    switch(retval->Content[0])
    {
    case 'a':
        retval->Content[0]='A';
        break;
    case 'b':
        retval->Content[0]='B';
        break;
    case 'c':
        retval->Content[0]='C';
        break;
    case 'd':
        retval->Content[0]='D';
        break;
    case 'e':
        retval->Content[0]='E';
        break;
    case 'f':
        retval->Content[0]='F';
        break;
    case 'g':
        retval->Content[0]='G';
        break;
    case 'h':
        retval->Content[0]='H';
        break;
    case 'i':
        retval->Content[0]='I';
        break;
    case 'j':
        retval->Content[0]='J';
        break;
    case 'k':
        retval->Content[0]='K';
        break;
    case 'l':
        retval->Content[0]='L';
        break;
    case 'm':
        retval->Content[0]='M';
        break;
    case 'n':
        retval->Content[0]='N';
        break;
    case 'o':
        retval->Content[0]='O';
        break;
    case 'p':
        retval->Content[0]='P';
        break;
    case 'q':
        retval->Content[0]='Q';
        break;
    case 'r':
        retval->Content[0]='R';
        break;
    case 's':
        retval->Content[0]='S';
        break;
    case 't':
        retval->Content[0]='T';
        break;
    case 'u':
        retval->Content[0]='U';
        break;
    case 'v':
        retval->Content[0]='V';
        break;
    case 'w':
        retval->Content[0]='W';
        break;
    case 'x':
        retval->Content[0]='X';
        break;
    case 'y':
        retval->Content[0]='Y';
        break;
    case 'z':
        retval->Content[0]='Z';
        break;
    }
    return retval;
}
Meow::Core::Types::String_ptr  Meow::Core::Types::String::SubString(int offset, int length )
{
    Meow::Core::Types::String_ptr retval=new Meow::Core::Types::String();
    retval->Content=Content.substr(offset,length);
    return retval;
}


Meow::Core::Types::String_ptr Meow::Core::Types::String::ToLowerCase()
{
    Meow::Core::Types::String_ptr retval=new Meow::Core::Types::String();
    retval->SetValue(Content);
    for ( int i=0; i<retval->Content.length(); i++ )
    {
        switch(retval->Content[i])
        {
        case 'A':
            retval->Content[i]='a';
            break;
        case 'B':
            retval->Content[i]='b';
            break;
        case 'C':
            retval->Content[i]='c';
            break;
        case 'D':
            retval->Content[i]='d';
            break;
        case 'E':
            retval->Content[i]='e';
            break;
        case 'F':
            retval->Content[i]='f';
            break;
        case 'G':
            retval->Content[i]='g';
            break;
        case 'H':
            retval->Content[i]='h';
            break;
        case 'I':
            retval->Content[i]='i';
            break;
        case 'J':
            retval->Content[i]='j';
            break;
        case 'K':
            retval->Content[i]='k';
            break;
        case 'L':
            retval->Content[i]='l';
            break;
        case 'M':
            retval->Content[i]='m';
            break;
        case 'N':
            retval->Content[i]='n';
            break;
        case 'O':
            retval->Content[i]='o';
            break;
        case 'P':
            retval->Content[i]='p';
            break;
        case 'Q':
            retval->Content[i]='q';
            break;
        case 'R':
            retval->Content[i]='r';
            break;
        case 'S':
            retval->Content[i]='s';
            break;
        case 'T':
            retval->Content[i]='t';
            break;
        case 'U':
            retval->Content[i]='u';
            break;
        case 'V':
            retval->Content[i]='v';
            break;
        case 'W':
            retval->Content[i]='w';
            break;
        case 'X':
            retval->Content[i]='x';
            break;
        case 'Y':
            retval->Content[i]='y';
            break;
        case 'Z':
            retval->Content[i]='z';
            break;
        }
    }
    return retval;
}

Meow::Core::Types::String_ptr Meow::Core::Types::String::ToUpperCase()
{
    Meow::Core::Types::String_ptr retval=new Meow::Core::Types::String();
    retval->SetValue(Content);
    for ( int i=0; i<retval->Content.length(); i++ )
    {
        switch(retval->Content[i])
        {
        case 'a':
            retval->Content[i]='A';
            break;
        case 'b':
            retval->Content[i]='B';
            break;
        case 'c':
            retval->Content[i]='C';
            break;
        case 'd':
            retval->Content[i]='D';
            break;
        case 'e':
            retval->Content[i]='E';
            break;
        case 'f':
            retval->Content[i]='F';
            break;
        case 'g':
            retval->Content[i]='G';
            break;
        case 'h':
            retval->Content[i]='H';
            break;
        case 'i':
            retval->Content[i]='I';
            break;
        case 'j':
            retval->Content[i]='J';
            break;
        case 'k':
            retval->Content[i]='K';
            break;
        case 'l':
            retval->Content[i]='L';
            break;
        case 'm':
            retval->Content[i]='M';
            break;
        case 'n':
            retval->Content[i]='N';
            break;
        case 'o':
            retval->Content[i]='O';
            break;
        case 'p':
            retval->Content[i]='P';
            break;
        case 'q':
            retval->Content[i]='Q';
            break;
        case 'r':
            retval->Content[i]='R';
            break;
        case 's':
            retval->Content[i]='S';
            break;
        case 't':
            retval->Content[i]='T';
            break;
        case 'u':
            retval->Content[i]='U';
            break;
        case 'v':
            retval->Content[i]='V';
            break;
        case 'w':
            retval->Content[i]='W';
            break;
        case 'x':
            retval->Content[i]='X';
            break;
        case 'y':
            retval->Content[i]='Y';
            break;
        case 'z':
            retval->Content[i]='Z';
            break;
        }
    }
    return retval;
}

Meow::Core::Types::Array_ptr Meow::Core::Types::String::Explode( std::string delimiter )
{
    Meow::Core::Types::Array_ptr lines=new Meow::Core::Types::Array();
    unsigned long lineno=0;
    std::string curstring="";
    for ( int i=0; i<Content.length(); i++ )
    {
        bool found=false;
        for ( int j=0; j<delimiter.length(); j++ )
        {
            if ( Content[i]==delimiter[j] )
            {
                String_ptr str=new String(curstring);
                lines->Set(lineno,(ptr)str);
                found=true;
                curstring="";
                lineno++;
                break;
            }
        }
        if ( !found )
        {
            char toappend[2];
            toappend[0]=Content[i];
            toappend[1]='\0';
            curstring.append(toappend);
        }
    }
    if ( curstring.length()>0 )
    {
        String_ptr str=new String(curstring);
        lines->Set(lineno,(ptr)str);
    }
    return lines;
}

Meow::Core::Types::Array_ptr Meow::Core::Types::String::CSVExplode( char delimiter )
{
    Meow::Core::Types::Array_ptr lines=new Meow::Core::Types::Array();
    bool   mode=false;
    string current="";
    unsigned long lineno=0;
    for ( unsigned int i=0; i<Content.length(); i++ )
    {
        // If we are in a quoted string, hunt for the end quote
        // otherwise just add it to the string.
        if ( mode )
        {
            if ( Content[i]=='"' )
            {
                // We need to make sure that the previous entry
                // in content was not \, if so, it is escaped
                if (i!=0 && Content[i-1]=='\\')
                {
                    current.resize(current.length()-1);
                    current.push_back('"');
                }
                else
                {
                    mode=false;
                }
            }
            else
            {
                current.push_back(Content[i]);
            }
        }
        else
        {
            if ( Content[i]==delimiter )
            {
                if (i!=0 && Content[i-1]=='\\')
                {
                    current.resize(current.length()-1);
                    current.push_back(delimiter);
                }
                else
                {
                    String_ptr str=new String(current);
                    lines->Set(lines->Count(),(ptr)str);
                    lineno++;
                    current="";
//                   break;
                    mode=false;
                }
            }
            else if ( Content[i]=='"' )
            {
                if (i!=0 && Content[i-1]=='\\')
                {
                    current.resize(current.length()-1);
                    current.push_back('"');
                }
                else
                {
                    mode=true;
                }
            }
            else
            {
                current.push_back(Content[i]);
            }
        }
    }

    if ( current.length()>0 )
    {
        String_ptr str=new String(current);
        lines->Set(lineno,(ptr)str);
        lineno++;
    }
    return lines;
}
Meow::Core::Types::String_ptr Meow::Core::Types::String::GetNextToken( int offset, char delimiter )
{
    string data="";
    size_t found=Content.find_first_of(delimiter,offset);
    String_ptr str=new String();
    str->SetValue(Content.substr(offset,found));
    return str;
}
Meow::Core::Types::String_ptr Meow::Core::Types::String::GetAfterToken( int offset, char delimiter )
{
    string data="";
    size_t found=Content.find_first_of(delimiter,offset);

    String_ptr str=new String();
    if ( found!=string::npos )
        str->SetValue(Content.substr(found+1));
    return str;
}
char Meow::Core::Types::String::FirstChar( )
{
    if (Content.length()==0)
        return '\0';
    return Content[0];
}

char Meow::Core::Types::String::LastChar( )
{
    if (Content.length()==0)
        return '\0';
    return Content[Content.length()-1];
}

Meow::Core::Types::String_ptr Meow::Core::Types::String::ShrinkBy(int amount)
{
    String_ptr retval_ptr=new String();
    std::string retval="";
    if ( Content.length()>=(amount*2) )
        retval=Content.substr(amount,Content.length()-(amount+1));
    retval_ptr->SetValue(retval);
    return retval_ptr;
}

Meow::Core::Types::String_ptr Meow::Core::Types::String::SkipLeadingTabs( )
{
    String_ptr retval_ptr=new String();
    unsigned long pos=Content.find_first_not_of('\t',0);
    if ( pos!=std::string::npos )
    {
        retval_ptr->Content=Content.substr(Content.find_first_not_of('\t',0));
    }
    return retval_ptr;
}

bool Meow::Core::Types::String::IsNumeric()
{
    bool retval=true;
    int PeriodCounter=0;
    for ( unsigned int i=0; i<Content.length() && retval; i++ )
    {
        if (! ( Content[i]=='1' || Content[i]=='2' || Content[i]=='3' || Content[i]=='4' ||
                Content[i]=='5' || Content[i]=='6' || Content[i]=='7' || Content[i]=='8' ||
                Content[i]=='9' || Content[i]=='0' || Content[i]=='.' ) )
        {
            retval=false;
        }
        if ( Content[i]=='.' )
            PeriodCounter++;
    }
    if ( retval && PeriodCounter>1 )
        retval=false;
    return retval;
}
bool Meow::Core::Types::String::CouldBeFloat()
{
    bool retval=true;
    int PeriodCounter=0;
    int NegativeCounter=0;
    for ( unsigned int i=0; i<Content.length() && retval; i++ )
    {
        if (! ( Content[i]=='1' || Content[i]=='2' || Content[i]=='3' || Content[i]=='4' ||
                Content[i]=='5' || Content[i]=='6' || Content[i]=='7' || Content[i]=='8' ||
                Content[i]=='9' || Content[i]=='0' || Content[i]=='.' || Content[i]=='-' ) )
        {
            retval=false;
        }
        if ( Content[i]=='.' )
            PeriodCounter++;
        if ( Content[i]=='-' )
            NegativeCounter++;
    }
    if ( retval && ( PeriodCounter!=1 || NegativeCounter>1 ) )
        retval=false;

    return retval;
}
bool Meow::Core::Types::String::CouldBeInteger()
{
    bool retval=true;
    for ( unsigned int i=0; i<Content.length() && retval; i++ )
    {
        if (! ( Content[i]=='1' || Content[i]=='2' || Content[i]=='3' || Content[i]=='4' ||
                Content[i]=='5' || Content[i]=='6' || Content[i]=='7' || Content[i]=='8' ||
                Content[i]=='9' || Content[i]=='0' || Content[i]=='-' ) )
        {
            retval=false;
        }
    }
    return retval;
}
void Meow::Core::Types::String::Append( Meow::Core::Types::String_ptr otherstr )
{
    ValueType_ptr hack=(ValueType_ptr)otherstr;
    if ( !otherstr.IsNull() )
        Content.append(hack->GetValueAsString());
}
void Meow::Core::Types::String::Append( std::string otherstr )
{
    Content.append(otherstr);
}

long Meow::Core::Types::String::GetValueAsInteger()
{
    return atol(Content.c_str());
}
double Meow::Core::Types::String::GetValueAsFloat()
{
//	if ( CouldBeFloat() )
    return atof(Content.c_str());
//	return 0.0;
}

std::string Meow::Core::Types::String::GetValueAsString()
{
    return Content;
}

void Meow::Core::Types::String::Append( ValueType_ptr other )
{
    Content.append(other->GetValueAsString());
}
void Meow::Core::Types::String::Increment( )
{
    if ( CouldBeFloat() )
    {
        double val=GetValueAsFloat();
        val=val+1.0;
        char pszBuf[256];
        sprintf(pszBuf,"%f",(float)val);
        Content=pszBuf;
    }
    else if ( CouldBeInteger( ) )
    {
        long val=GetValueAsInteger();
        val++;
        char pszBuf[256];
        sprintf(pszBuf,"%d",(int)val);
        Content=pszBuf;
    }
    else if ( !Content.compare("true") || !Content.compare("false") )
    {
        if ( Content.compare("true") )
            Content="true";
        else
            Content="false";
    }
    else
        ValueType::Increment();
}
void Meow::Core::Types::String::Decrement( )
{
    if ( CouldBeFloat() )
    {
        double val=GetValueAsFloat();
        val=val-1.0;
        char pszBuf[256];
        sprintf(pszBuf,"%f",(float)val);
        Content=pszBuf;
    }
    else if ( CouldBeInteger( ) )
    {
        long val=GetValueAsInteger();
        val--;
        char pszBuf[256];
        sprintf(pszBuf,"%d",(int)val);
        Content=pszBuf;
    }
    else if ( !Content.compare("true") || !Content.compare("false") )
    {
        if ( Content.compare("true") )
            Content="true";
        else
            Content="false";
    }
    else
        ValueType::Decrement();
}
void Meow::Core::Types::String::Let( ValueType_ptr other )
{
    Content=other->GetValueAsString();
}
void Meow::Core::Types::String::Repeat( int times )
{
    std::string result="";
    for ( int i=0; i<times; i++ )
    {
        result.append(Content);
    }
    Content=result;
}
void Meow::Core::Types::String::Add( ValueType_ptr other )
{
    if ( CouldBeFloat() && other->CouldBeFloat() )
    {
        double result=other->GetValueAsFloat() + GetValueAsFloat();
        char pszBuf[256];
        sprintf(pszBuf,"%f",(float)result);
        Content=pszBuf;
    }
    else if ( CouldBeInteger() && other->CouldBeInteger() )
    {
        long result=other->GetValueAsInteger() + GetValueAsInteger();
        char pszBuf[256];
        sprintf(pszBuf,"%d",(int)result);
        Content=pszBuf;
    }
    else
    {
        Append(other);
    }
}
void Meow::Core::Types::String::Subtract( ValueType_ptr other )
{
    if ( CouldBeFloat() && other->CouldBeFloat() )
    {
        double result=GetValueAsFloat() - other->GetValueAsFloat();
        char pszBuf[256];
        sprintf(pszBuf,"%f",(float)result);
        Content=pszBuf;
    }
    else if ( CouldBeInteger() && other->CouldBeInteger() )
    {
        long result=GetValueAsInteger() - other->GetValueAsInteger();
        char pszBuf[256];
        sprintf(pszBuf,"%d",(int)result);
        Content=pszBuf;
    }
    else
    {
        ValueType::Subtract(other);
    }
}
void Meow::Core::Types::String::Divide( ValueType_ptr other )
{

    if ( CouldBeFloat() && other->CouldBeFloat() )
    {
        if ( other->GetValueAsFloat() != 0.0 )
        {
            double result=GetValueAsFloat() / other->GetValueAsFloat();
            char pszBuf[256];
            sprintf(pszBuf,"%f",(float)result);
            Content=pszBuf;
        }
        else
        {
        	Exceptions::DivideByZero exception;
        	exception.SetMessage("Cannot divide this number by zero.");
        	throw exception;
        }
    }
    else if ( CouldBeInteger() && other->CouldBeInteger() )
    {
        if ( other->GetValueAsInteger() != 0 )
        {
            long result=GetValueAsInteger() / other->GetValueAsInteger();
            char pszBuf[256];
            sprintf(pszBuf,"%d",(int)result);
            Content=pszBuf;
        }
        else
        {
        	Exceptions::DivideByZero exception;
			exception.SetMessage("Cannot divide this number by zero.");
			throw exception;
        }
    }
    else
    {
        ValueType::Divide(other);
    }

}
void Meow::Core::Types::String::Multiply( ValueType_ptr other )
{
    if ( CouldBeFloat() && other->CouldBeFloat() )
    {
        double result=GetValueAsFloat() * other->GetValueAsFloat();
        char pszBuf[256];
        sprintf(pszBuf,"%f",(float)result);
        Content=pszBuf;
    }
    else if ( CouldBeInteger() && other->CouldBeInteger() )
    {
        long result=GetValueAsInteger() * other->GetValueAsInteger();
        char pszBuf[256];
        sprintf(pszBuf,"%d",(int)result);
        Content=pszBuf;
    }
    else
    {
        ValueType::Multiply(other);
    }
}
void Meow::Core::Types::String::Negate( )
{
    if ( CouldBeFloat() )
    {
        double result=0.0-GetValueAsFloat();
        char pszBuf[256];
        sprintf(pszBuf,"%f",(float)result);
        Content=pszBuf;
    }
    else if ( CouldBeInteger() )
    {
        long result=0-GetValueAsInteger();
        char pszBuf[256];
        sprintf(pszBuf,"%d",(int)result);
        Content=pszBuf;
    }
    else if ( !Content.compare("true") )
        Content="false";
    else if ( !Content.compare("false") )
        Content="true";
    else
    {
        ValueType::Negate();
    }

}
void Meow::Core::Types::String::Modulus( ValueType_ptr other )
{
    if ( CouldBeInteger() && other->CouldBeInteger() )
    {
        long result= GetValueAsInteger() % other->GetValueAsInteger();
        char pszBuf[256];
        sprintf(pszBuf,"%d",(int)result);
        Content=pszBuf;
    }
    else
    {
        ValueType::Modulus(other);
    }
}
bool Meow::Core::Types::String::GreaterThan( ValueType_ptr other )
{
//	if ( !Content.compare(other->GetValueAsString()) )
//		return false;

    if ( CouldBeFloat() && other->CouldBeFloat() )
        return GetValueAsFloat()>other->GetValueAsFloat();
    else if ( CouldBeInteger() && other->CouldBeInteger() )
        return GetValueAsInteger()>other->GetValueAsInteger();
    else if ( (!Content.compare("true") || !Content.compare("false") ) &&
              ( !other->GetValueAsString().compare("true") || !other->GetValueAsString().compare("false") ) )
    {
        return !Content.compare("true");
    }
    return Content.length()>other->GetValueAsString().length();
}
bool Meow::Core::Types::String::LessThan( ValueType_ptr other )
{
//	if ( !Content.compare(other->GetValueAsString()) )
//		return false;
    if ( CouldBeFloat() && other->CouldBeFloat() )
        return GetValueAsFloat()<other->GetValueAsFloat();
    else if ( CouldBeInteger() && other->CouldBeInteger() )
        return GetValueAsInteger()<other->GetValueAsInteger();
    else if ( (!Content.compare("true") || !Content.compare("false") ) &&
              ( !other->GetValueAsString().compare("true") || !other->GetValueAsString().compare("false") ) )
        return !Content.compare("false");
    return Content.length()<other->GetValueAsString().length();
}
bool Meow::Core::Types::String::Equals( ValueType_ptr other )
{
    return !Content.compare(other->GetValueAsString());
}
bool Meow::Core::Types::String::NotEquals( ValueType_ptr other )
{
    return Content.compare(other->GetValueAsString());
}
void Meow::Core::Types::String::And( ValueType_ptr other )
{
    if ( (!Content.compare("true") || !Content.compare("false") ) &&
            ( !other->GetValueAsString().compare("true") || !other->GetValueAsString().compare("false") ) )
    {
        bool lhs=!Content.compare("true");
        bool rhs=!other->GetValueAsString().compare("false");
        lhs=lhs && rhs;
        if ( lhs )
            Content="true";
        else
            Content="false";
    }
    else
        ValueType::And(other);
}
void Meow::Core::Types::String::Or( ValueType_ptr other )
{

    if ( (!Content.compare("true") || !Content.compare("false") ) &&
            ( !other->GetValueAsString().compare("true") || !other->GetValueAsString().compare("false") ) )
    {
        bool lhs=!Content.compare("true");
        bool rhs=!other->GetValueAsString().compare("false");
        lhs=lhs || rhs;
        if ( lhs )
            Content="true";
        else
            Content="false";
    }
    else
        ValueType::And(other);
}
void Meow::Core::Types::String::Xor( ValueType_ptr other )
{
    ValueType::Xor(other);
}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::String::Left()
{
    ValueType_ptr retval;

    if ( Content.length()==1 )
    {
        String_ptr retval_as_string=new String();
        retval_as_string->Content=Content;
        retval=(ValueType_ptr)retval_as_string;
        Content="";
    }
    else
    {
        String_ptr retval_as_string=new String();
        retval_as_string->Content=Content.substr(0,1);
        Content=Content.substr(1);
        retval=(ValueType_ptr)retval_as_string;
    }
    return retval;

}
Meow::Core::Types::ValueType_ptr Meow::Core::Types::String::Right( )
{
    ValueType_ptr retval;

    if ( Content.length()==1 )
    {
        String_ptr retval_as_string=new String();
        retval_as_string->Content=Content;
        retval=(ValueType_ptr)retval_as_string;
        Content="";
    }
    else
    {
        String_ptr retval_as_string=new String();
        retval_as_string->Content=Content.substr(-1);
        Content=Content.substr(0,Content.length()-1);
        retval=(ValueType_ptr)retval_as_string;
    }

    return retval;
}
bool Meow::Core::Types::String::GetValueAsBoolean()
{
    if (Content=="true" || Content=="1" || Content=="TRUE" || Content=="T" || Content=="t" )
        return true;
    return false;
}
void      * Meow::Core::Types::String::GetValueAsVoid()
{
    return (void*)Content.c_str();
}
long        Meow::Core::Types::String::GetVoidLength()
{
    return Content.length()+1;
}
unsigned long Meow::Core::Types::String::Count( )
{
    unsigned long retval=Content.length();
    return retval;
}
bool Meow::Core::Types::String::Contains( char character )
{
    return Content.find(character)!=string::npos;
}

Meow::Core::Types::String_ptr Meow::Core::Types::String::Between( std::string starts, std::string ends)
{
    size_t offset=Content.find(starts);
    std::string ReturnData;
    if ( offset!=string::npos )
        ReturnData=Content.substr(offset);

    offset=ReturnData.find(ends);
    if ( offset!=string::npos )
        ReturnData=ReturnData.substr(0,offset+ends.length());
    return new String(ReturnData);
}
Meow::Core::Types::Integer_ptr Meow::Core::Types::String::IndexOf( std::string needle )
{
    Integer_ptr index=new Integer(-1);
    size_t offset=Content.find(needle);
    if ( offset!=string::npos )
        index=new Integer(offset);
    return index;
}
Meow::Core::Types::Array_ptr Meow::Core::Types::String::Chunk( int stringLength )
{
    size_t cursor=0;
    Meow::Core::Types::Array_ptr elements=new Array();
    while ( cursor < Content.length() )
    {
        std::string thisElement=Content.substr(cursor,stringLength);
        cursor=cursor+stringLength;
        String_ptr wrapThisElement=new String(thisElement);
        elements->Set(elements->Count(),(ptr)wrapThisElement);
    }
    return elements;
}
std::string Meow::Core::Types::String::Trim( std::string text )
{
	String_ptr retval=new String(text);
	while ( retval->FirstChar() == ' ' )
	{
	   retval=retval->SubString(1,retval->Count());
	}
	while ( retval->LastChar() == ' ' || retval->LastChar() == '\n' || retval->LastChar() == '\r' )
	{
	   retval=retval->SubString(0,retval->Count()-1);
	}
	return retval->GetValueAsString();
}
Meow::Core::Types::String_ptr Meow::Core::Types::String::SubstituteVariables( Core::Types::Array_ptr properties )
{
	Core::Types::String_ptr retval=new Core::Types::String();
	// Substitution variables are enclosed in [ ]

	// So work through the content
	unsigned long length=Content.length();
	std::string currentToken;
	bool intoken=false;
	for ( unsigned long i=0; i<length; i++ )
	{
		char c=Content[i];
		switch (c)
		{
		case '[':
			if ( intoken )
				retval->Append("[" + currentToken);
			intoken=true;
			currentToken="";
			break;
		case ']':
			if ( intoken )
			{
				if ( properties->IsSet(currentToken) )
				{
					// Assume its either a ValueType, an Object [Kernel] or Object [non Kernel]
					Core::ptr property=properties->Get(currentToken);
					if ( !property.IsNull() )
					{
						if ( property->IsInstanceOf("ValueType") )
							retval->Append((Core::Types::ValueType_ptr)property);
						else if ( property->IsInstanceOf("Object") )
							retval->Append(((Meow::Object_ptr)property)->ToString());
						else
							retval->Append(((Object_ptr)property)->GetMeowClassName());
					}
				}
				else				
					retval->Append("[" + currentToken + "]");				
				currentToken="";
				intoken=false;
			}
			break;
		default:
			if ( intoken )
				currentToken+=c;
			else
			{
				char pszBuf[2];
				pszBuf[0]=c;
				pszBuf[1]='\0';
				retval->Append(std::string(pszBuf));
			}
		}
	}

	return retval;	
}
