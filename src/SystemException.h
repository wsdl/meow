/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _SYSTEMEXCEPTION_H_
#define _SYSTEMEXCEPTION_H_
#include <SystemException.h>
#include <string>
#include <stdlib.h>
#define SYSTEM_EXCEPTION_MESSAGE_MAX 32

using namespace std;

class SystemException
{
public:
	inline SystemException() : MessageStackPtr(0), LineNumber(0),Initiator(NULL) { };
	inline ~SystemException() { };
	inline void SetMessage( string message )
	{
		if ( MessageStackPtr<SYSTEM_EXCEPTION_MESSAGE_MAX )
		{
			Messages[MessageStackPtr]=message;
			MessageStackPtr++;
		}
	}

	inline void SetLineNumber( short linenumber )
	{
		LineNumber=linenumber;
	}

	inline short GetLineNumber( )
	{
		return LineNumber;
	}

	inline void SetInitiator( void * object )
	{
		Initiator=object;
	}

	inline void * GetInitiator( )
	{
		return Initiator;
	}

	inline bool HasMoreMessages( )
	{
		return MessageStackPtr>0;
	}

	inline string GetNextMessage( )
	{
		string returnit;
		returnit=Messages[MessageStackPtr];
		if ( MessageStackPtr>0 )
			MessageStackPtr--;
		return returnit;
	}

	inline string Diagnostic()
	{
		std::string retval="";
		while ( HasMoreMessages() )
			retval+=GetNextMessage() + " ";
		return retval;
	}

	private:
		string Messages[SYSTEM_EXCEPTION_MESSAGE_MAX];
		short MessageStackPtr;
		short LineNumber;
		void * Initiator;
};

#endif
