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
* Description:  This file contains the header file of the CNWNetworkViagCbEngine class.
*
*/



#ifndef CNWNETWORKHZCACHEHANDLER_H
#define CNWNETWORKHZCACHEHANDLER_H

//  INCLUDES
#include <rmmcustomapi.h>
#include <e32property.h>
#include "CNWNetworkViagTimer.h"
#include "MHZCachesInitialisationObserver.h"
#include "MNWNetworkEFReaderObserver.h"
#include "CNWNetworkEFReader.h"

// CONSTANTS

// DATA TYPES
enum THomeZoneCacheId
    {
    EHZ_HomeZoneCache1 = 1,
    EHZ_HomeZoneCache2 = 2,
    EHZ_HomeZoneCache3 = 3,
    EHZ_CityZoneCache = 4,
    EHZ_AllCaches = 100
    };    

enum TInitializingStatus
    {
    ECBSHZCacheReader_NotInitialised = 0,
    ECBSHZCacheReader_Initialising_Cache_HZ1,
    ECBSHZCacheReader_Ready = 100
    };
    
// FORWARD DECLARATIONS

// CLASS DECLARATION
class MNWNetworkEFReaderObserver;
/**
*  Handles HomeZone cache reading, searching and updating
*  
*
*  @lib networkhandling.lib
*  @since Series 60_3.0
*/
NONSHARABLE_CLASS( CNWNetworkHZCbCacheHandler ) : public CBase, 
    public MNWNetworkEFReaderObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNWNetworkHZCbCacheHandler* NewL( RMmCustomAPI& aCustomAPI );
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkHZCbCacheHandler( );

    public:

        /**
        *   This function is called for finding an item from cache.
        *
        *   @param  aCacheId            Source cache
        *   @return                     Result code. Index inside the cache. 
        *                               KErrNotFound if match didn't happen
        */
        TInt Find( THomeZoneCacheId aCacheId, 
            RMmCustomAPI::TViagCacheRecordContent aRecord );

        /**
        *   This function is called when adding an item in cache.
        *
        *   @param  aCache              Target cache
        *   @param  aRecord             A record to be added
        *   @return                     Result code.
        */
        TInt Insert( const THomeZoneCacheId aCacheId,
            const RMmCustomAPI::TViagCacheRecordContent aRecord );
             
        /**
        *   This function is called for removing an item from cache.
        *
        *   @param  aRecord               Target cache
        *   @param  aCacheId              A record to be removed
        *   @return                       Result code.
        */
        TInt Remove( const THomeZoneCacheId aCacheId, const TInt aIndex );
     
        /**
        *   This function is called for clearing a cache,
        *
        *   @param  aRecord               Target cache
        *   @return                       Result code.
        */
        TInt Clear( const THomeZoneCacheId aCacheId );

        /**
        *   This function is called for refreshing local cache from SIM data
        *
        *   @return                       None
        */
        void InitialiseHomeZoneCachesAsync( 
            MHZCachesInitialisationObserver * aObserver );
        
        /**
        *   This function is called for getting object internal status
        *
        *   @param  aStatus               New status
        *   @return                       Objects current status
        */
        TInitializingStatus InitialisingStatus( ) const;


    private:

        /**
        * C++ default constructor.
        */
        CNWNetworkHZCbCacheHandler( RMmCustomAPI& aCustomAPI );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( );
        
        /**
        *   This function is called internally form reading HZ cache items 
        *   from SIM to an array.
        *
        *   @param  aDestArray            Items to be insereted
        *   @param  aCacheId              Source cache.
        *   @param  aNumberofItems        The amount of items to be read.
        *   @return                       Result code.
        */
        TInt ReadCacheFromSIM( 
            RArray<RMmCustomAPI::TViagCacheRecordContent> & aDestArray,
            const THomeZoneCacheId aCacheId, const TInt aNumberofItems );

        /**
        *   This function is called internally to get a pointer in local 
        *   caches.
        *
        *   @param  THomeZoneCacheId      Souce cache.
        *   @return                       Cache pointer.
        */
        RArray<RMmCustomAPI::TViagCacheRecordContent> * Cache( 
            const THomeZoneCacheId aCacheId );

        /**
        *   This function is called internally when adding a HomeZone
        *   cache item in cache.
        *
        *   @param  aRecord               A record to be added.
        *   @param  aCacheId              Target cacge.
        *   @return                       Result code.
        */
        TInt DoInsertInHomeZoneCache( 
            const RMmCustomAPI::TViagCacheRecordContent aRecord,
            const THomeZoneCacheId aCacheId );

        /**
        *   This function is called  internally when adding a CityZone
        *   cache item in cache.
        *
        *   @param  aRecord               A Record to be added
        *   @return                       Result code.
        */
        TInt DoInsertInCityZoneCache( 
            const RMmCustomAPI::TViagCacheRecordContent aRecord );
            
        /**
        *   This function is called  internally for find empty slot
        *   from a cache,
        *
        *   @param  aCacheId              Source cache
        *   @return                       Result code.
        */
        TInt FindNullEntry( const THomeZoneCacheId aCacheId );

        /**
        *   This function is called for setting object internal status
        *
        *   @param  aStatus               New status
        *   @return                       None
        */
        void SetInitialisingStatus( 
            const TInitializingStatus aStatus );

        
    public: // From MNWNetworkEFReaderObserver
       
        /**
        *   This function is called when Cache is ready for reading.
        *
        *   @return                       None
        *
        */            
        void ReadCbCacheSync( TInt aStatus );
            
        
     private:
        // Custom API for SIM activities.
        RMmCustomAPI& iCustomAPI;   // Uses only
        
        // Caches
        RArray<RMmCustomAPI::TViagCacheRecordContent> iSimCache1;
        RArray<RMmCustomAPI::TViagCacheRecordContent> iSimCache2;
        RArray<RMmCustomAPI::TViagCacheRecordContent> iSimCache3;
        RArray<RMmCustomAPI::TViagCacheRecordContent> iDynCache;
        // Internal status
        TInitializingStatus    iInitialisingStatus;
        // Observer to notify
        MHZCachesInitialisationObserver * iObserver;
        // Cache Reader
        CNWNetworkEFReader *   iSimCacheMonitor;
    };

#endif      // CNWNETWORKHZCACHEHANDLER_H
            
// End of File
