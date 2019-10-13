// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Log.h>
#include <Core/Types/ValueType.h>
#include <Core/Object.h>
#include <time.h>
#include <iomanip>
#include <inc/configuration.h>
#include <sstream>
#include <string>
#include <Core/Peers/OSSL.h>

#define EOL "\n"

#ifdef WIN32
	#define LOGBASE 
	#define LOGFILE "meow.log"
	#define OUTPUTSTREAM iStream
#else
	#define LOGBASE "/var/log/meow/"
	#define LOGFILE "meow.log"
	#define OUTPUTSTREAM iStream
#endif

using namespace std;

Meow::Core::LogCallback::LogCallback()
{
    SetMeowClassName("LogCallback");
    
}

Meow::Core::LogCallback::~LogCallback()
{
}

bool Meow::Core::LogCallback::Log( int dateData, std::string header, std::string context, std::string message )
{
	return false;
}

Meow::Core::Log::Log( std::string application ):
    iDebug(false),
    iTransaction(false),
    iInformation(true),
    iWarning(true),
    iError(true),
    iCompiler(false),
    iSequel(false),
    iSequelResult(false),
    iClient(false),
    iMeowLink(false),
    iSecurity(false),
    iEvents(false),
    iHeader(false),
    iOffset(0),
    iData(false),
    iCout(false),
    iRemoteWrite(false),
    iAbortOnCompileMessage(false),
    iWorkflow(false),
    iTrace(false),
    iPerformance(false)
{

    SetMeowClassName("Log");
    iRecords=new Core::Types::Stack();
    iProfile=new Core::Types::Array();
    
    time_t ts=time(NULL);
    struct tm *current_time=gmtime(&ts);
    
    char datestamp[26];
    sprintf(datestamp, "%.2d.%.2d.%.4d",current_time->tm_mday,
    			current_time->tm_mon+1,
    			current_time->tm_year+1900);
    
    std::string logname=std::string(LOGBASE) + LOGFILE + std::string(".") + application + std::string(".") + datestamp ;
    iStream.open( logname.c_str(),ios::app);
    
    Core::Types::Array_ptr iRemoteWriteCache=new Core::Types::Array();

    if ( Configuration::GetConfiguration()->IsSet("remote_logging") )
    {
        iRemoteWrite=true;
        stringstream ss;
        ss << "[" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mday << "/" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mon + 1 << "/" <<
           current_time->tm_year+1900 << " " <<
           current_time->tm_hour << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_min << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_sec << "| ";
        ss << "Log: " << logname << EOL;
        ss << "-------------------------------------------------------------------------------------------------\n";
        Core::Types::String_ptr line=new Core::Types::String(ss.str());
        iRemoteWriteCache->Set(iRemoteWriteCache->Count(),(ptr)line);
    }
    else
    {
        OUTPUTSTREAM << "[" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mday << "/" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mon + 1 << "/" <<
                     current_time->tm_year+1900 << " " <<
                     current_time->tm_hour << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_min << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_sec << "| ";
        OUTPUTSTREAM << "Log: " << logname << EOL;
        OUTPUTSTREAM << "-------------------------------------------------------------------------------------------------\n";
        if (iFlush) Flush();
    }
}

