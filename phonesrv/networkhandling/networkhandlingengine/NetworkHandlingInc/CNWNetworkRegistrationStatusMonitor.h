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
* Description:  This file contains the header file of the CNWNetworkRegistrationStatusMonitor class.
*
*/


#ifndef CNWNETWORKREGISTRATIONSTATUS_H
#define CNWNETWORKREGISTRATIONSTATUS_H

//  INCLUDES
#include "CNWNetworkMonitorBase.h"
#include <e32property.h>

// FORWARD DECLARATIONS
class CNWMessageHandler;
class CNWNetworkRegistrationStatusReceiver;
// CLASS DECLARATION

/**
*  Monitors network registration status change event
*  from MMEtel and offers API to query that information. 
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkRegistrationStatusMonitor : public CNWNetworkMonitorBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        IMPORT_C static CNWNetworkRegistrationStatusMonitor* NewL(
            CNWMessageHandler& aOwner, 
            RMobilePhone& aPhone, 
            TNWInfo& aNetworkInfo, 
            RMmCustomAPI& aCustomAPI );

        /**
        * Destructor.
        */
        virtual ~CNWNetworkRegistrationStatusMonitor();
        
    public:
        
        /**
        * Casts the MMETel type network status to NW type.
        * 
        */
        void UpdateNetworkInfo();
        
        /**
        * Updates registration status P&S property
        * 
        */
        void UpdateRegistrationStatusProperty();

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
        * Executes the NotifyNetworkRegistrationStatusChange function.
        * 
        */
        void IssueRequest();

        

    private:
    
        /**
        * C++ default constructor.
        */
        CNWNetworkRegistrationStatusMonitor( 
            CNWMessageHandler& aOwner, 
            RMobilePhone& aPhone, 
            TNWInfo& aNetworkInfo, 
            RMmCustomAPI& aCustomAPI
            );
            
        /**
        * Second-phase constructor.
        */
        void ConstructL();                    

    private:    // Data
        // Receives networkregistration status in startup
        CNWNetworkRegistrationStatusReceiver* iNetworkRegistrationReceiver;
        // Current registration status info.
        RMobilePhone::TMobilePhoneRegistrationStatus iRegistrationStatus;
        // The interface to Publish And Subscribe
        RProperty iProperty;
   };

#endif      // CNWNETWORKREGISTRATIONSTATUS_H
            
// End of File
