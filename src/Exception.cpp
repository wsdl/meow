#include <Exception.h>
#include <string>
#include <Object.h>
#include <sstream>

Meow::Exception::Exception( ) :
    MessageStackPtr(0)
{
    ClassName="MeowException";
}

Meow::Exception::~Exception()
{
}

std::string Meow::Exception::Diagnostic()
{
    std::ostringstream oss;
    oss << "Exception class: " << GetMeowClassName () << "\n";
    while (HasMoreMessages())
    {
        oss << "\t" << "Exception message: " << GetNextMessage() << "\n";
    }
    return oss.str();
}

void Meow::Exception::SetMeowClassName( std::string classname )
{
    ClassName=classname;
}
std::string Meow::Exception::GetMeowClassName( )
{
    return ClassName;
}
void Meow::Exception::SetMessage( std::string message )
{
	Core::Log::GetLog()->Warning(GetMeowClassName(),"sss","Exception Data [",message.c_str(),"]");
    if ( MessageStackPtr<MEOW_EXCEPTION_MESSAGE_MAX )
    {
        Messages[MessageStackPtr]=message;
        MessageStackPtr++;
    }
}
bool Meow::Exception::HasMoreMessages( )
{
    return MessageStackPtr>0;
}
std::string Meow::Exception::GetNextMessage( )
{
    std::string returnit = "";
    if ( MessageStackPtr>0 )
    {
        returnit=Messages[MessageStackPtr-1];
        MessageStackPtr--;
    }
    return returnit;
}