Meow::Core::Log::Log():
    iDebug(false),
    iTransaction(false),
    iInformation(false),
    iWarning(false),
    iError(false),
    iCompiler(false),
    iSequel(false),
    iSequelResult(false),
    iClient(false),
    iMeowLink(false),
    iSecurity(false),
    iEvents(false),
    iHeader(false),
    iOffset(0),
    iData(false),
    iCout(false),
    iRemoteWrite(false),
    iAbortOnCompileMessage(false),
    iWorkflow(false),
    iTrace(false),
    iPerformance(false)
{

    SetMeowClassName("Log");
    iRecords=new Core::Types::Stack();
    iProfile=new Core::Types::Array();
    char processID[1024];
    sprintf(processID,"%ld",GetProcessID());
    std::string logname=std::string(LOGBASE) + LOGFILE + std::string(".") + processID;
    iStream.open( logname.c_str(),ios::app);
    time_t ts=time(NULL);
    struct tm *current_time=gmtime(&ts);
    Core::Types::Array_ptr iRemoteWriteCache=new Core::Types::Array();

    if ( Configuration::GetConfiguration()->IsSet("remote_logging") )
    {
        iRemoteWrite=true;
        stringstream ss;
        ss << "[" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mday << "/" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mon + 1 << "/" <<
           current_time->tm_year+1900 << " " <<
           current_time->tm_hour << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_min << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_sec << "| ";
        ss << "Log: " << logname << EOL;
        ss << "-------------------------------------------------------------------------------------------------\n";
        Core::Types::String_ptr line=new Core::Types::String(ss.str());
        iRemoteWriteCache->Set(iRemoteWriteCache->Count(),(ptr)line);
    }
    else
    {
        OUTPUTSTREAM << "[" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mday << "/" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mon + 1 << "/" <<
                     current_time->tm_year+1900 << " " <<
                     current_time->tm_hour << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_min << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_sec << "| ";
        OUTPUTSTREAM << "Log: " << logname << EOL;
        OUTPUTSTREAM << "-------------------------------------------------------------------------------------------------\n";
        if (iFlush) Flush();
    }
}
Meow::Core::Log::~Log()
{
    OUTPUTSTREAM << "-------------------------------------------------------------------------------------------------\n";
    Flush();
    DumpTrace();
    iStream.close();
}

Meow::Core::Log_ptr Meow::Core::Log::GetLog()
{
    if ( iLog.IsNull() )
        iLog=new Log();
    return iLog;
}
Meow::Core::Log_ptr Meow::Core::Log::GetApplicationLog( std::string application )
{
	if ( iApplicationLogs->IsSet(application) )
		return (Core::Log_ptr)iApplicationLogs->Get(application);
	Log_ptr log=new Log(application);
	log->ReconfigureFrom("ebay");
	iApplicationLogs->Set(application,(Core::ptr)log);
	return log;
}

void Meow::Core::Log::Reconfigure( )
{
    if (  Configuration::GetConfiguration()->IsSet("remote_logging") )
        iRemoteWrite=true;
    if ( Configuration::GetConfiguration()->IsSet("log_debug") ) // && !Configuration::GetConfiguration()->Get("log_debug").compare("true") )
        iDebug=true;
    if ( Configuration::GetConfiguration()->IsSet("log_information")) //  && !Configuration::GetConfiguration()->Get("log_information").compare("true")  )
        iInformation=true;
    if ( Configuration::GetConfiguration()->IsSet("log_warning") ) //  && !Configuration::GetConfiguration()->Get("log_warning").compare("true")  )
        iWarning=true;
    if ( Configuration::GetConfiguration()->IsSet("log_error") ) //  && !Configuration::GetConfiguration()->Get("log_error").compare("true")  )
        iError=true;
    if ( Configuration::GetConfiguration()->IsSet("log_compiler") ) //  && !Configuration::GetConfiguration()->Get("log_compiler").compare("true")  )
        iCompiler=true;
    if ( Configuration::GetConfiguration()->IsSet("log_sql")) //  && !Configuration::GetConfiguration()->Get("log_debug").compare("true")  )
        iSequel=true;
    if ( Configuration::GetConfiguration()->IsSet("log_sqlresult")) //  && !Configuration::GetConfiguration()->Get("log_debug").compare("true")  )
        iSequelResult=true;
    if ( Configuration::GetConfiguration()->IsSet("log_client") ) // && !Configuration::GetConfiguration()->Get("log_debug").compare("true")  )
        iClient=true;
    if ( Configuration::GetConfiguration()->IsSet("log_meowlink") ) //  && !Configuration::GetConfiguration()->Get("log_debug").compare("true")  )
        iMeowLink=true;
    if ( Configuration::GetConfiguration()->IsSet("log_security") ) //  && !Configuration::GetConfiguration()->Get("log_security").compare("true")  )
        iSecurity=true;
    if ( Configuration::GetConfiguration()->IsSet("log_events") )
        iEvents=true;
    if ( Configuration::GetConfiguration()->IsSet("log_header") )
        iHeader=true;
    if ( Configuration::GetConfiguration()->IsSet("log_data") )
        iData=true;
    if ( Configuration::GetConfiguration()->IsSet("log_flush") )
        iFlush=true;
    if ( Configuration::GetConfiguration()->IsSet("log_cout") )
        iCout=true;
    if ( Configuration::GetConfiguration()->IsSet("log_compiler_abort") )
        iAbortOnCompileMessage=true;
    if ( Configuration::GetConfiguration()->IsSet("log_workflow") )
        iWorkflow=true;
    if ( Configuration::GetConfiguration()->IsSet("log_on") )
        iTrace=true;
    if ( Configuration::GetConfiguration()->IsSet("log_transaction") )
        iTransaction=true;

}

