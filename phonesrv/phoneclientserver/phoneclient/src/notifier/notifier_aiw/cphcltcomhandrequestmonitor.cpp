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
* Description:  Call Notify
*
*/



//  Include Files
#include    "cphcltcomhandrequestmonitor.h"
#include    "rphcltcallnotify.h" 
#include    <mphcltcomhandrequestobserver.h>
#include    <e32base.h>


// ============================= LOCAL FUNCTIONS ===============================



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltComHandRequestMonitor::CPhCltComHandRequestMonitor
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltComHandRequestMonitor::CPhCltComHandRequestMonitor(
    RPhCltCallNotify& aCallNotify):  
        CActive( EPriorityStandard ), iCallNotify(aCallNotify)
    {
    CActiveScheduler::Add( this );
    }
    
// -----------------------------------------------------------------------------
// CPhCltComHandRequestMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCltComHandRequestMonitor* CPhCltComHandRequestMonitor::NewL(RPhCltCallNotify& aCallNotify) 
    {
    CPhCltComHandRequestMonitor* self = new ( ELeave ) CPhCltComHandRequestMonitor(aCallNotify); 
    return self;
    }
    
    // Destructor
CPhCltComHandRequestMonitor::~CPhCltComHandRequestMonitor()
    {  
    Cancel();
    }
    
// -----------------------------------------------------------------------------
// CPhCltCallNotify::RespondComHandClient
// 
// -----------------------------------------------------------------------------
//
TInt CPhCltComHandRequestMonitor::RespondComHandClient( 
    const TInt aResultCode )
    {
    return iCallNotify.RespondComHandClient( aResultCode );
    }    

// -----------------------------------------------------------------------------
// CPhCltComHandRequestMonitor::RunL()
// 
// -----------------------------------------------------------------------------
//    
void CPhCltComHandRequestMonitor::RunL()
    {
    TInt error = iStatus.Int();
    if ( !error ) 
        {
        iObserver->ComHandRequest( );    
        }   
    }


// -----------------------------------------------------------------------------
// CPhCltComHandRequestMonitor::CancelNotifyComHandCommand()
// 
// -----------------------------------------------------------------------------
//    
void CPhCltComHandRequestMonitor::DoCancel()
    {
    iCallNotify.CancelNotifyComHandCommand();
    }



// -----------------------------------------------------------------------------
// CPhCltComHandRequestMonitor::NotifyComHandCommand
// 
// -----------------------------------------------------------------------------
//
void CPhCltComHandRequestMonitor::NotifyComHandCommand( 
    MPhCltComHandRequestObserver* aObserver,
    TDes8& aCommandArgs )
    {
    if ( !IsActive() )
        {
        iObserver = aObserver;
        iCallNotify.NotifyComHandCommand( 
            iStatus, 
            aCommandArgs );
        SetActive();
        }
    }
    

    



// End of File
