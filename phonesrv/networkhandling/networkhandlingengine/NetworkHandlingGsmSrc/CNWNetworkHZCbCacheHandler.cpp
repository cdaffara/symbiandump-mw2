/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of 
*                CNWNetworkHZCbCacheHandler class member functions.
*
*/



// INCLUDE FILES
#include    <e32math.h>  // for Math::Random
#include    "NWLogger.h"
#include    "CNWNetworkHZCbCacheHandler.h"

// FORWARD DECLARATIONS
class Math;

// DEFINITIONS
const TInt KMaxNumberOfItemsInHomeZoneCache = 21;
const TInt KMaxNumberOfItemsInCityZoneCache = 5;

// LOCAL FUNCTIONS
LOCAL_C void PrintCacheContent( 
    const RArray<RMmCustomAPI::TViagCacheRecordContent> * aCache, 
    const TInt aCacheId )
    {
    for (TInt i = 0; i < aCache->Count(); i++ )
        {
        RMmCustomAPI::TViagCacheRecordContent rec = (*aCache)[i];
        TBuf<70> buffer;
        _LIT(KData,"Cache[%d], Position[%d], LacId[%d], CellId[%d]");
        buffer.Format(KData, aCacheId, i, rec.iLac, rec.iCellId);
        NWLOGSTRING2( KNWINT, "NW: CBS HZ: PrintCacheContent, %S", &buffer );
        }
    }


// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::NewL
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CNWNetworkHZCbCacheHandler * CNWNetworkHZCbCacheHandler::NewL( 
    RMmCustomAPI& aCustomAPI )
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW CNWNetworkHZCbCacheHandler::NewL() Begin " );
    
    CNWNetworkHZCbCacheHandler * self = 
        new (ELeave) CNWNetworkHZCbCacheHandler( aCustomAPI );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW CNWNetworkHZCbCacheHandler::NewL() End " );
    return self;
    }

// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::CNWNetworkHZCbCacheHandler
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CNWNetworkHZCbCacheHandler::CNWNetworkHZCbCacheHandler( 
    RMmCustomAPI& aCustomAPI ) : iCustomAPI (aCustomAPI) 
    { 
    NWLOGSTRING( KNWOBJECT, 
        "NW CNWNetworkHZCbCacheHandler::\
        CNWNetworkHZCbCacheHandler() called " );
    }


// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::ConstructL
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkHZCbCacheHandler::ConstructL()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW CNWNetworkHZCbCacheHandler::ConstructL() Begin " );

    iSimCacheMonitor = CNWNetworkEFReader::NewL( iCustomAPI, this );
    SetInitialisingStatus( ECBSHZCacheReader_NotInitialised );
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: ConstructL() End " );
    }

       
// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::~CNWNetworkHZCbCacheHandler
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CNWNetworkHZCbCacheHandler::~CNWNetworkHZCbCacheHandler()
    {
    NWLOGSTRING( KNWOBJECT, 
        "NW CNWNetworkHZCbCacheHandler::\
        ~CNWNetworkHZCbCacheHandler() Begin " );
    
    if ( iSimCacheMonitor )
        {
        iSimCacheMonitor->Cancel();
        }
    delete iSimCacheMonitor;
    iSimCache1.Close();
    iSimCache2.Close();
    iSimCache3.Close();
    iDynCache.Close();    
    
    NWLOGSTRING( KNWOBJECT, 
        "NW CNWNetworkHZCbCacheHandler::\
        ~CNWNetworkHZCbCacheHandler() End " );
    }

