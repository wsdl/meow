/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _MYSQLLIB_H_
#define _MYSQLLIB_H_
#include <Persistence/DatabaseDriver.h>
#include <Exceptions/NotFound.h>
extern "C" {
Meow::Persistence::DatabaseDriver_ptr GetInstance( Meow::Registry_ptr registry ) throw (Meow::Exceptions::NotFound);
}

#endif
