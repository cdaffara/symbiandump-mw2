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



#ifndef CNWNETWORKVIAGCBENGINE_H
#define CNWNETWORKVIAGCBENGINE_H

//  INCLUDES
#include "MNWNetworkTimer.h"
#include <nwhandlingengine.h>
#include "CNWGsmMessageHandler.h"
#include "CNWNetworkHZCbCacheHandler.h"
#include "MHZCachesInitialisationObserver.h"
#include <mcbsmcnobserver.h> //For McbsMcnObserver
#include <rmmcustomapi.h>
#include <e32property.h>

// CONSTANTS
//timer values
#ifdef TESTINGVIAG
const TInt KViagTimerSixtySeconds = 6000000;
#else
const TInt KViagTimerSixtySeconds = 60000000;
#endif
    //operation values to HomeZone cache accessing
const TInt KAdd = 1;
const TInt KRemove = 2;
const TInt KCompare = 3;
//values for Viag stuff
//Defined in Viag HomeZone spec
const TInt KDynCacheCount = 5;
//The normal length of CB message for Viag HomeZone
const TInt KNWViagHzMsgLength = 12;
const TInt KNWViagHzCoordinateLength = 6;
//The maximun radius for HomeZone
const TInt KNWViagHomeZoneMaxRad = 10000;
//specified in HomeZone Mobile Terminal Service specification spec
const TUint KNWHomeZoneMsgId = 221; 

//Cache start and end values
const TInt KFirstCacheValue = 1;
const TInt KLastCacheValue = 4;
// Cache indexes
const TInt KFirstRecordIndex = 0;
const TInt KLastRecordIndex = 20;


// FORWARD DECLARATIONS
class CNWGsmSessionImplementation;
class CNWNetworkViagTimer;
class CMcn;
class CNWGsmMessageHandler;

// CLASS DECLARATION