void Meow::Core::Log::Indent()
{
    iOffset=iOffset+3;
}

void Meow::Core::Log::Outdent()
{
    iOffset=iOffset-3;
}

void Meow::Core::Log::WriteHeader( std::string context, std::string identifier )
{
    if ( !iTrace )
        return;
    time_t ts=time(NULL);
    struct tm *current_time=gmtime((time_t*)&ts);


    /* current_time->tm_sec=second;
    current_time->m_min=minute;
    current_time.tm_hour=hour;
    current_time.tm_mday=day;
    current_time.tm_mon=month-1;
    current_time.tm_year=year-1900; */
    if ( !iCout & !iRemoteWrite )
    {
        OUTPUTSTREAM << "[" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mday << "/" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mon + 1 << "/" <<
                     current_time->tm_year+1900 << " " <<
                     current_time->tm_hour << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_min << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_sec << "|";
        OUTPUTSTREAM  << identifier << Offset() << context << "::> ";
    }
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << "[" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mday << "/" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mon + 1 << "/" <<
           current_time->tm_year+1900 << " " <<
           current_time->tm_hour << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_min << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_sec << "|";
        ss << identifier << Offset() << context << "::> ";
        Core::Types::String_ptr stringLine=new Core::Types::String(ss.str());
        iRemoteWriteCache->Set(iRemoteWriteCache->Count(),(ptr)stringLine);
    }
    else
    {
        cout << "[" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_mday << "/" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_mon + 1 << "/" <<
             current_time->tm_year+1900 << " " <<
             current_time->tm_hour << ":" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_min << ":" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_sec << "|";
        cout  << identifier << Offset() << context << "::> ";

    }
}

void Meow::Core::Log::Debug( std::string context, std::string format, ... )
{

#ifdef DEBUG
    DoLog(iDebug,"___")

#endif
}
void Meow::Core::Log::Information( std::string context, std::string format, ... )
{

    if ( !iTrace )
        return;
    if (!iInformation) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";

        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(I)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(I)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            if ( iCout )
            	cout << message;
            else if ( iRemoteWrite )
            {
            	stringstream ss; ss << message;
            	String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            	stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
            	OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
        Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;

    if (iFlush) Flush();

}
void Meow::Core::Log::Warning( std::string context, std::string format,  ... )
{
    if ( !iTrace )
        return;
    if (!iWarning) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(W)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(W)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
        Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();


}
void Meow::Core::Log::Error( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iError) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;
            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(E)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(E)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            if ( iCout )
            	cout << message;
            else if ( iRemoteWrite )
            {
            	stringstream ss; ss << message;
            	String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            	stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
            	OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;

        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();


}
void Meow::Core::Log::Compiler( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iCompiler) return;


    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(C)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(C)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();
    if ( Configuration::GetConfiguration()->IsSet("log_compiler_abort") )
        exit(1);



}
void Meow::Core::Log::Performance( std::string context, std::string format,  ... )
{
    if ( !iTrace )
        return;
    if (!iPerformance) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(P)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(P)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();


}

void Meow::Core::Log::Flush()
{
    iStream.flush();
}

Meow::Core::Log_ptr Meow::Core::Log::iLog=new Meow::Core::Log();
Meow::Core::Types::Array_ptr Meow::Core::Log::iApplicationLogs=new Meow::Core::Types::Array();


Meow::Core::PerformanceRecord::PerformanceRecord()
{
    entry_time=0;
    exit_time=0;
    recordkey="";
}

