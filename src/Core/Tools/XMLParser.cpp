#include <Core/Tools/XMLParser.h>
#include <Core/Types/String.h>
#include <expat.h>

Meow::Core::Tools::XMLElement::XMLElement()
{
		SetMeowClassName("Core/Tools/XMLElement");

}

Meow::Core::Tools::XMLElement::~XMLElement()
{
}

static void XMLCALL startElement( void * object, const char * name, const char ** attr )
{
    Meow::Core::Types::Array_ptr attributes=new Meow::Core::Types::Array();
    Meow::Core::Tools::XMLParser_ptr xmlobj=(Meow::Core::Tools::XMLParser*)object;
    string cppName=name;
    for (int i = 0; attr[i]; i += 2)
    {
        string name=attr[i];
        string value=attr[i+1];
        Meow::Core::Types::String_ptr valueas=new Meow::Core::Types::String();
        valueas->SetValue(value);
        attributes->Set(name,(Meow::Core::ptr)valueas);
    }
    Meow::Core::Tools::XMLElement_ptr stackMember=new Meow::Core::Tools::XMLElement();
    stackMember->elementAttributes=attributes;
    stackMember->elementName=cppName;
    xmlobj->GetElementStack()->Push((Meow::Core::ptr)stackMember);
    xmlobj->StartElement(cppName,attributes);
}
static void XMLCALL characterData( void * object, const char * data, int length )
{

	Meow::Core::Tools::XMLParser_ptr xmlobj=(Meow::Core::Tools::XMLParser*)object;
    std::string data_str=data;
    xmlobj->CharacterData(data_str.substr(0,length));
    Meow::Core::Tools::XMLElement_ptr stackMember=(Meow::Core::Tools::XMLElement_ptr)xmlobj->GetElementStack()->Top();
    stackMember->characterData.append(data_str.substr(0,length));	
}
static void XMLCALL endElement( void * object, const char * name )
{
	Meow::Core::Tools::XMLParser_ptr xmlobj=(Meow::Core::Tools::XMLParser*)object;
    string cppName=name;
    xmlobj->EndElement(cppName);
    Meow::Core::ptr cleanup=xmlobj->GetElementStack()->Pop();
}

Meow::Core::Tools::XMLParser::XMLParser()
{
    SetMeowClassName("XMLParser");
//    parser=(void*)XML_ParserCreate("ISO-8859-1");
    parser=(void*)XML_ParserCreate("utf-8");
    parsing=false;
    iStack=new Core::Types::Stack();
}
Meow::Core::Tools::XMLParser::~XMLParser()
{
    if ( parser!=NULL )
        XML_ParserFree((XML_ParserStruct*)parser);
}

bool Meow::Core::Tools::XMLParser::ParseString( string document )
{
	// cout << "ParseString" << document << endl;
	// cout << "---------------------------------------------" << endl;
    bool retval=false;
    parsing=true;
    XML_SetUserData((XML_ParserStruct*)parser,this);
    XML_SetElementHandler((XML_ParserStruct*)parser,startElement,endElement);
    XML_SetCharacterDataHandler((XML_ParserStruct*)parser,characterData);
    retval=XML_Parse((XML_ParserStruct*)parser,document.c_str(),document.length(),true) != XML_STATUS_ERROR;

    parsing=false;
    return retval;
}
bool Meow::Core::Tools::XMLParser::ParsePartString( string document, bool finished )
{
	// cout << "ParsePartString" << document << endl;
	// cout << "---------------------------------------------" << endl;
    if ( !parsing )
    {
        XML_SetUserData((XML_ParserStruct*)parser,this);
        XML_SetElementHandler((XML_ParserStruct*)parser,startElement,endElement);
        XML_SetCharacterDataHandler((XML_ParserStruct*)parser,characterData);
    }
    return XML_Parse((XML_ParserStruct*)parser,document.c_str(),document.length(),finished) != XML_STATUS_ERROR;

}
int  Meow::Core::Tools::XMLParser::GetLineNumber( )
{
    return XML_GetCurrentLineNumber((XML_ParserStruct*)parser);
}
string Meow::Core::Tools::XMLParser::GetLastError( )
{
    return XML_ErrorString(XML_GetErrorCode((XML_ParserStruct*)parser));
}
void Meow::Core::Tools::XMLParser::StartElement( string element, Meow::Core::Types::Array_ptr attributes )
{
}
void Meow::Core::Tools::XMLParser::EndElement( string element )
{
}
void Meow::Core::Tools::XMLParser::CharacterData( string data )
{
}
Meow::Core::Types::Stack_ptr Meow::Core::Tools::XMLParser::GetElementStack()
{
	return iStack;
}
