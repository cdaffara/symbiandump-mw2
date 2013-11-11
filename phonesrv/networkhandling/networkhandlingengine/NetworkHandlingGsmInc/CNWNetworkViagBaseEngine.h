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
* Description:  Monitors network connection failure.
*
*/



#ifndef CNWNETWORKVIAGBASEENGINE_H
#define CNWNETWORKVIAGBASEENGINE_H

//  INCLUDES
#include <nwhandlingengine.h>
#include "CNWMessageHandler.h"

#include "CNWNetworkMonitorBase.h"
#include "CNWGsmMessageHandler.h"
#include "MNWNetworkTimer.h"
#include <etelmm.h>
#include <e32property.h>
#include <charconv.h>
#include <f32file.h>


// FORWARD DECLARATIONS
class CNWGsmMessageHandler;
class CNWNetworkViagSimReader;
class CNWNetworkViagRefresh;
class CNWGsmNetworkCellReselectionHandler;
class CNWNetworkViagCbEngine;
class CNWNetworkViagLcEngine;
class CNWNetworkSystemEventMonitor;
class CNWGsmMessageHandler;
class CNWNetworkViagTimer;

// CLASS DECLARATION


//CONSTANTS

// Refresh file list
typedef TBuf16<242> TViagRefreshFiles;


//Minimal Imsi checking length
const TInt KNWViagMinImsiLength = 5;
const TInt KNWViagMcc = 262;
const TInt KNWViagMnc = 7;
const TInt KNWZoneTagLength = 12;

//Viag IMSI start from 26207
_LIT( KNWViagImsiIdentity, "26207" );
//Viag IMSI for 3g
_LIT( KNWViagImsi3gIdentity, "26208" );


//  HZ Elementary files 
enum TNWNetworkElementFile
    {
    EFileSubscribedOne      = 0x6F81, 
    EFileSubscribedTwo      = 0x6F82,
    EFileSubscribedThree    = 0x6F83,
    EFileSubscribedFour     = 0x6F84,
    EFileHZ                 = 0x6F60,
    EFileTags               = 0x6F80,
    EFileSettings           = 0x6F87,
    EFileProgrammableList   = 0x6FC6, // EFopl
    EFileProgrammableOpereator = 0x6FC5,    // EFpnn       
    EFileNetworkProvider       = 0x6F14,        
    EFileNetworkProvidersecond = 0x6F18,              
    EFileServiceProvider       = 0x6F46,  // EFspn 
    EFileUnknown               = 0x2000
    };
    

// Homezone alogorithms
enum TNWViagAlgo
    {
    EWNAlgoNone,
    EWNAlgoLc, 
    EWNAlgoCb
    };

// Homezone initilaization states
enum TNWInit
    {
    EInitNotReady,
    EInitOngoing,
    EInitReady
    };
 
struct TNWZoneTags
    {
    TBuf<KNWZoneTagLength> iZoneTag;
    };
// Zone Tags Array. 
typedef CArrayFixFlat< TNWZoneTags > CZoneTagsArray;

