/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
#include    <cphcltcallnotify.h> 
#include    <cphcltdialdata.h>
#include    <mphcltdialrequestobserver.h>

#include    "rphcltserver.h" 
#include    "cphcltextphonedialdata.h" 
#include    "rphcltcallnotify.h" 
#include    "cphcltdialrequestmonitor.h"
#include    "cphcltdialemergencyrequestmonitor.h"
#include    "cphcltcomhandrequestmonitor.h"

// ============================= LOCAL FUNCTIONS ===============================



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltCallNotify::CPhCltCallNotify
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltCallNotify::CPhCltCallNotify() 
    {
    }
    
// -----------------------------------------------------------------------------
// CPhCltCallNotify::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPhCltCallNotify* CPhCltCallNotify::NewL()
    {
    CPhCltCallNotify* self = new ( ELeave ) CPhCltCallNotify; 
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
    // Destructor
EXPORT_C CPhCltCallNotify::~CPhCltCallNotify()
    {  
    delete iComHandMonitor;
    delete iDialMonitor;
    delete iDialEmergencyMonitor;
    delete iCallNotify;
    }

// -----------------------------------------------------------------------------
// CPhCltCallNotify::Open
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPhCltCallNotify::Open( RPhCltServer& aServer )
    {
    TInt retVal = iCallNotify->Open( aServer );
    return retVal;
    }

// -----------------------------------------------------------------------------
// CPhCltCallNotify::Close
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhCltCallNotify::Close()
    {
    iCallNotify->Close();
    }

// -----------------------------------------------------------------------------
// CPhCltCallNotify::NotifyCallAttempt
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhCltCallNotify::NotifyDialRequest( 
    MPhCltDialRequestObserver* aObserver )
    {
    iDialMonitor->NotifyDialRequest( aObserver );
    }


// -----------------------------------------------------------------------------
// CPhCltCallNotify::CancelNotifyCallAttempt
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhCltCallNotify::CancelNotifyDialRequest()
    {
    iDialMonitor->Cancel();
    }


// -----------------------------------------------------------------------------
// CPhCltCallNotify::RespondClient
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPhCltCallNotify::RespondClient( const TInt aResultCode )
    {
    return iDialMonitor->RespondClient( aResultCode  );
    }

// -----------------------------------------------------------------------------
// CPhCltCallNotify::NotifyEmergencyCall()
// 
// -----------------------------------------------------------------------------
//  
EXPORT_C void CPhCltCallNotify::NotifyEmergencyCall( 
    MPhCltDialEmergencyRequestObserver* aObserver, 
    TDesC& aEmergencyNumber )
    {   
    iDialEmergencyMonitor->NotifyEmergencyCall( aObserver, aEmergencyNumber );
    }

// -----------------------------------------------------------------------------
// CPhCltCallNotify::CancelNotifyEmergencyCall()
// 
// -----------------------------------------------------------------------------
//  
EXPORT_C void CPhCltCallNotify::CancelNotifyEmergencyCall() const
    {
    iDialEmergencyMonitor->Cancel();
    }

// -----------------------------------------------------------------------------
// CPhCltCallNotify::RespondEmergencyToClient()
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPhCltCallNotify::RespondEmergencyToClient( 
    const TInt aResultCode )
    {
    return iDialEmergencyMonitor->RespondEmergencyToClient( aResultCode  );
    }

// -----------------------------------------------------------------------------
// CPhCltCallNotify::NotifyComHandCommand
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhCltCallNotify::NotifyComHandCommand(  
    MPhCltComHandRequestObserver* aObserver, TDes8& aCommandArgs )
    {
    iComHandMonitor->NotifyComHandCommand( aObserver, aCommandArgs );
    }
    
// -----------------------------------------------------------------------------
// CPhCltCallNotify::CancelNotifyComHandCommand
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CPhCltCallNotify::CancelNotifyComHandCommand() const
    {
    iComHandMonitor->Cancel();
    }

// -----------------------------------------------------------------------------
// CPhCltCallNotify::RespondComHandClient
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPhCltCallNotify::RespondComHandClient( 
    const TInt aResultCode )
    {
    return iComHandMonitor->RespondComHandClient( aResultCode );
    }
    
    
void CPhCltCallNotify::ConstructL() 
    {
    iCallNotify = new (ELeave) RPhCltCallNotify();
    iDialMonitor = CPhCltDialRequestMonitor::NewL(*iCallNotify);
    iComHandMonitor = CPhCltComHandRequestMonitor::NewL(*iCallNotify);
    iDialEmergencyMonitor = CPhCltDialEmergencyRequestMonitor::NewL(*iCallNotify);
    }

// End of File