/**
*  Monitors current network change event from MMEtel.
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkViagCbEngine : public CBase, 
                             public MCbsMcnObserver,
                             protected MNWNetworkTimer,
                             public MHZCachesInitialisationObserver
    {
    private:
        struct TViagZoneInfo
            {
            TUint32 iX;
            TUint32 iY;
            TUint32 iR2;
            TUint iZoneId;
            TBuf<13> iName;
            RMmCustomAPI::TActiveFlag iActiveFlag;
            };
        // Array of TViagZoneInfo struct data.
        typedef CArrayFixFlat< TViagZoneInfo > CViagZoneArray;
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNWNetworkViagCbEngine* NewL(
                        CNWNetworkViagBaseEngine& aBase,
                        TNWInfo& aNetworkInfo,
                        RMmCustomAPI& aCustomAPI,
                        TNWInterInfo& aInterNetworkInfo,
                        CNWGsmNetworkCellReselectionHandler& 
                        aCellReselectionHandler);
        /**
        * Destructor.
        */
        virtual ~CNWNetworkViagCbEngine();


    public: // Functions from MCbsMcnObserver

        /**
        *   This method is called when a message of a subscribed topic 
        *   is received from the network.
        *
        *   @param  aMcnMessage         MCN message received from the network.
        *   @return                     Result code
        */
        TInt CellInfoChanged( const TCbsMcnMessage& aMcnMessage );

        /**
        * Is called by CNWNetworkCurrentNetworkMonitor in case of 
        * cell re-selection.
        * @param 
        * @return Error code.
        */
        void CellReselection();



		 /**
        * Is called by CNWNetworkViagBaseEngine in case of Sat refresh to
        * HZ-EF or 
        * @param None.
        * @return
        */
        void SatRefreshCompleteNotification();

        /**
        * Is called by CNWNetworkViagBaseEngine in case of Cb algo needed 
        * to stop 
        * @param None.
        * @return
        */
    
        void StopCbAlgo();
        
        /**
        * Updates Viag HomeZone parameters from SIM.
        * 
        * @param 
        * @return Error code.
        */
        TInt UpdateViagHzParams();

        /**
        * Initialises caches
        * 
        */
        void InitilizeCaches();

    protected:  // Functions from MNWNetworkTimer
        
        /**
        * This method is called when network timer expires.
        *
        * @param
        * @return
        */
        void HandleTimeOut();

    public:  // Functions from MHZCachesInitialisationObserver
        /**
        * This method is called when HomeZone caches are read
        *
        * @param    aError
        * @return   None
        */
        void NotifyCachesInitialised( );
        

    private:

        /**
        * C++ default constructor.
        */
        CNWNetworkViagCbEngine( CNWNetworkViagBaseEngine& aBase,
                              TNWInfo& aNetworkInfo,
                              RMmCustomAPI& aCustomAPI,
                              TNWInterInfo& aInterNetworkInfo,
                              CNWGsmNetworkCellReselectionHandler& 
                              aCellReselectionHandler
                              );



        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
     private:  // New functions
        
        /**
        * Checks if there are active Home/CityZone(s).
        * 
        * @param 
        * @return Error code.
        */
        void IsActiveZone();


        /**
        * Registers to listen CBS messages.
        * 
        * @param aTopic     Topic id of the CBS messages to listen.
        * @return Error code.
        */
       void RegisterToCBSL( TUint16 aTopic );

        /**
        * Handles the zone indicators.
        * 
        * @param 
        * @return 
        */
        void StartIndicatorHandlingL( TBool  aRefreshOccured );

        /**
        * Handles the CBS messages.
        * 
        * @param  aMcnMessage         MCN message received from the network.
        * @return Possible error code
        */
        TInt HandleCBSMessages( const TCbsMcnMessage& aMcnMessage );
        
        /**
        * Checks if current zones coordinates are within some active 
        * Home-/CityZone limits.
        * @param  aX    Current zones X-coordinate. 
        * @param  aY    Current zones Y-coordinate. 
        * @return Possible error code
        */
        TInt CheckCellInHomeZoneArray( TUint32 aX, TUint32 aY );

        /**
        * Adds aZone to the HomeZone cache.
        * 
        * @param aZone target zone cache
        * @param aCellInfo cell information to be added
        * @return Possible error code
        */
        TInt AddToHomeZoneCache( const THomeZoneCacheId aZone, 
            const RMmCustomAPI::TViagCacheRecordContent aCellInfo );

        /**
        * Searches HomeZone caches. Stops in first match.
        * 
        * @param aCellInfo      Cell to searched
        * @param aActiveZones   List of zones to be searched
        * @return               Possible error code
        */
        TInt SearchHomeZoneCaches( 
            const RMmCustomAPI::TViagCacheRecordContent aCellInfo,
            const CViagZoneArray * aActiveZones );

        /**
        * Resets Viag's and MCN display's TNWInfo values.
        *
        * @param
        * @return
        */
        void ResetIndicatorValues();
        
        /**
        * Writes Viag-values to networkinfo, shared data and system agent.
        *
        * @param aIndicatorType 	Indicates which zone it is.
        * @param aCacheId 	The Id of the cache that text tag must be read from.
        * @return
        */
        void WriteViagValues( TInt aIndicatorType, TInt aCacheId );

        /**
        * Clear all HomeZone caches..
        *
        * @param None.
        * @return None.
        */
        void ClearHomeZoneCaches();

        /**
        * Validate the content of received HomeZone CBS messag
        *
        * @param aMsgData contains the date to be validate
        * @return ETrue if the aMsgData is valid. EFalse otherwise.
        */
        TBool ValidateHZCBSMessage( const TDesC & aMsgData );

        /**
        * Removes a first match cell caches
        *
        * @param aCellInfo      Source cell id
        * @return ETrue if operation was succesfull
        */
        TBool RemoveFromHomeZoneCaches(
            const RMmCustomAPI::TViagCacheRecordContent aCellInfo );

        /**
        * This method is called internally for handling cache insert operation
        * in CBS message handling
        *
        * @param aCache         Target cache
        * @param aCellInfo      Source cell id
        * @return ETrue if operation was succesfull
        */
        TInt DoHandleAddToCBSHZCache( 
            const THomeZoneCacheId aCache,
            const RMmCustomAPI::TViagCacheRecordContent aCellInfo );
 
    private:    // Data

        CViagZoneArray* iActiveZoneArray;

        /**
        * The type for array of Viag elements.
        */
        typedef CArrayFixFlat< RMmCustomAPI::TViagElement > TViagElements;

        TViagElements* iElements;
        // Viag parameters
        RMmCustomAPI::TViagParams iParams;

        //Enumeration for currently active zones
        enum TActiveZones
            {
            ENoneActive,
            EHomeActive,
            ECityActive,
            EBothActive
            };
        TActiveZones iIsActiveZones;

        //Indicates if we are listening VIAG messages.
        enum
            {
            EListeningNone,
            EListeningViag
            } iCBSState;

        //Indicates if 60 second timer is on.
        enum 
            {
            EIdle,
            ECbsSixtySecondsWaiting
            } iState;

        //Timer
        CNWNetworkViagTimer *   iTimer;
        //current cell info
        RMmCustomAPI::TViagCacheRecordContent iCellInfo;
        // Base servers for HomeZone algoritms     
         CNWNetworkViagBaseEngine& iBase;
        // Reference to the network info structure.
        TNWInfo&                iNWNetworkInfo;
        // Reference to the customAPI connection
        RMmCustomAPI&           iCustomAPI;
        // Pointer to MCN object
        CMcn*                   iMcn;
        // Reference to the internal network info structure.
        TNWInterInfo&           iInterNetworkInfo;
        //// Reference to the CNWGsmNetworkCellReselectionHandler
        CNWGsmNetworkCellReselectionHandler& iCellReselectionHandler;
        // Cached HomeZone CBS Message
        TCbsMcnMessage          iCachedHZCBSMessage;
        // Interface to CBS HomeZone/CityZone cache services
        CNWNetworkHZCbCacheHandler * iCacheHandler;
    };

#endif      // CNWNetworkViagCbEngine_H   
            
// End of File
