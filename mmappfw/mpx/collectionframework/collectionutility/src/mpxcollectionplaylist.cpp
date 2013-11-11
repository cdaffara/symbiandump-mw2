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
* Description:  implementation of collection playlist
*
*/

#include <e32math.h>
#include <mpxcollectionpath.h>
#include <mpxcmn.h>
#include <mpxcollectionutility.h>
#include <mpxcollectionmessage.h>
#include <mpxmediageneraldefs.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxmedia.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include <mpxlog.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxcollectionmessagedefs.h>
#include <mpxmessagecontainerdefs.h>

#include <mpxcollectionopenutility.h>

#include "mpxcollectionplaylistobserver.h"
#include "mpxcollectionplaylist.h"

// CONSTANTS
const TInt KIncrementalFetchSize = 1000;
const TInt KIncrementalDelayNone = 0;
const TInt KIncrementalDelay = 100000;  // 100 ms

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPlaylist* CMPXCollectionPlaylist::NewL()
    {
    MPX_DEBUG1("-->CMPXCollectionPlaylist::NewL()"); 
    CMPXCollectionPlaylist* p = new(ELeave) CMPXCollectionPlaylist(NULL);
    CleanupStack::PushL(p);
    p->ConstructL();
    CleanupStack::Pop(p);
    MPX_DEBUG2("<--CMPXCollectionPlaylist::NewL() playlist 0x%08x", p);
    return p;
    }

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPlaylist* CMPXCollectionPlaylist::NewL(
    const CMPXCollectionPlaylist& aPlaylist,
    MMPXCollectionPlaylistObserver* aPlObs /*= NULL*/)
    {
    MPX_DEBUG2("-->CMPXCollectionPlaylist::NewL(pl, plobs) with plobs 0x%08x", aPlObs); 
    CMPXCollectionPlaylist* p = new(ELeave) CMPXCollectionPlaylist(aPlObs);
    CleanupStack::PushL(p);
    p->ConstructL(aPlaylist);
    CleanupStack::Pop(p);
    MPX_DEBUG3("<--CMPXCollectionPlaylist::NewL(pl, plobs) playlist 0x%08x, , plobs 0x%08x",
               p, aPlObs);  
    return p;
    }
    
// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPlaylist* CMPXCollectionPlaylist::NewL(
    const CMPXCollectionPath& aPath,
    MMPXCollectionPlaylistObserver* aPlObs /*= NULL*/)
    {
    MPX_DEBUG2("-->CMPXCollectionPlaylist::NewL(path, plobs) with plobs 0x%08x", aPlObs); 
    CMPXCollectionPlaylist* p = new(ELeave) CMPXCollectionPlaylist(aPlObs);
    CleanupStack::PushL(p);
    p->ConstructL(aPath);
    CleanupStack::Pop(p);
    MPX_DEBUG3("<--CMPXCollectionPlaylist::NewL(pl, plobs) playlist 0x%08x, , plobs 0x%08x",
               p, aPlObs);  
    return p;
    }
    
// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPlaylist* CMPXCollectionPlaylist::NewL(
    RReadStream& aStream,
    MMPXCollectionPlaylistObserver* aPlObs /*= NULL*/)
    {
    MPX_DEBUG2("-->CMPXCollectionPlaylist::NewL(strm, plobs) with plobs 0x%08x", aPlObs); 
    CMPXCollectionPlaylist* p = new(ELeave) CMPXCollectionPlaylist(aPlObs);
    CleanupStack::PushL(p);
    p->ConstructL(aStream);
    CleanupStack::Pop(p);
    MPX_DEBUG3("<--CMPXCollectionPlaylist::NewL(pl, plobs) playlist 0x%08x, , plobs 0x%08x",
               p, aPlObs);  
    return p;
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
CMPXCollectionPlaylist::CMPXCollectionPlaylist(
    MMPXCollectionPlaylistObserver* aPlObs)
:   iRepeatMode(ERepeatOff), iShuffle(EFalse), iRemote(EFalse), 
    iItemIndex(KErrNotFound),iEmbedded(EFalse), iPlObs(aPlObs),
    iRepeatEnabled(ETrue),
    iShuffleEnabled(ETrue), iAutoPlaylist(EFalse),
    iAutoPlay(ETrue), iSingleItemPlaylist(EFalse),
    iPreInitPlugin(ETrue)
    {
    }
    
// ----------------------------------------------------------------------------
// 2nd phase constructor. 
// ----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::ConstructL(const CMPXCollectionPath& aPath)
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::ConstructL(aPath)");
    iPath=CMPXCollectionPath::NewL(aPath); 
    iIncOpenUtil = CMPXCollectionOpenUtility::NewL( this, KMcModePlaylist );
    SetShuffleL(EFalse, EFalse);
    if (!iAutoPlaylist)
        {
        DoIncrementalOpenL();
        }
    }
    
// ----------------------------------------------------------------------------
// 2nd phase constructor. 
// ----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::ConstructL(
    const CMPXCollectionPlaylist& aPlaylist)
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::ConstructL(aPlaylist)");
    iIncOpenUtil = CMPXCollectionOpenUtility::NewL( this, KMcModePlaylist );
    CopyContentL(aPlaylist);
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor. 
// ----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::ConstructL(RReadStream& aStream)
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::ConstructL(aStream)");
    InternalizeL(aStream);
    iIncOpenUtil = CMPXCollectionOpenUtility::NewL( this, KMcModePlaylist );
    if (!iAutoPlaylist)
        {
        DoIncrementalOpenL();
        }
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor. 
// ----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::ConstructL()
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::ConstructL()");
    iPath = CMPXCollectionPath::NewL();
    iIncOpenUtil = CMPXCollectionOpenUtility::NewL( this, KMcModePlaylist );
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPlaylist::~CMPXCollectionPlaylist()
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::~CMPXCollectionPlaylist");
    delete iPath;
    iItemIndexes.Close();
    if (iColUtil)
        {
        iColUtil->Close();
        }
    delete iIncOpenUtil;
    }

