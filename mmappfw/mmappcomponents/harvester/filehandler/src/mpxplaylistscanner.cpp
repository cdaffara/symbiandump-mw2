/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class to handle playlist extraction
*
*/


#include <e32base.h>
#include <mpxplaylistengine.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxcollectiontype.h>
#include <mpxmediageneraldefs.h>
#include "mpxplaylistscanner.h"
#include "mpxplaylistscanobserver.h"
#include "mpxfilescanstateobserver.h"

// CONSTANTS
const TInt KExtractionCount = 2;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------------
//
CMPXPlaylistScanner::CMPXPlaylistScanner( MMPXPlaylistScanObserver& aObs,
                                          MMPXFileScanStateObserver& aStateObs,
                                          RPointerArray<CMPXCollectionType>& aTypes ) : 
                                          iObserver( aObs ),
                                          iStateObserver( aStateObs ),
                                          iSupportedTypes(aTypes)
    {
    }


// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::ConstructL()
    {
    iPlEngine = CMPXPlaylistEngine::NewL( *this );
    iPlList = new(ELeave) CDesCArrayFlat(1); // granularity
    iPlUpdateList = new(ELeave) CDesCArrayFlat(1); // granularity
    
    iPropArray = CMPXMediaArray::NewL();
    iUpdatePropArray = CMPXMediaArray::NewL();
    }


// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXPlaylistScanner* CMPXPlaylistScanner::NewL(
                                       MMPXPlaylistScanObserver&  aObs,
                                       MMPXFileScanStateObserver& aStateObs,
                                       RPointerArray<CMPXCollectionType>& aTypes )
    {
    CMPXPlaylistScanner* self = new(ELeave) CMPXPlaylistScanner( aObs,
                                                                 aStateObs,
                                                                 aTypes );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Virtual destructor
// ---------------------------------------------------------------------------
//
CMPXPlaylistScanner::~CMPXPlaylistScanner()
    {
    if( iPlList )
        {
        iPlList->Reset();
        delete iPlList;
        }
    if( iPlUpdateList )
        {
        iPlUpdateList->Reset();
        delete iPlUpdateList;
        }
    
    if( iPropArray )
        {
        iPropArray->Reset();
        }
    delete iPropArray;
    
    if( iUpdatePropArray )
        {
        iUpdatePropArray->Reset();
        }
    delete iUpdatePropArray;
    
    if( iPlEngine )
        {
        iPlEngine->CancelRequests();     
        }
    delete iPlEngine;
    }

// ---------------------------------------------------------------------------
// Start the playlist scanning procedure
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::ScanL()
    {
    iNewPlPending = iPlList->Count();
    iUpdatePlPending = iPlUpdateList->Count();
    
    // Add all files in the list to the playlist engine queue.
    //
    if( iNewPlPending || iUpdatePlPending )
        {
        for( TInt i=0; i<iNewPlPending; ++i )
            {
            iPlEngine->InternalizePlaylistL( (*iPlList)[i] );
            }
        for( TInt i=0; i<iUpdatePlPending; ++i )
            {
            iPlEngine->InternalizePlaylistL( (*iPlUpdateList)[i] );
            }
        iExtracting = ETrue;
        }
    // No files to extract
    // 
    else
        {
        iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::EScanPlaylists,
                                                  KErrNone );    
        }   
    }

// ---------------------------------------------------------------------------
// Cancel the playlist scanning procedure
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::Cancel()
    {
    if( iExtracting )
        {
        iPlEngine->CancelRequests(); 
        iExtracting = EFalse;
        TRAP_IGNORE(iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::EScanPlaylists, 
                                                             KErrCancel );  
                    );      
        }
    }

// ---------------------------------------------------------------------------
// Reset the object
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::Reset()
    {
    iPlList->Reset();
    iPlUpdateList->Reset();
    iPropArray->Reset();
    iUpdatePropArray->Reset();
    }

// ---------------------------------------------------------------------------
// Add a file to the list to be extracted
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::AddPlaylistToScanL( const TDesC& aFile )
    {
    iPlList->AppendL( aFile );
    }

