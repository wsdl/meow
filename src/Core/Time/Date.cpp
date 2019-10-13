// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Time/Date.h>
#include <Core/Types/Float.h>
#include <Core/Types/Integer.h>
#include <Core/Log.h>
#ifdef WIN32
	#include <windows.h>
	#include <time.h>
#else
	#include <time.h>
#endif
#include <iostream>
#include <stdio.h>

#define an_hour 3600
#define working_max 17
#define working_min 9
#define working_lunch 13
#define working_lunch_threshold 15


Meow::Core::Time::Date::Date( Meow::Core::Types::ValueType_ptr val )
{
    SetMeowClassName("Date");
    ts=val->GetValueAsInteger();
}

Meow::Core::Time::Date::Date( std::string date_as_string )
{
    SetMeowClassName("Date");
    // Todo - implement a parsing from a standard Meow::Core::Types::String format
    ts=0;
    Core::Types::ValueType_ptr dd=new Core::Types::Integer(1);
    Core::Types::ValueType_ptr mm=new Core::Types::Integer(1);
    Core::Types::ValueType_ptr yyyy=new Core::Types::Integer(1970);
    Core::Types::ValueType_ptr HH=new Core::Types::Integer(9);
    Core::Types::ValueType_ptr MM=new Core::Types::Integer(0);
    Core::Types::ValueType_ptr SS=new Core::Types::Integer(0);
    if ( date_as_string.length() == 10 )
    {
        dd=new Meow::Core::Types::String(date_as_string.substr(0,2));
        mm=new Meow::Core::Types::String(date_as_string.substr(3,2));
        yyyy=new Meow::Core::Types::String(date_as_string.substr(6,4));
    }
	else if ( date_as_string.length() == 19 )
	{
	    dd=new Meow::Core::Types::String(date_as_string.substr(0,2));
        mm=new Meow::Core::Types::String(date_as_string.substr(3,2));
        yyyy=new Meow::Core::Types::String(date_as_string.substr(6,4));
		HH=new Meow::Core::Types::String(date_as_string.substr(11,2));
		MM=new Meow::Core::Types::String(date_as_string.substr(14,2));
		SS=new Meow::Core::Types::String(date_as_string.substr(17,2));
	}
    struct tm current_time;
    current_time.tm_sec=SS->GetValueAsInteger();
    current_time.tm_min=MM->GetValueAsInteger();
    current_time.tm_hour=HH->GetValueAsInteger();
    current_time.tm_mday=dd->GetValueAsInteger();
    current_time.tm_mon=mm->GetValueAsInteger()-1;
    current_time.tm_year=yyyy->GetValueAsInteger()-1900;
#ifdef WIN32
    ts=(unsigned long)mktime(&current_time);
#else
    ts=(unsigned long)timegm(&current_time);
#endif
}