// -----------------------------------------------------------------------------
// Copy playlist content from other playlist except observers and session to 
// collection server
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPlaylist::CopyContentL(
    const CMPXCollectionPlaylist& aPlaylist)
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::CopyContentL(aPlaylist)");
    iRepeatMode=aPlaylist.iRepeatMode;
    iRepeatEnabled=aPlaylist.iRepeatEnabled;
    iShuffle=aPlaylist.iShuffle;
    iShuffleEnabled=aPlaylist.iShuffleEnabled;
    iRemote=aPlaylist.iRemote;
    delete iPath;
    iPath = NULL;
    iPath=CMPXCollectionPath::NewL(aPlaylist.Path());
    ::CopyArrayL(aPlaylist.iItemIndexes.Array(), iItemIndexes);
    iItemIndex=aPlaylist.iItemIndex;
    iEmbedded=aPlaylist.iEmbedded;
    iAutoPlaylist=aPlaylist.iAutoPlaylist;
    iAutoPlay=aPlaylist.iAutoPlay;
    iSingleItemPlaylist=aPlaylist.iSingleItemPlaylist;
    iPreInitPlugin=aPlaylist.iPreInitPlugin;
    
    if( !iColUtil )
        {
        iColUtil=MMPXCollectionUtility::NewL(this ,KMcModePlaylist);  
        }
    
    if (!iAutoPlaylist)
        {
        DoIncrementalOpenL();
        }
    }

