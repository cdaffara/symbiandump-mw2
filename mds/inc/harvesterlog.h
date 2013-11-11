/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*/


#ifndef HARVESTERLOG_H
#define HARVESTERLOG_H

#include <e32debug.h>
#include <utf.h>

#define HARVESTER_MASTER_LOG

#ifdef _DEBUG

#ifdef HARVESTER_MASTER_LOG

#define WRITELOG(a) RDebug::Print(_L(a))
#define WRITELOG1(a,b) RDebug::Print(_L(a),(b))
#define WRITELOG2(a,b,c) RDebug::Print(_L(a),(b),(c))
#define WRITELOG3(a,b,c,d) RDebug::Print(_L(a),(b),(c),(d))

#define HLOG(a)  RDebug::Print((a))
#define HLOG1(a, b)  RDebug::Print((a), (b))
#define HLOG2(a, b, c)  RDebug::Print((a), (b), (c))
#define HLOG3(a, b, c, d)  RDebug::Print((a), (b), (c), (d))
#define HLOG4(a, b, c, d, e)  RDebug::Print((a), (b), (c), (d), (e))
#define HLOG5(a, b, c, d, e, f)  RDebug::Print((a), (b), (c), (d), (e), (f))

#else //HARVESTER_MASTER_LOG

#define WRITELOG(a)
#define WRITELOG1(a,b)
#define WRITELOG2(a,b,c)
#define WRITELOG3(a,b,c,d)

#define HLOG(a)
#define HLOG1(a, b)
#define HLOG2(a, b, c) 
#define HLOG3(a, b, c, d) 
#define HLOG4(a, b, c, d, e) 
#define HLOG5(a, b, c, d, e, f) 

#endif //HARVESTER_MASTER_LOG

#else  //_DEBUG

#define WRITELOG(a)
#define WRITELOG1(a,b)
#define WRITELOG2(a,b,c)
#define WRITELOG3(a,b,c,d)

#define HLOG(a)
#define HLOG1(a, b)
#define HLOG2(a, b, c) 
#define HLOG3(a, b, c, d) 
#define HLOG4(a, b, c, d, e) 
#define HLOG5(a, b, c, d, e, f) 

#endif //_DEBUG

#endif  // HARVESTERLOG_H
