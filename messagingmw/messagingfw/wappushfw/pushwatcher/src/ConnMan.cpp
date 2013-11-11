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
//
    
#include <commdb.h>
#include <iapprefs.h>
#include <wapmessage.h>
#include "ConnMan.h"

// Comment out the following define to use PPP - crashes program in WINS if 
// either RAS is not set up or theres no means of connecting to a phone!  

#define __USE_REAL_PPP_CONNECTION

/**
 * Factory construction method
 * Use this method to allocate and construct a new CConnectionManager object
 * @param aLog 
 *			in: a interface for run-time logging
 * @return  CConnectionManager*  - pointer to newly instantiated object
 */
CConnectionManager* CConnectionManager::NewL(MWapPushLog& aLog)
	{
	CConnectionManager* self = new (ELeave) CConnectionManager(aLog);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); //self
	return self;
	}


/**
 * Constructor
 * @param aLog a interface for run-time logging
 */
CConnectionManager::CConnectionManager(MWapPushLog& aLog) :	
	CActive(CActive::EPriorityStandard), 
	iLog(aLog),
	iConnectionExists(EFalse)
	{
	}

/**
 * Destructor. Deletes the queue of CO Watchers, and 
 * closes the PPP connection.
 */
CConnectionManager::~CConnectionManager()
	{
	__LOG_DEBUG("CConnectionManager destructor called");

	Cancel();
	delete iCLUnsecWatcher;
	delete iCLSecureWatcher;
	}

/**
 * Secondary constructor for the connection manager. 
 * Creates connectionless watchers (unsecure & secure).
 */
void CConnectionManager::ConstructL()
	{
	__LOG_DEBUG("CConnectionManager::NewL called");

	CActiveScheduler::Add(this);

	// Create and start CL watchers
	iCLUnsecWatcher = CCLUnsecureWatcher::NewL(iLog,*this);	
	TRAPD(error, iCLSecureWatcher = CCLSecureWatcher::NewL(iLog, *this));	
	
	// Secure Watcher should not leave if it is not supported in Wap Stack 
	// as we want push watcher to start. 
	if (error != KErrNotSupported)
		{
		User::LeaveIfError(error);
		}
	}


/**
 * The Connection Manager is an Active Object. It is managing other AO's.  However,
 * it is not waiting for an event to complete. RunL() should never get called.
 * 
 */
void CConnectionManager::RunL()
	{
	__ASSERT_DEBUG(0, User::Invariant());
	User::Panic(_L("WapPush Connection Manager"), KErrNotSupported); 
	}


/**
 * If RunL() leaves, this function gets called to handle the leave.
 * @param TInt aError
 *						in: errror code for the event which generated the leave
 * @return TInt error code for the function
 */
TInt CConnectionManager::RunError(TInt aError)
	{
	User::Panic(_L("Untrapped RunError"),aError);
	return(KErrNone);
	}


/**
 * Cancel any active aynchronous request this object has made. 
 */
void CConnectionManager::DoCancel()
	{
	iCLUnsecWatcher->Cancel();
	iCLSecureWatcher->Cancel();
	}


/**
 * Request that a new connection-orientated connection is created.
 * This will be called by the SIA content type handler, passing in 
 * a connection point 
 * @param TPushConnPoint aConnPoint
 *					in: structur containing connection point information
 */
void CConnectionManager::CMOpenConnectionL(TPushConnPoint& /*aConnPoint*/)
	{
	__LOG_DEBUG("CConnectionManager: CMOpenCOConnectionL called")
	// Do nothing as we do not support CO Push

	}

/**
 * Signal that the connection-orientated watcher has completed
 * Remove the watcher from the list. 
 * If there are no more CO watchers, close PPP connection
 * @param aCOWatcher completed watcher
 * @param aError completion error code 
 */
void CConnectionManager::CMWatcherComplete(CCOWatcherBase& /*aCOWatcher*/, TInt /*aError*/)
	{
	__LOG_DEBUG("ConnMan: CMWatcherComplete");
	}