// -----------------------------------------------------------------------------
// Advances path to next item
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPlaylist::Next( 
    TBool aIgnoreRepeat /*=EFalse*/ )
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::Next()");
    TInt nextPlIndex( KErrNotFound );
    TBool ret( NextIndex( aIgnoreRepeat, nextPlIndex ));
    if ( KErrNotFound != nextPlIndex )
        {
        iPath->Set( iItemIndexes[nextPlIndex] );
        if ( iShuffle && iItemIndex == Count()-1 )
            {
            MPX_TRAPD( err, SetShuffleL( iShuffle, ETrue ));
            if ( KErrNone != err )
                {
                ret = EFalse;
                iItemIndex = 0;
                iPath->Set( iItemIndexes[0] );
                }
            }
        else
            {
            iItemIndex = nextPlIndex;
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// Gets the next index to play
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPlaylist::NextIndex( 
    TBool aIgnoreRepeatOneMode,
    TInt& aPlaylistIndex ) const
    {
    MPX_FUNC("CMPXCollectionPlaylist::NextIndex()");

    TInt count( Count() );
    TBool ret( ETrue );
    if ( count <= 0 )
        {
        ret = EFalse;
        aPlaylistIndex = KErrNotFound;
        }
    else 
        {
        MPX_ASSERT( iItemIndex != -1 );
        aPlaylistIndex = iItemIndex;

        // If not in repeat one mode, then go to the next item,
        // otherwise leave index unchanged
        if (( ERepeatOne != iRepeatMode ) ||
            ( aIgnoreRepeatOneMode ))
            {
            // If not the last one in the list, find the next valid item
            if ( count-1 != aPlaylistIndex )
                {
                do
                    {
                    ++aPlaylistIndex;
                    }
                while( aPlaylistIndex != count && 
                        iPath->IdOfIndex( iItemIndexes[aPlaylistIndex] )
                        == KMPXInvalidItemId );
                }

            // If Last one in list, set to first
            if ( count-1 == iItemIndex || count == aPlaylistIndex )
                {
                if ( ERepeatOff == iRepeatMode )
                    {
                    ret = EFalse;
                    }

                // If shuffle mode is on, choose a song to be the first song in the new
                // randomized list.  This chooses the middle song, not exactly random,
                // but this ensures that if this method is called multiple times
                // it would return a consistent result.  The reason is because this
                // method maybe called multiple times in a row by different clients, 
                // so we want to make sure that each client returns the same result.
                // If shuffle is off then choose the first item in list.
                if ( iShuffle )
                    {
                    aPlaylistIndex = count / 2;
                    }
                else
                    {
                    aPlaylistIndex = 0;
                    }
                }
            }
        }
    MPX_DEBUG3("CMPXCollectionPlaylist::NextIndex %d %d", ret, aPlaylistIndex);
    return ret;
    }


// -----------------------------------------------------------------------------
// Gets the path index of the item given it's position in the playlist.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCollectionPlaylist::PathIndex( TInt aPlaylistIndex ) const
    {
    MPX_FUNC("CMPXCollectionPlaylist::PathIndex");
    MPX_ASSERT(aPlaylistIndex < Count());
    return iItemIndexes[aPlaylistIndex];
    }

// -----------------------------------------------------------------------------
// Advances path to previous item
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPlaylist::Previous( 
    TBool aIgnoreRepeat /*=EFalse*/ )
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::Previous");
    TBool ret=ETrue;

    TInt count( Count() );
    if ( count <= 0 )
        {
        ret = EFalse;
        }
    else if ( ERepeatOne != iRepeatMode ||
              aIgnoreRepeat )
        {
        // If first one in list, set to last
        if ( 0 == iItemIndex )
            {
            if (iShuffle)
                { // re-shuffle
                TRAP_IGNORE(SetShuffleL(iShuffle, EFalse));
                }
            SetToLast();
            }
        else
            {
            // Find the next valid item
            //
            do
                {
                --iItemIndex;
                if (iItemIndex>=0 && iItemIndex < iItemIndexes.Count())
                    {
                    iPath->Set(iItemIndexes[iItemIndex]);
                    }    
                }
            while( iPath->Id() == KMPXInvalidItemId && 
                   iItemIndex != KErrNotFound );
                   
            if( iItemIndex < 0 )
                {
                iItemIndex=0;
                ret=Previous();
                }
            }
        }
    MPX_DEBUG2("CMPXCollectionPlaylist::PreviousL %i", iPath->Index());
    return ret;
    }

// -----------------------------------------------------------------------------
// Sets path to first item
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPlaylist::SetToFirst()
    {
    if (Count())
        {    
        iItemIndex=0;
        iPath->Set(iItemIndexes[iItemIndex]);
        }
    }
    
// -----------------------------------------------------------------------------
// Sets path to last item
// -----------------------------------------------------------------------------
//   
EXPORT_C void CMPXCollectionPlaylist::SetToLast()
    {
    if (Count())
        {    
        iItemIndex=Count()-1;
        iPath->Set(iItemIndexes[iItemIndex]);
        }
    }

// -----------------------------------------------------------------------------
// Sets path to a particular index
// -----------------------------------------------------------------------------
//   
EXPORT_C void CMPXCollectionPlaylist::SetToIndex( TInt aIndex )
    {
    MPX_ASSERT( aIndex >= -1 && aIndex < Count() );
    iItemIndex = aIndex;
    
    // Special case, the collection path does not support -1 no selection
    if( iItemIndex != -1 )
        {
        iPath->Set(iItemIndexes[iItemIndex]);
        }
    }

// -----------------------------------------------------------------------------
// Returns the number of items in the same container
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCollectionPlaylist::Count() const
    {
    return iItemIndexes.Count();
    }

// -----------------------------------------------------------------------------
// Returns the depth into the collection (0 = root level, 1 = first level etc.)
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt CMPXCollectionPlaylist::Levels() const
    {
    return iPath ? iPath->Levels() : 0; 
    }

// -----------------------------------------------------------------------------
// Returns play ordinal of current item in the playlist
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt CMPXCollectionPlaylist::Index() const
    {
    TInt index(KErrNotFound);
    if (Count() && iItemIndex < Count())
        {
        index = iItemIndex;
        }
    return index;
    }

// -----------------------------------------------------------------------------
// Is the collection stored on a remote device
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPlaylist::Remote() const
    {
    MPX_DEBUG2("-->CMPXCollectionPlaylist::Remote iColUtil 0x%08x", iColUtil);
    CMPXCollectionPlaylist *s = const_cast<CMPXCollectionPlaylist*>(this);
    if (!s->iColUtil && iPath)
        {
        TRAP_IGNORE(s->iColUtil=MMPXCollectionUtility::NewL(s, KMcModePlaylist));
        if (s->iColUtil)
            {
            s->iRemote=s->iColUtil->Collection().IsRemote(*iPath);
            }
        }
    MPX_DEBUG2("<--CMPXCollectionPlaylist::Remote iColUtil 0x%08x", iColUtil);
    return iRemote;
    }

// -----------------------------------------------------------------------------
// Repeat mode
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCollectionPlaylist::TRepeatMode 
    CMPXCollectionPlaylist::RepeatMode() const
    {
    return iRepeatMode;
    }

// -----------------------------------------------------------------------------
// Shuffle
// -----------------------------------------------------------------------------
//    
EXPORT_C TBool CMPXCollectionPlaylist::Shuffle() const
    {
    return iShuffle;
    }

// -----------------------------------------------------------------------------
// Collection path
// -----------------------------------------------------------------------------
//    
EXPORT_C const CMPXCollectionPath& CMPXCollectionPlaylist::Path() const
    {
    return *iPath;
    }
    
// -----------------------------------------------------------------------------
// Get media properties for current item
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMPXCollectionPlaylist::MediaL(
    const TArray<TMPXAttribute>& aAttrs,
    MMPXCollectionMediaObserver& aMediaObs)
    {
    MPX_DEBUG4("-->CMPXCollectionPlaylist::MediaL 0x%08x, iColUtil 0x%08x, obs 0x%08x", 
               this, iColUtil, &aMediaObs);
    iMediaObs=&aMediaObs;
    if (!iColUtil)
        {
        iColUtil=MMPXCollectionUtility::NewL(this ,KMcModePlaylist);
        }

#ifdef _DEBUG
    // For debug purposes, to test whether we can ever reach an invalid item
    if( iPath->Id() == KMPXInvalidItemId )
        {
        MPX_DEBUG1("CMPXCollectionPlaylist::MediaL Unloaded item");
        }
#endif //_DEBUG
        
    iColUtil->Collection().MediaL(*iPath, aAttrs);
    MPX_DEBUG3("<--CMPXCollectionPlaylist::MediaL 0x%08x, iColUtil 0x%08x", 
               this, iColUtil);
    }

// -----------------------------------------------------------------------------
// Sets repeat mode
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPlaylist::SetRepeatMode(TRepeatMode aMode)
    {
    MPX_DEBUG2("CMPXCollectionPlaylist::SetRepeatMode(%d)", aMode);
    if ( iRepeatEnabled )
        {
        iRepeatMode=aMode;
        }
    else
        {
        iRepeatMode = ERepeatOff;
        }
    }

// -----------------------------------------------------------------------------
// Sets repeat enabled
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPlaylist::SetRepeatEnabled( TBool aEnable )
    {
    MPX_DEBUG2("CMPXCollectionPlaylist::SetRepeatEnabled(%d)", aEnable);
    iRepeatEnabled = aEnable;
    if ( !iRepeatEnabled )
        {
        SetRepeatMode( ERepeatOff );
        }
    }

// -----------------------------------------------------------------------------
// Sets shuffle
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMPXCollectionPlaylist::SetShuffleL(
    TBool aShuffle,
    TBool aCurrentToTop)
    {
    MPX_DEBUG3("CMPXCollectionPlaylist::SetShuffleL(%d, %d)", aShuffle, aCurrentToTop);
    
    if ( iShuffleEnabled )
        {
        iShuffle=aShuffle;
        }
    else
        {
        iShuffle = EFalse;
        }
    iItemIndexes.Reset();
    iItemIndex = KErrNotFound;
    TInt currentIndex(iPath->Index());
   
    if (iShuffle)
        {
        if ( iPath->Count() > 0 )
            {
            //
            // First, append item indexes in default order (i.e. 0,1,2...)
            // into a temp array
            //
            RArray<TInt> items;
            CleanupClosePushL(items);
            for (TInt i=0;i<iPath->Count();++i)
                {
                items.AppendL(i); 
                }
            //
            // Get a seed for randomizing
            //
            TTime time;
            time.UniversalTime();
            TInt64 seed = time.Int64();  

            // Make sure current one is first in list.
            if (aCurrentToTop)
                {
                items.Remove(currentIndex);
                iItemIndexes.AppendL(currentIndex);
                }

            TInt count = items.Count();
            //
            // Pull the item indexes from the temp array, randomly,
            // removing pulled item so there are no repetitions
            //    
            for (TInt ii=0;ii<count;++ii)
                {
                TInt randIndex=Math::Rand(seed)%items.Count();
                TInt index=items[randIndex];
                items.Remove(randIndex);
                iItemIndexes.AppendL(index);
                }    
            CleanupStack::PopAndDestroy(&items);
            iItemIndex = 0;
            iPath->Set( iItemIndexes[iItemIndex] );
            }
        }
     else 
        {
        for (TInt i=0;i<iPath->Count();++i)
            {
            iItemIndexes.AppendL(i);// Append items in default order, i.e. 0,1,2...
            }
        // Set to current index
        iItemIndex = currentIndex;
        }
    }

// -----------------------------------------------------------------------------
// Sets shuffle enabled
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMPXCollectionPlaylist::SetShuffleEnabledL( TBool aEnable )
    {
    MPX_DEBUG2("CMPXCollectionPlaylist::SetShuffleEnabled(%d)", aEnable);
    if ( aEnable != iShuffleEnabled )
        {
        iShuffleEnabled = aEnable;
        SetShuffleL( EFalse, EFalse );
        }
    }

// -----------------------------------------------------------------------------
// Externalize object
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPlaylist::ExternalizeL(
    RWriteStream& aStream) const
    {
    aStream<<*iPath;
    aStream.WriteInt32L(iRepeatMode);
    aStream.WriteInt32L(iRemote);
    
    TInt n=iItemIndexes.Count();
    aStream.WriteInt32L(n);
    for (TInt ii=0;ii<n;++ii)
        {
        aStream.WriteInt32L(iItemIndexes[ii]);
        }           
    aStream.WriteInt32L(iShuffle);
    aStream.WriteInt32L(iItemIndex);
    aStream.WriteInt32L(iEmbedded);
    aStream.WriteInt32L(iRepeatEnabled);
    aStream.WriteInt32L(iShuffleEnabled);
    aStream.WriteInt32L(iAutoPlaylist);
    aStream.WriteInt32L(iAutoPlay);
    aStream.WriteInt32L(iSingleItemPlaylist);
    aStream.WriteInt32L(iPreInitPlugin);
    }

// -----------------------------------------------------------------------------
// Internalize object
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPlaylist::InternalizeL(RReadStream& aStream)
    {
    if (iPath)
        {
        delete iPath;
        iPath = NULL;
        }
    iPath=CMPXCollectionPath::NewL();
    aStream>>*iPath;
    iRepeatMode=static_cast<TRepeatMode>(aStream.ReadInt32L());
    iRemote=aStream.ReadInt32L();
    iItemIndexes.Reset();
    TInt n=aStream.ReadInt32L();
    for (TInt ii=0;ii<n;++ii)
        {
        iItemIndexes.AppendL(aStream.ReadInt32L());
        }     
    iShuffle=aStream.ReadInt32L();
    iItemIndex=aStream.ReadInt32L();
    iEmbedded=aStream.ReadInt32L();    
    iRepeatEnabled=aStream.ReadInt32L();
    iShuffleEnabled=aStream.ReadInt32L();
    iAutoPlaylist=aStream.ReadInt32L();
    iAutoPlay=aStream.ReadInt32L();
    iSingleItemPlaylist=aStream.ReadInt32L();
    iPreInitPlugin=aStream.ReadInt32L();
    if (iColUtil)
        {
        iColUtil->Close();
        iColUtil = NULL;
        }
    iColUtil=MMPXCollectionUtility::NewL(this);
    }
    
// -----------------------------------------------------------------------------
// Handle media from colllection
// -----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::HandleCollectionMediaL(
    const CMPXMedia& aMedia, 
    TInt aError)
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::HandleCollectionMediaL");
    iMediaObs->HandleCollectionMediaL(aMedia, aError);
    }