Meow::Core::Time::Date::Date( std::string date_as_string, std::string format )
{

    SetMeowClassName("Date");
    // Todo - implement a parsing from a standard Meow::Core::Types::String format
    ts=0;
    Core::Types::ValueType_ptr dd=new Core::Types::Integer(1);
    Core::Types::ValueType_ptr mm=new Core::Types::Integer(1);
    Core::Types::ValueType_ptr yyyy=new Core::Types::Integer(1970);
    Core::Types::ValueType_ptr HH=new Core::Types::Integer(9);
    Core::Types::ValueType_ptr MM=new Core::Types::Integer(0);
    Core::Types::ValueType_ptr SS=new Core::Types::Integer(0);
    if ( !format.compare("dd/mm/yyyy") && date_as_string.length() == 10 )
    {
        dd=new Meow::Core::Types::String(date_as_string.substr(0,2));
        mm=new Meow::Core::Types::String(date_as_string.substr(3,2));
        yyyy=new Meow::Core::Types::String(date_as_string.substr(6,4));
    }
    else if ( !format.compare("dd/mm/yyyy HH:MM") &&  date_as_string.length() == 16  )
    {
        dd=new Meow::Core::Types::String(date_as_string.substr(0,2));
        mm=new Meow::Core::Types::String(date_as_string.substr(3,2));
        yyyy=new Meow::Core::Types::String(date_as_string.substr(6,4));
        HH=new Meow::Core::Types::String(date_as_string.substr(11,2));
        MM=new Meow::Core::Types::String(date_as_string.substr(14,2));
	}
	else if ( !format.compare("yyyymmdd") && date_as_string.length() == 8  )
	{
        dd=new Meow::Core::Types::String(date_as_string.substr(6,2));
        mm=new Meow::Core::Types::String(date_as_string.substr(4,2));
        yyyy=new Meow::Core::Types::String(date_as_string.substr(0,4));			
	}
	else if ( !format.compare("yyyymmdd hh:mm") && date_as_string.length() == 14  )
	{
		dd=new Meow::Core::Types::String(date_as_string.substr(6,2));
        mm=new Meow::Core::Types::String(date_as_string.substr(4,2));
        yyyy=new Meow::Core::Types::String(date_as_string.substr(0,4));	
		HH=new Meow::Core::Types::String(date_as_string.substr(9,2));
        MM=new Meow::Core::Types::String(date_as_string.substr(12,2));
	}
    else if ( !format.compare("yyyy-mm-dd HH:MM:SS") &&  date_as_string.length() == 19  )
    {
        yyyy=new Meow::Core::Types::String(date_as_string.substr(0,4));
        mm=new Meow::Core::Types::String(date_as_string.substr(5,2));
        dd=new Meow::Core::Types::String(date_as_string.substr(8,2));
        HH=new Meow::Core::Types::String(date_as_string.substr(11,2));
        MM=new Meow::Core::Types::String(date_as_string.substr(14,2));
        SS=new Meow::Core::Types::String(date_as_string.substr(17,2));
	}
			
    struct tm current_time;
    current_time.tm_sec=SS->GetValueAsInteger();
    current_time.tm_min=MM->GetValueAsInteger();
    current_time.tm_hour=HH->GetValueAsInteger();
    current_time.tm_mday=dd->GetValueAsInteger();
    current_time.tm_mon=mm->GetValueAsInteger()-1;
    current_time.tm_year=yyyy->GetValueAsInteger()-1900;
#ifdef WIN32
    ts=(unsigned long)mktime(&current_time);
#else
    ts=(unsigned long)timegm(&current_time);
#endif
}

Meow::Core::Time::Date::Date( long date_as_long )
{
    SetMeowClassName("Date");
    // Todo - implement a parsing from a standard Meow::Core::Types::String format
    ts=date_as_long;

}
Meow::Core::Time::Date::Date(
    short hour,
    short minute,
    short second,
    short day,
    short month,
    short year )
{
    SetMeowClassName("Date");
    // We assume everything is GMT
#ifdef WIN32
    struct tm current_time;
    current_time.tm_sec=second;
    current_time.tm_min=minute;
    current_time.tm_hour=hour;
    current_time.tm_mday=day;
    current_time.tm_mon=month-1;
    current_time.tm_year=year-1900;

    ts=(unsigned long)mktime(&current_time);
#else
    struct tm current_time;
    current_time.tm_sec=second;
    current_time.tm_min=minute;
    current_time.tm_hour=hour;
    current_time.tm_mday=day;
    current_time.tm_mon=month-1;
    current_time.tm_year=year-1900;

    ts=(unsigned long)timegm(&current_time);
#endif
}
Meow::Core::Time::Date::Date()
{
    SetMeowClassName("Date");
    ts=(unsigned long)time(NULL);
}

Meow::Core::Time::Date::~Date()
{

}
#ifdef WIN32
	#ifdef GetCurrentTime
		#undef GetCurrentTime
	#endif
#endif
Meow::Core::Time::Date_ptr Meow::Core::Time::Date::GetCurrentTime()
{
    return new Date();
}

short Meow::Core::Time::Date::GetHourOfDay( bool usegmtime )
{
    struct tm *current_time;
    if ( usegmtime)
        current_time=gmtime((time_t*)&ts);
    else
        current_time=localtime((time_t*)&ts);

    if ( current_time!=NULL )
        return current_time->tm_hour;
    return 0;
}
short Meow::Core::Time::Date::GetMinuteOfHour( bool usegmtime )
{
    struct tm *current_time;
    if ( usegmtime)
        current_time=gmtime((time_t*)&ts);
    else
        current_time=localtime((time_t*)&ts);
    if ( current_time!=NULL )
        return current_time->tm_min;
    return 0;
}
short Meow::Core::Time::Date::GetSecondOfMinute( bool usegmtime )
{
    struct tm *current_time;
    if ( usegmtime)
        current_time=gmtime((time_t*)&ts);
    else
        current_time=localtime((time_t*)&ts);
    if ( current_time!=NULL )
        return current_time->tm_sec;
    return 0;
}
short Meow::Core::Time::Date::GetDayOfMonth( bool usegmtime )
{
    struct tm *current_time;
    if ( usegmtime)
        current_time=gmtime((time_t*)&ts);
    else
        current_time=localtime((time_t*)&ts);
    return current_time->tm_mday;
}
short Meow::Core::Time::Date::GetMonthOfYear(bool usegmtime )
{
    struct tm *current_time;
    if ( usegmtime)
        current_time=gmtime((time_t*)&ts);
    else
        current_time=localtime((time_t*)&ts);
    if ( current_time!=NULL )
        return current_time->tm_mon+1;
    return 0;
}
short Meow::Core::Time::Date::GetYear( bool usegmtime )
{
    struct tm *current_time;
    if ( usegmtime)
        current_time=gmtime((time_t*)&ts);
    else
        current_time=localtime((time_t*)&ts);
    if ( current_time!=NULL )
        return current_time->tm_year+1900;
    return 0;
}
short Meow::Core::Time::Date::GetDayOfWeek( bool usegmtime )
{
    struct tm *current_time;
    if ( usegmtime)
        current_time=gmtime((time_t*)&ts);
    else
        current_time=localtime((time_t*)&ts);
    if ( current_time!=NULL )
        return current_time->tm_wday;
    return 0;
}

