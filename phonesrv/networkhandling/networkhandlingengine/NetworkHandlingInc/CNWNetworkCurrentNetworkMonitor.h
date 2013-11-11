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
*                CNWNetworkCurrentNetworkMonitor class.
*
*/



#ifndef CNWCURRENTNETWORKMONITOR_H
#define CNWCURRENTNETWORKMONITOR_H

//  INCLUDES
#include "CNWNetworkMonitorBase.h"
#include "CNWMessageHandler.h"
#include <etelmm.h>


// CLASS DECLARATION
class CNWNetworkCurrentNetworkReceiver;
/**
*  Monitors current network change events from MM Etel.
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/

class CNWNetworkCurrentNetworkMonitor : public CNWNetworkMonitorBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
       
        static CNWNetworkCurrentNetworkMonitor* NewL( CNWMessageHandler& aOwner,
                                         RMobilePhone& aPhone,
                                         TNWInfo& aNetworkInfo,
                                         RMmCustomAPI& aCustomAPI,
                                         TNWInterInfo& aInterNetworkInfo );
        /**
        * Destructor.
        */
        virtual ~CNWNetworkCurrentNetworkMonitor();
        
        /**
        * Updates the MM ETel type network info to NW type.
        * 
        */
        void UpdateNetworkInfo();
        
        /**
        * Executes the NotifyCurrentNetworkChange function.
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
        * By default EPOC constructor is private.
        */
        void ConstructL(  );
         
        /**
        * C++ default constructor.
        */
        CNWNetworkCurrentNetworkMonitor( CNWMessageHandler& aOwner,
                                         RMobilePhone& aPhone,
                                         TNWInfo& aNetworkInfo,
                                         RMmCustomAPI& aCustomAPI,
                                         TNWInterInfo& aInterNetworkInfo );
        

        
    private:    // Data
         // Current network receiver
        CNWNetworkCurrentNetworkReceiver*            iCurrentNetworkReceiver;
        // Location area info (GSM).
        RMobilePhone::TMobilePhoneLocationAreaV1     iLocationArea;
        // Network info.
        RMobilePhone::TMobilePhoneNetworkInfoV2      iNetworkInfo;
        // Network info package.
        RMobilePhone::TMobilePhoneNetworkInfoV2Pckg  iNetworkInfoPckg;
        // Reference to the internal network info structure.
        TNWInterInfo&     iInterNetworkInfo;

        };

#endif      // CNWCURRENTNETWORKMONITOR_H  

//  End of File
