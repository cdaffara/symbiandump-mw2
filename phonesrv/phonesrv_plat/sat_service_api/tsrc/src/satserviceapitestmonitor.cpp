/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Test class for SAT Service API 
*
*/


#include <stiflogger.h>

#include "satserviceapitest.h"
#include "satserviceapitestmonitor.h"

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
//
CSatServiceAPIMonitor::CSatServiceAPIMonitor( CStifLogger& aLog, 
                                              CSATServiceAPI& aTest ) :
                                              CActive( EPriorityStandard ),
                                              iLog( aLog ),
                                              iTest( aTest )
    { 
    CActiveScheduler::Add( this );   
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CSatServiceAPIMonitor::~CSatServiceAPIMonitor()
    {
    Cancel();
    }
    
// ---------------------------------------------------------------------------
// CSatServiceAPIMonitor::Activate
// activates active object if not active
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CSatServiceAPIMonitor::Activate()
    {
    iLog.Log(_L("CSatServiceAPIMonitor::Activate calling "));
    if ( !IsActive() )
        {
        SetActive();
        iLog.Log(_L("CSatServiceAPIMonitor::Activate, waiting RunL"));        
        }        
    }

// ---------------------------------------------------------------------------
// CSatServiceAPIMonitor::Status
// returns active object status
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TRequestStatus& CSatServiceAPIMonitor::Status()
    {
    return iStatus;    
    } 
    
// -----------------------------------------------------------------------------
// Handles the request completion.
// -----------------------------------------------------------------------------
//
void CSatServiceAPIMonitor::RunL()
    {
    iLog.Log( _L("CSatServiceAPIMonitor::RunL - Signaling!") );
    iTest.Signal( iStatus.Int() );  
    }
    
// -----------------------------------------------------------------------------
// Cancels the pending request
// -----------------------------------------------------------------------------
//
void CSatServiceAPIMonitor::DoCancel()
    {
    iLog.Log( _L("CSatServiceAPIMonitor::DoCancel() calling - done") );
    }
    
// -----------------------------------------------------------------------------
// Error occurs in RunL
// -----------------------------------------------------------------------------
//   
TInt CSatServiceAPIMonitor::RunError( TInt aError )
    {
    iLog.Log( _L("CSatServiceAPIMonitor::RunError() calling - done") );
    return aError;
    }
