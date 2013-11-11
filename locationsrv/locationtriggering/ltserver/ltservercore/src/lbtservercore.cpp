/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  LBT Sever core
*
*/



// INCLUDE FILES
#include <e32property.h>
#include "lbtservercore.h"
#include "lbtserverconsts.h"
#include "lbtsystemstatelistener.h"
#include "lbtsession.h"
#include "lbtserverlogiccontainer.h"
#include "lbtstatuspskeys.h"
#include "lbtdiskspacelistner.h"
#include "lbtlogger.h"

// Priorty of server
// This magic number has its origin in Symbian development.
// The priority of a server active object should be 950.
enum
    {
    EPriority=950
    };

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CLbtServerCore::CLbtServerCore
// Default Constructor
// ---------------------------------------------------------
//
CLbtServerCore::CLbtServerCore(TServerType aServerType)
        	   : CServer2(EPriority, aServerType)
    {
    }
    
// ---------------------------------------------------------
// CLbtServerCore::ConstructL
// Symbian default constructor
// ---------------------------------------------------------
//
void CLbtServerCore::ConstructL()
	{
	FUNC_ENTER("CLbtServerCore::ConstructL");
	iListener = CLbtSystemStateListener::NewL(*this);	
	iContainer = CLbtServerLogicContainer::NewL();
	iDiskSpaceListner = NULL;
	RProperty property;
	CleanupClosePushL(property);
	// First define the P&S UID for location triggering
	// status information API
	
	//Allow all to ready from status information
	_LIT_SECURITY_POLICY_PASS(EReadPolicyAlwaysPass);
	
	// Read policy is always pass and write device data capability
	// is required to write to the status information P&S key
	TInt retValue = property.Define( KPSUidLbtStatusInformation,
									 KLbtLocationTriggeringSupervisionStatus,
									 RProperty::EInt,
									 EReadPolicyAlwaysPass,
									 TSecurityPolicy(ECapabilityWriteDeviceData) );

	if( !(retValue == KErrNone || retValue == KErrAlreadyExists) )
		{
		User::Leave(retValue);
		}
	
	// Attach to the property and set the status to server not ready
	User::LeaveIfError( property.Attach(KPSUidLbtStatusInformation, 
										KLbtLocationTriggeringSupervisionStatus,
										EOwnerThread) );
										
	User::LeaveIfError( property.Set(KPSUidLbtStatusInformation, 
									 KLbtLocationTriggeringSupervisionStatus, 
									 ELbtLocationTriggeringSupervisionNotReady));
	
	CleanupStack::PopAndDestroy(); // property
	
	// Set the server thread priority to low since the system
	// bootup is ongoing
	RThread thread;
	thread.SetPriority(EPriorityLess);
	thread.Close();
	}

// ---------------------------------------------------------
// CLbtServerCore::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------
//
CLbtServerCore* CLbtServerCore::NewLC()
	{
	FUNC_ENTER("CLbtServerCore::NewLC");	
	CLbtServerCore* self = new (ELeave) CLbtServerCore();
	CleanupStack::PushL(self);
	self->ConstructL();
    return self;
	}

// ---------------------------------------------------------
// CLbtServerCore::~CLbtServerCore
// Destructor
// ---------------------------------------------------------
//
CLbtServerCore::~CLbtServerCore()
	{
	FUNC_ENTER("CLbtServerCore::~CLbtServerCore");	
	Cancel();
	delete iListener;
	delete iContainer;
	delete iDiskSpaceListner;
	}
	
// ---------------------------------------------------------
// CLbtServerCore::StartL
// Starts to listen to system state events
// ---------------------------------------------------------
//
void CLbtServerCore::StartL(const TDesC &aName)
	{
	FUNC_ENTER("CLbtServerCore::StartL");	
	CServer2::StartL(aName);
	// Listen to system state events
	iListener->StartToListenL();
	}

// ---------------------------------------------------------
// CLbtServerCore::NewSessionL
// Returns a new session object
// ---------------------------------------------------------
//
CSession2* CLbtServerCore::NewSessionL(const TVersion& aVersion,
        			   				   const RMessage2& aMessage) const
	{
	FUNC_ENTER("CLbtServerCore::NewSessionL");	
	// Check if we're the right version
    TVersion version(KLbtMajorVersionNumber,
                     KLbtMinorVersionNumber,
                     KLbtBuildVersionNumber);
    if (!User::QueryVersionSupported(version, aVersion))
        {
        User::Leave(KErrNotSupported);
        }
    
    // The SecureId identifies all the sessions of a client. It is stored 
    // internally in the Session object. Location Triggering Server has a
    // specific requirement to delete all session triggers of a client when
    // the last session is closed.
    // When the last session for a client is closed, this session id 
    // is used to delete all the session triggers for the client
	CLbtSession* session = CLbtSession::NewL(*iContainer, aMessage.SecureId());
	return session;
	}

// ---------------------------------------------------------
// CLbtServerCore::HandleStartupCompletionL
// ---------------------------------------------------------
//
void CLbtServerCore::HandleStartupCompletionL( )
	{
	FUNC_ENTER("CLbtServerCore::HandleStartupCompletionL");	
	// We can delete the listner object since system
	// is up and we can load server logic
	delete iListener;
	iListener = NULL;
	
	// Set the server thread priority back to Normal since 
	// the system bootup is now complete
	RThread thread;
	thread.SetPriority(EPriorityNormal);
	thread.Close();
	
	// This will load the server logic ecom plugin 
	TRAPD(error, iContainer->LoadServerLogicL());
	
	if( error == KErrDiskFull )
		{
		if( !iDiskSpaceListner )
		    {
		    iDiskSpaceListner = CLbtDiskSpaceListner::NewL( *this );
		    }
		iDiskSpaceListner->StartToListen();
		}
	}

// ---------------------------------------------------------
// CLbtServerCore::NumberOfClientSessions
// ---------------------------------------------------------
//
TInt CLbtServerCore::NumberOfClientSessions(TSecureId aSecureId)
	{
	FUNC_ENTER("CLbtServerCore::NumberOfClientSessions");	
	TInt count = 0;
	iSessionIter.SetToFirst();
	CLbtSession* session = static_cast <CLbtSession*> (iSessionIter++);
	
	while(session)
		{
		if(session->SecureId() == aSecureId)
			{
			count++;
			}
		session = static_cast <CLbtSession*> (iSessionIter++);
		}
	return count;
	}

// ---------------------------------------------------------
// CLbtServerCore::HandleDiskSpaceChange
// ---------------------------------------------------------
//
void CLbtServerCore::HandleDiskSpaceChange()
	{
	// Try to load strategy data again
	TRAPD(error, iContainer->LoadServerLogicL());
	
	if( error == KErrDiskFull )
	    {
	    iDiskSpaceListner->StartToListen();
	    }
	else
	    {
	    delete iDiskSpaceListner;
	    iDiskSpaceListner = NULL;
	    }    
	}
// end of file

