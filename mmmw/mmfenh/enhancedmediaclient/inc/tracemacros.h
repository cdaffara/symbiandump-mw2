/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Debug traces marco Definitions.
*
*/


#ifndef TRACES_MACRO_H
#define TRACES_MACRO_H

// In debug binaries, print debug traces, else do nothing
#ifdef _DEBUG

#include <e32debug.h>

#define EMC_TRACE1(str) \
    RDebug::Print(str)
#define EMC_TRACE2(str,val1) \
    RDebug::Print(str,val1)
#define EMC_TRACE3(str,val1,val2) \
    RDebug::Print(str,val1,val2)
#define EMC_TRACE4(str,val1,val2,val3) \
    RDebug::Print(str,val1,val2,val3)
#define EMC_TRACE5(str,val1,val2,val3,val4) \
    RDebug::Print(str,val1,val2,val3,val4)
#define EMC_TRACE6(str,val1,val2,val3,val4,val5) \
    RDebug::Print(str,val1,val2,val3,val4,val5)
#else
#define EMC_TRACE1(str)
#define EMC_TRACE2(str,val1)
#define EMC_TRACE3(str,val1,val2)
#define EMC_TRACE4(str,val1,val2,val3)
#define EMC_TRACE5(str,val1,val2,val3,val4)
#define EMC_TRACE6(str,val1,val2,val3,val4,val5)
#endif // _DEBUG

#endif // TRACES_MACRO_H