void Meow::Core::Time::Date::AddHour( short no_hours )
{
    ts=ts+(no_hours*3600);
}
void Meow::Core::Time::Date::AddMinute( short no_mins )
{
    ts=ts+(no_mins*60);
}
void Meow::Core::Time::Date::AddSecond( short no_seconds )
{
    ts=ts+no_seconds;
}

void Meow::Core::Time::Date::AddDay( short days )
{
    ts=ts+(days*86400);
}

void Meow::Core::Time::Date::SubtractHour( short no_hours )
{
    ts=ts-(no_hours*3600);
}
void Meow::Core::Time::Date::SubtractMinute( short no_mins )
{
    ts=ts-(no_mins*60);
}
void Meow::Core::Time::Date::SubtractSecond( short no_seconds )
{
    ts=ts-no_seconds;
}

void Meow::Core::Time::Date::SubtractDay( short days )
{
    ts=ts-(days*86400);
}

void Meow::Core::Time::Date::SubtractMonth( short months )
{
    // We can't do this the usual way, we need to find out how many days in this month
    // whenever it is, and subtract from it
    struct tm *current_time=gmtime((time_t*)&ts);
    current_time->tm_mon=current_time->tm_mon-months;
    ts=(unsigned long)mktime(current_time);
}
void Meow::Core::Time::Date::SetToStartOfDay( )
{
    SubtractHour(GetHourOfDay());
    SubtractMinute(GetMinuteOfHour());
    SubtractSecond(GetSecondOfMinute());
}
void Meow::Core::Time::Date::SetToEndOfDay( )
{
    struct tm *current_time=gmtime((time_t*)&ts);
    current_time->tm_sec=59;
    current_time->tm_min=59;
    current_time->tm_hour=23;
    ts=(unsigned long)mktime(current_time);
}
void Meow::Core::Time::Date::AddMonth( short months )
{
    // We can't do this the usual way, we need to find out how many days in this month
    // whenever it is, and add to it
    struct tm *current_time=gmtime((time_t*)&ts);
    current_time->tm_mon=current_time->tm_mon+months;
    ts=(unsigned long)mktime(current_time);
}
void Meow::Core::Time::Date::AddYear( short years )
{
    // We can't do this the usual way, we need to find out how many days in this month
    // whenever it is, and add to it
    struct tm *current_time=gmtime((time_t*)&ts);
    current_time->tm_year=current_time->tm_year+years;
    ts=(unsigned long)mktime(current_time);
}

bool Meow::Core::Time::Date::Equals( Meow::Core::Time::Date_ptr another )
{
    return ts==another->ts;
}
void Meow::Core::Time::Date::Add( Meow::Core::Time::Date_ptr another )
{
    ts=ts+another->ts;
}
bool Meow::Core::Time::Date::IsNumeric()
{
    return true;
}
bool Meow::Core::Time::Date::CouldBeFloat()
{
    return false;
}
bool Meow::Core::Time::Date::CouldBeInteger()
{
    return true;
}
long Meow::Core::Time::Date::GetValueAsInteger()
{
    return ts;
}
double Meow::Core::Time::Date::GetValueAsFloat()
{
    return (double)ts;
}
std::string Meow::Core::Time::Date::GetValueAsString()
{
    char temp[255];
    sprintf(temp,"%d",(int)ts);

    std::string reval=temp;
    return reval;
}

