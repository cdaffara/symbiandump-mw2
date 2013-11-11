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
* Description:  This file contains the header file of the CNWNetworkModeMonitor class.
*
*/


#ifndef CNWNETWORKMODEMONITOR_H
#define CNWNETWORKMODEMONITOR_H

//  INCLUDES
#include "CNWNetworkMonitorBase.h"
#include <networkhandlingdomainpskeys.h>
#include <e32property.h>

// FORWARD DECLARATIONS
class CNWMessageHandler;

// CLASS DECLARATION

/**
*  Monitors network mode change event from MMEtel and offers API to query mode information.
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkModeMonitor : public CNWNetworkMonitorBase
    {
    public:  // Constructors and destructor
       
       /**
        * Two-phased constructor.
        */
        IMPORT_C static CNWNetworkModeMonitor* NewL( CNWMessageHandler& aOwner,
                                           RMobilePhone& aPhone,
                                           TNWInfo& aNetworkInfo,
                                           RMmCustomAPI& aCustomAPI );
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkModeMonitor();

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

    private:

        /**
        * Updates the MMETel type network mode to PE type.
        */
        void UpdateNetworkInfo();

        /**
        * Executes the NotifyNetworkModeChange function.
        */
        void IssueRequest();

         /**
        * Updates networkMode PS key the Publish & Subscribe.
        */
        void UpdateNetworkMode();

    
    private:
    
        /**
        * C++ default constructor.
        */
        CNWNetworkModeMonitor( CNWMessageHandler& aOwner,
                               RMobilePhone& aPhone,
                               TNWInfo& aNetworkInfo,
                               RMmCustomAPI& aCustomAPI );
    
        /**
        * Second-phase constructor.
        */
        void ConstructL(); 

    private:    // Data
        // Current network mode
        RMobilePhone::TMobilePhoneNetworkMode iNetworkMode;

    };

#endif      // CNWNETWORKMODEMONITOR_H  
            
// End of File