Meow::Core::PerformanceRecord::~PerformanceRecord()
{
}

Meow::Core::ProfileRecord::ProfileRecord()
{
    key="";
    minimum_time=32768;
    maximum_time=0;
    hits=0;
    average_time=0;
}

Meow::Core::ProfileRecord::~ProfileRecord()
{
}

void Meow::Core::Log::TraceMethodEntry(
    std::string methodname )
{
#ifdef PERFORMANCE_PROFILE
//    PerformanceRecord_ptr entry=new PerformanceRecord();
//    iRecords->Push((ptr)entry);
//    entry->entry_time=(unsigned long)time(NULL);
//    entry->recordkey= methodname;
#endif
}

void Meow::Core::Log::TraceMethodExit( )
{
#ifdef PERFORMANCE_PROFILE
//    unsigned long diff;
//    PerformanceRecord_ptr entry;
//    ProfileRecord_ptr profile;
//    entry=(PerformanceRecord_ptr)iRecords->Pop();

//    entry->exit_time=(unsigned long)time(NULL);
//    diff=entry->exit_time-entry->entry_time;
//    if ( iProfile->IsSet(entry->recordkey) )
//        profile=(ProfileRecord_ptr)iProfile->Get(entry->recordkey);
//    else
//    {
//        profile=new ProfileRecord();
//        iProfile->Set(entry->recordkey,(ptr)profile);
//        profile->minimum_time=0;
//    }
//    if ( profile->minimum_time>diff )
//        profile->minimum_time=diff;
//    if ( profile->maximum_time<diff )
//        profile->maximum_time=diff;
//    profile->average_time=((profile->average_time*profile->hits)+diff)/(profile->hits+1);
//    profile->hits++;
#endif
}

void Meow::Core::Log::DumpTrace()
{
#ifdef PERFORMANCE_PROFILE
//    Stack_ptr keys=iProfile->GetKeys();
//    while ( !keys->IsEmpty() )
//    {
//       Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
//        ProfileRecord_ptr record=(ProfileRecord_ptr)iProfile->Get(key->GetValue());
//        OUTPUTSTREAM << key->GetValue() << "(" << record->minimum_time << " <= " << record->average_time << " <= " << record->maximum_time << " ) #" << record->hits << EOL;
//    }
#endif
}

void Meow::Core::Log::FullTraceMethodEntry( std::string classname,
		Core::Types::Array_ptr key,
        std::string methodname,
        std::string format,
        ... )
{
    if ( !iTrace )
        return;
#ifdef PERFORMANCE_PROFILE_X
    va_list ap;

    iStream  << ">>Call " << Offset() << classname << ":: ";
    for ( int i=0; i<key->Count(); i++ )
    {
        if ( i>0 )
            OUTPUTSTREAM << "/";
       Core::Types::String_ptr element=(Core::Types::String_ptr)key->Get(i);
        OUTPUTSTREAM << element->GetValue();
    }

    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int message=va_arg(ap,int);
            if ( message ) OUTPUTSTREAM << "true";
            else OUTPUTSTREAM << "false";
            break;
        }
        case 'f':
        {
            double message=va_arg(ap,double);
            OUTPUTSTREAM << message;
            break;
        }
        case 'd':
        {
            double message=va_arg(ap,double);
            OUTPUTSTREAM << message;
            break;
        }
        case 'v':
        {
            void * void_star=va_arg(ap,void*);
            ValueType_ptr value=void_star;
            OUTPUTSTREAM << value->GetValueAsString();
            break;
        }
        /*		case 'o':
        			{
        				void * void_star=va_arg(ap,void*);

        				Object_ptr object=void_star;
        				OUTPUTSTREAM << object->ToString();
        				break;
        			}*/
        case 'k':
        {
            void * void_star=va_arg(ap,void*);
            Object_ptr object=void_star;
            OUTPUTSTREAM << "kref:" << object->GetMeowClassName() << "/" << void_star;
            break;
        }
        /*		case 'x':
        			{
        				void * exception_as_void_star=va_arg(ap,void*);
        				Meow::Exception * excep=(Meow::Exception*)exception_as_void_star;
        				OUTPUTSTREAM << "Exception:" << excep->GetMeowClassName() << " ";
        				while ( excep->HasMoreMessages() )
        	 	           OUTPUTSTREAM << "[" << excep->GetNextMessage() << "]";
        	 	        break;
        			}*/
        }
    }
    va_end(ap);
    OUTPUTSTREAM << EOL;
    TraceMethodEntry(classname + "::" + methodname);
    iOffset++;
