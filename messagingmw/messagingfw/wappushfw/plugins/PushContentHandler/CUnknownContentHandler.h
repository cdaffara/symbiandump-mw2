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

#ifndef __CUNKNOWNCONTENTHANDLER_H__
#define __CUNKNOWNCONTENTHANDLER_H__

// System includes
//
#include <e32base.h>
#include <csmsgetdetdescinterface.h>
#include <push/ccontenthandlerbase.h>

// Local includes
//


// Contants
//
const TUid KUidPushUnknownContentHandler	= { 0x101F3E60 };
_LIT(KUnknownContentHandlerData, "*");

// Forward class declarations
//
class CSmsMessage;
class CUnknownPushMsgEntry;
class CWapPushMsgUtils;
class TWspPrimitiveDecoder;

/**
 * CUnknownContentHandler provides the default plugin when the 
 * specific plugin for a data type cannot be matched: 
 */ 
//##ModelId=3B659DB203A2
class CUnknownContentHandler : public CContentHandlerBase
/**
@publishedPartner
@released
*/
	{
public:

	static CUnknownContentHandler* NewL();

	virtual ~CUnknownContentHandler();

private:	// Methods from CPushHandlerBase

	virtual void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);

	virtual void HandleMessageL(CPushMessage* aPushMsg);

	virtual void  CancelHandleMessage();

	virtual void CPushHandlerBase_Reserved1();

	virtual void CPushHandlerBase_Reserved2();
	
private: // Methods from CActive

	virtual void DoCancel();

	virtual void RunL();

	virtual TInt RunError(TInt aError);

private:

	CUnknownContentHandler();

	void ConstructL();

	void  ProcessingPushMsgEntryL();

	void  StoreMsgL(CSmsMessage* aSmsMsg);

	void  SaveUnknownPushMsgEntryL();

	void  SetUnknownPushMsgEntryFieldsL(CUnknownPushMsgEntry& aUnknownPushMsgEntry);


	TMsvId SetBioServiceIdL();

	TBool BioMessageTypeL();

	void  SaveBioMessageEntryL();

	TBool GetContentFileNameL(TFileName& aFilename);
	
	TBool DecodeWAPContentDispositionHeaderL(TFileName& aFilename, const TDesC8& aFieldValue);
	
	TBool DecodeWAPContentDispositionParameterL(TWspPrimitiveDecoder& aDecoder, TFileName& aFilename, TInt& aBytesRead);

	TBool DecodeWAPContentDispositionTypedParameterL(TWspPrimitiveDecoder& aDecoder, TFileName& aFilename, TInt& aBytesRead);

	TBool DecodeWAPContentDispositionUntypedParameterL(TWspPrimitiveDecoder& aDecoder, TFileName& aFilename, TInt& aBytesRead);
private:

	TUid				iBioMsgUID;

	CWapPushMsgUtils*	iWapPushUtils;

	enum TState { EProcessing, EDone };
	
	CSmsGetDetDescInterface* iSmsDetDesc;

	};

#endif	// __CUNKNOWNCONTENTHANDLER_H__

