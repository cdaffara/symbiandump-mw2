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
* Description:  Message handler
*
*/



#ifndef CNWMESSAGEHANDLER_H 
#define CNWMESSAGEHANDLER_H 

//  INCLUDES
#include <nwhandlingengine.h>
#include <cnwsession.h>
#include <mmtsy_names.h>
#include <etel.h>
#include <rmmcustomapi.h>
#include <etelmm.h>

// CONSTANTS
// Max length of network provider name.
const TInt KNWSubscriberIdLength = 15;
// Max value of the SubscriberIdCounter.
const TInt KMaxSubscriberIdRequests = 10;


// DATA TYPES
// Subscriber identity
typedef TBuf<KNWSubscriberIdLength> TNWSubscriberId;

// CLASS DECLARATION
//Internal Network info definition
struct TNWInterInfo
    {
    TUint iCellId;                                       // Cell Id
    TUint iLac;                                          // Location area code
    TNWSubscriberId iSubscriberId;                       // Subscriber identity
    TBool iAreaKnown;                                    // Location Area known used for CELL_DCH mode
    };


// FORWARD DECLARATIONS
class CNWNetworkMonitorBase;
class CNWNetworkCurrentNetworkMonitor;
class CNWNetworkModeMonitor;
class CNWNetworkRegistrationStatusMonitor;
class CNWProgrammableOperatorNameCommand;
class CNWServiceProviderNameCommand;
class CNWNetworkProviderNameCommand;
class CNWSubscriberIdCommand;
class RMmCustomAPI;

// CLASS DECLARATION

/**
*  Handles network related messages.
*
*  @lib networkhandling.dll
*  @since Series 60_2.8
*/
class CNWMessageHandler : public CBase
    {
    public:  // Destructor

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CNWMessageHandler();

    public: // New functions

        /**
        * Reroutes messages to the client
        * @param aMessage is the message id
        */
        IMPORT_C virtual void SendMessage( 
                MNWMessageObserver::TNWMessages aMessage );


        #ifdef TEF_LOGGING_ENABLED
        /**
        * Help function for logging.
        * @param aMessage MNWMessageObserver::TNWMessages aMessage 
        * @return message name, given the id of message.
        */
        TPtrC GetNameByMessage( MNWMessageObserver::TNWMessages aMessage ) const;
        #endif

        /**
        * Handle Subscribe ID change
        */
        virtual void HandleSubscriberIdChange();

        /**
        * Reroutes error messages to the client
        * @param aOperation is failed operation
        * @param aErrorCode is returned Symbion OS Error Code
        */
        void SendErrorMessage( 
                MNWMessageObserver::TNWOperation aOperation, 
                TInt aErrorCode );
        
        /**
        * Update Operator name reading status
        */
        virtual void HandleUpdateReadingStatus(  
                const TNWRead& aElementFile, TBool aReadStatus );

    protected:

        /**
        * C++ default constructor.
        */
        IMPORT_C CNWMessageHandler( CNWSession& aNetworkData, 
                                    MNWMessageObserver& aMessageObserver,
                                    TNWInfo& aNWInfo );

        /**
        * Initializes base class member variables
        */
        void BaseConstructL();


    protected:  // Data
        // Array holding network monitors.
        RPointerArray<class CNWNetworkMonitorBase>  iMonitorContainer;
        //Pointer to the CNWNetworkProviderNameCommand object
        CNWNetworkProviderNameCommand*      iNetworkProviderNameCommand;
        //Pointer to the CNWSubscriberIdCommandCommand object
        CNWSubscriberIdCommand*             iSubscriberIdCommand;
        //Pointer to the CNWProgrammableOperatorNameCommand object
        CNWProgrammableOperatorNameCommand* iProgrammableOperatorNameCommand;
        //Pointer to the CNWServiceProviderNameHandler object
        CNWServiceProviderNameCommand*      iServiceProviderNameCommand;
        // Tells if phone is registered to network
        TBool                       iIsRegistered;
        // CNWSession object which handles network related commands
        CNWSession&   iNetworkData;
        //Reference to MNWMessageObserver
        MNWMessageObserver&         iMessageObserver;
        // Reference to the client's network info struct
        TNWInfo&                    iNetworkInfo;
        // ETel server connection
        RTelServer                  iServer;
        // ETel phone connection
        RMobilePhone                iPhone;
        //Custom API object
        RMmCustomAPI                iCustomAPI;
        // Pointer to the internal network info structure.
        TNWInterInfo                iInterNetworkInfo;
        // Previous network status
        TNWStatus iPreviousStatus;
    };

#endif      // CNWMessageHandler_H   
            
// End of File
