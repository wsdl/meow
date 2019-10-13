// Copyright (c) 2008, 2009, 2010, 2011, 2012, 2013 David Spencer.
// All rights reserved.
// Version: 3.1
// Change History:
//	16/11/2013 @DavidSpencer
//		Carried out conformance to new naming strategy.
//		Carried out conformance to Object pattern.
#ifndef _CORE_DEBUG_H_
#define _CORE_DEBUG_H_

namespace Meow {
	namespace Core {
		class Object;
	}
}

typedef struct _DebugRecord {
	Meow::Core::Object * pointer;
	struct _DebugRecord * next;
} DebugRecord;

typedef struct _DebugRecordMain {
	unsigned long counter;
	DebugRecord * top;
	bool permit;
} DebugRecordMain;

void  GlobalInit( );

DebugRecordMain * GetGlobalActiveRecorder( );

void          AddGlobalActivePointer(
    Meow::Core::Object * pointer
);
void          RemoveFromGlobalActivePointer(
    Meow::Core::Object * pointer
);
void          CleanupGlobalActivePointer();

DebugRecordMain * GetGlobalInactiveRecorder( );

void          AddGlobalInactivePointer(
   Meow::Core::Object * pointer
);
void          RemoveFromGlobalInactivePointer(
   Meow::Core::Object * pointer
);
void          CleanupGlobalInactivePointer();

#endif
