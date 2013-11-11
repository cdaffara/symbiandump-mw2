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
// A test proxy class for the CLbsSuplPush class
// 
//

#ifndef __TE_LBSSUPLPUSH_H__
#define __TE_LBSSUPLPUSH_H__

/**
@file
@internalComponent
@prototype
*/

#include <lbs/lbssuplpush.h>

/**
A test proxy class for the CLbsSuplPush class.
*/
class CTe_LbsSuplPush : public CBase, public MLbsSuplPushObserver
	{
private:
	/** A pointer on the external observer of the request status events */
	static MLbsSuplPushObserver* iTestObserver;	
	
public:
	static inline void SetTestObserver(MLbsSuplPushObserver* aTestObserver);
	static CTe_LbsSuplPush* NewL(TLbsSuplPushChannel aChannel, MLbsSuplPushObserver& aObserver);
	virtual ~CTe_LbsSuplPush();
	
	TInt SuplInit(TLbsSuplPushRequestId& aReqId, const TDesC8& aMsg,TInt aReserved);
	
private:
	CTe_LbsSuplPush(MLbsSuplPushObserver& aObserver);
	void ConstructL(TLbsSuplPushChannel aChannel);
	
	virtual void OnSuplInitComplete(TLbsSuplPushChannel aChannel, TLbsSuplPushRequestId aReqId, 
			TInt aError, TInt aReserved);
	
private:
	/** CLbsSuplPush* iImpl is the internal handle to the original object */
	CLbsSuplPush* iImpl;
	
	/** The observer of the request status events */
	MLbsSuplPushObserver& iObserver;
	};

/**
Sets the external observer of the request status events.

@param aTestObserver [In] A pointer on the observer to set.
*/
inline void CTe_LbsSuplPush::SetTestObserver(MLbsSuplPushObserver* aTestObserver)
	{
	iTestObserver = aTestObserver;
	}

#endif //__TE_LBSSUPLPUSH_H__
