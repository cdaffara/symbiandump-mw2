/*
* Copyright (c) 2002 - 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of 
*                the CNWNetworkSelectionSettingMonitor class.
*
*/


#ifndef CNWNETWORKSELECTIONSETTINGMONITOR_H
#define CNWNETWORKSELECTIONSETTINGMONITOR_H

//  INCLUDES
#include "CNWNetworkMonitorBase.h"
#include <networkhandlingdomainpskeys.h>
#include <e32property.h>

// FORWARD DECLARATIONS
class CNWMessageHandler;
class RMobilePhone;

// CLASS DECLARATION

/**
*  Monitors network selection setting change event from MMEtel and offers API 
*  to query mode information.
*  
*
*  @lib networkhandling.lib
*  @since S60_5.0
*/
class CNWNetworkSelectionSettingMonitor : public CNWNetworkMonitorBase
    {
    public:  // Constructors and destructor
       
       /**
        * Two-phased constructor.
        */
        static CNWNetworkSelectionSettingMonitor* 
                                NewL( CNWMessageHandler& aOwner,
                                           RMobilePhone& aPhone, 
                                           TNWInfo& aNetworkInfo, // ref. to network info struct 
                                           RMmCustomAPI& aCustomAPI  );
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkSelectionSettingMonitor();
        
        
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
        * Executes the NotifyNetworkSelectionSettingChange function.
        */
        void IssueRequest();

         /**
        * Updates network selection setting in TNWInfo Struct.
        */
        void UpdateNetworkSelectionSetting();

    
    private:
    
        /**
        * C++ default constructor.
        */
        CNWNetworkSelectionSettingMonitor( CNWMessageHandler& aOwner,
                               RMobilePhone& aPhone,
                               TNWInfo& aNetworkInfo, // ref.to network info struct 
                               RMmCustomAPI& aCustomAPI   );
    
       
    private:    // Data
        // Network info.
        RMobilePhone::TMobilePhoneNetworkSelectionV1  iNetworkSelection;
        // Network info package.
        RMobilePhone::TMobilePhoneNetworkSelectionV1Pckg iNetworkSelectionPckg;
         
    };

#endif      // CNWNetworkSelectionSettingMonitor_H  
            
// End of File
