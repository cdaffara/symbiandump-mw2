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



#ifndef CNWGSMMESSAGEHANDLER_H 
#define CNWGSMMESSAGEHANDLER_H 

//  INCLUDES
#include <nwhandlingengine.h>
#include "CNWMessageHandler.h"
#include "CNWGsmSessionImplementation.h"
#include "CNWGsmNetworkCellReselectionHandler.h"


// FORWARD DECLARATIONS
class MNWMessageObserver;
class CNWGsmSessionImplementation;

// CLASS DECLARATION

/**
*  Handles GSM protocol specific network related messages.
*
*  @lib networkhandling.dll
*  @since Series 60_2.8
*/
class CNWGsmMessageHandler : public CNWMessageHandler
    {
    public:

        /**
        * Two-phased constructor.
        */
        static CNWGsmMessageHandler* NewL( 
                        CNWGsmSessionImplementation& aNetworkData, 
                        MNWMessageObserver& aMessageObserver,
                        TNWInfo& aNWInfo );

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CNWGsmMessageHandler();

    public: // New functions

        /**
        * Reroutes messages to the client
        * @param aMessage is the message id
        */
        void SendMessage( MNWMessageObserver::TNWMessages aMessage );

        /**
        * C++ default constructor.
        */
        CNWGsmMessageHandler( CNWGsmSessionImplementation& aNetworkData, 
                              MNWMessageObserver& aMessageObserver,
                              TNWInfo& aNWInfo );

        /**
        * Handle Subscribe ID change
        */
        void HandleSubscriberIdChange();
        
        /**
        * Update Operator name reading status
        */
        void HandleUpdateReadingStatus( const TNWRead& aElementFile, 
                TBool aReadStatus );

    private:

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    

    private:    // Data
        //Handles cell reselection message
        CNWGsmNetworkCellReselectionHandler* iCellReselectionHandler;
    };

#endif      // CNWGsmMessageHandler_H   
            
// End of File
