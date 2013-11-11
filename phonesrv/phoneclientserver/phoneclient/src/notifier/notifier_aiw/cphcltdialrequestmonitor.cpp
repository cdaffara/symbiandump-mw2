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
#include    <mphcltdialrequestobserver.h>

#include    "rphcltserver.h" 
#include    "cphcltextphonedialdata.h" 
#include    "cphcltdialdata.h"
#include    "cphcltdialrequestmonitor.h"
#include    "rphcltcallnotify.h" 




// ============================= LOCAL FUNCTIONS ===============================



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhCltDialRequestMonitor::CPhCltDialRequestMonitor
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhCltDialRequestMonitor::CPhCltDialRequestMonitor(RPhCltCallNotify& aCallNotify) 
    :  CActive( EPriorityStandard ), 
       iCallNotify(aCallNotify),
      iDataLengthPckg ( iDialDataLength )
    {
    CActiveScheduler::Add( this );
    }
    
// -----------------------------------------------------------------------------
// CPhCltDialRequestMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhCltDialRequestMonitor* CPhCltDialRequestMonitor::NewL(
        RPhCltCallNotify& aCltNotify)
    {
    CPhCltDialRequestMonitor* self = new ( ELeave ) CPhCltDialRequestMonitor(aCltNotify); 
    return self;
    }
    
    // Destructor
CPhCltDialRequestMonitor::~CPhCltDialRequestMonitor()
    {  
    Cancel();
    }

// -----------------------------------------------------------------------------
// CPhCltDialRequestMonitor::NotifyCallAttempt
// 
// -----------------------------------------------------------------------------
//
void CPhCltDialRequestMonitor::NotifyDialRequest( 
    MPhCltDialRequestObserver* aObserver )
    {
    if ( !IsActive() )
        {
        iCallNotify.NotifyCallAttempt( iStatus, iDataLengthPckg ); 
        iObserver = aObserver;
        SetActive();
        }
    }



// -----------------------------------------------------------------------------
// CPhCltDialRequestMonitor::RespondClient
// 
// -----------------------------------------------------------------------------
//
TInt CPhCltDialRequestMonitor::RespondClient( const TInt aResultCode )
    {
    return iCallNotify.RespondClient( aResultCode  );
    }

// -----------------------------------------------------------------------------
// CPhCltDialRequestMonitor::RunL()
// 
// -----------------------------------------------------------------------------
//    
void CPhCltDialRequestMonitor::RunL()
    {
    TInt error = iStatus.Int();
    if ( !error ) 
        {
        // Lazy initialization for dial data.
        CPhCltDialData *dialData = (CPhCltDialData*) CPhCltExtPhoneDialData::NewL();

        CleanupStack::PushL( dialData );
            
        HBufC8* package = HBufC8::NewLC ( iDialDataLength );
        TPtr8 ptr ( package->Des());
        
        //Fetch dial data from server
        iCallNotify.GetDialData( ptr );
        ((CPhCltExtPhoneDialData*)dialData)->SetParamsL( *package );
        CleanupStack::PopAndDestroy( package );
        CleanupStack::Pop( dialData );
        iObserver->DialRequest( dialData );
        }
    
    }

TInt CPhCltDialRequestMonitor::RunError(TInt /*aErr*/)
    {
    // RunL failed because of leave in SetParamsL,
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhCltDialRequestMonitor::DoCancel()
// 
// -----------------------------------------------------------------------------
//    
void CPhCltDialRequestMonitor::DoCancel()
    {
    iCallNotify.CancelNotifyCallAttempt();    
    }


// End of File
