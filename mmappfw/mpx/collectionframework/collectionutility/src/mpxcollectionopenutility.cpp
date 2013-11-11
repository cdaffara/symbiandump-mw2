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
* Description:  Incremental OpenL() utility
*
*/


#include <e32base.h>
#include <mpxmessage.h>
#include <mpxcollectionpath.h>
#include <mpxcollectionutility.h>
#include <mpxcollectionmessagedefs.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxmediageneraldefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxincrementalopendefs.h>
#include <mpxmediacontainerdefs.h>
#include <mpxcollectionmessage.h>
#include <mpxlog.h>
#include <mpxcollectionopenlresultdef.h>
#include <mpxcollectioncommanddefs.h>
#include "mpxcollectionopenutility.h"
#include <e32math.h>

// CONSTANTS
const TInt KArrayGranularity = 5;

// ---------------------------------------------------------------------------
// static function to compare two datablock items
// used for sorting datablocks
// ---------------------------------------------------------------------------
//
static TInt CompareAsc( const TMPXOpenDataBlock& aFirst,
                        const TMPXOpenDataBlock& aSecond )
    {
    return aFirst.iOffset > aSecond.iOffset;
    }

// ---------------------------------------------------------------------------
// static function to compare two datablock items
// used for sorting datablocks
// ---------------------------------------------------------------------------
//
static TInt CompareDsc( const TMPXOpenDataBlock& aFirst,
                        const TMPXOpenDataBlock& aSecond )
    {
    return aFirst.iOffset < aSecond.iOffset;
    }

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXCollectionOpenUtility::CMPXCollectionOpenUtility( MMPXCollectionObserver* aObs,
                                                      TUid aMode ) 
                                                    : iFetchStep(EFetchNone),
                                                      iObs( aObs ),
                                                      iIncrementalChunks(KArrayGranularity),
                                                      iMode( aMode )
                                                      
    {
    }


// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXCollectionOpenUtility::ConstructL()
    {
    iTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionOpenUtility* CMPXCollectionOpenUtility::NewL( 
                                                 MMPXCollectionObserver* aObs,  
                                                 TUid aMode )
    {
    CMPXCollectionOpenUtility* self = 
                       new( ELeave ) CMPXCollectionOpenUtility( aObs, aMode );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXCollectionOpenUtility::~CMPXCollectionOpenUtility()
    {
    if( iCollection )
      {
      iCollection->Close();
      }
    
    if( iTimer )
        {
        iTimer->Cancel();
        delete iTimer;
        }
    
    iIncrementalChunks.Close();
    delete iPath;
    delete iMedia;
    }

// ---------------------------------------------------------------------------
// Start the incremental fetching operation
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionOpenUtility::StartL( TArray<TMPXAttribute> /*aAttrs*/,
                                                 TInt aChunkSize,
                                                 TInt aOffset,
                                                 TDirection aDirection,
                                                 TMPXAttribute aKeyAttribute )
    {
    // Assert we are idle
    ASSERT( iFetchStep == EFetchNone );
    MPX_DEBUG1("CMPXCollectionOpenUtility::StartL <---");
    // Copy the operation data
    iFetchInfo.iSize = aChunkSize;
    iFetchInfo.iOffset = aOffset;
    iFetchDirection = aDirection;
    iKeyAttribute = aKeyAttribute;
    iData = NULL;
    delete iMedia;
    iMedia = NULL;
    delete iPath; 
    iPath = NULL;
    // Start reading
    iFetchStep = EFetchCount;
    RunNext();
    MPX_DEBUG1("CMPXCollectionOpenUtility::StartL --->");
    }

// ---------------------------------------------------------------------------
// Start the incremental fetching operation
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionOpenUtility::StartL( const CMPXCollectionPath& aPath,
                                                 TArray<TMPXAttribute> /*aAttrs*/,
                                                 TInt aChunkSize,
                                                 TInt aOffset,
                                                 TDirection aDirection,
                                                 TMPXAttribute aKeyAttribute )
    {
    // Assert we are idle and isolated mode
    MPX_ASSERT( iFetchStep == EFetchNone );
    MPX_ASSERT( iMode == KMcModeIsolated || iMode == KMcModePlaylist );
    MPX_DEBUG1("CMPXCollectionOpenUtility::StartL aPath <---");
    if( !iCollection )
        {
        iCollection = MMPXCollectionUtility::NewL( this, iMode ); 
        }
    
    // Copy the operation data
    MPX_DEBUG_PATH( aPath );
    iFetchInfo.iSize = aChunkSize;
    iFetchInfo.iOffset = aOffset;
    iFetchDirection = aDirection;
    iKeyAttribute = aKeyAttribute;
    iData = NULL;
    delete iMedia;
    iMedia = NULL;
    
    delete iPath;
    iPath = NULL;
    iPath = CMPXCollectionPath::NewL( aPath );
    
    // Start the fetch operation by setting up the context
    // to the correct path
    //
    iFetchStep = EFetchPath;
    RunNext();
    
    MPX_DEBUG1("CMPXCollectionOpenUtility::StartL --->");
    }

// ---------------------------------------------------------------------------
// Stop the incremental fetching operation
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionOpenUtility::Stop()
    {
    // Cancel all outstanding collection open requests
    MPX_DEBUG1("CMPXCollectionOpenUtility::Stop <---");
    if( iCollection )
        {
        iCollection->Collection().CancelRequest();
        }
    iTimer->Cancel();
    iIncrementalChunks.Reset();
    delete iMedia; 
    iMedia = NULL;
    iData = NULL;
    iFetchStep = EFetchNone;
    MPX_DEBUG1("CMPXCollectionOpenUtility::Stop --->");
    }

// ---------------------------------------------------------------------------
// Set the direction of the incremental fetching algorithm
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionOpenUtility::SetDirection( TDirection aDirection )
    {
    iFetchDirection = aDirection;

    if( iFetchStep == EFetchItems || iFetchStep == EFetchCommand || iFetchStep == EFetchCount )
        {
        TBool skipFirst = iFetchStep == EFetchCommand ? ETrue : EFalse;
        
        if( iFetchDirection == EFetchDown )
            {
            DoSortAscend( skipFirst );
            }
        else if( iFetchDirection == EFetchUp )
            {
            DoSortDescend( skipFirst );
            }
        else if( iFetchDirection == EFetchNormal )
            {
            TRAP_IGNORE( DoSortNormalL( skipFirst ) );
            }
        }
    }

// ---------------------------------------------------------------------------
// Set the fetching delay
// ---------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionOpenUtility::SetDelay( TInt aDelay )
    {
    iFetchDelay = aDelay;
    }

// ---------------------------------------------------------------------------
// Get the current path
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPath* CMPXCollectionOpenUtility::PathL()
    {
    CMPXCollectionPath* p(NULL);
    
    if( iCollection )
        {
        p = iCollection->Collection().PathL();
        }
    return p;
    }
    
// ---------------------------------------------------------------------------
// Handle Collection Message
// ---------------------------------------------------------------------------
//
void CMPXCollectionOpenUtility::HandleCollectionMessage(CMPXMessage* aMsg, TInt aErr)
    {
    if( aErr == KErrNone && aMsg )
        {
        TRAP_IGNORE( DoHandleCollectionMessageL( *aMsg ) );
        }
    }

 // ---------------------------------------------------------------------------
 // HandleOpenL
 // ---------------------------------------------------------------------------
 //
void CMPXCollectionOpenUtility::HandleOpenL(const CMPXMedia& aEntries,
                                            TInt aIndex,
                                            TBool aComplete,
                                            TInt aError)
    {
    if( iFetchStep == EFetchCount )
        {
        DoHandleCountL( aEntries, aIndex, aError );
        }
    else if( iFetchStep == EFetchItems )
        {
        DoHandleFetchItemsL( aEntries, aIndex, aError );
        }
    else
        {
        MPX_DEBUG1("CMPXCollectionOpenUtility::HandleOpenL EFetchPath/EFetchNone");
        iFetchStep = EFetchNone;
        iObs->HandleOpenL( aEntries, aIndex, aComplete, aError );
        }
    }
 
 // ---------------------------------------------------------------------------
 // HandleOpenL
 // ---------------------------------------------------------------------------
 //
void CMPXCollectionOpenUtility::HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,
                                            TInt aError)
    {
    iObs->HandleOpenL( aPlaylist, aError );
    }
 
 // ---------------------------------------------------------------------------
 // Handle Command Complete
 // ---------------------------------------------------------------------------
 //
void CMPXCollectionOpenUtility::HandleCommandComplete(CMPXCommand* aCommandResult, 
                                                      TInt aError )
    {
    // Continue fetching items
    iFetchStep = EFetchItems;
    TRAP_IGNORE( DoHandleCommandCompleteL( *aCommandResult, aError ) );
    }
 
 // ---------------------------------------------------------------------------
 // Handle Collection Media
 // ---------------------------------------------------------------------------
 //
void CMPXCollectionOpenUtility::HandleCollectionMediaL(const CMPXMedia& /*aMedia*/, 
                                                       TInt /*aError*/)
    {
    // Do Nothing
    }

// ---------------------------------------------------------------------------
// Handle Collection Messages
// ---------------------------------------------------------------------------
//
void CMPXCollectionOpenUtility::DoHandleCollectionMessageL( const CMPXMessage& aMsg )
    {
    TMPXMessageId id( aMsg.ValueTObjectL<TMPXMessageId>( KMPXMessageGeneralId ) );
    if ( KMPXMessageGeneral == id &&
         iFetchStep == EFetchPath )
        {
        TInt event( aMsg.ValueTObjectL<TInt>( KMPXMessageGeneralEvent ) );
        TInt type( aMsg.ValueTObjectL<TInt>( KMPXMessageGeneralType ) );
        TInt data( aMsg.ValueTObjectL<TInt>( KMPXMessageGeneralData) );
        if ( event == TMPXCollectionMessage::EPathChanged && 
            type == EMcPathChangedByOpen )
            {
            MPX_DEBUG1("CMPXCollectionOpenUtility::DoHandleCollectionMessageL -- path opened");
            
            if( data == EMcContainerOpened)
                {
                iFetchStep = EFetchCount;
                RunNext();
                }
            else if( data == EMcItemOpened )  // only 2 cases
                {
                // Playlist, simply forward back to the observer
                iCollection->Collection().OpenL();
                iFetchStep = EFetchNone;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// Handle Command complete
// ---------------------------------------------------------------------------
//
void CMPXCollectionOpenUtility::DoHandleCommandCompleteL(CMPXCommand& /*aCommandResult*/, 
                                                         TInt aError)
    {
    if( iSelecting )
        {
        iSelecting = EFalse;
        }
    else if( aError == KErrNone )
        {
        // Get the data to return to the client, callback at HandleOpen()
        //
        // Implemented because the command tells the plugin to fetch the 
        // next block of data. However, the actual media object containing 
        // the current browse data may have been re-allocated. We cannot
        // assume that the data pointer remains the same. Therefore, it is
        // safer to refetch the data from the collection via OpenL()
        // This call should be FAST because the data has already been read at this point
        // 
        MPX_DEBUG1("CMPXCollectionOpenUtility::HandleCommandComplete OpenL <---");
        if( iIncrementalChunks.Count() == 1 || iFirstOpen )
            {
            DoSelectIndexL();
            }
        iCollection->Collection().OpenL();
        }
    else if( aError == KErrNotReady )
        {
        // Cache was deleted, need to restart the fetching operation
        Stop();
        iFetchStep = EFetchCount;
        iCollection->Collection().OpenL();
        }
    else // aError != KErrNone 
        {
        // Error occured, such as not supported
        MPX_DEBUG2("CMPXCollectionOpenUtility::HandleCommandComplete Error %i <---", aError);
        iFetchStep = EFetchNone;
        
        CMPXMedia* temp = CMPXMedia::NewL();
        CleanupStack::PushL( temp );    
        iObs->HandleOpenL( *temp, 0, ETrue, aError );
        CleanupStack::PopAndDestroy( temp );
        }    
    }

// ---------------------------------------------------------------------------
// Fetch count step
// ---------------------------------------------------------------------------
//
void CMPXCollectionOpenUtility::DoFetchCountL()
    {
    if( !iCollection )
        {
        iCollection = MMPXCollectionUtility::NewL( this, iMode ); 
        }
    
    // Ask for the total item count
    // Cache current iPath
    delete iPath;
    iPath = NULL;
    iPath = iCollection->Collection().PathL();
    
    iCollection->Collection().OpenL();
    }

// ---------------------------------------------------------------------------
// Handle the count step
// ---------------------------------------------------------------------------
//
void CMPXCollectionOpenUtility::DoHandleCountL(const CMPXMedia& aEntries, 
                                               TInt aIndex,
                                               TInt aError)
    {
    ASSERT( iFetchStep == EFetchCount );
    
    MPX_DEBUG2("CMPXCollectionOpenUtility::DoHandleCountL error %i <---", aError);
    TBool done(EFalse);
    if( aError == KErrNone )
        {
        // Do we support incremental for this OpenL()?
        //
        TBool supportIncremental(EFalse);
        if( aEntries.IsSupported( KMPXCollectionOpenLSupportsIncremental ) )
            {
            supportIncremental = aEntries.ValueTObjectL<TBool>( 
                                        KMPXCollectionOpenLSupportsIncremental );
            }
        
        if( supportIncremental )
            {
            TInt count(0);
            if( aEntries.IsSupported(KMPXMediaArrayCount) )
                {
                count = aEntries.ValueTObjectL<TInt>(KMPXMediaArrayCount);
                }
            
            if( iFetchInfo.iOffset == KErrNotFound )
                {
                // If the client did not specify an offset
                // we use the index the collection is pointing to
                // as the starting offset
                //
                iFetchInfo.iOffset = aIndex;
                }
            if( count > 0 )
                {
                // Perform the chunking here
                DoSetupChunksL( count );
                DoCompactTaskListL( aEntries );
                
                MPX_DEBUG2("CMPXCollectionOpenUtility::DoHandleCountL count %i", iIncrementalChunks.Count() );
                if( iIncrementalChunks.Count() )
                    {
                    // Start running the fetching algorithm
                    iFetchStep = EFetchItems; 
                    iFirstOpen = ETrue;
                    
                    // Sync up the path, as there could have been items deleted
                    delete iPath;
                    iPath = NULL;
                    iPath = iCollection->Collection().PathL();
                    
                    if( iObs )
                        {
                        iObs->HandleOpenL( aEntries, aIndex, EFalse, aError );
                        }
                    RunNext();
                    }
                else 
                    {
                    // Update index before returning
                    DoSelectIndexL();
                    iFetchStep = EFetchNone;
                    iCollection->Collection().OpenL();
                    done = EFalse;
                    }     
                }
            else
                {
                done = ETrue;
                }    
            }
        else // !supportIncremental
            {
            // Just return the results
            //
            MPX_DEBUG1("CMPXCollectionOpenUtility::DoHandleCountL incremental not supported");
            done = ETrue;    
            }        
        
        }
    if( (aError != KErrNone) || done )
        {
        // Stop the OpenL() operation
        iFetchStep = EFetchNone;
        if( iObs )
            {
            iObs->HandleOpenL( aEntries, aIndex, ETrue, aError );
            }
        }
    }

// ---------------------------------------------------------------------------
// Fetch items step
// ---------------------------------------------------------------------------
//
void CMPXCollectionOpenUtility::DoFetchItemsL()
    {
    // Construct a command to fetch the next chunk
    //
    MPX_DEBUG1("CMPXCollectionOpenUtility::DoFetchItemsL <---" );
    if( iIncrementalChunks.Count() > 0 )
        {
        CMPXCommand* command = CMPXCommand::NewL();
        CleanupStack::PushL( command );
    
        // Construct the command
        //
        TInt colId( iPath->Id(0) );
        TInt offset( iIncrementalChunks[0].iOffset );
        command->SetTObjectValueL( KMPXCommandGeneralId,
                                   KMPXCommandIdIncrementalOpenL );
        command->SetTObjectValueL( KMPXCommandGeneralCollectionId,
                                   colId );
        command->SetCObjectValueL(KMPXCollectionCommandIdIncOpenLPath,iPath );
        command->SetTObjectValueL(KMPXCollectionCommandIdIncOpenLOffset, 
                                  offset );
        command->SetTObjectValueL(KMPXCollectionCommandIdIncOpenLNumItems, iFetchInfo.iSize);
        
        // If some partial data is available
        // try to optimize this by using the ascending or decending keys
        //
        if( iData )
            {
            if( offset-1 > 0 && offset-1 < iData->Count() &&
                iData->AtL(offset-1)->IsSupported(iKeyAttribute) )
                {
                command->SetTObjectValueL(KMPXCollectionCommandIdIncOpenLAscDsc, EReadAscending);
                const TDesC& key = iData->AtL(offset-1)->ValueText(iKeyAttribute);
                command->SetTextValueL(KMPXCollectionCommandIdIncOpenLKeyItem,key);
                }
            else if( offset+iFetchInfo.iSize < iData->Count() &&
                     iData->AtL(offset+iFetchInfo.iSize)->IsSupported(iKeyAttribute) )
                {
                command->SetTObjectValueL(KMPXCollectionCommandIdIncOpenLAscDsc, EReadDescending);
                const TDesC& key = iData->AtL(offset+iFetchInfo.iSize)->ValueText(iKeyAttribute);
                command->SetTextValueL(KMPXCollectionCommandIdIncOpenLKeyItem,key);
                }
            }
        
        // Command sent to the collection, callback HandleCommandComplete()
        //
        MPX_DEBUG2("CMPXCollectionOpenUtility::DoFetchItemsL offset %i <---", offset );
        iFetchStep = EFetchCommand;
        iCollection->Collection().CommandL( *command );
        CleanupStack::PopAndDestroy( command );
        }
    MPX_DEBUG1("CMPXCollectionOpenUtility::DoFetchItemsL --->" );
    }

// ---------------------------------------------------------------------------
// Handle Completion of fetch items step
// ---------------------------------------------------------------------------
//  
void CMPXCollectionOpenUtility::DoHandleFetchItemsL( const CMPXMedia& aEntries,
                                                     TInt aIndex, 
                                                     TInt aError )
    {
    MPX_DEBUG1("CMPXCollectionOpenUtility::DoHandleFetchItemsL <---" ); 
    // Task is done, and compact the list
    //
    if( iIncrementalChunks.Count() )
        {
        iIncrementalChunks.Remove(0);
        DoCompactTaskListL( aEntries );
        }
    else
        {
        MPX_DEBUG1("CMPXCollectionOpenUtility::DoHandleFetchItemsL No more chunks" ); 
        }
        
    TBool complete = iIncrementalChunks.Count() == 0 ? ETrue : EFalse;
    
    // Keep a reference here 
    //
    delete iMedia;
    iMedia = NULL;
    iMedia = CMPXMedia::NewL( aEntries );
    iData = NULL;
    if( aEntries.IsSupported( KMPXMediaArrayContents ) )
        {
        iData = aEntries.Value<CMPXMediaArray>( KMPXMediaArrayContents );
        User::LeaveIfNull(iData);
        }
    
    // Run the next step of the fetching algorithm
    // Do not run if stopped
    //
    TInt count(iIncrementalChunks.Count());
    if( count != 0 && iFetchStep != EFetchNone )   
        {
        // Update the path, as the item IDs will be filled
        //
        MPX_DEBUG2("CMPXCollectionOpenUtility::DoHandleFetchItemsL run next %i", count ); 
        delete iPath;
        iPath = NULL;
        iPath = iCollection->Collection().PathL();
    
        // Command to fetch next chunk
        //
        RunNext();
        }
    else
        {
        // All done
        iFetchStep = EFetchNone;
        }

    // Callback to observer
    //
    if( iObs )
        {
        iFirstOpen = EFalse;
        MPX_DEBUG1("CMPXCollectionOpenUtility::DoHandleFetchItemsL callback" ); 
        iObs->HandleOpenL( aEntries, aIndex, complete, aError );
        }
    
    MPX_DEBUG1("CMPXCollectionOpenUtility::DoHandleFetchItemsL -->" );     
    }

// ---------------------------------------------------------------------------
// Run the next step
// ---------------------------------------------------------------------------
//    
void CMPXCollectionOpenUtility::RunNext()
    {
    TCallBack cb( Callback,this );
    iTimer->Cancel();
    iTimer->Start( TTimeIntervalMicroSeconds32( iFetchDelay ),
                      TTimeIntervalMicroSeconds32( iFetchDelay ),
                      cb);
    }

// ---------------------------------------------------------------------------
// Set some callback
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionOpenUtility::Callback( TAny* aAny )
    {
    TRAP_IGNORE( ((CMPXCollectionOpenUtility*)aAny)->HandleCallbackL() );
    return ETrue;
    }

// ---------------------------------------------------------------------------
// HandleCallback from iTimer
// ---------------------------------------------------------------------------
//
void CMPXCollectionOpenUtility::HandleCallbackL()
    {
    switch( iFetchStep )
        {
        case EFetchPath:
            {
            iCollection->Collection().OpenL( *iPath );
            break;
            }    
        case EFetchCount:
            {
            DoFetchCountL();
            break;
            }
        case EFetchItems:
            {
            DoFetchItemsL();
            break;
            }
        case EFetchNone:
            {
            break; // do nothing    
            }
        default:
            {
            MPX_ASSERT(0);
            break;
            }
        }
    // Run once only
    iTimer->Cancel();
    }

// ---------------------------------------------------------------------------
// Setup the chunks
// ---------------------------------------------------------------------------
// 
void CMPXCollectionOpenUtility::DoSetupChunksL( TInt aCount )
    {
    // Number of chunks we are going to have
    // And the chunk the offset is going to be at
    //
    iIncrementalChunks.Reset();
    
    TInt numChunks(0);
    if( aCount > 0 )
        {
        // Truncate and add 1 if any modulus, no Ceil() function 
        numChunks = (TInt)aCount/iFetchInfo.iSize;
        if( aCount % iFetchInfo.iSize )
            {
            numChunks++;    
            }
        }
    TInt offSetChunk = (TInt) iFetchInfo.iOffset / iFetchInfo.iSize ;
    
    // First chunk
    TMPXOpenDataBlock chunk;
    chunk.iOffset = offSetChunk*iFetchInfo.iSize;
    chunk.iSize = iFetchInfo.iSize;
    iIncrementalChunks.AppendL( chunk );
    
    // Left and right of the first chunk
    TInt left = offSetChunk-1;
    TInt right = offSetChunk+1;
    TInt count(numChunks-1);
    while( count > 0 )
        {
        // Append Left Chunk
        //
        TInt temp(left);
        if( left < 0 )
            {
            // Wrap around to end of the list
            temp = numChunks + left;
            }
        chunk.iOffset = temp*iFetchInfo.iSize;
        iIncrementalChunks.AppendL( chunk );    
        MPX_DEBUG2("Adding Chunk %i", temp);
        
        count--;
        left--;
        
        // Append Right Chunk
        if( temp != right%numChunks )
            {
            chunk.iOffset = (right%numChunks)*iFetchInfo.iSize;
            iIncrementalChunks.AppendL( chunk );
            MPX_DEBUG2("Adding Chunk %i", right%numChunks);
            
            count--;
            right++;
            }
        }
    
    // Set the direction and re-order as necessary
    SetDirection( iFetchDirection );
    }

// ---------------------------------------------------------------------------
// Sort the internal array ascending
// ---------------------------------------------------------------------------
// 
void CMPXCollectionOpenUtility::DoSortAscend( TBool aSkipFirst )
    {
    if( iIncrementalChunks.Count() > 0 )
        {
        TMPXOpenDataBlock tmp;
        if( aSkipFirst )
            {
            tmp = iIncrementalChunks[0];
            iIncrementalChunks.Remove(0);
            }
        iIncrementalChunks.Sort( CompareAsc );
        
        if( aSkipFirst )
            {
            iIncrementalChunks.Insert( tmp, 0 );    
            }
        }
    
#ifdef _DEBUG
    TInt c=iIncrementalChunks.Count();
    for( TInt i=0; i<c; ++i )
        {
        MPX_DEBUG2("Order %i", iIncrementalChunks[i].iOffset);
        }
#endif // _DEBUG
    }

// ---------------------------------------------------------------------------
// Sort the internal array decending
// ---------------------------------------------------------------------------
// 
void CMPXCollectionOpenUtility::DoSortDescend( TBool aSkipFirst )
    {
    if( iIncrementalChunks.Count() > 0 )
        {
        TMPXOpenDataBlock tmp;
        if( aSkipFirst )
            {
            tmp = iIncrementalChunks[0];
            iIncrementalChunks.Remove(0);
            }
        iIncrementalChunks.Sort( CompareDsc );
        
        if( aSkipFirst )
            {
            iIncrementalChunks.Insert( tmp, 0 );    
            }
        }
               
#ifdef _DEBUG
    TInt c=iIncrementalChunks.Count();
    for( TInt i=0; i<c; ++i )
        {
        MPX_DEBUG2("Order %i", iIncrementalChunks[i].iOffset);
        }
#endif // _DEBUG
    }

// ---------------------------------------------------------------------------
// Sort the internal array in normal form
// ---------------------------------------------------------------------------
// 
void CMPXCollectionOpenUtility::DoSortNormalL( TBool aSkipFirst )
    {
    DoSortAscend( aSkipFirst );
    
    RArray<TMPXOpenDataBlock> temp;
    CleanupClosePushL( temp );
    
    TBool start = aSkipFirst ? 1:0;
    TInt count( iIncrementalChunks.Count() );
    TInt c(count);
    if( aSkipFirst )
        {
        temp.AppendL( iIncrementalChunks[0] );
        count--;    
        }
    
    TInt left(iIncrementalChunks.Count()/2);
    TInt right = left+1;
    while( count > 0)
        {
        if( left>=start )
            {
            temp.AppendL( iIncrementalChunks[left] );
            left--;
            count--;
            }
        if( right<c )
            {
            temp.AppendL( iIncrementalChunks[right] );
            right++;
            count--;
            }
        }
    
    // Copy the results
    iIncrementalChunks.Reset();
    c= temp.Count();
    for( TInt i=0; i<c; ++i )
        {
        iIncrementalChunks.AppendL( temp[i] );
        }
    CleanupStack::PopAndDestroy( &temp );
    }

// ---------------------------------------------------------------------------
// Compact the internal task list
// ---------------------------------------------------------------------------
// 
void CMPXCollectionOpenUtility::DoCompactTaskListL( const CMPXMedia& aMedia )
    {
    RArray<TMPXOpenDataBlock> datablocks;
    CleanupClosePushL( datablocks );
    
    TInt c( iIncrementalChunks.Count() );

    if( aMedia.IsSupported(KMPXCollectionOpenLAllResultRange) )
        {
        // De-serialize from global data
        //
        const TDesC& buf = aMedia.ValueText(KMPXCollectionOpenLAllResultRange);
        CBufBase* buffer(NULL);
        MPXUser::CreateBufferL( buf, buffer );
        CleanupStack::PushL( buffer );
        ::CreateFromBufferL( *buffer, datablocks );
        CleanupStack::PopAndDestroy( buffer );
        
        // Remove un-necessary fetches
        TInt c2( datablocks.Count() );
        for( TInt i=c-1; i>=0; --i )
            {
            TMPXOpenDataBlock& chunk = iIncrementalChunks[i];
            for( TInt j=0; j<c2; ++j )
                {
                TMPXOpenDataBlock b = datablocks[j];
                
                if( chunk.iOffset >= b.iOffset && 
                    chunk.iOffset+chunk.iSize <= b.iOffset+b.iSize )
                    {
                    // Don't need to fetch what is already
                    // available
                    //
                    iIncrementalChunks.Remove( i );
                    break;
                    }
                }
            }
        }
    
    CleanupStack::PopAndDestroy( &datablocks );    
    }

// ---------------------------------------------------------------------------
// Update the selection index
// ---------------------------------------------------------------------------
// 
void CMPXCollectionOpenUtility::DoSelectIndexL()
    {
    CMPXCommand* command = CMPXCommand::NewL();
    CleanupStack::PushL( command );

    // Construct the command
    //
    command->SetTObjectValueL( KMPXCommandGeneralId,
                               KMPXCommandIdCollectionSelect );
    iCollection->Collection().CommandL( *command );
    iSelecting = ETrue;
    CleanupStack::PopAndDestroy( command );
    }
// END OF FILE