// ---------------------------------------------------------------------------
// Add a file to the list to be updated
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::AddUpdatedPlaylistToScanL( const TDesC& aFile )
    {
    iPlUpdateList->AppendL( aFile );
    }
    
// ---------------------------------------------------------------------------
// Handles playlist import
// Note: aCompleted is not in use for Increment 8, for future usage.
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::HandlePlaylistL( CMPXMedia* aPlaylist,
                                           const TInt aError,
                                           const TBool /*aCompleted*/ )
    {
    // Only add parsable playlists
    //     
    if( aError == KErrNone )
        {
        // We take ownership of the media object
        //
        CleanupStack::PushL( aPlaylist );
        
        // Find what collection this belongs to
        //
        const TDesC& file = aPlaylist->ValueText( 
                     TMPXAttribute( KMPXMediaIdGeneral, EMPXMediaGeneralUri));
        TParsePtrC parse( file );
        TInt index(KErrNotFound); 
        TInt count( iSupportedTypes.Count() );
        for (TInt i=0; i <count; ++i)
            {
            TInt index2(KErrNotFound);    
            const CDesCArray& exts = iSupportedTypes[i]->Extensions();
            if (!exts.FindIsq(parse.Ext(), index2))
                { // found 
                index = i;
                break;
                }
            }
        
        // Found?
        if( index == KErrNotFound )
            {
            // Unsupported by a collection
            //
            CleanupStack::PopAndDestroy( aPlaylist );      
            }
        else
            {
            // Set collection ID and add to array
            //
            aPlaylist->SetTObjectValueL( TMPXAttribute( KMPXMediaIdGeneral, 
                                             EMPXMediaGeneralCollectionId),
                                         iSupportedTypes[index]->Uid() );
            if( iNewPlPending )
                {
                iPropArray->AppendL( aPlaylist );  // owned by array
                }
            else
                {
                iUpdatePropArray->AppendL( aPlaylist ); // owned by array
                } 
            CleanupStack::Pop( aPlaylist );
            }
        }
    else // an error occured
        {
        // Playlist engine should have set this to NULL
        // (Just in case)
        delete aPlaylist; 
        }
        
    if( iNewPlPending )
        {
        iNewPlPending--;
        }
    else // Some update ones pending
        {
        iUpdatePlPending--;    
        }
        
    // All done!
    //
    TInt pending = iNewPlPending + iUpdatePlPending;  
    if( pending == 0 && iExtracting )
        {
        CompleteFilesL();
        iExtracting = EFalse;
        
        iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::EScanPlaylists, 
                                                 KErrNone );
        }
    // Enough to send to the collection
    //
    else if( iPropArray->Count() + iUpdatePropArray->Count() 
             == KExtractionCount )
        {
        CompleteFilesL();
        }
    } //lint !e961

// ---------------------------------------------------------------------------
// Handles Playlist export to a file
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::HandlePlaylistL(const TDesC& /*aPlaylistUri*/,
                                          const TInt /*aError*/)
    {
    // not used
    }

// ---------------------------------------------------------------------------
// Checks if a file is a playlist file 
// ---------------------------------------------------------------------------
//
TBool CMPXPlaylistScanner::IsPlaylistFileL( const TDesC& aFile )
    {
    return iPlEngine->IsPlaylistL( aFile );
    }

// ---------------------------------------------------------------------------
// Upload the next list of files
// ---------------------------------------------------------------------------
//
void CMPXPlaylistScanner::CompleteFilesL()
    {
    TInt err(KErrNone);
    if( iPropArray->Count() )
        {
        TRAP( err, iObserver.AddPlaylistToCollectionL( *iPropArray ) );
        }
    if( iUpdatePropArray->Count() )
        {
        TRAP( err, iObserver.UpdatePlaylistToCollectionL( *iUpdatePropArray ) );
        }
        
    if( err != KErrNone )
        {
        // Cancel requests and notify end
        iStateObserver.HandleScanStateCompleteL( MMPXFileScanStateObserver::EScanPlaylists, 
                                                  err );        
        iPlEngine->CancelRequests();        
        }
    Reset();
    }

