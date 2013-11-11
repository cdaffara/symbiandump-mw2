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

#ifndef __CUNKNOWNAPPHANDLER_H__
#define __CUNKNOWNAPPHANDLER_H__

// System includes
#include <e32base.h>
#include <push/cpushhandlerbase.h>

// Local includes

// Constants
//
const TUid KUidPushUnknownAppHandler	= { 0x101F3E5C };
_LIT(KPushUnknownHandlerData,"*");

/** CUnknownAppHandler is used to handle all the Push Messages with
 *  unknown AppID.  
 * It provides the default App plugin handler when the 
 * specific plugin handler for a data type cannot be matched: 
 */ 
//##ModelId=3B659E6E01A4
class CUnknownAppHandler : public CPushHandlerBase
/**
@internalComponent
@released
*/
	{
public:	// Methods

	//##ModelId=3B659E6E01DA
	static CUnknownAppHandler* NewL();

	//##ModelId=3B659E6E01CE
	virtual ~CUnknownAppHandler();


private:	// Methods from CPushHandlerBase

	//##ModelId=3B659E6E01C2
	virtual void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);

	//##ModelId=3B659E6E01BE
	virtual void CancelHandleMessage();

	//##ModelId=3B659E6E01C5
	virtual void HandleMessageL(CPushMessage* aPushMsg);

	virtual void CPushHandlerBase_Reserved1();

	virtual void CPushHandlerBase_Reserved2();

protected:	// Methods from CActive

	//##ModelId=3B659E6E01BD
	void DoCancel();
	//##ModelId=3B659E6E01BC
	void RunL();

private:
	//##ModelId=3B659E6E01B8
	CUnknownAppHandler();

	//##ModelId=3B659E6E01AF
	void ConstructL();

	};

#endif    // __CUNKNOWNAPPHANDLER_H__

