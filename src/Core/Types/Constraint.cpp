// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#include <Core/Types/Constraint.h>
#ifdef WIN32
	#include <tre.h>
	/* #define tre_regfree regfree
	#define tre_regcomp regcomp
	#define tre_regexec regexec
	#define REG_OK REG_NOERROR */
#else
	#include <tre/tre.h>
#endif
#include <Core/Log.h>
Meow::Core::Types::Constraint::Constraint( std::string regexp )
{
    SetMeowClassName("Constraint");
    iExpression=NULL;
    if ( !regexp.compare("string") )
        regexp=CONSTRAINT_STRING;
    else if ( !regexp.compare("int") )
        regexp=CONSTRAINT_INTEGER;
    else if ( !regexp.compare("integer") )
        regexp=CONSTRAINT_INTEGER;
    else if ( !regexp.compare("float") )
        regexp=CONSTRAINT_FLOAT;
    else if ( !regexp.compare("date") )
        regexp=CONSTRAINT_DATE;
    else if ( !regexp.compare("datetime") )
        regexp=CONSTRAINT_DATETIME;
    else if ( !regexp.compare("time") )
        regexp=CONSTRAINT_TIME;
    else if ( !regexp.compare("boolean") )
        regexp=CONSTRAINT_BOOLEAN;
    else if ( !regexp.compare("email") )
        regexp=CONSTRAINT_EMAIL;
    else if ( !regexp.compare("url") )
        regexp=CONSTRAINT_URL;
    else if ( !regexp.compare("phone") )
        regexp=CONSTRAINT_PHONE;
    else if ( !regexp.compare("postcode") )
        regexp=CONSTRAINT_POSTCODE;
    else if ( !regexp.compare("ocodeurl") )
        regexp=CONSTRAINT_OCODEURL;
    else if ( !regexp.compare("ocodekey") )
        regexp=CONSTRAINT_OCODEKEY;
	else if ( !regexp.compare("password") )
        regexp=CONSTRAINT_PASSWORD;

    SetExpression(regexp);
}
Meow::Core::Types::Constraint::Constraint()
{
    SetMeowClassName("Constraint");
    iExpression=NULL;
}
Meow::Core::Types::Constraint::~Constraint()
{
    if ( iExpression!=NULL )
    {
        regex_t *pattern=(regex_t*)iExpression;
        tre_regfree(pattern);
        free(pattern);
        iExpression=NULL;
    }
}
bool Meow::Core::Types::Constraint::Test( Meow::Core::Types::ValueType_ptr other )
{
    if ( iExpression==NULL )
    {
	Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sss",
                                   "Can't test [",
                                   other->GetValueAsString().c_str(),
                                   "] because no expression set to test against.");
        return false;
    }
    regex_t *pattern=(regex_t*)iExpression;
    int rc=REG_OK;
    rc=tre_regexec(pattern,other->GetValueAsString().c_str(),0,NULL,0);
    switch ( rc )
    {
    case REG_NOMATCH:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "No match found.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_BADPAT:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Invalid regular expression.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_ECOLLATE:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Unknown collecting element.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_ECTYPE:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Unknown character class name.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_EESCAPE:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Trailing backslash in expression.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_ESUBREG:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Invalid back reference.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_EBRACK:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Imbalance of [ and ] in expression.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_EPAREN:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Impalance of ( and ) in expression.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_EBRACE:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Impalance of { and } in expression.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;
    case REG_ESPACE:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "sssss",
                                   "Out of memory.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;

    case REG_BADRPT:
        Log::GetLog()->Error("Meow::Constraint::Test",
                                   "s",
                                   "Bad use of repetition operators.[",
                                   iOriginal.c_str(),
                                   "]=[",
                                   other->GetValueAsString().c_str(),
                                   "]");
        break;

    }
    return rc==REG_OK;
}
std::string Meow::Core::Types::Constraint::GetExpression()
{
    return iOriginal;
}
void Meow::Core::Types::Constraint::SetExpression( std::string regexp )
{
    if (iExpression!=NULL )
        free(iExpression);

    iExpression=malloc(sizeof(regex_t));
    regex_t * pattern=(regex_t*)iExpression;
    int rc=tre_regcomp(pattern,regexp.c_str(),REG_EXTENDED);
    if ( rc!=REG_OK )
    {
        free(iExpression);
        iExpression=NULL;
        switch ( rc )
        {
        case REG_NOMATCH:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "No match found.:",
										regexp.c_str());
            break;
        case REG_BADPAT:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Invalid regular expression:",
										regexp.c_str());
            break;
        case REG_ECOLLATE:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Unknown collecting element:",
										regexp.c_str());
            break;
        case REG_ECTYPE:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Unknown character class name:",
										regexp.c_str());
            break;
        case REG_EESCAPE:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Trailing backslash in expression:",
										regexp.c_str());
            break;
        case REG_ESUBREG:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Invalid back reference:",
										regexp.c_str());
            break;
        case REG_EBRACK:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Imbalance of [ and ] in expression:",
										regexp.c_str());
            break;
        case REG_EPAREN:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Impalance of ( and ) in expression:",
										regexp.c_str());
            break;
        case REG_EBRACE:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Impalance of { and } in expression:",
										regexp.c_str());
            break;
            /*case REG_EBADBR:
            	CoreLog::GetLog()->Error("ValueType::MatchesRegularExpression",
            		"s",
            		"Invalid content in { and }.");
            	break; */
        case REG_ESPACE:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Out of memory:",
										regexp.c_str());
            break;
        case REG_BADRPT:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Bad use of repetition operators:",
										regexp.c_str());
            break;
 	case REG_ERANGE:
            Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                       "ss",
                                       "Bad use of range operator:",
										regexp.c_str());
            break;
	default:
  		Log::GetLog()->Error("Meow::Constraint::SetExpression",
                                   "sd",
                                   "Unknown error:",
                                   rc);
        }
    }
    else
        iOriginal=regexp;
}