std::string Meow::Core::Time::Date::Format( std::string format, bool usegmtime )
{
	std::string retval; 
	if ( ts == 0 )
		retval="";
	else
	{
		struct tm *current_time;
		if ( usegmtime )
		    current_time=gmtime((time_t*)&ts);
		else
		    current_time=localtime((time_t*)&ts);
		char * buffer=(char*)malloc(format.length()*20);
		strftime(buffer,format.length()*20,format.c_str(),current_time);
		retval=buffer;
		free(buffer);
	}
    return retval;
}

void Meow::Core::Time::Date::Increment( )
{
    ts++;
}
void Meow::Core::Time::Date::Decrement( )
{
    ts--;
}
void Meow::Core::Time::Date::Let( Meow::Core::Types::ValueType_ptr other )
{
    ts=other->GetValueAsInteger();
}
void Meow::Core::Time::Date::Add( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts+other->GetValueAsInteger();
}
void Meow::Core::Time::Date::Subtract( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts-other->GetValueAsInteger();
}
void Meow::Core::Time::Date::Divide( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts/other->GetValueAsInteger();
}
void Meow::Core::Time::Date::Multiply( Meow::Core::Types::ValueType_ptr other )
{
    ts=ts*other->GetValueAsInteger();
}
bool Meow::Core::Time::Date::GreaterThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts>other->GetValueAsInteger();
}
bool Meow::Core::Time::Date::LessThan( Meow::Core::Types::ValueType_ptr other )
{
    return ts<other->GetValueAsInteger();
}
bool Meow::Core::Time::Date::Equals( Meow::Core::Types::ValueType_ptr other )
{
    return ts==other->GetValueAsInteger();
}
bool Meow::Core::Time::Date::NotEquals( Meow::Core::Types::ValueType_ptr other )
{
    return ts!=other->GetValueAsInteger();
}


void Meow::Core::Time::Date::SetSecondOfMinute( int second )
{
    SubtractSecond(GetSecondOfMinute());
    AddSecond(second);
}
void Meow::Core::Time::Date::SetMinuteOfHour( int minute )
{
    SubtractMinute(GetMinuteOfHour());
    AddMinute(minute);
}
void Meow::Core::Time::Date::SetHourOfDay( int hour )
{
    SubtractHour(GetHourOfDay());
    AddHour(hour);
}
void Meow::Core::Time::Date::SetDayOfMonth( int day )
{
    SubtractDay(GetDayOfMonth());
    AddDay(day);
}
void Meow::Core::Time::Date::SetMonthOfYear( int month )
{
    SubtractMonth(GetMonthOfYear()-1);
    AddMonth(month-1);
}
void Meow::Core::Time::Date::SetYear( int year )
{
    struct tm *current_time=gmtime((time_t*)&ts);
    current_time->tm_year=year-1900;
    ts=(unsigned long)mktime(current_time);
}
void Meow::Core::Time::Date::FromStandardString( std::string timestring )
{
	Core::Types::String_ptr timeString=new Core::Types::String(timestring);
	Core::Types::String_ptr yearPart=timeString->SubString(0,4);
	Core::Types::String_ptr monthPart=timeString->SubString(5,2);
	Core::Types::String_ptr dayPart=timeString->SubString(8,2);

	Core::Types::String_ptr hh=timeString->SubString(11,2);
	Core::Types::String_ptr mm=timeString->SubString(14,2);
	Core::Types::String_ptr ss=timeString->SubString(17,2);
	
	struct tm current_time;
    current_time.tm_sec=ss->GetValueAsInteger();
    current_time.tm_min=mm->GetValueAsInteger();
    current_time.tm_hour=hh->GetValueAsInteger();
    current_time.tm_mday=dayPart->GetValueAsInteger();
    current_time.tm_mon=monthPart->GetValueAsInteger()-1;
    current_time.tm_year=yearPart->GetValueAsInteger()-1900;
#ifdef WIN32
    ts=(unsigned long)mktime(&current_time);
#else
    ts=(unsigned long)timegm(&current_time);
#endif
}
Meow::Core::Time::Date_ptr Meow::Core::Time::Date::NextWorkingDay( int number )
{
	Meow::Core::Time::Date_ptr returnDate=new Meow::Core::Time::Date(GetValueAsInteger());
 	returnDate->AddDay(number);
	// Is it a saturday or a sunday?
	short day=returnDate->GetDayOfWeek();
	if ( day==0 )
		returnDate->AddDay(1);
	if ( day==6 )
		returnDate->AddDay(2);
	return returnDate;	
}
