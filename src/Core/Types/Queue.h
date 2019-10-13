// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Renamed SetClassName and GetClassName to SetMeowClassName and GetMeowClassName
//		to avoid collision with Visual C++ when used with Win32.
#ifndef _CORE_QUEUE_H_
#define _CORE_QUEUE_H_
#include <Core/Object.h>

namespace Meow {
	namespace Core {
		namespace Types {
			class QueueElement;
			typedef Reference<QueueElement> QueueElement_ptr;

			class QueueElement : public Object
			{
			public:
				QueueElement();
				~QueueElement();
				ptr Item;
				QueueElement_ptr Next;
			};

			class QueueIterater;

			class Queue : public Object
			{
			public:
				Queue();
				~Queue();

				void Push( ptr item );
				void Push( string data );
				ptr Pull();
				ptr First();
				void Clear();
				bool IsEmpty();
				unsigned long  Count();

			private:
				friend class QueueIterater;

				QueueElement_ptr TopObject;
				QueueElement_ptr BottomObject;
				unsigned long itemcount;
			};
			typedef Reference<Queue> Queue_ptr;

			class QueueIterater : public Object
			{
			public:
				QueueIterater();
				~QueueIterater();

				void SetQueue( Queue_ptr queue );

				ptr First( );
				ptr GetNextElement( );
				bool HasMoreElements( );

			private:
				Queue_ptr Queue;
				unsigned long elementposition;
			};
			typedef Reference<QueueIterater> QueueIterater_ptr;
		}
	}
} // End of namespace
#endif
