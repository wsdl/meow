#include <Core/Types/Boolean.h>
#include <XML/PageParser.h>
#include <Session.h>
#include <Registry.h>

Meow::XML::PageParameter::PageParameter()
{
	SetMeowClassName("XML/PageParameter");
}
Meow::XML::PageParameter::~PageParameter()
{

}
Meow::XML::PageEvent::PageEvent()
{
	SetMeowClassName("XML/PageEvent");

	iParameters=new Core::Types::Array();

}
Meow::XML::PageEvent::~PageEvent()
{

}
Meow::XML::PageDiv::PageDiv()
{
	SetMeowClassName("XML/PageDiv");
	iSecurity=new Core::Types::Array();
	iRefresh=new Core::Types::Boolean(false);
	iShowBusy=false;
	iHandler="gMeowGlobal.HTMLHandler";
}
Meow::XML::PageDiv::~PageDiv()
{
}

Meow::XML::PageParser::PageParser() :
		iHTTPS(false),
		iHTTP(false)
{
	SetMeowClassName("XML/PageParser");
	iElementStack=new Core::Types::Stack();
	iResources=new Core::Types::Array();
	iStyles=new Core::Types::Array();
	iPrintStyles=new Core::Types::Array();
	iHeaderLines=new Core::Types::Array();
	iGroups=new Core::Types::Array();
	iParameters=new Core::Types::Array();
	iOnLoadEvents=new Core::Types::Array();
	iLoadedEvents=new Core::Types::Array();
	iDivs=new Core::Types::Array();
	iContent=new Core::Types::Array();
	iMetas=new Core::Types::Array();
}
Meow::XML::PageParser::~PageParser()
{

}
void Meow::XML::PageParser::StartElement( string element, Core::Types::Array_ptr attributes )
{
	Meow::Core::Tools::XMLElement_ptr currentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Top();
	if ( element == "event" )
	{
		// Create a new event
		PageEvent_ptr event=new PageEvent();

		if ( attributes->IsSet("name") )
			event->eventName=((Core::Types::ValueType_ptr)attributes->Get("name"))->GetValueAsString();
		// Now lets study the parent to figure out where we are
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		if ( parentElement->elementName == "load" )
			iOnLoadEvents->Set(iOnLoadEvents->Count(),(Core::ptr)event);
		else if ( parentElement->elementName == "loaded" )
			iLoadedEvents->Set(iLoadedEvents->Count(),(Core::ptr)event);
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	}
	else if ( element == "div" )
	{
		PageDiv_ptr div=new PageDiv();
		std::string id="";
		if ( attributes->IsSet("id") )
					id=((Core::Types::ValueType_ptr)attributes->Get("id"))->GetValueAsString();
		iDivs->Set(id,(Core::ptr)div);
	}

}
void Meow::XML::PageParser::EndElement( string element )
{
	Meow::Core::Tools::XMLElement_ptr currentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Top();

	if ( element == "javascript" )
		iResources->Set(iResources->Count(),new Core::Types::String(currentElement->characterData));
	else if ( element == "css" )
		iStyles->Set(iStyles->Count(),new Core::Types::String(currentElement->characterData));
	else if ( element == "printcss" )
		iPrintStyles->Set(iStyles->Count(),new Core::Types::String(currentElement->characterData));

	else if ( element == "title" )
		iTitle=currentElement->characterData;
	else if ( element == "headerline" )
		iHeaderLines->Set(iHeaderLines->Count(),new Core::Types::String(currentElement->characterData));
	else if ( element == "substitute" )
	{
		PageParameter_ptr parameterDefinition=new PageParameter();
		if ( currentElement->elementAttributes->IsSet("type") )
				parameterDefinition->iType=((Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("type"))->GetValueAsString();
		if ( currentElement->elementAttributes->IsSet("name") )
				parameterDefinition->iName=((Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("name"))->GetValueAsString();
		parameterDefinition->iDefaultValue=currentElement->characterData;
		// Lets see if we have a session
		Session_ptr session=(Session_ptr)Registry::GetRegistry()->GetCurrentSession();
		if ( !session.IsNull() )
		{
			Core::Types::ValueType_ptr value=(Core::Types::ValueType_ptr)session->GetVariable(parameterDefinition->iName);
			if ( !value.IsNull() )
				parameterDefinition->iDefaultValue=value->GetValueAsString();
		}
		iParameters->Set(parameterDefinition->iName,(Core::ptr)parameterDefinition);
	}
	else if ( element == "parameter")
	{
		// We need to see what the previous one in the stack is
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		// Now lets study the parent to figure out where we are

		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		if ( parentElement->elementName == "parameters" )
		{
			// Its a page parameter
			PageParameter_ptr parameterDefinition=new PageParameter();
			if ( currentElement->elementAttributes->IsSet("type") )
				parameterDefinition->iType=((Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("type"))->GetValueAsString();
						if ( currentElement->elementAttributes->IsSet("regexp") )
										parameterDefinition->iRegularExpression=((Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("regexp"))->GetValueAsString();
						if ( currentElement->elementAttributes->IsSet("name") )
													parameterDefinition->iName=((Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("name"))->GetValueAsString();
						parameterDefinition->iDefaultValue=currentElement->characterData;
			iParameters->Set(parameterDefinition->iName,(Core::ptr)parameterDefinition);
		}
		else if ( parentElement->elementName == "event" )
		{
			// Its an event, we need to investigate the parent of the parent!
			Meow::Core::Tools::XMLElement_ptr grandParentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
			PageParameter_ptr parameterDefinition=new PageParameter();
			/* std::string elementName;
				Core::Types::Array_ptr elementAttributes;
				std::string characterData; */
			if ( currentElement->elementAttributes->IsSet("type") )
				parameterDefinition->iType=((Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("type"))->GetValueAsString();
			if ( currentElement->elementAttributes->IsSet("regexp") )
							parameterDefinition->iRegularExpression=((Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("regexp"))->GetValueAsString();
			if ( currentElement->elementAttributes->IsSet("name") )
										parameterDefinition->iName=((Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("name"))->GetValueAsString();
			parameterDefinition->iDefaultValue=currentElement->characterData;
			if ( grandParentElement->elementName == "load" )
			{
				PageEvent_ptr ev=(PageEvent_ptr)iOnLoadEvents->Get(iOnLoadEvents->Count()-1);
				ev->iParameters->Set(ev->Count(),(Core::ptr)parameterDefinition);
			}
			else if ( grandParentElement->elementName == "loaded" )
			{
				PageEvent_ptr ev=(PageEvent_ptr)iLoadedEvents->Get(iLoadedEvents->Count()-1);
				ev->iParameters->Set(ev->Count(),(Core::ptr)parameterDefinition);
			}
			GetElementStack()->Push((Core::ptr)grandParentElement);
		}
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	}
	else if ( element == "http" )
		iHTTP=true;
	else if ( element == "https" )
		iHTTPS=true;
	else if ( element == "group" )
	{
		// There are two declarations possible here
		// 1. Part of a div declaration ( <div/security/group>)
		// 2. Part of a header declaration header/security/group
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr grandParentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		if ( grandParentElement->elementName== "div" )
		{
			// We need to know the id of the div to add our security to it
			Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)grandParentElement->elementAttributes->Get("id");
			PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
			div->iSecurity->Set(div->iSecurity->Count(),new Core::Types::String(currentElement->characterData));
		}
		else if ( grandParentElement->elementName== "header" )
		{
			iGroups->Set(iGroups->Count(),new Core::Types::String(currentElement->characterData));
		}
		GetElementStack()->Push((Core::ptr)grandParentElement);
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	}
	else if ( element == "component" )
	{
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		// We need to find out which div to set this on
		Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)parentElement->elementAttributes->Get("id");
		PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
		div->iComponent=currentElement->characterData;
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	}
	else if ( element == "key" )
	{
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		// We need to find out which div to set this on
		Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)parentElement->elementAttributes->Get("id");
		PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
		div->iKey=currentElement->characterData;
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	}
	else if ( element == "method" )
	{
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		// We need to find out which div to set this on
		Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)parentElement->elementAttributes->Get("id");
		PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
		div->iMethod=currentElement->characterData;
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	}
	else if ( element == "arguments" )
	{
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		// We need to find out which div to set this on
		Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)parentElement->elementAttributes->Get("id");
		PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
		div->iParameters=currentElement->characterData;
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	}
	else if ( element == "refresh" )
	{
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		// We need to find out which div to set this on
		Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)parentElement->elementAttributes->Get("id");
		PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
		div->iRefresh=new Core::Types::String(currentElement->characterData);
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	} 
	else if ( element == "leadtime" )
    {
        // DS - 23/01/2018 - add in lead time.
        // iLeadTime
        Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		// We need to find out which div to set this on
		Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)parentElement->elementAttributes->Get("id");
		PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
		div->iLeadTime=currentElement->characterData;
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
    }
	else if ( element == "showBusy" )
	{
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		// We need to find out which div to set this on
		Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)parentElement->elementAttributes->Get("id");
		PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
		div->iShowBusy=true;
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	}
	else if ( element == "handler" )
	{
		Meow::Core::ptr currentElementPtr=GetElementStack()->Pop();
		Meow::Core::Tools::XMLElement_ptr parentElement=(Meow::Core::Tools::XMLElement_ptr)GetElementStack()->Pop();
		// We need to find out which div to set this on
		Core::Types::ValueType_ptr divName=(Core::Types::ValueType_ptr)parentElement->elementAttributes->Get("id");
		PageDiv_ptr div=(PageDiv_ptr)iDivs->Get(divName->GetValueAsString());
		div->iHandler=currentElement->characterData;
		GetElementStack()->Push((Core::ptr)parentElement);
		GetElementStack()->Push((Core::ptr)currentElementPtr);
	} 
	else if ( element == "content" )
	{
//		iContent=currentElement->characterData;
		PageContent_ptr content=new PageContent();
		content->iContent=currentElement->characterData;
		if ( currentElement->elementAttributes->IsSet("security") )
		{
			Core::Types::String_ptr security=(Core::Types::String_ptr)currentElement->elementAttributes->Get("security");
			content->iSecurity=security->Explode("|");
		}
		iContent->Set(iContent->Count(),(Core::ptr)content);
	}
	else if ( element == "meta" )
	{
		
		Core::Types::ValueType_ptr metaName=(Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("name");
		Core::Types::ValueType_ptr metaContent=(Core::Types::ValueType_ptr)currentElement->elementAttributes->Get("content");
		// <meta name="theme-color" content="#999999" />
		iMetas->Set(metaName->GetValueAsString(),(Core::ptr)metaContent);
	}
}
void Meow::XML::PageParser::CharacterData( string data )
{

}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetResources()
{
	return iResources;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetStyles()
{
	return iStyles;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetPrintStyles()
{
	return iPrintStyles;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetHeaderLines()
{
	return iHeaderLines;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetGroups()
{
	return iGroups;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetParameters()
{
	return iParameters;
}
std::string Meow::XML::PageParser::GetTitle()
{
	return iTitle;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetOnLoadEvents()
{
	return iOnLoadEvents;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetLoadedEvents()
{
	return iLoadedEvents;
}
bool Meow::XML::PageParser::GetHTTPS()
{
	return iHTTPS;
}
bool Meow::XML::PageParser::GetHTTP()
{
	return iHTTP;
}
// std::string  Meow::XML::PageParser::GetContent()
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetContent()
{
	return iContent;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetDivs()
{
	return iDivs;
}
Meow::Core::Types::Array_ptr Meow::XML::PageParser::GetMetas()
{
	return iMetas;
}
Meow::XML::PageContent::PageContent()
{
	SetMeowClassName("XML/PageContent");
}
Meow::XML::PageContent::~PageContent()
{
	
}
