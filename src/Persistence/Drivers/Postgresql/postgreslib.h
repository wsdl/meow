/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */
#ifndef _PGSQLLIB_H_
#define _PGSQLLIB_H_
#include <Persistence/DatabaseDriver.h>
#include <Exceptions/NotFound.h>
extern "C" {
Meow::Persistence::DatabaseDriver_ptr GetInstance( Meow::Registry_ptr registry ) throw (Meow::Exceptions::NotFound);
}

#endif
