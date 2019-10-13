#include <TransientObject.h>

Meow::TransientObject::TransientObject()
{
    SetMeowClassName("TransientObject");
}
Meow::TransientObject::~TransientObject()
{
}
void Meow::TransientObject::__Destroy( )
{
}
bool Meow::TransientObject::IsPurgeable( )
{
    return false;
}
void Meow::TransientObject::Restore( )
{
	// This is the only one we have to change the behaviour of as if the
	// object is not found in the live container it calls this to see if it can be restored
	// from a storage environment.
	Exceptions::NotFound nf;
	throw nf;
}
void Meow::TransientObject::Passivate( )
{
}
void Meow::TransientObject::_Delete( ) 
{
}
void Meow::TransientObject::Create( )
{
}

