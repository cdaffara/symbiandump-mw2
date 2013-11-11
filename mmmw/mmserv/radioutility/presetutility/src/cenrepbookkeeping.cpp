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
* Description:
*
*
*/

#include <s32mem.h> // RWriteStream, RReadStream

#include "cenrepbookkeeping.h"
#include "trace.h"


const TUint8 KNbrOfBitsPerByte = 8;


#define BITSPERLWORD ( sizeof(iBookKeepingData.iUsedIndexLWords[0]) * KNbrOfBitsPerByte)


// ============= MEMBER FUNCTIONS ==============

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCenrepBookKeeping::CCenrepBookKeeping()
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
CCenrepBookKeeping::~CCenrepBookKeeping()
    {
    FUNC_LOG;
    Clear();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TInt CCenrepBookKeeping::PresetCount() const
    {
    FUNC_LOG;
    return iBookKeepingData.iCount;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TInt CCenrepBookKeeping::FirstIndex() const
    {
    FUNC_LOG;
    TInt retIndex( KErrGeneral );
    TUint32 bitMask( 0 );
    for ( TInt lwordOffset = 0; ; ++lwordOffset)
        {
        if ( 0 !=iBookKeepingData.iUsedIndexLWords[lwordOffset] )
            {
            bitMask = 1;
            for ( TUint8 bitOffset( 0 ); BITSPERLWORD > bitOffset; ++bitOffset )
                {
                if ( KMaxNumberOfPresetStations <= ( ( BITSPERLWORD * lwordOffset ) + bitOffset ) )
                    {
                    retIndex = KErrNotFound;
                    break;
                    }
                if ( 0 != ( iBookKeepingData.iUsedIndexLWords[lwordOffset] & bitMask) )
                    {
                    retIndex = ( BITSPERLWORD * lwordOffset ) + bitOffset;
                    break;
                    }
                bitMask = bitMask << 1;                
                }
            }
        else if ( KMaxNumberOfPresetStations < ( BITSPERLWORD * (lwordOffset + 1) ) )
            {
            retIndex = KErrNotFound;
            break;            
            }
        if ( KErrGeneral != retIndex )
            {
            break;
            }
        }
    INFO_1( "retIndex: %i", retIndex );
    return retIndex;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TInt CCenrepBookKeeping::NextIndex( TInt aFromIndex )
    {
    FUNC_LOG;
    TInt retIndex( KErrGeneral );
    TInt startIndex( aFromIndex + 1 );
    TUint32 bitMask( 0 );
    TUint8 bitOffset( 0 );
    for ( TInt lwordOffset(startIndex/BITSPERLWORD); ; ++lwordOffset)
        {
        if ( lwordOffset == (startIndex/BITSPERLWORD) )
            {
            bitMask = 1 << (startIndex%BITSPERLWORD);
            bitOffset = (startIndex%BITSPERLWORD);
            }
        else
            {
            bitMask = 1;
            bitOffset = 0;
            }
        if ( 0 !=iBookKeepingData.iUsedIndexLWords[lwordOffset] )
            {            
            for ( ; BITSPERLWORD > bitOffset; ++bitOffset )
                {
                if ( KMaxNumberOfPresetStations <=  ( ( BITSPERLWORD * lwordOffset ) + bitOffset ) )
                    {
                    retIndex = KErrNotFound;
                    break;
                    }
                if ( 0 != ( iBookKeepingData.iUsedIndexLWords[lwordOffset] & bitMask) )
                    {
                    retIndex = ( BITSPERLWORD * lwordOffset ) + bitOffset;
                    break;
                    }
                bitMask = bitMask << 1;                
                }
            }
        else if ( KMaxNumberOfPresetStations < ( BITSPERLWORD * (lwordOffset + 1) ) )
            {
            retIndex = KErrNotFound;
            break;            
            }
        if ( KErrGeneral != retIndex )
            {
            break;
            }
        }
    INFO_1( "retIndex: %i", retIndex );
    return retIndex;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCenrepBookKeeping::AddIndex( const TInt aIndex )
    {
    FUNC_LOG;
    if  (
        ( KMaxNumberOfPresetStations >iBookKeepingData.iCount ) &&
        ( !IsIndexUsed( aIndex ) )
    )
        {
        TInt lwordOffset(aIndex/BITSPERLWORD);
        TUint bitOffset( (aIndex%BITSPERLWORD) );
        TUint32 bitMask( 1 );
        bitMask  = bitMask << bitOffset;
       iBookKeepingData.iUsedIndexLWords[lwordOffset] |= bitMask;    
        ++iBookKeepingData.iCount;       
        }
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCenrepBookKeeping::RemoveIndex( const TInt aIndex )
    {
    FUNC_LOG;
    if  ( 
        ( 0 <iBookKeepingData.iCount ) &&
        ( IsIndexUsed( aIndex ) )
    ) 
        {
        TInt lwordOffset(aIndex/BITSPERLWORD);
        TUint bitOffset( (aIndex%BITSPERLWORD) );
        TUint32 bitMask( 1 );
        bitMask = bitMask << bitOffset;
        bitMask = ~bitMask;
       iBookKeepingData.iUsedIndexLWords[lwordOffset] &= bitMask;    
        --iBookKeepingData.iCount;       
        }
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCenrepBookKeeping::Clear()
    {
    FUNC_LOG;
    for ( TInt lwordOffset(0); LENGTHOFUSEDINDEXLWORDSARRAY > lwordOffset; ++lwordOffset)
        {
       iBookKeepingData.iUsedIndexLWords[lwordOffset] = 0;
        }
   iBookKeepingData.iCount = 0;    
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TInt CCenrepBookKeeping::FindUnusedIndex() const
    {
    FUNC_LOG;
    TInt retIndex( KErrGeneral );
    TUint32 bitMask( 0 );
    for ( TInt lwordOffset = 0; ; ++lwordOffset)
        {
        if ( 0xFFFFFFFF !=iBookKeepingData.iUsedIndexLWords[lwordOffset] )
            {
            bitMask = 1;
            for ( TUint8 bitOffset = 0; BITSPERLWORD > bitOffset; ++bitOffset )
                {
                if ( KMaxNumberOfPresetStations <=  ( ( BITSPERLWORD * lwordOffset ) + bitOffset ) )
                    {
                    retIndex = KErrNotFound;
                    break;
                    }
                if ( 0 == ( iBookKeepingData.iUsedIndexLWords[lwordOffset] & bitMask) )
                    {
                    retIndex = ( BITSPERLWORD * lwordOffset ) + bitOffset;
                    break;
                    }
                bitMask = bitMask << 1;                
                }
            }
        else if ( KMaxNumberOfPresetStations < ( BITSPERLWORD * (lwordOffset + 1) ) )
            {
            retIndex = KErrNotFound;
            break;            
            }
        if ( KErrGeneral != retIndex )
            {
            break;
            }
        }
    INFO_1( "retIndex: %i", retIndex );
    return retIndex;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TBool CCenrepBookKeeping::IsIndexUsed( const TInt aIndex )
    {
    FUNC_LOG;
    TInt lwordOffset(aIndex/BITSPERLWORD);
    TUint bitOffset( (aIndex%BITSPERLWORD) );
    TUint32 bitMask( 1 );
    bitMask = bitMask << bitOffset;
    return ( iBookKeepingData.iUsedIndexLWords[lwordOffset] & bitMask);    
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TPtr8 CCenrepBookKeeping::BookKeepingData()
    {
    TPtr8 bookKeepingData( (TUint8*)&iBookKeepingData, sizeof( iBookKeepingData ), sizeof( iBookKeepingData ) );
    return bookKeepingData;
    }
