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
* Description:  Service provider settings backup helper performer
*
*/

#include <e32base.h>
#include <spsettings.h>
#include <spentry.h>
#include <centralrepository.h>

#include "spsapilogger.h"
#include "spsbackuphelperperformer.h"
#include "spcrkeys.h"

// Constants

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CSpsBackupHelperPerformer* CSpsBackupHelperPerformer::NewL()
    {
    CSpsBackupHelperPerformer* self = new ( ELeave ) CSpsBackupHelperPerformer;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CSpsBackupHelperPerformer::~CSpsBackupHelperPerformer ()
    {
    delete iServiceProviderSettings;
    iEntries.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
CSpsBackupHelperPerformer::CSpsBackupHelperPerformer()
    {

    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperPerformer::ConstructL()
    {
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::ConstructL IN" );
    iServiceProviderSettings = CSPSettings::NewL();
    
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::ConstructL OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperPerformer::PerformL()
    {
    // Read restored settings
    ReadSettingsL();
    
    // Reset to factory settings
    ResetToFactorySettingsL();
    
    // Write extra service id's
    FinalizeRestoreL();
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperPerformer::ReadSettingsL()
    {
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::ReadSettingsL IN" );
    RIdArray ids;
    CleanupClosePushL( ids );
    iServiceProviderSettings->FindServiceIdsL( ids );
    
    // Make sure that id's are in correct order!
    for( TInt i = 0; i < ids.Count(); i++ )
        {
        TRAP_IGNORE( 
            CSPEntry* entry = CSPEntry::NewLC();
            User::LeaveIfError( 
                iServiceProviderSettings->FindEntryL( ids[i], *entry ) );
            
            // Make sure that all properties are loaded
            entry->GetAllProperties(); 
            iEntries.AppendL( entry );
            CleanupStack::Pop( entry );
            )

        }
    
    XSPSLOGSTRING2( 
        "CSpsBackupHelperPerformer::ReadSettingsL Service count=%i", 
        iEntries.Count() );
    
    CleanupStack::PopAndDestroy( &ids );
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::ReadSettingsL OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperPerformer::FinalizeRestoreL()
    {
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::FinalizeRestoreL IN" );
    
    // Determine how many settings exists after factory settings
    TUint nextFreeServiceId = NextFreeServiceIdL();
    
    // Restore old settings
    // Make sure that id's are in correct order!
    for( TInt i = 0; i < iEntries.Count(); i++ )
        {
        // Start inputting restored settings from next free serviceid
        if( iEntries[i]->GetServiceId() == nextFreeServiceId )
            {
            // Service id to be inserver is same as next free serviceid
            nextFreeServiceId = iEntries[i]->GetServiceId();
            iServiceProviderSettings->AddEntryL( *iEntries[i] );
            nextFreeServiceId++; // Entry added
            }
        else if( iEntries[i]->GetServiceId() > nextFreeServiceId )
            {
            // Change inserted service id when service id is larger
            // than current service count.
            // This is done since backed up settings can be non linear
            SetNextFreeServiceIdL( iEntries[i]->GetServiceId() );
            nextFreeServiceId = iEntries[i]->GetServiceId();
            iServiceProviderSettings->AddEntryL( *iEntries[i] );
            nextFreeServiceId++; // Entry added
            }
        
        }
    
    XSPSLOGSTRING2( 
        "CSpsBackupHelperPerformer::FinalizeRestoreL Next free serviceid=%i", 
        nextFreeServiceId );
    
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::FinalizeRestoreL OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperPerformer::ResetToFactorySettingsL()
    {
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::ResetToFactorySettingsL IN" );
    // Ok if leave ( No service provider settings )
    CRepository* cenrep = CRepository::NewLC( KCRUidSPSettings );
    User::LeaveIfError( cenrep->Reset() );
    CleanupStack::PopAndDestroy( cenrep );
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::ResetToFactorySettingsL OUT" );
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
TInt CSpsBackupHelperPerformer::NextFreeServiceIdL()
    {
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::NextFreeServiceIdL IN" );
    TInt ret(0);
    CRepository* cenrep = CRepository::NewLC( KCRUidSPSettings );
    
    User::LeaveIfError( cenrep->Get( KServiceIdCounter, ret ) );
    
    CleanupStack::PopAndDestroy( cenrep );
    XSPSLOGSTRING2( "CSpsBackupHelperPerformer::NextFreeServiceIdL OUT, %i", ret );
    return ret;
    }

// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void CSpsBackupHelperPerformer::SetNextFreeServiceIdL( TInt aServiceId )
    {
    XSPSLOGSTRING2( "CSpsBackupHelperPerformer::SetNextFreeServiceIdL IN, %i", aServiceId );
    CRepository* cenrep = CRepository::NewLC( KCRUidSPSettings );
    
    User::LeaveIfError( cenrep->Set( KServiceIdCounter, aServiceId ) );
    
    CleanupStack::PopAndDestroy( cenrep );
    XSPSLOGSTRING( "CSpsBackupHelperPerformer::SetNextFreeServiceIdL OUT" );
    }


// End of file