// -----------------------------------------------------------------------------
// HandleCollectionMessage
// -----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::HandleCollectionMessage(
    CMPXMessage* aMessage, 
    TInt aErr)
    {
    if (iPlObs)
        {
        TRAP_IGNORE(DoHandleCollectionMessageL(aMessage, aErr));
        } // pre-init playlist, don't bother to update itself, will be re-synced
    }

// -----------------------------------------------------------------------------
// Handle open event
// -----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::HandleOpenL(
    const CMPXMedia& aEntries,
    TInt aIndex,
    TBool aComplete,
    TInt aError)
    {
    MPX_DEBUG4("-->CMPXCollectionPlaylist::HandleOpenL 0x%08x, iPlObs 0x%08x, iCollectonObs 0x%08x",
              this, iPlObs, iCollectionObs);
    if (KErrNone == aError)
        {
        if ( aEntries.IsSupported(KMPXMediaGeneralNonPermissibleActions ) )
            {
            // check for auto playlist, it is not writable and cacheable
            TMPXGeneralNonPermissibleActions attr(
                    aEntries.ValueTObjectL<TMPXGeneralNonPermissibleActions>( 
                    KMPXMediaGeneralNonPermissibleActions ) );
            if ( (attr & EMPXCache) && (attr & EMPXWrite))
                {
                iAutoPlaylist = ETrue;
                }
            }                                        
        
        if (iPlObs)
            {
            DoHandleIncompleteOpenL(aEntries,aComplete);    
            }
        else if (iCollectionObs)
            { // callback right after swap, notify the playlist of the originator
            iCollectionObs->HandleOpenL(aEntries, aIndex, aComplete, aError);
            }
        }
    
    MPX_DEBUG4("<--CMPXCollectionPlaylist::HandleOpenL 0x%08x, iPlObs 0x%08x, iCollectonObs 0x%08x",
            this, iPlObs, iCollectionObs);
    }

