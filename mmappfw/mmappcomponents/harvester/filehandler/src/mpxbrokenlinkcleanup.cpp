/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active object to cleanup broken links
*
*/


#include <e32base.h>
#include <mpxlog.h>
#include <badesca.h>

#include "mpxfilescanstateobserver.h"
#include "mpxbrokenlinkcleanupobserver.h"
#include "mpxbrokenlinkcleanup.h"

// CONSTANTS
const TInt KDeleteGranularity = 10;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXBrokenLinkCleanup::CMPXBrokenLinkCleanup( MMPXFileScanStateObserver& aObserver,
                                              MMPXBrokenLinkObserver& aBrokenLinkObserver ) 
                                           : CActive( EPriorityNull ),
                                             iStateObserver( aObserver ),
                                             iBrokenLinkObserver( aBrokenLinkObserver )
    {
    CActiveScheduler::Add( this );
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXBrokenLinkCleanup::ConstructL()
    {
    iBrokenLinks = new(ELeave) CDesCArrayFlat(2);  // magic
    }

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXBrokenLinkCleanup* CMPXBrokenLinkCleanup::NewL( MMPXFileScanStateObserver& aObserver,
                                                    MMPXBrokenLinkObserver& aBrokenLinkObserver )
    {
    CMPXBrokenLinkCleanup* self = new( ELeave ) CMPXBrokenLinkCleanup( aObserver,
                                                                       aBrokenLinkObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Virtual destructor
// ---------------------------------------------------------------------------
//
CMPXBrokenLinkCleanup::~CMPXBrokenLinkCleanup()
    {
    Cancel();
    Reset();
    delete iBrokenLinks;
    iColIds.Close();
    iDBRefs.Close();
    }

// ---------------------------------------------------------------------------
// Start the broken link cleanup
// ---------------------------------------------------------------------------
//
void CMPXBrokenLinkCleanup::Start()
    {
    MPX_DEBUG1("CMPXBrokenLinkCleanup::Start <---");
    
    // If this isn't already running
    //
    if( !IsActive() )
        {
        iCleaningUp = ETrue;
        
        // Set Active
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );    
        MPX_DEBUG1("CMPXBrokenLinkCleanup::Started");
        }
    }

// ---------------------------------------------------------------------------
// Reset this object's state
// ---------------------------------------------------------------------------
//
void CMPXBrokenLinkCleanup::Reset()
    {
    iBrokenLinks->Reset();
    iColIds.Reset();
    iDBRefs.Reset();
    }

// ---------------------------------------------------------------------------
// Adds a broken link for cleanup
// ---------------------------------------------------------------------------
//
void CMPXBrokenLinkCleanup::AddBrokenLinkL( const TDesC& aFile,
                                            TInt aColId,
                                            CMPXHarvesterDB* aDb )
    {
    iBrokenLinks->AppendL( aFile );
    iColIds.AppendL( aColId );
    iDBRefs.AppendL( aDb );
    }

// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
//
void CMPXBrokenLinkCleanup::DoCancel()
    {
    if( iCleaningUp )
        {
        MPX_DEBUG1("CMPXBrokenLinkCleanup::DoCancel <---");
        // Callback to observer 
        Reset();
        TRAP_IGNORE( iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::ECleanupBrokenLink,
                                                              KErrCancel ) );
        iCleaningUp = EFalse;
        }
    }
    

// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
//
void CMPXBrokenLinkCleanup::RunL()
    {
    TBool done(EFalse);
    TRAPD( err, done = DoBrokenLinkL() );
    if( KErrNone != err || done )
        {
        // Callback to observer 
        MPX_DEBUG1("CMPXBrokenLinkCleanup::RunL -- Done");
        TRAP_IGNORE( iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::ECleanupBrokenLink,
                                                              err ) );
        iCleaningUp = EFalse;
        }
    else
        {
        MPX_DEBUG1("CMPXBrokenLinkCleanup::RunL -- Run again");
        iStatus = KRequestPending;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );    
        }
    }

// ---------------------------------------------------------------------------
// From CActive
// ---------------------------------------------------------------------------
//
TInt CMPXBrokenLinkCleanup::RunError(TInt aError)
    {
    MPX_DEBUG1("CMPXBrokenLinkCleanup::RunError <---");
    Reset();
    TRAP_IGNORE( iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::ECleanupBrokenLink,
                                                          aError ) );
    iCleaningUp = EFalse;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Perform one iteration of the broken link check
// ---------------------------------------------------------------------------
//
TBool CMPXBrokenLinkCleanup::DoBrokenLinkL()
    {
    MPX_DEBUG1("CMPXBrokenLinkCleanup::DoBrokenLinkL <---");
    TBool done(EFalse);
    
    // Call back to observer to perform the cleanup
    TInt count(iBrokenLinks->Count());
    iBrokenLinkObserver.HandleBrokenLinkL( *iBrokenLinks,
                                           iColIds,
                                           iDBRefs,
                                           count<KDeleteGranularity?count:KDeleteGranularity );
    
    // Loop through and remove KDeleteGranularity elements
    // from the broken links array
    //
    for( TInt i=0; i<KDeleteGranularity; ++i )
        {
        // No more to delete
        if( count == 0 )
            {
            done = ETrue;
            break;
            }
        
        iBrokenLinks->Delete(0);
        count--;
        }
    MPX_DEBUG1("CMPXBrokenLinkCleanup::DoBrokenLinkL --->");
    return done;
    }
// END OF FILE
