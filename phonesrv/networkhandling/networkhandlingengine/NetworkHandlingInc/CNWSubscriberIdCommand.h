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



#ifndef CNWSUBSCRIBERIDCOMMAND_H
#define CNWSUBSCRIBERIDCOMMAND_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CNWMessageHandler;
class RMobilePhone;
struct TNWInterInfo;

// CLASS DECLARATION

/**
*  Executes commands related to subsriber id.
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWSubscriberIdCommand : public CActive
    {
    public:  // Constructors and destructor
    
        /**
        * Two-phased constructor.
        */
        static CNWSubscriberIdCommand* NewL(
                                    CNWMessageHandler& aOwner,
                                    RMobilePhone& aPhone,
                                    TNWInterInfo& aInterNetworkInfo );
        
        /**
        * Destructor.
        */
        virtual ~CNWSubscriberIdCommand();


    protected:  // Functions from CActive

        /**
        * Cancels asyncronous request(s).
        * 
        */
        void DoCancel();

        /**
        * Informs object that asyncronous request is ready.
        * 
        */
        void RunL();

    public:  // New functions
        
        /**
        * Starts async. SubsciberId query.
        * 
        */
        void IssueRequest();
        

    private:

        /**
        * C++ default constructor.
        */
        CNWSubscriberIdCommand( CNWMessageHandler& aOwner,
                                RMobilePhone& aPhone,
                                TNWInterInfo& aInterNetworkInfo );


    private:    // Data
        // CNWMessageHandler object that owns this object
        CNWMessageHandler& iOwner;
        // Reference to the MMETel phone connection
        RMobilePhone&   iPhone;
        // Reference to internal network info struct
        TNWInterInfo& iInterNetworkInfo;
        //Counter for subscriberId attempts
        TInt iSubscriberIdCounter;

    };

#endif      // CNWSubscriberIdCommand_H   
            
// End of File