#endif
}



void Meow::Core::Log::FullTraceMethodExit( std::string classname,
		Core::Types::Array_ptr key,
        std::string methodname,
        std::string format,
        ... )
{
    if ( !iTrace )
        return;
#ifdef PERFORMANCE_PROFILE_X
    va_list ap;

    iStream  << "<<Call " << Offset() << classname << ":: ";
    for ( int i=0; i<key->Count(); i++ )
    {
        if ( i>0 )
            OUTPUTSTREAM << "/";
       Core::Types::String_ptr element=(Core::Types::String_ptr)key->Get(i);
        OUTPUTSTREAM << element->GetValue();
    }

    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int message=va_arg(ap,int);
            if ( message ) OUTPUTSTREAM << "true";
            else OUTPUTSTREAM << "false";
            break;
        }
        case 'f':
        {
            double message=va_arg(ap,double);
            OUTPUTSTREAM << message;
            break;
        }
        case 'd':
        {
            double message=va_arg(ap,double);
            OUTPUTSTREAM << message;
            break;
        }
        case 'v':
        {
            void * void_star=va_arg(ap,void*);
            ValueType_ptr value=void_star;
            OUTPUTSTREAM << value->GetValueAsString();
            break;
        }
        /*		case 'o':
        			{
        				void * void_star=va_arg(ap,void*);

        				Object_ptr object=void_star;
        				OUTPUTSTREAM << object->ToString();
        				break;
        			}*/
        case 'k':
        {
            void * void_star=va_arg(ap,void*);
            Object_ptr object=void_star;
            OUTPUTSTREAM << "kref:" << object->GetMeowClassName() << "/" << void_star;
            break;
        }
        /*		case 'x':
        			{
        				void * exception_as_void_star=va_arg(ap,void*);
        				Meow::Exception * excep=(Meow::Exception*)exception_as_void_star;
        				OUTPUTSTREAM << "Exception:" << excep->GetMeowClassName() << " ";
        				while ( excep->HasMoreMessages() )
        	 	           OUTPUTSTREAM << "[" << excep->GetNextMessage() << "]";
        	 	        break;
        			}*/
        }
    }
    va_end(ap);
    OUTPUTSTREAM << EOL;
    iOffset--;
    TraceMethodExit();
    if (iFlush) Flush();
#endif
}



std::string Meow::Core::Log::Offset()
{
    return std::string(iOffset, ' ');
}

void Meow::Core::Log::Sequel( std::string context, std::string format, ... )
{

    if ( !iTrace )
        return;
    if (!iSequel) return;
    va_list ap;
    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {


                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(S)",context,logmessage );
    }
    if ( !continueit )
        return;

    WriteHeader(context,"(S)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();


}
void Meow::Core::Log::SequelResult( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iSequelResult) return;
    va_list ap;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);

        continueit=!iCallback->Log((int)ts,"(R)",context,logmessage );
    }
    if ( !continueit )
        return;

    WriteHeader(context,"(R)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();


}

void Meow::Core::Log::Client( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iClient) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;

                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(Z)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(Z)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();


}