/**
*  Monitors network connection failure.
*
*  @lib Networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkViagBaseEngine : CBase 
                               , protected MNWNetworkTimer
    {
    public:  // Constructors and destructor
        
                                  
       
        /**
        * Two-phased constructor.
        */
        static CNWNetworkViagBaseEngine* NewL(
                        CNWGsmNetworkCellReselectionHandler& aOwner,
                        CNWGsmMessageHandler& aMessageHandler,
                        RMmCustomAPI& aCustomAPI,
                        TNWInfo& aNetworkInfo,
                        TNWInterInfo& aInterNetworkInfo
                        );
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkViagBaseEngine();

        /**
        *   Is called by CNWNetworkCurrentNetworkMonitor in case of cell re-selection.
        *
        *   @param 
        *   @return  None.
        */
        void CellReselection();

        /**
        *   Is called when the HZ file of caches are refreshed .
        *
        *   @param 
        *   @return  None.
        */
        void SatRefreshCompleteNotification();
        
        
        /**
        *   Reroutes a message to the Client object.
        *
        *   @param aMessage     Messge to the client.   
        *   @return  None.
        */
        void SendMessage( MNWMessageObserver::TNWMessages aMessage );

        /**
        *   updates a cache
        *
        *   @param aElementFile     Element file in the sim.
        *   @param aResponse        Response information from sim.
        *   @return  None.
        */
        TBool UpdateCache( 
                TNWNetworkElementFile aElementFile,
                const TDesC8& aResponse );
        
        /**
        *   Sets Lc to supported or not supported.
        *   @param aLcSupported     Is Lc algorithm supported.
        *   @return  None.
        */
        void SetLcSupported( TBool aLcSupported );

        /**
        *   Writes Viag-values to networkinfo, shared data and system agent.
        *
        *   @param aIndicatorType     Indicate which zone is 
        *   @param aViagText        Text to display in UI.
        *   @return  None.
        */
           void WriteViagValues( const TNWViagIndicatorType aIndicatorType,    //Indicates which zone it is
                                 const TInt& aZoneId );       // Viag text 

        /**
        *   Handle system events
        *   @param aValue .
        *   @param aState 
        *   @return  None.
        */         
        void HandleSystemEventL( const TInt aValue, const TInt aState );
        
        
        /**
        *   Set ZoneTags to Zonetags array
        *   @param zoneTags        Tags to be added to an array. 
        *   @return  None.
        */         
        void SetZoneTag( TInt aZoneId, const TDesC& zoneStruct );
        
        /**
        *   Creates iViagrefresh class. Gets called when CBS server is ready. 
        *   @param   None. 
        *   @return  None.
        */  
        void StartupReadyL();
        
        /**
        *   Notifies SAT server that all SIM files are read. 
        *   @param   None. 
        *   @return  None.
        */  
        void CompleteRefresh();
        
        /**
        *   Updates EF reading status. 
        *   @param   aElementFile    file to update
        *   @param   aReadStatus     True or False. True indicates file has been read.
        *   @return  None.
        */
        void UpdateEFReadingState( const TNWRead& aElementFile, TBool aReadStatus );
        
        /**
        *   Starts the timer for waiting cell reselection notification 
        *   @param   None. 
        *   @return  None.
        */  
        void StartTimer();
        
        /**
        *   Stops the timer. 
        *   @param   None. 
        *   @return  None.
        */  
        void StopTimer();
        
        /**
        * Decodes 7-bit message representation.
        * @since - 3.1
        * @param aTag    text tag to be decoded.
        * @return None
        */
        void DefaultAlphabetDecode( const TDesC8& aTag, TDes& aZoneText );
    protected: // From MNWNetworkTimer
        /**
        *   Calls Cellreselction after timeout. 
        *   @param   None. 
        *   @return  None.
        */  
        void HandleTimeOut();
        
    private:

        
          /**
        * C++ default constructor.
        */
        CNWNetworkViagBaseEngine( CNWGsmNetworkCellReselectionHandler& aOwner,
                                  CNWGsmMessageHandler& aMessageHandler,
                                  RMmCustomAPI& aCustomAPI,
                                  TNWInfo& aNetworkInfo,
                                  TNWInterInfo& aInterNetworkInfo
                                );
                                  
       
          
         /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        *   Initialize Viag-values.
        *
        *   @return  None.
        */
        void ViagInitialize();
                
        /**
        * Checks if it's Viag SIM card we have in ME
        * @since -
        * @param -
        * @return ETrue or EFalse
        */
        TBool IsViagImsi( ) const;

        /**
        * Initializes CharacterSetConverter and
        * connects to file server
        * @since 3.1
        * @param -
        * @return None
        */
        void InitializeCharSetConverterL();
    
        
    private:    // Data
        // Reference to owner of the component      
        CNWGsmNetworkCellReselectionHandler& iOwner;
        // Message Handler
        CNWGsmMessageHandler& iMessageHandler;
        // Reference to CustomAPI
        RMmCustomAPI& iCustomAPI;
        // Reference to the network info structure.
        TNWInfo& iNWNetworkInfo;
        // Reference to the internal network info structure.
        TNWInterInfo& iInterNetworkInfo;
        // LC support boolean
        TBool iLcSupported;
        // Cell Broadcast activation boolean.
        TBool iCbActivation;
        // Is sim initialized
        TBool iSimOk;
        // General activation 
        TBool iActivation;
        // Ininitializion ready
        TNWInit iInitReady;
        // Viag sim reader connection
        CNWNetworkViagSimReader* iViagSimReader;
        // Viag Refresh connection
        CNWNetworkViagRefresh* iViagRefresh;

        // Viag CB engine connection
        CNWNetworkViagCbEngine* iViagCbEngine;
        // Viag LC engine connection
        CNWNetworkViagLcEngine* iViagLcEngine;
        // Array of Element files.
        RArray<TNWNetworkElementFile> iElementFiles;    
  
        // Active viag algorithm. 
        TNWViagAlgo    iViagActiveAlgo;
        // Sim status monitor
        CNWNetworkSystemEventMonitor* iSimStateMonitor;
        // Call State Monitor
        CNWNetworkSystemEventMonitor* iCallStateMonitor;
        // Active call
        TBool iActiveCall;  

        CZoneTagsArray* iZoneTags;
        // Reading onGoing
        TBool iReadingOnGoing;
        // Indicates when Service Provider Name has been read after refresh. ETrue when ready. 
        TBool iSPNEFRead;
        // Indicates when NetworkProvider Name is read.
        TBool iNPNEFRead;
        // Indicates when Programmable opearator name is read.
        TBool iProgEFRead;
        // Indicates when when Home Zone Elementary Files has been read.
        TBool iViagEFRead;
        // Timer
        CNWNetworkViagTimer * iTimer;
        // GPRS state monitor
        CNWNetworkSystemEventMonitor* iGPRSStateMonitor;
        // Character set converter
        CCnvCharacterSetConverter* iCharacterSetConverter;
        // file server session
        RFs iFs;
    };

#endif      // CNWNetworkViagBaseEngine_H   
            
// End of File

