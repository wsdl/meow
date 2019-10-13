// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetClassName and GetClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#ifndef _CORE_STACK_H_
#define _CORE_STACK_H_
#include <Core/Object.h>
// DS - we need to use vectors here because we need an iterator.
#include <vector>
namespace Meow {
	namespace Core {
		namespace Types {
			class StackElement;
			typedef Reference<StackElement> StackElement_ptr;

			class StackIterater;
			class Stack;
			typedef Reference<Stack> Stack_ptr;

			class Stack : public Object
			{
			public:
				Stack();
				~Stack();

				void Push( Meow::Core::ptr item );
				Meow::Core::ptr Pop();
				Meow::Core::ptr Top();
				void Clear();
				bool IsEmpty();
				unsigned long Count();
				Meow::Core::Types::Stack_ptr Flip();
			private:
				friend class Meow::Core::Types::StackIterater;
				std::vector<Core::ptr> elements;
			};
			class StackIterater : public Object
			{
			public:
				StackIterater();
				~StackIterater();
				void SetStack( Meow::Core::Types::Stack_ptr stack );
				Meow::Core::ptr First( );
				Meow::Core::ptr GetNextElement( );
				bool HasMoreElements( );
			private:
				Stack_ptr stack;
				unsigned long elementposition;
			};
			typedef Reference<StackIterater> StackIterater_ptr;
		}
	}
} // End of namespace

#endif
