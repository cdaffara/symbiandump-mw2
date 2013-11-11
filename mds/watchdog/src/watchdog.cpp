/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Monitoring application for servers restarting & IAD
*
*/

#include "mdscommoninternal.h"
#include "watchdog.h"

_LIT( KHarvesterServerProcess, "HarvesterServer*" );
_LIT( KHarvesterServerExe, "harvesterserver.exe" );

// Print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif


// ======== MEMBER FUNCTIONS ========
// ---------------------------------------------------------------------------
// Constructor.
// ---------------------------------------------------------------------------
//
CWatchdog* CWatchdog::NewL()
    {
    PRINT(_L("CWatchdog::NewL()"));
    CWatchdog* self = new ( ELeave ) CWatchdog();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CWatchdog::CWatchdog() : 
    CActive( CActive::EPriorityStandard ), iState(EIdle)
    {
    // Add to active scheduler.
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CWatchdog::ConstructL()
    {
    PRINT(_L("CWatchdog::ConstructL()"));
        
    TFullName name;
        
    // find Harvester Server
    TFindProcess findProcess( KHarvesterServerProcess );
    if ( findProcess.Next(name) == KErrNone )
        {                
        PRINT(_L("CWatchdog::ConstructL() - found server, start listening"));
        
        User::LeaveIfError( iProcess.Open(name) );
        iState = ERunning;
        // logon to get termination signal
        iProcess.Logon(iStatus);        
        SetActive();
        }        
    else
        {               
        PRINT(_L("CWatchdog::ConstructL() - start Harvester server"));
        // start new Harvester            
        Start();
        }
    
    PRINT(_L("CWatchdog::ConstructL() - create observer"));
    iShutdownObserver = CWDShutdownObserver::NewL( *this );        
    iSelfShutdownObserver = CWDSelfShutdownObserver::NewL( *this ); 
    }

void CWatchdog::Start()
    {  
    PRINT(_L("CWatchdog::Start()"));

    // Double check that harvester server is not already running
    TFullName name;
    TInt res( KErrNone );
   
    // find really Harvester Server, using TFindServer to avoid logon a dead process
    TFindServer findServer( KHarvesterServerProcess );
    if ( findServer.Next(name) == KErrNone )
        {
        TFindProcess findProcess( KHarvesterServerProcess );
        if ( findProcess.Next(name) == KErrNone )
            {
            PRINT(_L("CWatchdog::Start() - server already running, start listening"));
            
            iProcess.Close();
            res = iProcess.Open(name);
            if ( res != KErrNone )
                {
                PRINT(_L("CWatchdog::Start() - error in starting listening "));
                return;
                }
            iState = EWaitingRendezvous;
            SetActive();
            return;
            }
        }  
    
    // close the panic process
    TFindProcess findProcess( KHarvesterServerProcess );
    if ( findProcess.Next(name) == KErrNone )
        {                
        iProcess.Close();
        }

    // Create the server process
    // KNullDesC param causes server's E32Main() to be run
    res = iProcess.Create( KHarvesterServerExe, KNullDesC );
    if ( res != KErrNone )
        {
        PRINT(_L("CWatchdog::ConstructL() - error in server creation"));
        return;
        }
    
    // start process and wait until it is constructed                
    iProcess.Rendezvous(iStatus);                                         
    
    if( iStatus != KRequestPending )
        {
        iProcess.Kill( 0 );   // abort startup
        }
    else
        {
        iProcess.Resume();    // logon OK - start the server   
        iState = EWaitingRendezvous;
        SetActive();
        }
    
    PRINT(_L("CWatchdog::ConstructL() - Start end"));
    }    
    

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CWatchdog::~CWatchdog()
    {
    delete iShutdownObserver;
    delete iSelfShutdownObserver;
    Cancel();
    }

// ---------------------------------------------------------------------------
// Active object's request handling.
// ---------------------------------------------------------------------------
//
void CWatchdog::RunL()
    {
    PRINT(_L("CWatchdog::RunL"));
    	
    	switch (iState)
    	{
          case EIdle:
              {
              PRINT(_L("CWatchdog::RunL() - EIdle"));
              break;
              }
              
          case EWaitingRendezvous:          
              {
              PRINT(_L("CWatchdog::RunL() - EWaitingRendezvous"));
              iState = ERunning;              
              // logon to get termination signal
              iProcess.Logon(iStatus);        
              SetActive();
              break;
              }             
              
          case ERunning:
              {
              PRINT(_L("CWatchdog::RunL() - server died"));
              // server died unexpectedly, start it    
              Start();
              break;
              }
              
          case EShuttingDown:
              {
              PRINT(_L("CWatchdog::RunL() - IAD shutdown"));
              // Do nothing              
              break;
              }
              
          case ERestarting:
              {
              PRINT(_L("CWatchdog::RunL() - IAD restart"));
              Start();
              break;
              }
            
          default:
              break; 
          
    	}
    
    }

// ---------------------------------------------------------------------------
// Active object's request error handling.
// ---------------------------------------------------------------------------
//
TInt CWatchdog::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Cancel the request.
// ---------------------------------------------------------------------------
//
void CWatchdog::DoCancel()
    {
    PRINT(_L("CWatchdog::DoCancel()"));
    iProcess.LogonCancel(iStatus);
    iProcess.Close();
    }


// -----------------------------------------------------------------------------
// CWatchdog::ShutdownNotification
// -----------------------------------------------------------------------------
//
void CWatchdog::ShutdownNotification()
    {

    iState = EShuttingDown;
    
    PRINT((_L("CWatchdog::ShutdownNotification() IsActive() = %d"), IsActive() ));
    
    
    }

// -----------------------------------------------------------------------------
// CWatchdog::ShutdownNotification
// -----------------------------------------------------------------------------
//
void CWatchdog::RestartNotification()
    {
    PRINT(_L("CWatchdog::RestartNotification()"));
    
    if (!IsActive())
        SetActive();
    
    iState = ERestarting;    
    TRequestStatus *status = &iStatus;
    User::RequestComplete(status, KErrNone);
        
    }

void CWatchdog::SelfShutdownNotification()
    {
    PRINT(_L("CWatchdog::SelfShutdownNotification()"));
    CActiveScheduler::Stop();
    }

void MainL()
    {
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );
    
    CWatchdog* w = CWatchdog::NewL();
    CleanupStack::PushL( w );
    CActiveScheduler::Start();
    
    CleanupStack::PopAndDestroy( 2, scheduler );
    }

TInt E32Main()
    {
    TInt err = KErrNoMemory;
   
    CTrapCleanup* cleanupStack = CTrapCleanup::New();
    if ( cleanupStack )
        {
        TRAP( err, MainL() );
        }
  
    delete cleanupStack;
    cleanupStack = NULL;
    return err;
    }