// -----------------------------------------------------------------------------
// Handle open event
// -----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::HandleOpenL(
    const CMPXCollectionPlaylist& aPlaylist,
    TInt aError)
    {
    (void)aPlaylist;
    (void)aError;
    }

// -----------------------------------------------------------------------------
// Set a new observer
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMPXCollectionPlaylist::SetObserver(
    MMPXCollectionMediaObserver& aMediaObs,
    MMPXCollectionPlaylistObserver* aPlObs /*= NULL*/,
    MMPXCollectionObserver* aCollectionObs /*= NULL*/)
    {
    MPX_DEBUG4("-->CMPXCollectionPlaylist::SetObserver 0x%08x, mobs 0x%08x, plobs 0x%08x", 
               this, &aMediaObs, aPlObs);
    iMediaObs=&aMediaObs;
    iPlObs = aPlObs;
    iCollectionObs = aCollectionObs;
    MPX_DEBUG2("<--CMPXCollectionPlaylist::SetObserver 0x%08x", this);
    }

// -----------------------------------------------------------------------------
// Is this an embedded playlist
// -----------------------------------------------------------------------------
// 
EXPORT_C TBool CMPXCollectionPlaylist::EmbeddedPlaylist() const
    {
    return iEmbedded;
    }
    
// -----------------------------------------------------------------------------
// Set a new observer
// -----------------------------------------------------------------------------
// 
EXPORT_C void CMPXCollectionPlaylist::SetEmbeddedPlaylist( TBool aEmbedded )
    {
    iEmbedded = aEmbedded;
    }

// ----------------------------------------------------------------------------
// Set properities
// ----------------------------------------------------------------------------
//     
EXPORT_C void CMPXCollectionPlaylist::SetL(const CMPXMedia& aMedia)
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::SetL()");
    if (!iColUtil)
        {
        iColUtil=MMPXCollectionUtility::NewL(this ,KMcModePlaylist);
        }
    
    CMPXCommand* cmd = CMPXCommand::NewL();
    CleanupStack::PushL( cmd );
    
    cmd->SetTObjectValueL( KMPXCommandGeneralId, KMPXCommandIdCollectionSet );
    cmd->SetTObjectValueL( KMPXCommandGeneralDoSync, EFalse );
    MPX_ASSERT( aMedia.IsSupported(KMPXMediaGeneralCollectionId) );
    TUid id( aMedia.ValueTObjectL<TUid>(KMPXMediaGeneralCollectionId) );
    cmd->SetTObjectValueL( KMPXCommandGeneralCollectionId, id.iUid );
    
    CMPXMedia* tmp = CMPXMedia::NewL(aMedia);
    CleanupStack::PushL( tmp );
    cmd->SetCObjectValueL<CMPXMedia>( KMPXCommandColSetMedia, tmp );
    CleanupStack::PopAndDestroy( tmp );   
     
    iColUtil->Collection().CommandL( *cmd );
    CleanupStack::PopAndDestroy( cmd );    
    }

// ----------------------------------------------------------------------------
// Remove outstanding requests
// ----------------------------------------------------------------------------
//     
EXPORT_C void CMPXCollectionPlaylist::CancelRequest()
    {
    if (iColUtil)
        {
        iColUtil->Collection().CancelRequest();
        }
    }    

// ----------------------------------------------------------------------------
// Invalidate the playlist
// ----------------------------------------------------------------------------
//     
EXPORT_C void CMPXCollectionPlaylist::Invalidate()
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::Invalidate");
    if( iPath )
        {
        iPath->Reset();
        }
    iItemIndexes.Reset();
    iItemIndex = KErrNotFound;
    iIncOpenUtil->Stop();
    }

