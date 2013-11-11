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
* Description:  This file contains the header file of the 
*                CNWServiceProviderNameCommand class.
*
*/



#ifndef CNWNETWORKPORVIDERNAMECOMMAND_H
#define CNWNETWORKPORVIDERNAMECOMMAND_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CNWMessageHandler;
class RMmCustomAPI;

// CLASS DECLARATION

/**
*  Executes commands related to network provider name.
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkProviderNameCommand : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNWNetworkProviderNameCommand* NewL(
                    CNWMessageHandler& aOwner,
                    RMmCustomAPI& aCustomAPI,
                    TNWInfo& aNetworkInfo );
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkProviderNameCommand();


    protected:  // Functions from CActive

        /**
        * Cancels asyncronous request(s).
        */
        void DoCancel();

        /**
        * Informs object that asyncronous request is ready.
        */
        void RunL();
        
    public:  // New functions
        
        /**
        * Starts async. Network provider name query.
        */
        void IssueRequest();
        

    private:  // New functions
        
        /**
        * Update network info to the client's TNWInfo struct
        */
        void UpdateNetworkInfo();


    private:

        /**
        * C++ default constructor.
        */
        CNWNetworkProviderNameCommand( 
                CNWMessageHandler& aOwner,
                RMmCustomAPI& aCustomAPI,
                TNWInfo& aNetworkInfo );

    private:    // Data
        // CNWMessageHandler object that owns this object
        CNWMessageHandler& iOwner;
        // Reference to the Custom API connection
        RMmCustomAPI& iCustomAPI;
        // Reference to network info struct
        TNWInfo& iNetworkInfo;
        // Network Provider Name
        TNWProviderName iNetworkProviderName;

    };

#endif      // CNWNetworkProviderNameCommand_H   
            
// End of File
