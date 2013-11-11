// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// The main startup and exportable methods for this plug-in DLL
// NOTE : Required plugin DLL methods and ordinal numbers
//

// #1 CreatePluginManagerL
// #2 CreatePluginL
// #3 CreateActivePluginL
//


#include <e32def.h>
#include <e32std.h>
#include <e32base.h>
#include <push/cpushhandlerbase.h>
#include <ecom/implementationproxy.h>
//#include "PushLog.h"

#include "t_pushfilter.h"





#if defined(__DLL__) && !defined(EKA2)
/**
 *	DLL entry point
 */
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return(KErrNone);
	}

#elif !defined(EKA2)
/**
 *	Main startup entry point
 */
GLDEF_C static TInt E32Main()
	{
	return(KErrNone);
	}

#endif 

/**
 *	Panic the client program.
 */
GLDEF_C void Panic(TTestPushMessageFilterPanic aPanic)
	{
	_LIT(KTestFilterPlugin,"TestPushFilter");
	User::Panic(KTestFilterPlugin,aPanic);
	}


//----------------------------------------------------------------------------------------------------------
const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x101F3E6E,	CTestPushMessageFilter::NewL),
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}




//----------------------------------------------------------------------------------------------------------


CTestPushMessageFilter* CTestPushMessageFilter::NewL()
	{
	CTestPushMessageFilter* me= new (ELeave) CTestPushMessageFilter();
	return me;
	}



CTestPushMessageFilter::CTestPushMessageFilter() :
	CWapPushMessageFilter()
	{
	}

CTestPushMessageFilter::~CTestPushMessageFilter()
	{
	}
	
void CTestPushMessageFilter::FilterMessage(const CPushMessage& /*aPushMessage*/, TRequestStatus& aStatus, TBool& aAllowMessage)
	{
	// a real plugin would not complete the request straight away but might be an active object that starts some
	// intensive process. If this isn't required then simply completing the request straight away as below if fine.

	aAllowMessage=ETrue;
	iFilterReq=&aStatus;
	User::RequestComplete(iFilterReq, KErrNone);
	}

void CTestPushMessageFilter::CancelFilter()
	{
	if (iFilterReq != NULL)
		User::RequestComplete(iFilterReq, KErrCancel);
	}

// Constants
_LIT(KReserved, "Reserved");

void CTestPushMessageFilter::CWapPushMessageFilter_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CTestPushMessageFilter::CWapPushMessageFilter_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}