// ----------------------------------------------------------------------------
// Restore Ordinal after path changed
// ----------------------------------------------------------------------------
//     
void CMPXCollectionPlaylist::RestoreOrdinalL(
    const CMPXMedia& aEntries,
    TInt /*aIndex*/)
    {
    MPX_DEBUG3("-->CMPXCollectionPlaylist::RestoreOrdinalL 0x%08x, iPlObs 0x%08x", 
               this, iPlObs);
    MPX_DEBUG_PATH(*iPath);
    MPX_DEBUG2("CMPXCollectionPlaylist::RestoreOrdinal %i", iPath->Index());
#ifdef _DEBUG
    MPX_DEBUG1("Original Play Order:");
    for (TInt ii = 0; ii < 15 && ii<Count(); ++ii)
        {
        MPX_DEBUG2("%d", iItemIndexes[ii]); 
        }
#endif        
    // update path     
    TMPXItemId curId = iPath->Id();
    
    if ( !aEntries.IsSupported(KMPXMediaGeneralContainerPath) )
        {
        if ( iPlObs )
            {
            iPlObs->HandleCollectionPlaylistChange (KErrEof );
            }
        }
    else
        {
        CMPXCollectionPath
                * path=aEntries.ValueCObjectL<CMPXCollectionPath> (KMPXMediaGeneralContainerPath );
        CleanupStack::PushL (path );
        // Check if path is for this playlist
        TInt levels = path->Levels ( );
        TBool valid(ETrue);
        if ( levels != iPath->Levels ( )-1 )
            {
            // Need to make sure the path was not clipped.
            // The number of levels between the old path  
            // and the new pathshould be the same.
            //
            valid = EFalse;
            }

        if ( valid )
            {
            TMPXItemId nextMostValidId=  KMPXInvalidItemId;

            if ( aEntries.IsSupported (KMPXMediaArrayContents ) )
                {
                RArray<TMPXItemId> ids;
                CleanupClosePushL (ids );
                const CMPXMediaArray * mediaArray=
                     aEntries.Value<CMPXMediaArray> (KMPXMediaArrayContents );
                User::LeaveIfNull(const_cast<CMPXMediaArray*>(mediaArray));
                TInt n=mediaArray->Count ( );
                for (TInt k=0; k<n;++k )
                    {
                    TMPXItemId id = (mediaArray->AtL(k)->ValueTObjectL<TMPXItemId>(
                                                           KMPXMediaGeneralId));

                    ids.AppendL (id );
                    }
                path->AppendL (ids.Array ( ) );

                // Determine the next most valid item id
                // by comparing previous item list vs current item list
                // First search down the array list, then search up the array list
                //
                TInt c = iItemIndexes.Count ( );
                for (TInt i=iItemIndex; i<c; ++i )
                    {
                    TMPXItemId prev = iPath->IdOfIndex ( i );
                    if ( ids.Find ( prev )!= KErrNotFound )
                        {
                        nextMostValidId = prev;
                        break;
                        }
                    }
                if ( nextMostValidId == KMPXInvalidItemId && c )
                    {
                    for (TInt i=iItemIndex-1; i>0; --i )
                        {
                        TMPXItemId prev = iPath->IdOfIndex ( i );
                        if ( ids.Find ( prev )!= KErrNotFound )
                            {
                            nextMostValidId = prev;
                            break;
                            }
                        }
                    }
                CleanupStack::PopAndDestroy (&ids );
                }
            //if (aIndex>=0 && aIndex<path->Count())
            if ( path->IndexOfId ( curId )!= KErrNotFound )
                {
                path->Set ( curId );
                }

            CleanupStack::Pop (path );
            delete iPath;
            iPath = path;

            if ( !iShuffle || !iShuffleEnabled )
                { // Just update the playlist content
                SetShuffleL (iShuffle, EFalse );
                }
            else
                {// shuffle is on, try to restore previous play order
                RArray<TInt> items;
                CleanupClosePushL (items );
                TInt count = iItemIndexes.Count ( );
                TInt cPath = iPath->Count ( );
                TInt granularity = cPath ? cPath : 1; // magic number 1
                RArray<TInt> flags(granularity); // flags for the items in the playlist
                CleanupClosePushL (flags );
                for (TInt ii=0; ii<cPath; ++ii )
                    { // Initialize the flags
                    flags.AppendL (0 );
                    }

                // copy all of indices from orignial array up to number of items in the path
                for (TInt i=0; i<count; ++i )
                    {
                    TInt itemIndex = iItemIndexes[i];
                    if ( itemIndex < cPath )
                        {
                        items.AppendL (itemIndex );
                        flags[itemIndex] = 1; // set the flag
                        } // Out of bound, not valid item anymore
                    }

                for (TInt j=0; j<cPath; ++j )
                    {
                    if ( !flags[j] )
                        {
                        items.AppendL (j );
                        }
                    }

                CleanupStack::PopAndDestroy (&flags );
                ::CopyArrayL (items.Array ( ), iItemIndexes );
                CleanupStack::PopAndDestroy (&items );
                }
            TInt err(KErrNone);

            TInt curIndex = iPath->IndexOfId (curId );
            if ( iItemIndex >=Count ( ) )
                { // items removed
                SetToLast ( );
                if ( KErrNotFound == curIndex )
                    { // current item removed at the end
                    err = KErrEof;
                    }
                }
            else
                {
                if ( KErrNotFound != curIndex )
                    {
                    // Look up the shuffle index
                    TInt itemIndex = iItemIndexes.Find (iPath->Index ( ) );
                    if ( KErrNotFound != itemIndex )
                        {
                        iItemIndex = itemIndex;
                        iPath->Set (iItemIndexes[iItemIndex] );
                        }
                    else
                        { // this should never happen unless something wrong
                        SetToLast ( );
                        err = KErrNotFound;
                        }
                    }
                else
                    { // current item removed, ensure the item was even valid
                    err = KErrNotFound;

                    // If the current ID was invalid, then the previous iItemIndex is 
                    // wrong as well. In this case, we use the next most valid ID
                    // that is found by comparing the previous ID list to the current id list 
                    // the next most valid ID has to exist in the new path
                    //
                    if ( nextMostValidId != KMPXInvalidItemId )
                        {
                        const TMPXItemId item = nextMostValidId; // compiler picks wrong set if no const
                        iPath->Set (item );
                        iItemIndex = iPath->Index ( );
                        }
                    else // back to first item
                        {
                        SetToFirst ( );
                        }
                    }
                }

            if ( iPlObs )
                {
                iPlObs->HandleCollectionPlaylistChange (err );
                }
            }
        else
            {
            CleanupStack::PopAndDestroy (path );
            }

        MPX_DEBUG_PATH(*iPath);
        MPX_DEBUG2("CMPXCollectionPlaylist::RestoreOrdinal %i", iPath->Index());
#ifdef _DEBUG
        MPX_DEBUG1("New Play Order:");
        for (TInt jj = 0; jj < 15 && jj<Count ( ); ++jj )
            {
            MPX_DEBUG2("%d", iItemIndexes[jj]);
            }
#endif
        }
    MPX_DEBUG3("<--CMPXCollectionPlaylist::RestoreOrdinalL 0x%08x, iPlObs 0x%08x",
            this, iPlObs);
    }