void Meow::Core::Log::MeowLink( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iMeowLink) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(L)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(L)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));

            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();


}
void Meow::Core::Log::Security( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iSecurity) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();

                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(@)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(@)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;
    if (iFlush) Flush();

}
void Meow::Core::Log::Events( std::string event, std::string username,Core::Types::Array_ptr parameters )
{
    if ( !iTrace )
        return;
    if (!iEvents) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        std::string logmessage;
        logmessage.append(parameters->GetValueAsString());
        continueit=!iCallback->Log((int)ts,"(*)",event + "(" + username + ")",logmessage );
    }
    if ( !continueit )
        return;

    if ( iRemoteWrite )
    {
        stringstream ss;
        time_t ts=time(NULL);
        struct tm *current_time=gmtime(&ts);

        ss << "["  <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mday << "/" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mon + 1 << "/" <<
           current_time->tm_year+1900 << " " <<
           current_time->tm_hour << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_min << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_sec << "|";

        ss << "(*) " << Offset() << username << "::> " << event ;
        ss << " [----------------\n";
        if ( ! Configuration::GetConfiguration()->IsSet("log_suppress_parameters_" + event) )
        {
        	Core::Types::Stack_ptr keys=parameters->GetKeys();
            while ( !keys->IsEmpty() )
            {
               Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
                ptr value=parameters->Get(key->GetValueAsString());
                ss << Offset() << "-" << key->GetValueAsString() << " Type:" << value->GetMeowClassName() << " Value:";
                if ( value->IsInstanceOf("ValueType") )
                {
                	Core::Types::ValueType_ptr valueAsValueType=(Core::Types::ValueType_ptr)value;
                    ss << Offset() << Offset() << valueAsValueType->GetValueAsString() << "\n";
                }
            }
        }
        else
            ss << "Hidden by rule log_suppress_parameters_" + event;

        ss << "[" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mday << "/" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_mon + 1 << "/" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_year+1900 << " " <<
           current_time->tm_hour << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_min << ":" <<
           std::setfill('0') << std::setw(2) <<
           current_time->tm_sec << "|";

        ss << "(*) " << Offset() << username << "::> " << event << " ---------------------]";
        ss << EOL;
       Core::Types::String_ptr stringLine=new Core::Types::String(ss.str());
        iRemoteWriteCache->Set(iRemoteWriteCache->Count(),(ptr)stringLine);
    }
    else if ( !iCout )
    {
        time_t ts=time(NULL);
        struct tm *current_time=gmtime(&ts);

        OUTPUTSTREAM << "["  <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mday << "/" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mon + 1 << "/" <<
                     current_time->tm_year+1900 << " " <<
                     current_time->tm_hour << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_min << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_sec << "|";

        OUTPUTSTREAM << "(*) " << Offset() << username << "::> " << event ;
        OUTPUTSTREAM << " [----------------\n";
        if ( ! Configuration::GetConfiguration()->IsSet("log_suppress_parameters_" + event) )
        {
        	Core::Types::Stack_ptr keys=parameters->GetKeys();
            while ( !keys->IsEmpty() )
            {
               Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
                ptr value=parameters->Get(key->GetValueAsString());
                OUTPUTSTREAM << Offset() << "-" << key->GetValueAsString();
				if ( value.IsNull() )
				{
                    OUTPUTSTREAM << " Type:" << "NULL" << " Value:" << Offset() << Offset() << "NULL" << "\n";				
                }
				else if ( value->IsInstanceOf("ValueType") )
                {
                	Core::Types::ValueType_ptr valueAsValueType=(Core::Types::ValueType_ptr)value;
                    OUTPUTSTREAM << " Type:" << value->GetMeowClassName() << " Value:" << Offset() << Offset() << valueAsValueType->GetValueAsString() << "\n";
                }
            }
        }
        else
            OUTPUTSTREAM << "Hidden by rule log_suppress_parameters_" + event;

        OUTPUTSTREAM << "[" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mday << "/" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_mon + 1 << "/" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_year+1900 << " " <<
                     current_time->tm_hour << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_min << ":" <<
                     std::setfill('0') << std::setw(2) <<
                     current_time->tm_sec << "|";

        OUTPUTSTREAM << "(*) " << Offset() << username << "::> " << event << " ---------------------]";
        OUTPUTSTREAM << EOL;
        if (iFlush) Flush();
    }
    else
    {
        time_t ts=time(NULL);
        struct tm *current_time=gmtime(&ts);

        cout << "["  <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_mday << "/" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_mon + 1 << "/" <<
             current_time->tm_year+1900 << " " <<
             current_time->tm_hour << ":" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_min << ":" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_sec << "|";

        cout << "(*) " << Offset() << username << "::> " << event ;
        cout << " [----------------\n";
        if ( ! Configuration::GetConfiguration()->IsSet("log_suppress_parameters_" + event) )
        {
        	Core::Types::Stack_ptr keys=parameters->GetKeys();
            while ( !keys->IsEmpty() )
            {
               Core::Types::String_ptr key=(Core::Types::String_ptr)keys->Pop();
                ptr value=parameters->Get(key->GetValueAsString());
				if ( !value.IsNull() )
				{
	                cout << Offset() << "-" << key->GetValueAsString() << " Type:" << value->GetMeowClassName() << " Value:";	
    	            if ( value->IsInstanceOf("ValueType") )
    	            {
    	            	Core::Types::ValueType_ptr valueAsValueType=(Core::Types::ValueType_ptr)value;
    	                cout << Offset() << Offset() << valueAsValueType->GetValueAsString() << "\n";
    	            }
				}
				else
	                cout << Offset() << "-" << key->GetValueAsString() << " Type:NULL Value:NULL\n";	

            }
        }
        else
            cout << "Hidden by rule log_suppress_parameters_" + event;


        cout << "[" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_mday << "/" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_mon + 1 << "/" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_year+1900 << " " <<
             current_time->tm_hour << ":" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_min << ":" <<
             std::setfill('0') << std::setw(2) <<
             current_time->tm_sec << "|";
        cout << "(*) " << Offset() << username << "::> " << event << " ---------------------]";
        cout << EOL;
        if (iFlush) Flush();
    }

}
void Meow::Core::Log::Header( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iHeader) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(H)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(H)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;

    if (iFlush) Flush();

}

