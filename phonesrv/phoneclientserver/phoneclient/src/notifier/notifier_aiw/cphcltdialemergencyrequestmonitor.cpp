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
#include    <cphcltemergencycall.h> // for TPhCltEmergencyNumber 
#include    "cphcltdialemergencyrequestmonitor.h"
#include    <mphcltdialemergencyrequestobserver.h>
#include    "rphcltserver.h" 
#include    "rphcltcallnotify.h" 

// ============================= LOCAL FUNCTIONS ===============================



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltDialEmergencyRequestMonitor::CPhCltDialEmergencyRequestMonitor
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltDialEmergencyRequestMonitor::CPhCltDialEmergencyRequestMonitor(RPhCltCallNotify& aCallNotify) 
    :  CActive( CActive::EPriorityStandard ),
    iCallNotify(aCallNotify),
    iEmergencyCallNumberPtr( NULL, 0, 0 )    
    {
    CActiveScheduler::Add( this );
    }
    
// -----------------------------------------------------------------------------
// CPhCltDialEmergencyRequestMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCltDialEmergencyRequestMonitor* CPhCltDialEmergencyRequestMonitor::NewL(
        RPhCltCallNotify& aCltNotify)
    {
    CPhCltDialEmergencyRequestMonitor* self = 
        new ( ELeave ) CPhCltDialEmergencyRequestMonitor(aCltNotify); 
    return self;
    }
    
    // Destructor
CPhCltDialEmergencyRequestMonitor::~CPhCltDialEmergencyRequestMonitor()
    {  
    Cancel();
    }


// -----------------------------------------------------------------------------
// CPhCltDialEmergencyRequestMonitor::RunL()
// 
// -----------------------------------------------------------------------------
//    
void CPhCltDialEmergencyRequestMonitor::RunL()
    {
    TInt error = iStatus.Int();
    if ( !error ) 
        {
        iObserver->DialEmergencyRequest( );    
        }
    }

TInt CPhCltDialEmergencyRequestMonitor::RunError(TInt /*aErr*/)
    {
    // RunL failed because of leave in SetParamsL,
    // 
    // TODO check how to react
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhCltDialEmergencyRequestMonitor::DoCancel()
// 
// -----------------------------------------------------------------------------
//    
void CPhCltDialEmergencyRequestMonitor::DoCancel()
    {
    iCallNotify.CancelNotifyEmergencyCall();
    }

// -----------------------------------------------------------------------------
// CPhCltDialEmergencyRequestMonitor::NotifyEmergencyCall()
// 
// -----------------------------------------------------------------------------
//  
void CPhCltDialEmergencyRequestMonitor::NotifyEmergencyCall( 
    MPhCltDialEmergencyRequestObserver* aObserver, 
    TDesC& aEmergencyNumber )
    {
    
    if ( !IsActive() )
        {
    
        iEmergencyCallNumberPtr.Set( reinterpret_cast<TText8*>( &aEmergencyNumber ), 
                             sizeof( TPhCltEmergencyNumber ),
                             sizeof( TPhCltEmergencyNumber ) );
        
        iObserver = aObserver;
        
        iCallNotify.NotifyEmergencyCall( 
            iStatus, 
            iEmergencyCallNumberPtr
             );
        SetActive();
        }
         
    }


// -----------------------------------------------------------------------------
// CPhCltDialEmergencyRequestMonitor::RespondEmergencyToClient()
// 
// -----------------------------------------------------------------------------
//
TInt CPhCltDialEmergencyRequestMonitor::RespondEmergencyToClient( 
    const TInt aResultCode )
    {
    return iCallNotify.RespondEmergencyToClient( aResultCode  );
    }


// End of File
