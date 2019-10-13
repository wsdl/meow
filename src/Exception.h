/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _MEOW_EXCEPTION_H_
#define _MEOW_EXCEPTION_H_
#include <Core/Object.h>
#define MEOW_EXCEPTION_MESSAGE_MAX 32

namespace Meow
{
	class Object;
	typedef Reference<Object> Object_ptr;

	class Exception
	{
	public:
		Exception();
		~Exception();
		void SetMeowClassName( std::string classname );
		std::string GetMeowClassName();
		void SetMessage( std::string message );
		bool HasMoreMessages( );
		std::string GetNextMessage( );
		std::string Diagnostic();
	protected:
		std::string ClassName;
		std::string Messages[MEOW_EXCEPTION_MESSAGE_MAX];
		short MessageStackPtr;
	};
}
#endif
