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



#ifndef CNWSERVICEPROVIDERNAMECOMMAND_H
#define CNWSERVICEPROVIDERNAMECOMMAND_H

//  INCLUDES
#include <e32base.h>
#include <etelmm.h>

// FORWARD DECLARATIONS
class CNWMessageHandler;


// CLASS DECLARATION

/**
*  Executes commands related to programmable operator logo.
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWServiceProviderNameCommand : public CActive
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CNWServiceProviderNameCommand(
                CNWMessageHandler& aOwner,
                RMobilePhone& aPhone,
                TNWInfo& aNetworkInfo );

        /**
        * Destructor.
        */
        virtual ~CNWServiceProviderNameCommand();

    public: // New functions
        
        /**
        * Starts async. Service provider name query.
        */
        void IssueRequest();
        
        /**
        * Sets allow send message. If refresh is ongoing boolean gets EFalse otherwise value is ETrue
        */
        void SetAllowSendMessage( TBool aAllowSending );

    private:

        /**
        * Updates MMEtel Service Provider Name types to PE types.
        */
        void UpdateNetworkInfo();

    protected:  // Functions from CActive

        /**
        * Cancels asyncronous request(s).
        */
        void DoCancel();

        /**
        * Informs object that asyncronous request is ready.
        */
        void RunL();

    private:
        // CNWMessageHandler object that owns this object
        CNWMessageHandler& iOwner;
        // Reference to the MMETel phone connection
        RMobilePhone&   iPhone;
        // Reference to network info struct
        TNWInfo& iNWNetworkInfo;
        // Service Provider Name
        RMobilePhone::TMobilePhoneServiceProviderNameV2 iParamServiceProviderName;
        // Service Provider Name package .
        RMobilePhone::TMobilePhoneServiceProviderNameV2Pckg iServiceProviderNamePckg;
        // Allow ServiceProvidernameChange message to be sent
        TBool iAllowSending;

    };

#endif      // CNWSERVICEPROVIDERNAMECOMMAND_H
            
// End of File