// -----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::InitialiseHomeZoneCaches
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CNWNetworkHZCbCacheHandler::InitialiseHomeZoneCachesAsync(
    MHZCachesInitialisationObserver * aObserver )
    {
    NWLOGSTRING( KNWINT, "NW: CB HZ:: InitialiseHomeZoneCachesAsync Begin" );
    iObserver = aObserver;
    TInt error( KErrNotFound );
    SetInitialisingStatus( ECBSHZCacheReader_Initialising_Cache_HZ1 );
    error = iSimCacheMonitor->StartMonitoringSimCache( this );
    if ( error != KErrNone )
        {
        NWLOGSTRING( KNWINT, 
            "NW:CB HZ:InitialiseHomeZoneCachesAsync Already active!" );
        NWLOGSTRING2( KNWINT, 
            "NW:CB HZ:InitialiseHomeZoneCachesAsync(),\
            error = %d", error );
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW CNWNetworkHZCbCacheHandler::\
        InitialiseHomeZoneCachesAsync() End " );
    }

// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::Find
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkHZCbCacheHandler::Find( THomeZoneCacheId aCacheId,
    RMmCustomAPI::TViagCacheRecordContent aRecord )
    {
    NWLOGSTRING3( KNWINT, 
        "NW: CNWNetworkHZCbCacheHandler::Find() Begin,\
        aRecord.iLac = %d, aRecord.iCellId = %d ",
        aRecord.iLac, aRecord.iCellId );
    
    if( InitialisingStatus() != ECBSHZCacheReader_Ready )
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkHZCbCacheHandler::Find(),\
             CBSHZCacheReader not ready!");
        
        return KErrNotReady;
        }
        
    TInt retVal = KErrNotFound;
    RArray<RMmCustomAPI::TViagCacheRecordContent> * cache;
    cache = Cache( aCacheId );
    ASSERT( cache );
    
    if( cache )
        {
        retVal = cache->Find( aRecord );
        }

    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkHZCbCacheHandler::Find() End,\
        retVal = %d ", retVal );
    return retVal;        
    }

// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::Insert
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkHZCbCacheHandler::Insert(
    const THomeZoneCacheId aCacheId,
    const RMmCustomAPI::TViagCacheRecordContent aRecord )    
    {
    NWLOGSTRING4( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler::Insert() Begin,\
         aCacheId = %d, aRecord.iLac = %d, aRecord.iCellId = %d",
         aCacheId, aRecord.iLac, aRecord.iCellId );
    
    if( InitialisingStatus() != ECBSHZCacheReader_Ready )
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkHZCbCacheHandler::Insert(),\
             CBSHZCacheReader not ready!" );
        return KErrNotReady;
        }

    TInt retVal = KErrNotFound;

   // Add HomeZone cells in SIM
    switch( aCacheId )
        {
        case EHZ_HomeZoneCache1:
        case EHZ_HomeZoneCache2:
        case EHZ_HomeZoneCache3:
            {
            retVal = DoInsertInHomeZoneCache( aRecord, aCacheId );
            }
            break;
        case EHZ_CityZoneCache:
            {
            retVal = DoInsertInCityZoneCache( aRecord );
            }
            break;
         default:
            // This should not ever happen.
            ASSERT( EFalse );
            break;
        }
            
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler::Insert() End,\
         retVal = %d ", retVal );
    return retVal;           
    }


// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::Remove
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkHZCbCacheHandler::Remove( const THomeZoneCacheId aCacheId, 
    const TInt aIndex )
    {
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler::Remove() Begin,\
         aCacheId = %d, aIndex = %d ", aCacheId, aIndex );
    
    if( InitialisingStatus() != ECBSHZCacheReader_Ready )
        {
        NWLOGSTRING( KNWOBJECT, 
            "NW: CNWNetworkHZCbCacheHandler::Remove(),\
             CBSHZCacheReader not ready!" );
        return KErrNotReady;
        }

   // TBD remove aIndex handling
    TInt retVal = KErrNotFound;
    RArray<RMmCustomAPI::TViagCacheRecordContent> * cache;
    cache = Cache( aCacheId );
    ASSERT( cache );
    
    if( cache )
        {
        TInt itemCount = cache->Count();
        
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkHZCbCacheHandler::Remove(),\
            cache->Count = %d ", itemCount );
        
        if( aIndex < itemCount )
            {
            switch( aCacheId )
                {
                case EHZ_HomeZoneCache1:
                case EHZ_HomeZoneCache2:
                case EHZ_HomeZoneCache3:
                    {
                    ASSERT( itemCount <= KMaxNumberOfItemsInHomeZoneCache );
                    RMmCustomAPI::TViagCacheRecordContent nullEntry;
                    nullEntry.iLac = 0xffff;
                    nullEntry.iCellId = 0xffff;

                    // Remove from SIM cache first
                    RMmCustomAPI::TViagCacheRecordId sim_cache;
                    sim_cache.iCacheId = aCacheId;
                    sim_cache.iRecordId = aIndex;

                    retVal = iCustomAPI.WriteViagHomeZoneCache( sim_cache,
                        nullEntry );
                    
                    NWLOGSTRING2( KNWOBJECT, 
                        "NW: CNWNetworkHZCbCacheHandler::Remove(),\
                        WriteViagHomeZoneCache() error code: %d ", retVal );
                    // ... and then from local cache
                    if( retVal == KErrNone )
                        {
                        cache->Remove( aIndex );
                        cache->Insert( nullEntry, aIndex );
                        cache->Compress();
                        }
                    retVal = KErrNone;
                    }
                    break;
                case EHZ_CityZoneCache:
                    {
                    ASSERT( itemCount <= KMaxNumberOfItemsInCityZoneCache );
                    cache->Remove( aIndex );
                    cache->Compress();
                    retVal = KErrNone;
                    break;
                    }
                default:
                    // This should never happen.
                    ASSERT( EFalse );
                    break;
                }
            }
        }
    
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler::Remove() End,\
        retVal = %d ", retVal );
    
    return retVal;
    }
    

// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::ReadCacheFromSIM
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkHZCbCacheHandler::ReadCacheFromSIM( 
    RArray<RMmCustomAPI::TViagCacheRecordContent> & aDestArray,
    const THomeZoneCacheId aCacheId, const TInt aNumberofItems )
    {
    NWLOGSTRING3( KNWINT, 
        "NW: CNWNetworkHZCbCacheHandler: ReadCacheFromSIM() Begin,\
        aCacheId = %d, aNumberofItems = %d ", aCacheId, aNumberofItems );
   
    ASSERT( aCacheId >= EHZ_HomeZoneCache1 && aCacheId <= EHZ_HomeZoneCache3 );
    ASSERT( aNumberofItems <= KMaxNumberOfItemsInHomeZoneCache );
   
    TInt retVal = KErrNone;
    RMmCustomAPI::TViagCacheRecordId cache;
    cache.iCacheId = aCacheId;
    aDestArray.Reset();
    
    for( TInt index = 0; index < aNumberofItems; index++ )
        {
         //Check all Record Ids (twenty of them)
        RMmCustomAPI::TViagCacheRecordContent record;
        cache.iRecordId = index;
        retVal = iCustomAPI.ReadViagHomeZoneCache( cache, record );
        if( retVal )
            {
            NWLOGSTRING2( KNWERROR, "NW: CNWNetworkHZCbCacheHandler::\
                ReadViagHomeZoneCache() error code: %d ", retVal );
            // Error happened. Stop here.
            break;
            }
        aDestArray.Insert( record, index );
        }
    
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkHZCbCacheHandler: ReadCacheFromSIM() End,\
        retVal = %d ", retVal );
    return retVal;
    }
 
 
// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::Cache
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
RArray<RMmCustomAPI::TViagCacheRecordContent> * CNWNetworkHZCbCacheHandler::
    Cache( const THomeZoneCacheId aCacheId )
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: Cache() Begin,\
        aCacheId = %d ", aCacheId );
    
    RArray<RMmCustomAPI::TViagCacheRecordContent> * retVal;
    
    switch( aCacheId )
        {
        case EHZ_HomeZoneCache1:
          retVal = &iSimCache1;
          break;
        case EHZ_HomeZoneCache2:
          retVal = &iSimCache2;
          break;
        case EHZ_HomeZoneCache3:
          retVal = &iSimCache3;
          break;
        case EHZ_CityZoneCache:
          retVal = &iDynCache;
          break;
        default:
          retVal = NULL;
          break;        
        }
    
    NWLOGSTRING( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: Cache() End" );
    
    return retVal;
    }


// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::DoInsertInHomeZoneCache
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkHZCbCacheHandler::DoInsertInHomeZoneCache( 
    const RMmCustomAPI::TViagCacheRecordContent aRecord,
    const THomeZoneCacheId aCacheId )
    {
    NWLOGSTRING4( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: DoInsertInHomeZoneCache() Begin,\
        aCacheId = %d, aRecord.iLac = %d, aRecord.iCellId = %d ",
        aCacheId, aRecord.iLac, aRecord.iCellId );
    
    TInt retVal = KErrNotFound;
    
    RArray<RMmCustomAPI::TViagCacheRecordContent> * cache;
    cache = Cache( aCacheId );
    ASSERT( cache );
    
    if( cache )
        {
        TInt pos = KErrNotFound;
        TInt itemCount = cache->Count();
        
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkHZCbCacheHandler: DoInsertInHomeZoneCache(),\
            itemCount = %d ", itemCount );
        ASSERT( itemCount <= KMaxNumberOfItemsInHomeZoneCache );
        
        TBool replace = EFalse;
        
        // Check if there free slots available.
        pos = FindNullEntry( aCacheId );

        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkHZCbCacheHandler: DoInsertInHomeZoneCache(),\
            Trying finding free slots: %d", pos );
        
        if( pos == KErrNotFound )
            {
            if( itemCount < KMaxNumberOfItemsInHomeZoneCache )
                {
                // Append at the end of cache
                pos = itemCount;
                }
            else
                {
                // We have replace some randomly selected item
                TUint32 random = Math::Random();
                pos = random % KMaxNumberOfItemsInHomeZoneCache;
                replace = ETrue;
                }
            }
         else
            {
            // Replace the founded nullEntry
            replace = ETrue;
            }
         
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkHZCbCacheHandler: DoInsertInHomeZoneCache(),\
            pos = %d", pos );
        
         ASSERT( pos >= 0 && pos < KMaxNumberOfItemsInHomeZoneCache );
         if( pos >= 0 && pos < KMaxNumberOfItemsInHomeZoneCache )
            {
            // Update SIM cache
            RMmCustomAPI::TViagCacheRecordId sim_cache;
            sim_cache.iCacheId = aCacheId;
            sim_cache.iRecordId = pos;
            TInt custError = iCustomAPI.WriteViagHomeZoneCache( 
                sim_cache, aRecord );
                
            NWLOGSTRING2( KNWOBJECT, 
                 "NW: CNWNetworkHZCbCacheHandler: DoInsertInHomeZoneCache(),\
                 WriteViagHomeZoneCache error code: %d", custError );
            
            if( custError )
                {
                // Failed
                pos = KErrNotFound;
                }
            else
                {
                // Update local cache
                if( replace )
                    {
                    cache->Remove( pos );
                    cache->Compress();
                    }
                cache->Insert( aRecord, pos );
                }
            }
        else
            {
            pos = KErrOverflow;
            }
        retVal = pos;
        }

    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: DoInsertInHomeZoneCache() End,\
        retVal = %d ", retVal );
   
    return retVal;
    
    }

// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::FindNullEntry
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
TInt CNWNetworkHZCbCacheHandler::FindNullEntry(
    const THomeZoneCacheId aCacheId ) 
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: FindNullEntry() Begin,\
        aCacheId = %d ", aCacheId );
    
    TInt retVal = KErrNotFound;

    // Try to find ununitiliased entries first...
    RMmCustomAPI::TViagCacheRecordContent tmpEntry;
    tmpEntry.iLac = 0xffff;
    tmpEntry.iCellId = 0xffff;
    TInt ui_pos = Find( aCacheId, tmpEntry );

     // ... then check nullEntries
    tmpEntry.iLac = 0;
    tmpEntry.iCellId = 0;
    TInt ne_pos = Find( aCacheId, tmpEntry );
    
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: FindNullEntry(),\
        ui_pos = %d, ne_pos = %d ", ui_pos, ne_pos );

    // ... and select the smaller but valid one.
    if( ne_pos > KErrNotFound && 
        ui_pos > KErrNotFound )
        {
        // Both kind of entryes were found. Select a smaller one
        if( ui_pos <= ne_pos )
            {
            retVal = ui_pos;
            }
        else
            {
            retVal = ne_pos;
            }
        }
    else if( ne_pos == KErrNotFound && 
             ui_pos > KErrNotFound )
        {
        // only uninitialised entry was found
        retVal = ui_pos;
        }
    else if( ne_pos > KErrNotFound && 
             ui_pos == KErrNotFound )
        {
        // only NULL entry was found
        retVal = ne_pos;
        }
    else
        {
        retVal = KErrNotFound;
        }
        
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: FindNullEntry() End,\
        retVal = %d ", retVal );
    
    return retVal;    
    }


// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::DoInsertInCityZoneCache
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkHZCbCacheHandler::DoInsertInCityZoneCache( 
    const RMmCustomAPI::TViagCacheRecordContent aRecord )
    {
    NWLOGSTRING3( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: DoInsertInCityZoneCache() Begin,\
        aRecord.iLac = %d, aRecord.iCellId = %d ", 
        aRecord.iLac, aRecord.iCellId );
    
    TInt retVal = KErrNotFound;
    RArray<RMmCustomAPI::TViagCacheRecordContent> * cache;
    cache = Cache( EHZ_CityZoneCache );
    ASSERT( cache );
    
    if( cache )
        {
        // CityZone items won't be saved in SIM ever.
        TInt pos = 0;
        cache->Insert( aRecord, pos );
        TInt itemCount = cache->Count();
        
        NWLOGSTRING2( KNWOBJECT, 
            "NW: CNWNetworkHZCbCacheHandler: DoInsertInCityZoneCache() Begin,\
            cache->Count() = %d ", itemCount );
        
        if( itemCount > KMaxNumberOfItemsInCityZoneCache )
            {
            cache->Remove( itemCount-1 );
            cache->Compress();
            }
        retVal = pos;
        }
    
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: DoInsertInCityZoneCache() End,\
        retVal = %d ", retVal );
    
    return retVal;
    }
    
    
// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::Clear
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkHZCbCacheHandler::Clear( const THomeZoneCacheId aCacheId )
    {
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: Clear() Begin,\
        aCacheId = %d ", aCacheId );
    
    if( InitialisingStatus() != ECBSHZCacheReader_Ready )
        {
        NWLOGSTRING( KNWINT, 
            "NW: CNWNetworkHZCbCacheHandler::Clear(),\
             CBSHZCacheReader not ready!");
        return KErrNotReady;
        }

    TInt retVal = KErrNotFound;
    RArray<RMmCustomAPI::TViagCacheRecordContent> * cache;
    cache = Cache( aCacheId );
    ASSERT( cache );
    
    if( cache )
        {
        retVal = KErrNone;
        
        switch( aCacheId )
            {
            case EHZ_HomeZoneCache1:
            case EHZ_HomeZoneCache2:
            case EHZ_HomeZoneCache3:
                {
                RMmCustomAPI::TViagCacheRecordContent nullEntry;
                nullEntry.iLac = 0xffff;
                nullEntry.iCellId = 0xffff;

                RMmCustomAPI::TViagCacheRecordId sim_cache;
                sim_cache.iCacheId = aCacheId;
                sim_cache.iRecordId = 0;

                TInt itemCount = cache->Count();
                ASSERT( itemCount <= KMaxNumberOfItemsInHomeZoneCache );
                NWLOGSTRING2( KNWOBJECT, 
                    "NW: CNWNetworkHZCbCacheHandler: Clear(),\
                    cache->Count() = %d ", itemCount );
                
                // Remove all items from SIM by writing nullEntrys
                for( TInt i = 0; i < KMaxNumberOfItemsInHomeZoneCache; i++ )
                    {
                    sim_cache.iRecordId = i;
                    TInt err = iCustomAPI.WriteViagHomeZoneCache( sim_cache, 
                        nullEntry );
                        
                    // Update local cache also
                    if( !err )
                        {
                        if( i < itemCount )
                            {
                            cache->Remove( i );
                            }
                        cache->Insert( nullEntry, i );
                        }
                    else
                        {
                        // Error occured
                        retVal = err;
                        break;
                        }
                    }
                }
                break;
            case EHZ_CityZoneCache:
                {
                cache->Reset();
                }
                break;     
            default:
                // This should never happen
                ASSERT( EFalse );
                break;
            }
        cache->Compress();
        }
    
    NWLOGSTRING2( KNWOBJECT, 
        "NW: CNWNetworkHZCbCacheHandler: Clear() End,\
        retVal = %d ", retVal );
    
    return retVal;
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::SetInitialisingStatus
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkHZCbCacheHandler::SetInitialisingStatus( 
    const TInitializingStatus aStatus )
    {
    NWLOGSTRING2( KNWINT, 
        "NW: CNWNetworkHZCbCacheHandler: SetInitialisingStatus() Begin,\
        aStatus = %d", aStatus );
    
    iInitialisingStatus = aStatus;
    
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkHZCbCacheHandler: SetInitialisingStatus() End" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkHZCbCacheHandler::InitialisingStatus
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInitializingStatus CNWNetworkHZCbCacheHandler::InitialisingStatus( ) const
    {
    NWLOGSTRING( KNWINT, 
        "NW: CNWNetworkHZCbCacheHandler: InitialisingStatus() called" );
    return iInitialisingStatus;
    }
    
// ----------------------------------------------------------------------------
// CNWNetworkViagBaseEngine::ReadCbCacheSync
// Reads cache files syncronously.
// ----------------------------------------------------------------------------
//
void CNWNetworkHZCbCacheHandler::ReadCbCacheSync( TInt aStatus )
    {
    // Read caches
    NWLOGSTRING2( KNWINT, "NW:CNWNetworkHZCbCacheHandler: ReadCbCacheSync()\
         Begin, aStatus = %d ", aStatus );
    
    TBool retVal = ETrue;
    if ( aStatus == KErrNone )
        {
        if( ReadCacheFromSIM( iSimCache1, EHZ_HomeZoneCache1,
            KMaxNumberOfItemsInHomeZoneCache ) )
            {
            // Error Happened. Data is not valid.
            iSimCache1.Reset();
            retVal = EFalse;
            }
         
        // Print cache content
        if ( retVal )
            {
            PrintCacheContent( &iSimCache1, EHZ_HomeZoneCache1 );
            if( ReadCacheFromSIM( iSimCache2, EHZ_HomeZoneCache2,
                KMaxNumberOfItemsInHomeZoneCache ) )
                {
                // Error Happened. Data is not valid.
                iSimCache2.Reset();
                retVal = EFalse;
                }
            }
        if ( retVal )
            {
            // Print cache content
            PrintCacheContent( &iSimCache2, EHZ_HomeZoneCache2 );
            if( ReadCacheFromSIM( iSimCache3, EHZ_HomeZoneCache3,
                KMaxNumberOfItemsInHomeZoneCache ) )
                {
                // Error Happened. Data is not valid.
                iSimCache3.Reset();
                retVal = EFalse;
                }
            }
    
        if ( retVal )
            {
            // Print cache content
            PrintCacheContent( &iSimCache3, EHZ_HomeZoneCache3 );
            }
        }
    else
        {
        NWLOGSTRING2( KNWINT, 
        "NW: CBS HZ ReadCbCacheSync Cache Reading FAILED = %d", aStatus );
        iSimCache1.Reset();
        iSimCache2.Reset();
        iSimCache3.Reset();
        }
    SetInitialisingStatus( ECBSHZCacheReader_Ready );
    if ( iObserver )
        {
        iObserver->NotifyCachesInitialised( );
        }
    
    NWLOGSTRING( KNWINT, "NW:CNWNetworkHZCbCacheHandler::\
        ReadCbCacheSync() End" );
    }

// End of File
    

