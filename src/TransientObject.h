/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _TRANSIENTOBJECT_H_
#define _TRANSIENTOBJECT_H_
#include <LifecycleObject.h>
namespace Meow
{
	class TransientObject;
	typedef Reference<TransientObject> TransientObject_ptr;

	class TransientObject : public LifecycleObject
	{
	public:
		TransientObject();
		~TransientObject();

		virtual void    __Destroy( );
		virtual bool    IsPurgeable( );
		virtual void    Restore( ) ;
		virtual void    Passivate( );
		virtual void    _Delete( );
		virtual void    Create( );
	}; // End of Class TransientObject

} // End of NameSpace Meow

#endif
