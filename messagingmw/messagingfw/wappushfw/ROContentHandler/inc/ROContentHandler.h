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
//

#ifndef __ROCONTENTHANDLER_H__
#define __ROCONTENTHANDLER_H__

// System includes
//
#include <e32base.h>
#include <push/ccontenthandlerbase.h>
#include "MessageComposer.h"
// Local includes
//
#include "ROCHCAFInterface.h"

// Constants
//
const TUid KUidPushROContentHandler	= { 0x20002247 };
_LIT(KROContentHandlerData,"application/vnd.oma.drm.rights+xml||application/vnd.oma.drm.rights+wbxml");


/** 
 * CROContentHandler handles Rights Object content  
 *
 * This handler takes ownership of the RO message and processes it
 * 
 * OVERVIEW:
 *	The main body of this class and its functionality lies within
 *	HandleMessage (asynchonous). This :
 *		1. takes ownership of the push message
 *		2. processes the received message
 *		3. finishes
 */
 
class CROContentHandler : public CContentHandlerBase

/**
@internalComponent
@released
*/
	{
public:	// Methods

	static CROContentHandler* NewL();

	virtual ~CROContentHandler();

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

	CROContentHandler();

	void  ConstructL();
	
	void  ConvertPushMsgL();
	
	void SaveROToXMLFileL(); 
	
	//call to ROCHCAFInterface
	void  ProcessToCAFL();
	
	void DeleteTempROFile();


private:	// Attributes

	enum TState { EConverting, EStoring, ECAFInterface, EDeleteFile, EDone };
	
	RFs					iFs;

	TInt				iPushMsgAction;	

	HBufC8*				iData;
	
	CROCHCAFInterface*	iCAFInterface;
	
	};


#endif	// __CROContentHandler_H__
