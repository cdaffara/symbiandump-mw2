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

#ifndef __CROAPPHANDLER_H__
#define __CROAPPHANDLER_H__

// System includes
#include <e32base.h>
#include <push/cpushhandlerbase.h>

// Local includes


// Forward class declarations
//
class CContentHandlerBase;

// Constants
//
const TUid KUidROAppHandler	= { 0x101F3E67 };//Change UID
// Note:Changing the Order of the AppId may cause TMatchParams in the dispatcher not to find the right Id
_LIT(KROppHandlerName,"x-wap-application:drm.ua||0x00000008");
_LIT(KROAppHandlerData,"X-WAP-Application-ID");

/** 
 * CROAppHandler Handles ROAP Applications. 
 *
 */ 
//##ModelId=3B659E6D024D
class CROAppHandler : public CPushHandlerBase
/**
@internalComponent
@released
*/
	{
public:	// Methods

	//##ModelId=3B659E6D029F
	static CROAppHandler* NewL();

	//##ModelId=3B659E6D0294
	virtual ~CROAppHandler();

private:	// Methods from CPushHandlerBase

	//##ModelId=3B659E6D0278
	void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);

	//##ModelId=3B659E6D0277
	void CancelHandleMessage();

	//##ModelId=3B659E6D0281
	void HandleMessageL(CPushMessage* aPushMsg);

	virtual void CPushHandlerBase_Reserved1();

	virtual void CPushHandlerBase_Reserved2();

private:	// Methods from CActive

	//##ModelId=3B659E6D0276
	void DoCancel();

	//##ModelId=3B659E6D0275
	void RunL();

private:	// Methods

	//##ModelId=3B659E6D026C
	CROAppHandler();

	//##ModelId=3B659E6D026B
	void ConstructL();

private:	// Attributes

	//Note: iContentHandlerPi Pointer is Not Owned by this Class 
	//##ModelId=3B659E6D025A
	CContentHandlerBase*		iContentHandlerPi;
	
	};

#endif	// __CROAPPHANDLER_H__
