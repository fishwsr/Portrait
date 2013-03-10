#pragma once

#ifndef __GXCOMMON_H
#define __GXCOMMON_H

/*/////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2000 Microsoft Corporation.  All Rights Reserved.
//
//  File: gxcommon.h
//
//
/////////////////////////////////////////////////////////////////////////////*/

#include <wtypes.h>
#include <basetsd.h>

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef TRUE
    #define TRUE 1
#endif

typedef char                INT8;
typedef unsigned char       UINT8;
typedef short               INT16;
typedef unsigned short      UINT16;
typedef int                 INT32;
typedef unsigned int        UINT32;
typedef __int64             INT64;
typedef unsigned __int64    UINT64;

#define GXMEMALLOC(__nBytes__) HeapAlloc(GetProcessHeap(), 0, __nBytes__)
#define GXMEMFREE(__p__) HeapFree(GetProcessHeap(), 0, __p__)

#ifndef MAKE_USERERROR
    #define MAKE_USERERROR(code)    MAKE_HRESULT(1,FACILITY_ITF,code)
#endif
#ifndef E_NOTINITIALIZED
    #define E_NOTINITIALIZED        MAKE_USERERROR(0xFFFC)
#endif
#ifndef E_ALREADYINITIALIZED
    #define E_ALREADYINITIALIZED    MAKE_USERERROR(0xFFFB)
#endif
#ifndef E_NOTFOUND
    #define E_NOTFOUND              MAKE_USERERROR(0xFFFA)
#endif
#ifndef E_INSUFFICIENTDATA
    #define E_INSUFFICIENTDATA      MAKE_USERERROR(0xFFF9)
#endif


#endif  // #ifndef __GXCOMMON_H
