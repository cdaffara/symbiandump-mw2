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

#ifndef __CPUSHHANDLERBASE_H__
#define __CPUSHHANDLERBASE_H__

// System includes
#include <e32base.h>

// Forward class declarations
class CPushMessage;
class CPluginKiller;
class MWapPushLog;
class MConnManObserver;

/** ECom interface UID for WAP Push Application plug-ins. */
const TUid KUidPushHandlerBase = { 0x101F3E5A };

/** 
Abstract base class for WAP Push Application plugins.

A WAP Push Application plugin is implemented as an ECom plug-in object that 
derives from this interface. Each plugin specifies in its ECom IMPLEMENTATION_INFO 
default_data field the Push Application IDs that it should handle. When the 
WAP Push Framework receives a push message, it examines the message's Application 
ID, and loads the appropriate plug-in to handle the message with HandleMessageL().

A plug-in can handle multiple Application IDs. Application IDs can be specified 
as URNs or WINA (http://www.wapforum.org/wina/push-app-id.htm) values. For 
example, a plug-in to handle MMS would set its default_data to "x-wap-application:mms.ua||0x00000004".

A plug-in must destroy itself when it is has finished handling the message. 
The framework supplies a CPluginKiller object that the plug-in calls to do 
this. 

@publishedPartner
@released
*/
class CPushHandlerBase : public CActive
	{
public:	// Methods

	inline static CPushHandlerBase* NewL(const TDesC& aMatchData);

	inline static CPushHandlerBase* NewL(const TDesC& aMatchData, const TUid& aInterfaceUid);

	inline virtual ~CPushHandlerBase();

	//Async. Functions
	/** 
	Handles a push message asynchronously.
	
	Implementations should store the passed aStatus using SetConfirmationStatus(), 
	and when handling is complete, complete it with SignalConfirmationStatus().
	
	@param aPushMsg 
	Push message. Ownership of the message is passed to the object.
	
	@param aStatus 
	Asynchronous status word 
	*/
	virtual void HandleMessageL(CPushMessage* aPushMsg,TRequestStatus& aStatus) =0;

	/** 
	Cancels an outstanding HandleMessageL() call. 
	*/
	virtual void CancelHandleMessage() =0;

	//Sync. Functions
	/** 
	Handles a push message synchronously.
	
	@param aPushMsg 
	Push message. Ownership of the message is passed to the object. 
	*/
	virtual void HandleMessageL(CPushMessage* aPushMsg) =0;

	inline void SetLogger(MWapPushLog& aLog);

	inline void SetKiller(CPluginKiller& aPluginKiller);

	inline void SetManager(MConnManObserver& aManager);

protected:	// Methods

	CPushHandlerBase();

	void SetConfirmationStatus(TRequestStatus& aStatus);

	void SignalConfirmationStatus(TInt aErr);

protected:	// Attributes

	/** Plugin killer utility object. */
	CPluginKiller*		iPluginKiller;

	/** Log interface. */
	MWapPushLog*		iLog;

    /** connection manager */
	MConnManObserver*	iManager;

	/** HandleMessageL() asynchronous status word. */
	TRequestStatus*		iConfirmStatus;

private:	// Attributes

	/** A unique UID used in interface destruction */
	TUid	iDtor_ID_Key;

private:	// BC-proofing

	/** Reserved for future expansion */
	virtual void CPushHandlerBase_Reserved1() =0;		

	/** Reserved for future expansion */
	virtual void CPushHandlerBase_Reserved2() =0;		

	/** Reserved for future expansion */
	TAny*		iCPushHandlerBase_Reserved;

	};

#include <push/cpushhandlerbase.inl>

#endif    // __PUSHBASEHAND_H__
