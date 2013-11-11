/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of LBT Server core system state listner
*
*/



#include <startupdomainpskeys.h>
#include <e32property.h>
#include <centralrepository.h>

#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	#include <ssm/ssmdomaindefs.h>
#endif

#ifndef RD_STARTUP_CHANGE
#include <PSVariables.h>
#include <sysstartup.h>
#endif
#include "lbtsystemstatelistener.h"
#include "lbtstartupobserver.h"
#include "lbttriggeringsettingskeys.h"
#include "lbtlogger.h"
#ifdef _DEBUG
#include "lbtservererror.h"
#endif
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtSystemStateListener::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtSystemStateListener* CLbtSystemStateListener::NewL(MLbtStartupObserver& aObserver)
    {
    FUNC_ENTER("CLbtSystemStateListener::NewL");	
    CLbtSystemStateListener* self = new ( ELeave ) CLbtSystemStateListener(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtSystemStateListener::~CLbtSystemStateListener
// Destructor
// ---------------------------------------------------------------------------
//
CLbtSystemStateListener::~CLbtSystemStateListener()
	{
	FUNC_ENTER("CLbtSystemStateListener::~CLbtSystemStateListener");	
	Cancel();	
	
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	iStateAwareSession.Close();
#else
	iStartupObserver.Close();
#endif
	}

// ---------------------------------------------------------------------------
// CLbtSystemStateListener::CLbtSystemStateListener
// Default C++ Constructor
// ---------------------------------------------------------------------------
//
CLbtSystemStateListener::CLbtSystemStateListener(MLbtStartupObserver& aObserver)
	: CActive( EPriorityStandard ), iObserver(aObserver)
	{
	CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// CLbtSystemStateListener::ConstructL()
// 2nd phase constuctor for instantiating member variables
// ---------------------------------------------------------------------------
//
void CLbtSystemStateListener::ConstructL()
	{
	FUNC_ENTER("CLbtSystemStateListener::ConstructL");		
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	User::LeaveIfError( iStateAwareSession.Connect( KSM2GenMiddlewareDomain3 ) );
#else
	User::LeaveIfError( iStartupObserver.Attach( KPSUidStartup, 
	                                                 KPSGlobalSystemState, EOwnerThread ) );    
#endif
	}

// ---------------------------------------------------------------------------
// CLbtSystemStateListener::DoCancel()
// Cancels any outstanding request
// ---------------------------------------------------------------------------
//
void CLbtSystemStateListener::DoCancel()
	{
	FUNC_ENTER("CLbtSystemStateListener::DoCancel");
	
#ifndef SYMBIAN_SYSTEM_STATE_MANAGEMENT
	iStartupObserver.Cancel();
#endif
	}

// ---------------------------------------------------------------------------
// CLbtSystemStateListener::StartToListenL()
// Starts to Listen to System State events
// ---------------------------------------------------------------------------
//
void CLbtSystemStateListener::StartToListenL()
	{
	FUNC_ENTER("CLbtSystemStateListener::StartToListenL");		

	iObserver.HandleStartupCompletionL();
	
/*
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
    TSsmState ssmState = iStateAwareSession.State();
    if( ssmState.MainState() != ESsmNormal )
        {
        iStateAwareSession.RequestStateNotification( iStatus );
        SetActive();
        }
    else{
        iObserver.HandleStartupCompletionL();
        }
#else
    TInt systemStatus = 0;
        User::LeaveIfError( iStartupObserver.Get( systemStatus ) );
            
        // If system state is not already normal then subsribe 
        // to system state event
    #ifndef RD_STARTUP_CHANGE
            if( systemStatus != ESWStateNormal )
    #else
                if( (systemStatus != ESwStateNormalRfOn )  &&
                    (systemStatus != ESwStateNormalRfOff ) &&
                    (systemStatus != ESwStateNormalBTSap ) )
    #endif  //RD_STARTUP_CHANGE
            {
            iStartupObserver.Subscribe( iStatus );
            SetActive();
            }
        else
            {
            iObserver.HandleStartupCompletionL();
            }
#endif */
	}

// ---------------------------------------------------------------------------
// CLbtSystemStateListener::RunL()
// Request completion event handler
// ---------------------------------------------------------------------------
//
void CLbtSystemStateListener::RunL()
	{
	FUNC_ENTER("CLbtSystemStateListener::RunL");
  
    
#ifdef SYMBIAN_SYSTEM_STATE_MANAGEMENT
    TSsmState ssmState = iStateAwareSession.State();
    if( ssmState.MainState() != ESsmNormal )
        {
        iStateAwareSession.AcknowledgeAndRequestStateNotification( iStatus.Int(), iStatus );
        SetActive();
        }
    else{
        iObserver.HandleStartupCompletionL();
        }
#else
    TInt systemStatus = 0;
      User::LeaveIfError( iStartupObserver.Get( systemStatus ) );
      switch( systemStatus )
          {
      #ifndef RD_STARTUP_CHANGE
          case ESWStateNormal:
      #else
          case ESwStateNormalRfOn:
          case ESwStateNormalRfOff:
          case ESwStateNormalBTSap:
      #endif // RD_STARTUP_CHANGE
              {
              iObserver.HandleStartupCompletionL();
              break;
              }
          default:
              {
              iStartupObserver.Subscribe( iStatus );
              SetActive();                
              break;
              }
          }
#endif
	}
	

TInt CLbtSystemStateListener::RunError(TInt /*aError*/)
    {
    FUNC_ENTER("CLbtSystemStateListener::RunError");		
    // The RunL leave because of the following reasons.
    // 1. Unable to obtain the value of the P&S key KPSUidStartup, 
    //       KPSGlobalSystemState.
    // 2. Unable to open the CenRep KCRUidLocationTriggeringSettings 
    //       (could be because of repository missing or corrupt).
    // 3. Unable to read settings from CenRep for key ELbtSleepTime
    //       (could be because of no such key or insufficient capabilities.
    // 4. CLbtServerCore::HandleStartupCompletionL leaves (because the
    //       loading of server Logic plugin failed).
    // These error cases happen because of programming error. So we will
    // just log an error and then panic the server if in debug mode.
    LBT_TRACE(KLbtLogServerCore|KLbtLogError,__FILE__, __LINE__, "Error %d in CLbtSystemStateListener::RunL()",aError);
#ifdef _DEBUG    
    User::Panic(KLbtServer(), ELbtServerStateListenerError);
#endif
    return KErrNone;
    }

//end of file
