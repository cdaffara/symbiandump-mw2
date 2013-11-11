/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Log commands 
*
*/


#ifndef FETCHLOG_H
#define FETCHLOG_H

// INCLUDES
#include <e32debug.h>
#include <utf.h>

#ifdef _DEBUG

#define WLOG(a) RDebug::Print(_L(a))
#define WLOG1(a,b) RDebug::Print(_L(a),(b))
#define WLOG2(a,b,c) RDebug::Print(_L(a),(b),(c))
#define WLOG3(a,b,c,d) RDebug::Print(_L(a),(b),(c),(d))

#define HLOG(a)  RDebug::Print((a))
#define HLOG1(a, b)  RDebug::Print((a), (b))
#define HLOG2(a, b, c)  RDebug::Print((a), (b), (c))
#define HLOG3(a, b, c, d)  RDebug::Print((a), (b), (c), (d))
#define HLOG4(a, b, c, d, e)  RDebug::Print((a), (b), (c), (d), (e))
#define HLOG5(a, b, c, d, e, f)  RDebug::Print((a), (b), (c), (d), (e), (f))

#else 

#define WLOG(a)
#define WLOG1(a,b)
#define WLOG2(a,b,c)
#define WLOG3(a,b,c,d)

#define HLOG(a)
#define HLOG1(a, b)
#define HLOG2(a, b, c) 
#define HLOG3(a, b, c, d) 
#define HLOG4(a, b, c, d, e) 
#define HLOG5(a, b, c, d, e, f) 

#endif

#endif  // FETCHLOG_H

// End of File

