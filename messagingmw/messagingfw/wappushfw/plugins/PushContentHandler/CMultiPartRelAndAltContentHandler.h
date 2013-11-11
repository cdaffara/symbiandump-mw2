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

#ifndef __CMULTIPARTRELANDALTCONTENTHANDLER_H__
#define __CMULTIPARTRELANDALTCONTENTHANDLER_H__

// System includes
//
#include <e32base.h>
#include <push/pushmessage.h>
#include <push/ccontenthandlerbase.h>
#include "CMultipartBinIterator.h"

// Local includes
//

// Constants
//
const TUid KUidPushMultiPartRelAndAltContentHandler	= { 0x101F3E65 };

// Forward class declarations
//
class CWapPushMsgUtils;

/**
 * The MultiPart/Alternative & Multipart/Alternative Content Handler 
 *
 * Saves the whole message in the Message Server, each part getting saved as a child
 * entry to the main part.
 * 
 */ 
//##ModelId=3B659EE400DB
class CMultiPartRelAndAltContentHandler : public CContentHandlerBase
/**
@internalComponent
@released
*/
	{
public:	// Methods

	static CMultiPartRelAndAltContentHandler* NewL();

	virtual ~CMultiPartRelAndAltContentHandler();

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

	CMultiPartRelAndAltContentHandler();

	void ConstructL();

	void LoadMultiPartMsgL();
	
	void HandlePartL();

	void NextPartL();

	void SaveMsgRootL();

	void LoadMsgDataL();

	void SetMsgCorruptFlagL();

private:	// Attributes

	enum TState { ELoadMsgData, EHandlePart, EDone, ENextPart };

	RFs						iFs;

	TInt					iState; //state machine

	TMsvId					iMsgEntryId;

	CWapPushMsgUtils*		iPushUtilities;

	CMultipartIteratorBase*	iMultiMessage;
	
	};


#endif    // __CMULTIPARTRELANDALTCONTENTHANDLER_H__
