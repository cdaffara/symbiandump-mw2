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
* Description:  Header class for CNWGsmNetworkCellReselectionHandler.cpp
*
*/



#ifndef CNWGSMNETWORKCELLRESELECTIONHANDLER_H
#define CNWGSMNETWORKCELLRESELECTIONHANDLER_H

//  INCLUDES
#include <nwhandlingengine.h>
#include <rmmcustomapi.h>
#include <cmcn.h>
#include <e32property.h>

// CONSTANTS
//For SharedData
const TInt KNWCbsNotStarted = 0;
const TInt KNWCbsStarted    = 1;


// FORWARD DECLARATIONS
class CNWNetworkViagBaseEngine;
class CNWNetworkMcnEngine;
class RMobilePhone;
class RMmCustomAPI;
class CNWGsmMessageHandler;
class CMcn;


// CLASS DECLARATION

/**
*  This file contains the header file of the class 
*  CNWGsmNetworkCellReselectionHandler, 
*  Handles cell reselection events
*
*  @lib Networkhandling.lib
*  @since -
*/
class CNWGsmNetworkCellReselectionHandler : public CActive
    {
    public:  // Constructors and destructor
       
        /**
        * Two-phased constructor.
        */
        static CNWGsmNetworkCellReselectionHandler* NewL( 
                    CNWGsmMessageHandler& aOwner,
                    RMobilePhone& aPhone, 
                    TNWInfo& aNetworkInfo, 
                    RMmCustomAPI& aCustomAPI,
                    TNWInterInfo& aInterNetworkInfo );

        /**
        * Destructor.
        */
        virtual ~CNWGsmNetworkCellReselectionHandler();

    public:        
        /**
        * Calls appropriate engines to handle cell reselection 
        */
        void HandleCellReselection( );

        /**
        * Returns pointer to CMcn object.
        * @return Ptr to CMcn object.
        */
        CMcn* GetMcnPtr();

        /**
        * Calls notify request from Publish and Subscribe
        */
        void NotifyCbsServerStateChanged();
        
        /**
        * Updates Operator Name reading status
        */
        void UpdateReadingStatus( const TNWRead& aElementFile, 
                TBool aReadStatus );
        
    protected:  // Functions from CActive

        /**
        * Cancels asyncronous request(s).
        */
        void DoCancel();

        /**
        * Informs object that asyncronous request is ready.
        */
        void RunL();

        /**
        * Function to handle error during running the operation
        * @return KErrNone, when error has been handled
        */
        virtual TInt RunError( TInt aError );
        
    public: // Functions from MSharedDataNotifyHandler 


    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        /**
        * C++ default constructor.
        */
        CNWGsmNetworkCellReselectionHandler( 
                        CNWGsmMessageHandler& aOwner,
                        RMobilePhone& aPhone,
                        TNWInfo& aNetworkInfo,
                        RMmCustomAPI& aCustomAPI,
                        TNWInterInfo& aInterNetworkInfo );


    private:    // Data
        // Ptr to CNWNetworkViagEngine object
      	CNWNetworkViagBaseEngine* iViagEngine;
        // Ptr to CNWNetworkMCNEngine object
        CNWNetworkMcnEngine* iMcnEngine;
        // CNWGsmSessionImplementation object that owns this object
        CNWGsmMessageHandler& iOwner;
        // Reference to the MMETel phone connection
        RMobilePhone& iPhone;
        // Reference to home network info struct
        TNWInfo& iNWNetworkInfo;
        // Reference to the Custom API connection
        RMmCustomAPI& iCustomAPI;
        // Reference to the internal network info structure.
        TNWInterInfo&     iInterNetworkInfo;
        //Mcn object for CBS messages
        CMcn* iMcn;
        //Indicating for CSB Server state
        TBool iMcnState;
        // The interface to Publish And Subscribe.
        RProperty               iProperty;

    };

#endif      // CNWGSMNETWORKCELLRESELECTIONHANDLER_H  
            
// End of File
