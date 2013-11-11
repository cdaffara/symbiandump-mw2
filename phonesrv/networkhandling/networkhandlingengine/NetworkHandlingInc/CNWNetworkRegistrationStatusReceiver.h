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
* Description:  This file contains the header file of the CNWNetworkRegistrationStatusReceiver class.
*
*/


#ifndef CNWNETWORKREGISTRATIONSTATUSRECEIVER_H
#define CNWNETWORKREGISTRATIONSTATUSRECEIVER_H

//  INCLUDES
#include <nwhandlingengine.h>

// FORWARD DECLARATIONS
class CNWNetworkRegistrationStatusMonitor;
class RMobilePhone;

// CLASS DECLARATION

/**
*  Monitors network registration status change event
*  from MMEtel and offers API to query that information. 
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkRegistrationStatusReceiver : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNWNetworkRegistrationStatusReceiver* NewL(
            CNWNetworkRegistrationStatusMonitor& aOwner, 
            RMobilePhone& aPhone, 
            RMobilePhone::TMobilePhoneRegistrationStatus& aRegistrationStatus );

        /**
        * Destructor.
        */
        virtual ~CNWNetworkRegistrationStatusReceiver();
    public:
        
        /**
        * Executes the NotifyNetworkRegistrationStatusChange function.
        * 
        */
        void IssueRequest();

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

    private:
    
        /**
        * C++ default constructor.
        */
        CNWNetworkRegistrationStatusReceiver( 
            CNWNetworkRegistrationStatusMonitor& aOwner, 
            RMobilePhone& aPhone, 
            RMobilePhone::TMobilePhoneRegistrationStatus& iRegistrationStatus
            );
            
        /**
        * Second-phase constructor.
        */
        void ConstructL();                    

    private:    // Data
        // Reference to registration status info.
        CNWNetworkRegistrationStatusMonitor& iOwner;
        // Reference to RMobilePhone
        RMobilePhone& iPhone;
        // Reference to current registration status info.
        RMobilePhone::TMobilePhoneRegistrationStatus& iRegistrationStatus;
 
    };

#endif      // CNWNETWORKREGISTRATIONSTATUSRECEIVER_H
// End of File
