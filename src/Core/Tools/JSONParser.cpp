#include <Core/Tools/JSONParser.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <sstream>
#include <Core/Types/Boolean.h>
#include <Core/Types/Integer.h>
#include <Core/Types/Float.h>

Meow::Core::Tools::JSONParser::JSONParser()
{
	SetMeowClassName("Core/Tools/JSONParser");
	iArray=new Core::Types::Array();
}
Meow::Core::Tools::JSONParser::~JSONParser()
{
	
}
bool Meow::Core::Tools::JSONParser::ParseString( string document )
{
	bool retval;
	// std::stringstream doc;
	// doc << document;
	
	Json::Reader parser;
	Json::Value root;
	try
	{
		retval=parser.parse(document,root);
		if ( !retval )
		{
			iError=parser.getFormattedErrorMessages();
		}
		else
		{
			// doc >> root;
	
			// ParseValue((void*)&root,iArray);
			// cout << "Parsed to:" << root.asCString() << endl;
			
			// iArray->Set("root",(Core::ptr)ParseValue(root));
			Core::Types::ValueType_ptr retval=ParseValue(root);
			iArray=(Core::Types::Array_ptr)retval;
			HandleData(iArray);
		}
	} catch ( std::exception &e )
	{
		iError=e.what();
		retval=false;
	}
	return retval;
}
void Meow::Core::Tools::JSONParser::HandleData( Core::Types::ValueType_ptr rootElement )
{
	cout << rootElement->GetValueAsString() << endl;
}

std::string Meow::Core::Tools::JSONParser::GetErrorMessage( )
{
	return iError;
}
Meow::Core::Types::ValueType_ptr Meow::Core::Tools::JSONParser::ParseValue( Json::Value & vNode )
{
	// Deref, if we can 
	// Json::Value vNode=*(Json::Value*)node;
	Core::Types::ValueType_ptr value;
	if ( vNode.isBool() )
		value=new Core::Types::Boolean(vNode.asBool());
	else if ( vNode.isInt() )
		value=new Core::Types::Integer(vNode.asInt());
	else if ( vNode.isUInt() )
		value=new Core::Types::Integer(vNode.asUInt());
	else if ( vNode.isIntegral() )
		value=new Core::Types::Integer(vNode.asInt());
	else if ( vNode.isDouble() )
		value=new Core::Types::Float(vNode.asDouble());
	else if ( vNode.isNumeric() )
		value=new Core::Types::Float(vNode.asDouble());
	else if ( vNode.isString() )
		value=new Core::Types::String(vNode.asString());
	else if ( vNode.isArray() || vNode.isObject() )
	{
		Json::Value::Members memberNames = vNode.getMemberNames();
		value=new Core::Types::Array();
		for(unsigned int i=0; i<memberNames.size(); ++i)
		{
			string memberName = memberNames[i];
			Json::Value jvalue = vNode[memberName];
			
			((Core::Types::Array_ptr)value)->Set(memberName,(Core::ptr)ParseValue(jvalue));
		}
	}
	else
	{
		// cerr << "Can't identify vnode" << endl;
	}
	return value;
}