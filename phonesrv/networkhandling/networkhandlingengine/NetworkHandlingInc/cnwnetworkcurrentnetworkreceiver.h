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
*                CNWNetworkCurrentNetworkReceiver class.
*
*/



#ifndef CNWCURRENTNETWORKRECEIVER_H
#define CNWCURRENTNETWORKRECEIVER_H

//  INCLUDES
#include <etelmm.h>


// CLASS DECLARATION
class CNWNetworkCurrentNetworkMonitor;
class RMobilePhone;
/**
*  Monitors current network change events from MM Etel.
*
*  @lib networkhandling.lib
*  @since Series 60_3.1
*/

class CNWNetworkCurrentNetworkReceiver : public CActive
    {
    public:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CNWNetworkCurrentNetworkReceiver( CNWNetworkCurrentNetworkMonitor& aOwner,
                                            RMobilePhone& aPhone, 
                                            RMobilePhone::TMobilePhoneNetworkInfoV2Pckg& aNetworkInfoPckg,
                                            RMobilePhone::TMobilePhoneLocationAreaV1& aLocationArea
                                            );
       
        /**
        * Destructor.
        */
        virtual ~CNWNetworkCurrentNetworkReceiver();
        
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

   
    private:    // Data
         // Reference to current network info
        CNWNetworkCurrentNetworkMonitor& iOwner;
        // Reference to RMobilePhone
        RMobilePhone& iPhone;
        // Network info package.
        RMobilePhone::TMobilePhoneNetworkInfoV2Pckg&  iNetworkInfoPckg;
        // Location area info (GSM).
        RMobilePhone::TMobilePhoneLocationAreaV1&     iLocationArea;
        };

#endif      // CNWCURRENTNETWORKRECEIVER_H  

//  End of File
