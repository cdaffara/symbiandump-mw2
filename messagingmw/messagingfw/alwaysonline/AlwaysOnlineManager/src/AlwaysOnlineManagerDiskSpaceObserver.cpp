/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*     CAlwaysOnlineDiskSpaceObserver implementation file
*
*
*/


// INCLUDE FILES
#include <e32base.h>
#include <MuiuServiceUtilities.h>          // Muiu Utilities
#include <systemwarninglevels.hrh>

#include "AlwaysOnlineManagerDiskSpaceObserver.h"
#include "AlwaysOnlineManager.h"
#include "AlwaysOnlineManagerLogging.h"


// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// CAlwaysOnlineDiskSpaceObserver::CAlwaysOnlineDiskSpaceObserver
//
// C++ constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CAlwaysOnlineDiskSpaceObserver::CAlwaysOnlineDiskSpaceObserver( 
    CAlwaysOnlineManager* aManager, 
    CMsvSession& aSession )
    : 
    CActive( CActive::EPriorityStandard ),
    iManager( *aManager ),
    iSession( aSession ),    
    iNewLimit( 0 ),
    iCurrentLimit( 0 )
    {
    CActiveScheduler::Add( this );
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineDiskSpaceObserver::NewL
//
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CAlwaysOnlineDiskSpaceObserver* CAlwaysOnlineDiskSpaceObserver::NewL( 
    CAlwaysOnlineManager* aManager, 
    CMsvSession& aSession )
    {
    CAlwaysOnlineDiskSpaceObserver* self = new ( ELeave )
        CAlwaysOnlineDiskSpaceObserver( aManager, aSession );
    return self;
    }

    
// ----------------------------------------------------------------------------
// CAlwaysOnlineDiskSpaceObserver::~CAlwaysOnlineDiskSpaceObserver
//
// Destructor
// ----------------------------------------------------------------------------
//
CAlwaysOnlineDiskSpaceObserver::~CAlwaysOnlineDiskSpaceObserver()
    {
    Cancel();
    }


// ----------------------------------------------------------------------------
// CAlwaysOnlineDiskSpaceObserver::DoCancel
//
// From active object framework
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineDiskSpaceObserver::DoCancel()
    {
    if ( IsActive() )
        {
        iSession.FileSession().NotifyDiskSpaceCancel( iStatus );
        }
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineDiskSpaceObserver::RunL
//
// From active object framework
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineDiskSpaceObserver::RunL()
    {
    if ( iStatus.Int() == KErrNone )
        {
        // timer or file server event?
        SetLimitAndActivateL();
        }
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineDiskSpaceObserver::RunError()
// ----------------------------------------------------------------------------
//
TInt CAlwaysOnlineDiskSpaceObserver::RunError( TInt /* aError */ )
    {
    // Just ignore any error and continue without
    // any handling to allow smooth execution. 
    return KErrNone;            
    }

// ----------------------------------------------------------------------------
// CAlwaysOnlineDiskSpaceObserver::SetLimitAndActivateL
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineDiskSpaceObserver::SetLimitAndActivateL()
    {
    KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineDiskSpaceObserver::SetLimitAndActivateL() Received DISKSPACE event");
    Cancel();

    // Handle the possible change
    HandleLimitChange();

    // Get current drive
    TInt currentDrive = iSession.CurrentDriveL();

    // Make the disk space check. The safety margin is used to keep disk space
    // from dropping below critical level
    if ( !MsvUiServiceUtilities::DiskSpaceBelowCriticalLevelWithOverheadL( 
            iSession, 0, KAOSafetyMargin ) )
        {           
        KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineDiskSpaceObserver::SetLimitAndActivateL() Event: Above Critical");
        iManager.HandleDiskSpaceEventL( EAOManagerDiskSpaceAboveCritical );
        }
    else
        {
        KAOMANAGER_LOGGER_WRITE("CAlwaysOnlineDiskSpaceObserver::SetLimitAndActivateL() Event: Below Critical");
        iManager.HandleDiskSpaceEventL( EAOManagerDiskSpaceBelowCritical );
        }        

    // Issue new request
    iSession.FileSession().NotifyDiskSpace( iCurrentLimit, currentDrive, iStatus );
    SetActive();    
    }

// ----------------------------------------------------------------------------
// CImumDiskSpaceObserverOperation::HandleLimitChange()
// ----------------------------------------------------------------------------
//
void CAlwaysOnlineDiskSpaceObserver::HandleLimitChange()    
    {
    if( !iNewLimit )
        {
        iCurrentLimit =  KDRIVECCRITICALTHRESHOLD + KAOSafetyMargin;
        }
    else
        {
        iCurrentLimit = iNewLimit;
        }
    } 

//  End of File  

