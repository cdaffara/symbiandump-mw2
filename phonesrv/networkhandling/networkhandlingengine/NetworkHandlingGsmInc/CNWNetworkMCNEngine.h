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
* Description:  This file contains the header file of the CNWNetworkMcnEngine class.
*
*/


#ifndef CNWNETWORKMCNENGINE_H
#define CNWNETWORKMCNENGINE_H

//  INCLUDES
#include <mcbsmcnobserver.h> //For MCbsMcnObserver
#include "MNWNetworkTimer.h"
#include <e32base.h>
#include <etelmm.h>


//#define TESTINGMCN
// CONSTANTS
const TUint KNWMcnMsgId = 50; 

#ifdef TESTINGMCN
const TInt KWaitTimeForNewMcn = 120000000;
const TInt KWaitTimeToClearOldMcnIndicator = 5000000;
const TInt KWaitTimeForNewMcnAfterClearingOld = KWaitTimeForNewMcn \
            - KWaitTimeToClearOldMcnIndicator;
#else
    //timer values
const TInt KWaitTimeForNewMcn = 120000000;
const TInt KWaitTimeToClearOldMcnIndicator = 5000000;
const TInt KWaitTimeForNewMcnAfterClearingOld = KWaitTimeForNewMcn \
            - KWaitTimeToClearOldMcnIndicator;
#endif

    //for shared data
const TInt KOptionalDefaultValue  = 1;


// FORWARD DECLARATIONS
class CNWGsmSessionImplementation;
class CNWNetworkMcnTimer;
class CMcnTopicArray;
class CMcn;
class CNWGsmMessageHandler;
class CRepository;

// CLASS DECLARATION

/**
* Monitors current network change event from MMEtel.
*
* @lib Networkhandling.lib
* @since Series 60_2.8 
*/
class CNWNetworkMcnEngine : public CActive,
                            public MCbsMcnObserver,
                            protected MNWNetworkTimer
    {
    public:  // Constructors and destructor
        /**
        * C++ default constructor.
        */
        CNWNetworkMcnEngine( CNWGsmMessageHandler& aMessageHandler,
                             TNWInfo& aNetworkInfo,
                             RMmCustomAPI& aCustomAPI,
                             CNWGsmNetworkCellReselectionHandler& 
                             aCellReselectionHandler );

        /**
        * Two-phased constructor.
        */
        static CNWNetworkMcnEngine* NewL( 
                             CNWGsmMessageHandler& aMessageHandler,
                             TNWInfo& aNetworkInfo,
                             RMmCustomAPI& aCustomAPI,
                             CNWGsmNetworkCellReselectionHandler& 
                             aCellReselectionHandler );

        /**
        * Destructor.
        */
        virtual ~CNWNetworkMcnEngine();

    public: // Functions from  MCbsMcnObserver
        /**
        *   This method is called when a message of a subscribed topic 
        *   is received from the network.
        *   @param  aMcnMessage         MCN message received from the network.
        *   @return                     Result code
        */
        TInt CellInfoChanged( const TCbsMcnMessage& aMcnMessage );

    public: // Functions from SharedDataI
        /**
        * Is executed when shared data's cell info display value changes.
        * @param aPtr
        * @return Error code.
        */
        static TInt SharedDataCallBack(TAny* aPtr);

        /**
        * Is called by CNWNetworkCurrentNetworkMonitor in case of
        * cell re-selection.
        */
        void CellReselection();


    private:
        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        /**
        * Reads current cell info display setting value from shared data
        * or Central Repository.
        * @return Error code.
        */
        void GetMcnDisplayValue();
 
        /**
        * Registers to listen CBS messages.
        * @param aTopic     Topic id of the CBS messages to listen.
        * @return Error code.
        */
       void RegisterToCBSL( TUint16 aTopic );

        /**
        * Handles the MCN display.
        * 
        */
        void StartIndicatorHandling();

        
        /**
        * Handles the CBS messages.
        * @param  aMcnMessage         MCN message received from the network.
        * @return Possible error code
        */
        TInt HandleCBSMessages( const TCbsMcnMessage& aMcnMessage );
 
        /**
        * Resets MCN display's TNWInfo values.
        *
        */
        void ResetIndicatorValues();
        
        
        
        /**
        * Notify MCN display changed.
        *
        */
        void NotifyMcnDisplayChanged();
        
    protected: // Functions from MNWNetworkTimer
        /**
        * This method is called when network timer expires.
        *
        */
        void HandleTimeOut();
        
protected:  // Functions from CActive

        /**
        * Cancels asyncronous request(s).
        */
        void DoCancel();

        /**
        * Informs object that asyncronous request is ready.
        */
        void RunL();
        
    private:    // Data

        //States
        enum
            {
            EListeningNone,
            EListeningMCN,
            } iCBSState;

        enum 
            {
            EIdle,
            ECbsWaitingToClearMcnIndicator,
            ECbsWaitingForNewMcn 
            } iState;

        //Timer
        CNWNetworkMcnTimer* iTimer;
        //new cellId that will be used to comparization
        TUint iComparedCellId;

        // Reference to CNWGsmMessageHandler
        CNWGsmMessageHandler&   iGsmMessageHandler;
        // Reference to the network info structure.
        TNWInfo&                iNWNetworkInfo;
        // Reference to the customAPI connection
        RMmCustomAPI&           iCustomAPI;
        // Reference to the CNWGsmNetworkCellReselectionHandler
        CNWGsmNetworkCellReselectionHandler& iCellReselectionHandler;
        // Pointer to MCN object
        CMcn*                   iMcn;
        //Boolean indicating if to listen MCN Display area
        TBool iListenMCNDisplay;
        
        //Handle to Central repository
        CRepository* iRepository;
    };

#endif      // CNWNETWORKMCNENGINE_H  
            
// End of File
