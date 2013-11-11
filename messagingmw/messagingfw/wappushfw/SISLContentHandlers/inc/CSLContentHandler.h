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

#ifndef __CSLCONTENTHANDLER_H__
#define __CSLCONTENTHANDLER_H__

// System includes
//
#include <e32base.h>
#include <push/ccontenthandlerbase.h>

#include  <push/sislpushmsgutils.h>
#include "CMsgParser.h"
// Constants
//
const TUid KUidPushSLContentHandler	= { 0x101F3E63 };
_LIT(KSLContentHandlerData,"text/vnd.wap.sl||application/vnd.wap.slc");

// Forward class declarations
//
class CSLPushMsgEntry;

/** 
 * CSLContentHandler Handles Service Loading Content  
 *
 */ 
//##ModelId=3B659DA80207
class CSLContentHandler : public CContentHandlerBase, public MWAPElementHandler
/**
@internalComponent
@released
*/
	{
public:	// Methods

	static CSLContentHandler* NewL();

	virtual ~CSLContentHandler();

private:	// Methods from CPushHandlerBase
	
	virtual void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);

	virtual void HandleMessageL(CPushMessage* aPushMsg);

	virtual void CancelHandleMessage();

	virtual void CPushHandlerBase_Reserved1();

	virtual void CPushHandlerBase_Reserved2();
private:
	// From MWAPElementHandler
	void HandleElementL ( const RString& aTag, const RString& aAttributeName, const RString& aAttributeValue );
	void HandleContentL ( const TDesC8& aBytes );

private:	// Methods from CActive

	virtual void DoCancel();

	virtual void RunL();

	virtual TInt RunError(TInt aError);

private:	// Methods

	CSLContentHandler();

	void ConstructL();

	void ParsePushMsgL();

	void SetSlPushMsgEntryFieldsL(CSLPushMsgEntry& aSlPushMsgEntry);

	void SearchingForDuplicatesMsgEntryL();

	void ProcessingPushMsgEntryL();

private:	// Attributes

	enum TState { EParsing, ESearching, EProcessing, EDone };

	CSISLPushMsgUtils*	iWapPushUtils;

	TInt				iPushMsgAction;

	HBufC*				iHrefBuf;

	TMsvId				iSlMsgEntryId;

	};

#endif	// __CSLCONTENTHANDLER_H__
