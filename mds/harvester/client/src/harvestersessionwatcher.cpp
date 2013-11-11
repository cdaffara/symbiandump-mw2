/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester session watcher
*
*/

#include "harvestersessionwatcher.h"

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
//
CHarvesterSessionWatcher* CHarvesterSessionWatcher::NewL( MHarvesterSessionObserver& aObserver )
	{
    CHarvesterSessionWatcher* self = new ( ELeave ) CHarvesterSessionWatcher( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
	}

// ---------------------------------------------------------------------------
// ~CHarvesterSessionWatcher
// ---------------------------------------------------------------------------
//
CHarvesterSessionWatcher::~CHarvesterSessionWatcher() // destruct
	{   
    Cancel();
    iObserver = NULL;
 	}

// ---------------------------------------------------------------------------
// CHarvesterSessionWatcher
// First-phase C++ constructor
// ---------------------------------------------------------------------------
//
CHarvesterSessionWatcher::CHarvesterSessionWatcher( MHarvesterSessionObserver& aObserver )
    : CActive( CActive::EPriorityStandard ), iState( EIdle ), iObserver( &aObserver )
  	{
    // Add to active scheduler.
    CActiveScheduler::Add( this );
	}

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CHarvesterSessionWatcher::ConstructL() // second-phase constructor
	{
    _LIT( KHarvesterServerProcess, "HarvesterServer*" );
    TFullName name;
        
    // find Harvester Server
    TFindProcess findProcess( KHarvesterServerProcess );
    if ( findProcess.Next(name) == KErrNone )
        {                      
        User::LeaveIfError( iProcess.Open(name) );
        iState = ERunning;
        // logon to get termination signal
        iProcess.Logon( iStatus );        
        SetActive();
        }
    else
        {
        User::Leave( KErrCommsBreak );
        }
	}

// ---------------------------------------------------------------------------
// DoCancel
// ---------------------------------------------------------------------------
//
void CHarvesterSessionWatcher::DoCancel()
	{
    iState = ESCancelled;
    iProcess.LogonCancel( iStatus );
    iProcess.Close();
	}

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//
void CHarvesterSessionWatcher::RunL()
	{ 
    switch (iState)
        {
          case EIdle:
          case ESCancelled:
              {
              break;
              }       
              
          case ERunning:
              {
              // server terminated, notify clients
              if( iObserver )
                  {
                  iObserver->HarvesterServerTerminated();
                  }
              break;
              }
            
          default:
              break; 
        }
	}
	
// ---------------------------------------------------------------------------
// RunError
// ---------------------------------------------------------------------------
//	
TInt CHarvesterSessionWatcher::RunError( TInt /*aError*/ )
    {   
    return KErrNone;
    }

