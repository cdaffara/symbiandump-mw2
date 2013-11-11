// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The main SUPL WAP Push Plug-in class. It implements the ECOM interface, listens for the incoming messages,
// and notifies the SPM via the SUPL Push API.
// 
//

#ifndef __LBSSUPLWAPPUSH_H__
#define __LBSSUPLWAPPUSH_H__

/**
@file
@internalComponent
@prototype
*/

#include <e32base.h>
#include <lbs/lbssuplpush.h>
#include <push/ccontenthandlerbase.h>


/**
The main SUPL WAP Push Plug-in class. It implements the ECOM interface, listens for the incoming 
messages, and notifies the SPM via the SUPL Push API.
 
@see CContentHandlerBase
*/
NONSHARABLE_CLASS(CLbsSuplWapPush) : public CContentHandlerBase, public MLbsSuplPushObserver
	{
public:
	static CLbsSuplWapPush* NewL();
	virtual ~CLbsSuplWapPush();
	
	virtual void OnSuplInitComplete(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, 
			TInt aError, TInt aReserved);
	
	// Methods from CPushHandlerBase
	virtual void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);
	virtual void HandleMessageL(CPushMessage* aPushMsg);
	virtual void CancelHandleMessage();
	virtual void CPushHandlerBase_Reserved1();
	virtual void CPushHandlerBase_Reserved2();

#ifndef TE_UNITTESTMODE_ON	
private:
#endif //TE_UNITTESTMODE_ON	

	CLbsSuplWapPush();
	void ConstructL();
	
	// Methods from CActive
	virtual void DoCancel();
	virtual void RunL();
	
private:
	/** The SUPL Push API */
	CLbsSuplPush* iSuplPush;
	}; 
	
#endif //__LBSSUPLWAPPUSH_H__