void Meow::Core::Log::Data( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iData) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"<->",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"<->");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;

    if (iFlush) Flush();

}
void Meow::Core::Log::Workflow( std::string context, std::string format, ... )
{
    if ( !iTrace )
        return;
    if (!iWorkflow) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";
        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"WFL",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"WFL");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);

            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            				if ( iCout )
            					cout << message;
            				else if ( iRemoteWrite )
            				{
            					stringstream ss; ss << message;
            					String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            					stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            				}
            				else
            					OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
               Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
       Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;

    if (iFlush) Flush();

}

void Meow::Core::Log::Transaction( std::string context, std::string format, ... )
{

    if ( !iTrace )
        return;
    if (!iInformation) return;

    bool continueit=true;
    if ( !iCallback.IsNull() )
    {
        time_t ts=time(NULL);
        va_list ap;
        std::string logmessage="";

        va_start(ap,format);
        for ( int i=0; i<format.length(); i++ )
        {
            char format_type=format[i];
            switch (format_type)
            {
            case 's':
            {
                char * message=va_arg(ap,char *);
                logmessage.append(message);
                break;
            }
            case 'i':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'l':
            {
                long message=va_arg(ap,long);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'b':
            {
                int messageAsBool=va_arg(ap,int);
                if ( messageAsBool )
                    logmessage.append("true");
                else
                    logmessage.append("false");
                break;
            }
            case 'f':

            {
                double message=va_arg(ap,double);
                stringstream ss;
                ss << message;
                logmessage.append(ss.str());

                break;
            }
            case 'd':
            {
                int message=va_arg(ap,int);

                stringstream ss;
                ss << message;
                logmessage.append(ss.str());
                break;
            }
            case 'k':
            {
                Object_ptr object=va_arg(ap,void *);
                std::string message="kref:" + object->GetMeowClassName();
                logmessage.append(message);
                break;

            }
            }
        }
        va_end(ap);
        continueit=!iCallback->Log((int)ts,"(T)",context,logmessage );
    }
    if ( !continueit )
        return;

    va_list ap;
    WriteHeader(context,"(T)");
    va_start(ap,format);
    for ( int i=0; i<format.length(); i++ )
    {
        char format_type=format[i];
        switch (format_type)
        {
        case 's':
        {
            char * message=va_arg(ap,char *);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'i':
        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'l':
        {
            long message=va_arg(ap,long);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;
        }
        case 'b':
        {
            int messageAsBool=va_arg(ap,int);
            std::string message="";
            if ( messageAsBool )
                message="true";
            else
                message="false";
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;

            break;
        }
        case 'f':

        {

            double message=va_arg(ap,double);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'd':

        {
            int message=va_arg(ap,int);
            if ( iCout )
                cout << message;
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message;
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message;
            break;

        }

        case 'v':

        {
            /*
            if ( iCout )
            	cout << message;
            else if ( iRemoteWrite )
            {
            	stringstream ss; ss << message;
            	String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
            	stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
            	OUTPUTSTREAM << message; */
            break;

        }

        case 'k':

        {

            Object_ptr object=va_arg(ap,void *);
            std::string message="kref:" + object->GetMeowClassName() + "/";
            if ( iCout )
                cout << message << object.Cast();
            else if ( iRemoteWrite )
            {
                stringstream ss;
                ss << message << object.Cast();
                Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
                stringLine->SetValue(stringLine->GetValue().append(ss.str()));
            }
            else
                OUTPUTSTREAM << message << object.Cast();
            break;

        }

        }

    }
    va_end(ap);
    if ( iCout )
        cout << EOL;
    else if ( iRemoteWrite )
    {
        stringstream ss;
        ss << EOL;
        Core::Types::String_ptr stringLine=(Core::Types::String_ptr)iRemoteWriteCache->Get(iRemoteWriteCache->Count()-1);
        stringLine->SetValue(stringLine->GetValue().append(ss.str()));
    }
    else
        OUTPUTSTREAM << endl;

    if (iFlush) Flush();

}    

void Meow::Core::Log::ReconfigureFrom( std::string extent )
{
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_debug") && !Configuration::GetConfiguration()->Get("log_debug").compare("true") )
        iDebug=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_information") && !Configuration::GetConfiguration()->Get("log_information").compare("true")  )
        iInformation=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_warning") && !Configuration::GetConfiguration()->Get("log_warning").compare("true")  )
        iWarning=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_error") && !Configuration::GetConfiguration()->Get("log_error").compare("true")  )
        iError=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_compiler")  && !Configuration::GetConfiguration()->Get("log_compiler").compare("true")  )
        iCompiler=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_sql") && !Configuration::GetConfiguration()->Get("log_sql").compare("true")  )
        iSequel=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_sqlresult")  && !Configuration::GetConfiguration()->Get("log_sqlresult").compare("true")  )
        iSequelResult=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_client") && !Configuration::GetConfiguration()->Get("log_client").compare("true")  )
        iClient=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_security")  && !Configuration::GetConfiguration()->Get("log_security").compare("true")  )
        iSecurity=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_events")  && !Configuration::GetConfiguration()->Get("log_events").compare("true")  )
        iEvents=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_header")  && !Configuration::GetConfiguration()->Get("log_header").compare("true")  )
        iHeader=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_data")  && !Configuration::GetConfiguration()->Get("log_data").compare("true")  )
        iData=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_flush")  && !Configuration::GetConfiguration()->Get("log_flush").compare("true")  )
        iFlush=true;
    if ( Configuration::GetConfiguration()->IsSet(extent + ".log_cout")  && !Configuration::GetConfiguration()->Get("log_cout").compare("true")  )
        iCout=true;
    if (  Configuration::GetConfiguration()->IsSet(extent + ".log_remote")  && !Configuration::GetConfiguration()->Get("log_remote").compare("true") )
        iRemoteWrite=true;
    if (  Configuration::GetConfiguration()->IsSet(extent + ".log_on")  && !Configuration::GetConfiguration()->Get("log_on").compare("true")  )
        iTrace=true;
    if (  Configuration::GetConfiguration()->IsSet(extent + ".log_transaction")  && !Configuration::GetConfiguration()->Get("log_transaction").compare("true")  )
        iTransaction=true;
}
void Meow::Core::Log::TraceOn()
{
    OUTPUTSTREAM << "TraceOn()" << endl;
    iTrace=true;
}
void Meow::Core::Log::TraceOff()
{
    OUTPUTSTREAM << "TraceOff()" << endl;
    iTrace=false;
}
void Meow::Core::Log::SetRemoteLogCallback( LogCallback_ptr log )
{
    iCallback=log;
}
