// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// tpushhandler.cpp
// 
//

// System includes
#include <e32base.h>
#include <e32std.h>
#include <ecom/implementationproxy.h>
#include "pushdispatcher.h"
#include <push/pushlog.h>
#include <push/cpushhandlerbase.h>
#include <push/pluginkiller.h>

// Local includes

_LIT(KReserved, "Reserved");

class CTPushAppHandler : public CPushHandlerBase
	{
	public:
	static CTPushAppHandler* NewL ();
	virtual ~CTPushAppHandler();

	private:

	void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);
	void CancelHandleMessage();
	void HandleMessageL(CPushMessage* aPushMsg);
	virtual void CPushHandlerBase_Reserved1();
	virtual void CPushHandlerBase_Reserved2();
	private:
	void DoCancel();
	void RunL();

	private:
	CTPushAppHandler();
	};

CTPushAppHandler* CTPushAppHandler::NewL ()
	{
	CTPushAppHandler* self = new (ELeave)CTPushAppHandler;
	return self;
	}

CTPushAppHandler::CTPushAppHandler ()
: CPushHandlerBase()
	{
	CActiveScheduler::Add(this);
	}

CTPushAppHandler::~CTPushAppHandler ()
	{
	__LOG_PTR_DEBUG("CTPushAppHandler:: Destructor Called"); 
	}

void CTPushAppHandler::HandleMessageL( CPushMessage* /* aPushMsg */, TRequestStatus& /* aStatus */ )
	{
	// do nothing
	}

void CTPushAppHandler::CancelHandleMessage()
	{
	// do nothing
	}

void CTPushAppHandler::HandleMessageL ( CPushMessage* aPushMsg )
	{
	 __LOG_PTR_DEBUG("CTPushAppHandler::HandleMessageL Sync Func. Called"); 	

	// plugin takes the ownership of the push message.
	CleanupStack::PushL ( aPushMsg );

	// Leave from here
	User::Leave ( KErrGeneral );
	}

void CTPushAppHandler::DoCancel()
	{
	}

void CTPushAppHandler::RunL()
	{
	SignalConfirmationStatus ( KErrNone );
	iPluginKiller->KillPushPlugin ();
	}

void CTPushAppHandler::CPushHandlerBase_Reserved1()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

void CTPushAppHandler::CPushHandlerBase_Reserved2()
	{
	User::Panic(KReserved, KErrNotSupported);
	}

const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(0x102822AC,	CTPushAppHandler::NewL)
	};


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}



#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
//
// DLL entry point.
	{
	return(KErrNone);
	}
#endif
