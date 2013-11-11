// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __CSIACONTENTHANDLER_H__
#define __CSIACONTENTHANDLER_H__

// System includes
//
#include <e32base.h>
#include <push/ccontenthandlerbase.h>

// Local includes
//
#include "pushcpoint.h"

// Constants
//
const TInt KNotifierBufferSize			= 100;
const TUid KUidPushSIAContentHandler	= { 0x101F3E62 };
_LIT(KSIAContentHandlerData,"application/vnd.wap.sia");

// Forward class declarations
//
class CWapPushMsgUtils;
class CCommsDatabase;

/** 
 * CSIAContentHandler Handles Session Initiation Application Content.
 *
 *	The SIA is outlined in WAP-189-PushOTA (p.16...) and provides a
 *	string listing the available connection points. Thus the body of the
 *	CPushMessage (in octets) must be parsed to retrieve this list, and
 *	then validate any connection points found. The first valid point will
 *	be used to open a connection via the Connection Manager. If no valid
 *	points exist then the handler simply quits.
 */ 
class CSIAContentHandler : public CContentHandlerBase
/**
@internalComponent
@released
*/
	{
public:	// Methods

	static CSIAContentHandler* NewL();

	virtual ~CSIAContentHandler();

private:	// Methods from CPushHandlerBase

	virtual void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);

	virtual void HandleMessageL(CPushMessage* aPushMsg);
	
	virtual void CancelHandleMessage();

	virtual void CPushHandlerBase_Reserved1();

	virtual void CPushHandlerBase_Reserved2();

private:	// Methods from CActive

	virtual void DoCancel();

	virtual void RunL();

	virtual TInt RunError(TInt aError);
	
private:	// Methods

	CSIAContentHandler();

	void ConstructL();

	void ParsePushMsgL();

	void OpenConnectionL();	

	void RequestConnectionL();

	void ObtainContactPointsListL();

	TBool RetrieveContactPoint();

	TBool ValidContactPointL();

private:	// Attributes

	CWapPushMsgUtils*	iWapPushUtils;

	CCommsDatabase*		iCommDb;
	
	//notifier

	RNotifier					iNotifier;

	TBuf8<KNotifierBufferSize>	iNotifierResponse;

	//class states
	enum TState {EParsing, ERequestingConn, EOpeningCOconn, EDone};

	//string listing all points available

	TPtrC8				iContactList;

	// current point being checked
	TPtrC8				iContactPoint;

	// the valid connection point passed to COMan
	TPushConnPoint		iConnPoint;
	
	}; 

#endif	// __CSIACONTENTHANDLER_H__
