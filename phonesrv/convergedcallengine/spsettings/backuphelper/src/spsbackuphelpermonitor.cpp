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
* Description:  Service provider settings backup helper monitor
*
*/

#include "spsapilogger.h"
#include "spsbackuphelpermonitor.h"
#include "spsbackuphelperperformer.h"




// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CSpsBackupHelperMonitor* CSpsBackupHelperMonitor::NewL()
    {
    CSpsBackupHelperMonitor* self = new ( ELeave ) CSpsBackupHelperMonitor;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CSpsBackupHelperMonitor::~CSpsBackupHelperMonitor()
    {
    Cancel();
    iProperty.Close();
    delete iPerformer;
    delete iABClient;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CSpsBackupHelperMonitor::CSpsBackupHelperMonitor() : 
    CActive( CActive::EPriorityStandard )
    {
    
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperMonitor::ConstructL()
    {
    TInt err( KErrNone );
    
    // Add to active scheduler
    CActiveScheduler::Add(this);
    
    // Set up the property to catch the BUR flag.
    TInt backupStateValue = 0;
    iProperty.Attach( KUidSystemCategory, conn::KUidBackupRestoreKey );

    // Check current state to see if we were started for backup purposes
    iProperty.Get(backupStateValue);
    
    if (!NoBackupRestore(backupStateValue))
        {
        iABClient = CActiveBackupClient::NewL();
            
        // Confirm that this data owner is ready for backup/restore operations
        iABClient->ConfirmReadyForBURL(KErrNone);
        }
    
    // Subscribe to the P&S flag to catch transitions
    Subscribe();
    
    // Construct performer
    iPerformer = CSpsBackupHelperPerformer::NewL();
    
    TRAP( err, ProcessBackupStateL(backupStateValue) );
    XSPSLOGSTRING2( "CSpsBackupHelperMonitor::ConstructL, %i", err );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperMonitor::ProcessBackupStateL( TInt aBackupStateValue )
    {
    XSPSLOGSTRING2( "CSpsBackupHelperMonitor::ProcessBackupStateL, %i", aBackupStateValue );
    
    // Do special logic when restore has ended
    if( RestoreOngoing( aBackupStateValue ) )
        {
        XSPSLOGSTRING( "CSpsBackupHelperMonitor::ProcessBackupStateL, restore started" );
        // Restore started.
        iParticipateRestore = ETrue;
        }
    else if( NoBackupRestore( aBackupStateValue ) && iParticipateRestore )
        {
        XSPSLOGSTRING( "CSpsBackupHelperMonitor::ProcessBackupStateL, restore ended" );
        // Increase priority so that restore process wont be interrupted so easily
        RProcess().SetPriority( ::EPriorityHigh );
        
        // Performer available and no backup or restore ongoing
        // -> Restore has been ended.
        iPerformer->PerformL();
        }
    
    if( NoBackupRestore( aBackupStateValue ) )
        {
        XSPSLOGSTRING( "CSpsBackupHelperMonitor::ProcessBackupStateL, cleanup" );
        iParticipateRestore = EFalse;
        }
    
    XSPSLOGSTRING( "CSpsBackupHelperMonitor::ProcessBackupStateL OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TBool CSpsBackupHelperMonitor::RestoreOngoing( TInt aBackupStateValue )
    {
    // Full or partial restore started/ongoing
    TBool ret = 
        ( ( ( aBackupStateValue & conn::EBURRestoreFull ) == 
            conn::EBURRestoreFull ) ||
        ( ( aBackupStateValue & conn::EBURRestorePartial ) == 
            conn::EBURRestorePartial ) );
    
    return ret;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TBool CSpsBackupHelperMonitor::NoBackupRestore( TInt aBackupStateValue )
    {
    // Not set or no backup or restore ongoing
    TBool ret = 
        ( ( aBackupStateValue & KBURPartTypeMask ) == conn::EBURUnset ||
        ( aBackupStateValue & KBURPartTypeMask ) == conn::EBURNormal );
    
    return ret;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperMonitor::Subscribe()
    {
    iStatus = KRequestPending;
    iProperty.Subscribe(iStatus);
    SetActive();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperMonitor::RunL()
    {
    XSPSLOGSTRING( "CSpsBackupHelperMonitor::RunL IN" );
    // re-subscribe to the flag to monitor future changes
    Subscribe();
    CheckStatusL();
    XSPSLOGSTRING( "CSpsBackupHelperMonitor::RunL OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CSpsBackupHelperMonitor::RunError( TInt /*aError*/ )
    {
    XSPSLOGSTRING( "CSpsBackupHelperMonitor::RunError" );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperMonitor::DoCancel()
    {
    iProperty.Cancel();
    }

void CSpsBackupHelperMonitor::CheckStatusL()
    {
    TInt backupInfo =0;
    iProperty.Get(backupInfo);
    
    // Process the mode change accordingly
    ProcessBackupStateL(backupInfo);

    if (NoBackupRestore(backupInfo))
        {
        delete iABClient;
        iABClient = NULL;
        }
    else 
        {
        if (iABClient == NULL)
            {
            iABClient = CActiveBackupClient::NewL();
            }
        
        TDriveList driveList;
        TBURPartType partType;
        TBackupIncType incType;
        TInt err;
        TRAP(err, iABClient->BURModeInfoL(driveList, partType, incType));
        if (err != KErrNone)
            {
            XSPSLOGSTRING("BURModeInfoL error");
            }
        
        TBool amIaffected = ETrue;
        
        if (partType == EBURRestorePartial || partType == EBURBackupPartial)
            {
            TRAP(err, amIaffected = iABClient->DoesPartialBURAffectMeL());
            if (err != KErrNone)
                {
                User::After(5000000);
                TRAP(err, amIaffected = iABClient->DoesPartialBURAffectMeL());
                }
            }
        
        if (amIaffected)
            {
            // Confirm that this data owner is ready for backup/restore operations
            iABClient->ConfirmReadyForBURL(KErrNone);
            }
        }
    }


// End of file