// -----------------------------------------------------------------------------
// DoHandleCollectionMessageL
// -----------------------------------------------------------------------------
//
void CMPXCollectionPlaylist::DoHandleCollectionMessageL(
    CMPXMessage* aMessage, 
    TInt aErr)
    {
    MPX_FUNC_EX("CMPXCollectionPlaylist::DoHandleCollectionMessageL");
    TInt msgGeneralId(0);
    if( aMessage )
        {
        msgGeneralId = aMessage->ValueTObjectL<TMPXMessageId>(KMPXMessageGeneralId);    
        }
    
    if (KErrNone == aErr && KMPXMessageGeneral == msgGeneralId )
        {
        TInt event(aMessage->ValueTObjectL<TInt>(KMPXMessageGeneralEvent));
        TInt type(aMessage->ValueTObjectL<TInt>(KMPXMessageGeneralType));
        if (event == TMPXCollectionMessage::EError && 
            type == EMcsMediaByPath)
            {
            MPX_DEBUG1("CMPXCollectionPlaylist::DoHandleCollectionMessageL -- Error message for MediaL");
            CMPXMedia* dummyMedia=CMPXMedia::NewL();
            CleanupStack::PushL(dummyMedia);
            iMediaObs->HandleCollectionMediaL(*dummyMedia, 
                                aMessage->ValueTObjectL<TInt>(KMPXMessageGeneralData));
            CleanupStack::PopAndDestroy(dummyMedia);
            }
        }
    else if( aMessage && msgGeneralId == KMPXMessageIdItemChanged)
        {
        MPX_DEBUG1("CMPXCollectionPlaylist::DoHandleCollectionMessageL -- KMPXMessageIdItemChanged");
        // Multiple messages
        //
        TBool refresh(EFalse);
        if( aMessage->IsSupported(KMPXMessageArrayContents) )
            {
            const CMPXMessageArray* messageArray =
                    aMessage->Value<CMPXMessageArray>(KMPXMessageArrayContents);
            User::LeaveIfNull(const_cast<CMPXMessageArray*>(messageArray));
            TInt count(messageArray->Count());
            for(TInt i=0; i<count; ++i )
                {
                refresh |= DoHandleCollectionChangeMessageL(*(messageArray->AtL(i)));
                }
            }
        // Single message
        //
        else
            {
            refresh = DoHandleCollectionChangeMessageL(*aMessage);
            }
        
        // Re-open the playlist if necessary
        //
        if( refresh )
            {
            MPX_DEBUG1("CMPXCollectionPlaylist::DoHandleCollectionMessageL - refresh due to KMPXMessageIdItemChanged");
            if (iAutoPlaylist)
                {
                iPlObs->HandleCollectionPlaylistChange(iItemIndexes.Count() > 0 ? KErrNone : KErrEof);
                }
            else
                {
                if( iPath->Levels() > 0 )
                    {
                    DoIncrementalOpenL();
                    iReopenForChange = ETrue;
                    }
                else
                    {
                    // Path has been clipped and not playable
                    // 
                    iPlObs->HandleCollectionPlaylistChange(KErrNotFound);
                    }
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CMPXCollectionPlaylist::DoHandleCollectionChangeMessageL
// -----------------------------------------------------------------------------
//
TBool CMPXCollectionPlaylist::DoHandleCollectionChangeMessageL(
    CMPXMessage& aMessage)
    {
    MPX_DEBUG3("-->CMPXCollectionPlaylist::DoHandleCollectionChangeMessageL 0x%08x, iPlObs 0x%08x", 
               this, iPlObs);
    TBool refresh(EFalse);
    TInt affectedIndex = KErrNotFound; // index of the id that is modified
    
    // Change event data
    //
    TUid collectionId( aMessage.ValueTObjectL<TUid>(KMPXMessageCollectionId) );
    
    TMPXChangeEventType changeType =
        aMessage.ValueTObjectL<TMPXChangeEventType>(KMPXMessageChangeEventType);

    TMPXGeneralCategory category(EMPXNoCategory);
    if( aMessage.IsSupported(KMPXMessageMediaGeneralCategory) )
        {
        category =
            aMessage.ValueTObjectL<TMPXGeneralCategory>(KMPXMessageMediaGeneralCategory);
        }
    
    TMPXItemId itemId(0);
    if( aMessage.IsSupported(KMPXMessageMediaGeneralId) )
        {
        itemId = aMessage.ValueTObjectL<TMPXItemId>(KMPXMessageMediaGeneralId);
        }
    
    TMPXItemId deprecatedId(0);
    if (aMessage.IsSupported(KMPXMessageMediaDeprecatedId))
        {
        deprecatedId = aMessage.ValueTObjectL<TMPXItemId>(KMPXMessageMediaDeprecatedId);
        }
    
    // Check if the current item is being modified
    //
    TMPXItemId curItemId( iPath->Id() );
    if( curItemId.ApproxEqual(itemId) )
        {
        affectedIndex = iPath->Index();
        // Deleted
        if( changeType == EMPXItemDeleted )
            {
            refresh = ETrue;
            MPX_DEBUG1("CMPXCollectionPlaylist::DoHandleCollectionChangeMessageL -- current item deleted");
            }
        // Modified
        else if( changeType == EMPXItemModified )
            {
            MPX_DEBUG1("CMPXCollectionPlaylist::DoHandleCollectionChangeMessageL -- modified");
            refresh = ETrue;      
            }
        // else inserted should never happen
        }
    // Not the current item being modified, check collection path
    // to see if the path needs to be refreshed
    //
    else
        {
        // Map Change Type
        //
        CMPXCollectionPath::TMPXCollectionPathChange 
                            pChangeType(CMPXCollectionPath::EGroupModified);
        if( category == EMPXPlaylist )
            {
            if( changeType == EMPXItemDeleted )
                {
                pChangeType = CMPXCollectionPath::EDeleted;    
                }
            else
                {
                pChangeType = CMPXCollectionPath::EGroupModified;    
                }
            }
        else if( category == EMPXCollection )
            {
            if( changeType == EMPXItemModified )
                {
                pChangeType = CMPXCollectionPath::EGroupModified;
                }
            }
        else if( changeType == EMPXItemInserted )
            {
            pChangeType = CMPXCollectionPath::EAdded;    
            }
        else if( changeType == EMPXItemModified )
            {
            pChangeType = CMPXCollectionPath::EModified;    
            }
        else if( changeType == EMPXItemDeleted )
            {
            pChangeType = CMPXCollectionPath::EDeleted;
            }
        
        // Check the collection path
        //
        if( iPath->Id(CMPXCollectionPath::ECollectionRoot) == collectionId.iUid )
            {
            TInt pUpdated = iPath->HandleChange( collectionId, itemId, 
                                               deprecatedId, pChangeType, affectedIndex );
            
            if( pUpdated == CMPXCollectionPath::EPathClipped )
                {
                // Path clipped, playlist is no longer valid
                //
                MPX_DEBUG1("CMPXCollectionPlaylist::DoHandleCollectionChangeMessageL -- path clipped");
                Invalidate();
                if (iPlObs)
                    {
                    iPlObs->HandleCollectionPlaylistChange(KErrEof);
                    }
                }
            else if( pUpdated == CMPXCollectionPath::EPathModified )
                {
                // Path modified, something in needs to be refreshed
                //
                MPX_DEBUG1("CMPXCollectionPlaylist::DoHandleCollectionChangeMessageL -- path modified");
                refresh = ETrue;   
                }
            }
        }

    // update playlist immediately when item is deleted in order to detect deletion of the last item in the playlist
     if (iAutoPlaylist && refresh)
        {
        // For autoplaylist, affected by the event.
        // Path clip will never happen for autoplaylist
		if ( changeType == EMPXItemDeleted )
            {
            // item deleted in the autoplaylist, update the iPath to remove the item
            // auso update iItemIndexes and iItemIndex
            iPath->Remove(affectedIndex);
            TInt itemIndex = KErrNotFound;
            for (TInt i=0; i<iItemIndexes.Count(); i++)
                {
                if (iItemIndexes[i] == affectedIndex)
                    {
                    // remove the item
                    iItemIndexes.Remove(i);
                    itemIndex = i;
                    }
                // because the item get removed, we still need to check the next item, if it's not the last
                if (i<iItemIndexes.Count() && iItemIndexes[i] > affectedIndex)
                    {
                    //index shift
                    --iItemIndexes[i];
                    }
                }
            if (iItemIndexes.Count()>0)
                {
                // Still has items
                if (iItemIndex == itemIndex)
                    {
                    // current item get removed, iItemIndex already move to next
                    if (iItemIndex >= iItemIndexes.Count()) 
                        {
                        iItemIndex = (iRepeatMode == ERepeatAll)? 0: iItemIndexes.Count()-1;
                        }
                    }
                else if (iItemIndex > itemIndex)
                    {
                    --iItemIndex;
                    }
                iPath->Set(iItemIndexes[iItemIndex]);
                }
            else
                {
                iItemIndex = KErrNotFound;
                }
            }
        }
    
    MPX_DEBUG4("<--CMPXCollectionPlaylist::DoHandleCollectionChangeMessageL 0x%08x, iPlObs 0x%08x, refresh %d", 
            this, iPlObs, refresh);
    return refresh;
    }

// -----------------------------------------------------------------------------
// Start the incremental fetching 
// -----------------------------------------------------------------------------
//    
void CMPXCollectionPlaylist::DoIncrementalOpenL()
    {
    if(iSingleItemPlaylist)
        {
        return;
        }
    // Stop the utility first if we were fetching
    //
    iIncOpenUtil->Stop();
    
    // Copy of the path
    //
    CMPXCollectionPath* copy = iPath->ContainerPathL();
    CleanupStack::PushL( copy );
    
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    TArray<TMPXAttribute> ary = attrs.Array();

    iIncOpenUtil->SetDelay( KIncrementalDelayNone );
    iIncOpenUtil->StartL( *copy, ary, KIncrementalFetchSize, iPath->Index() ,
                          CMPXCollectionOpenUtility::EFetchDown );
    iIncOpenUtil->SetDelay( KIncrementalDelay );
    CleanupStack::PopAndDestroy( &attrs );
    CleanupStack::PopAndDestroy( copy );
    }

// -----------------------------------------------------------------------------
// Handle Incremental Open results
// -----------------------------------------------------------------------------
//  
void CMPXCollectionPlaylist::DoHandleIncompleteOpenL( const CMPXMedia& aMedia, TBool aComplete)
    {
    MPX_FUNC("<--CMPXCollectionPlaylist::DoHandleIncompleteOpenL");
    if (iAutoPlaylist)
        {
        // Fill incomplete playlist, pending until inc open is used for playlist
        /*
        TInt index = iPath->IndexOfId(KMPXInvalidItemId)
        if (!=KErrNotFound)
            {
            }
        */
        }
    else
        {
        CMPXCollectionPath* newPath = iIncOpenUtil->PathL();
        
        // Re-select the previous item, if it failed then run
        // the restore ordinal algorithm.
        //
        TInt index = newPath->IndexOfId(iPath->Id());
        if( index != KErrNotFound && !iReopenForChange )
            {
            newPath->Set( index );
            
            delete iPath;
            iPath = NULL;
            iPath = newPath;
            }
        else 
            {
            delete newPath;
            
            TBool checkValid = (index == KErrNotFound) && (iReopenForChange);
            if ( aComplete || (!checkValid))
                {                
                TBool checkForOpen = (index == KErrNotFound) && (!iReopenForChange);
                if ( !checkForOpen)
                    {
                    RestoreOrdinalL ( aMedia, 0);
                    iReopenForChange = EFalse;
                    }
                }
            }      
        }
    }
// -----------------------------------------------------------------------------
// AutoPlay
// -----------------------------------------------------------------------------
//    
EXPORT_C TBool CMPXCollectionPlaylist::AutoPlay() const
    {
    return iAutoPlay;
    }

// -----------------------------------------------------------------------------
// Set AutoPlay
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMPXCollectionPlaylist::SetAutoPlay(TBool aAutoPlay)
    {
    iAutoPlay = aAutoPlay;
    }
    
    
// -----------------------------------------------------------------------------
// Set single item playlist
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCollectionPlaylist::SetSingleItemPlaylist()
    {
    iSingleItemPlaylist = ETrue;
    }
    
    
// -----------------------------------------------------------------------------
// Is single item playlist
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXCollectionPlaylist::IsSingleItemPlaylist()
    {
    return iSingleItemPlaylist;
    }
    
// -----------------------------------------------------------------------------
// PreInitPlugin
// -----------------------------------------------------------------------------
//    
EXPORT_C TBool CMPXCollectionPlaylist::PreInitPlugin() const
    {
    return iPreInitPlugin;
    }

// -----------------------------------------------------------------------------
// Set AutoPlay
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMPXCollectionPlaylist::SetPreInitPlugin(TBool aPreInitPlugin)
    {
    iPreInitPlugin = aPreInitPlugin;
    }
    

// End of file
