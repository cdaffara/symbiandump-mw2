// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// The HTTP Security plug-in provides an implementation of security policy for use by HTTP
// when configuring WTLS in the WAP stack.
// 
//

/**
 @file CWapPushMessageFilter.h
*/

#ifndef __CWAPPUSHMESSAGEFILTER_H__
#define __CWAPPUSHMESSAGEFILTER_H__

// System includes
//
#include <e32base.h>

// Forward class declarations
//
class CPushMessage;

/** the data of the filter plugin that the plugin server searches for */
_LIT(KWapPushFilterPluginData, "WapPushFilter");

/** the UID of Push Message Filter type plugins, of which there should ONLY be one of */
const TUid KUidPushMessageFilter = { 0x101F3E66 };

/** interface for wap push message filtering.

	This is the interface that the filter plugin must provide. It also provides a utility function 
	to load the filter plugin, provided as part of pushutils library.
*/
class CWapPushMessageFilter : public CBase
/**
@internalTechnology
@deprecated
*/
	{
public:	// Methods

	inline static CWapPushMessageFilter* NewL();

	inline virtual ~CWapPushMessageFilter();

	/** takes a message and checks to see if it should be accepted or discarded
		@param aPushMessage The push message to be checked
		@param aStatus returns complete once the message has been checked
		@param aAllowMessage changed to ETrue if the message is to be allowed to be dispatched
	*/
	virtual void FilterMessage(const CPushMessage& aPushMessage, TRequestStatus& aStatus, TBool& aAllowMessage) =0;

	/** Cancel the filtering process
	*/
	virtual void CancelFilter() =0;

protected:	// Methods

	inline CWapPushMessageFilter();

protected:	// Attributes

	TRequestStatus*	iFilterReq;

private:	// Attributes

	/** A unique UID used in interface destruction 
	 */
	TUid	iDtor_ID_Key;

private:	// BC-proofing

/**
	Intended Usage	:	Reserved for future expansion.
	@since			6.0
 */
	virtual void CWapPushMessageFilter_Reserved1() =0;		

/**
	Intended Usage	:	Reserved for future expansion.
	@since			6.0
 */
	virtual void CWapPushMessageFilter_Reserved2() =0;		

	/** Reserved for future expansion
	 */
	TAny*		iCWapPushMessageFilter_Reserved;

	};

#include "CWapPushMessageFilter.inl"

#endif	// __CWAPPUSHMESSAGEFILTER_H__
