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

#ifndef __CMULTIPARTMIXEDCONTENTHANDLER_H__
#define __CMULTIPARTMIXEDCONTENTHANDLER_H__

// System includes
//
#include <e32base.h>
#include "cmultipartiteratorbase.h"
#include <push/ccontenthandlerbase.h>
#include "CMultipartBinIterator.h"
#include <push/pushmessage.h>

// Local includes
//

// Constants
//
const TUid KUidPushMultiPartMixedContentHandler	= { 0x101F3E64 };
_LIT(KMultiPartMixedContentHandlerData, "application/vnd.wap.multipart.mixed||multipart/mixed");

/**
 * The MultiPart/Mixed Content Handler 
 *
 * Takes apart the mulitpart message, creating a new sub message from each part, then calls on
 * new Push Handlers to process each sub message.
 * 
 */ 
//##ModelId=3B659EE90241
class CMultiPartMixedContentHandler : public CContentHandlerBase
/**
@internalComponent
@released
*/
	{
public:	// Methods

	static CMultiPartMixedContentHandler* NewL();

	virtual ~CMultiPartMixedContentHandler();

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

	CMultiPartMixedContentHandler();

	void ConstructL();

	void LoadMultipartMsgL();

	void HandlePartL();

	void NextPartL();

private:	// Attributes

	enum TState {  ELoadMsgData, EHandlePart, ENextPart, EDone };

	CMultipartIteratorBase*		iMultiMessage;

	CContentHandlerBase*		iContentHandler;
	
	};


#endif	// __CMULTIPARTMIXEDCONTENTHANDLER_H__
