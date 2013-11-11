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

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __CCONTENTHANDLERBASE_H__
#define __CCONTENTHANDLERBASE_H__

// System includes
//
#include <e32base.h>
#include <msvstd.h>
#include <push/cpushhandlerbase.h>
#include <push/pushmessage.h>


// Constants
//
/** ECom interface UID for WAP Push Content Handler plug-ins. */
const TUid KUidPushContentHandlerBase	= { 0x101F3E5E };


/** 
Abstract base class for WAP Push Content Handler plug-ins.

A WAP Push Content Handler plug-in processes a WAP Push message that contains 
data of a specific media type. A Content Handler plug-in is loaded by a WAP Push Application
Handler plug-in (CPushHandlerBase-derived object), and the message is passed to handle 
through HandleMessageL().

A WAP Push Content Handling plugin is implemented as an ECom plug-in object derived from 
the CContentHandlerBase interface.

Each Content Handler plug-in should specify the media type that it handles. Content Handler 
plug-ins can handle multiple media types: for example, a plug-in to handle SI messages can
specify it's media type as "text/vnd.wap.si||application/vnd.wap.sic".

@publishedPartner
@released
*/
class CContentHandlerBase : public CPushHandlerBase
	{
public:

	inline static CContentHandlerBase* NewL(const TDesC& aMatchData);

	inline virtual ~CContentHandlerBase();

	inline void SetParent(TMsvId aParentID);

protected:	// Methods

	inline CContentHandlerBase();

	inline void Complete(TInt aError);

	inline void IdleComplete();

protected:

	/** Parent message server entry. */
	TMsvId				iParentID;

	/** Unused. */
	TInt				iState; //state machine

	/** The derived class should set this to ETrue on receiving an asynchronous request. */
	TBool				iAcknowledge;

	/** Unused. */
	CPushMessage*		iMessage;

	};

#include <push/ccontenthandlerbase.inl>

#endif    // __CCONTENTHANDLERBASE_H__
 
