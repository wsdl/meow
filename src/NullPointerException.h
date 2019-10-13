/* Copyright     :  (c) 2007,2008 David Spencer. All Rights Reserved.
 * Author        :  David Spencer
 */

#ifndef _NULLPOINTEREXCEPTION_H_
#define _NULLPOINTEREXCEPTION_H_
#include <SystemException.h>

class NullPointerException : public SystemException
{
public:
    inline NullPointerException() { };
    inline ~NullPointerException() { };

};

#endif
