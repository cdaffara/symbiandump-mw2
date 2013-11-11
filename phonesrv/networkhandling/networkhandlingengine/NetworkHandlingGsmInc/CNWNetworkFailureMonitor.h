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
* Description:  Monitors network connection failure.
*
*/



#ifndef CNWNETWORKFAILUREMONITOR_H
#define CNWNETWORKFAILUREMONITOR_H

//  INCLUDES
#include "CNWNetworkMonitorBase.h"
#include <etelmm.h>


// FORWARD DECLARATIONS
class CNWGsmMessageHandler;

// CLASS DECLARATION

/**
*  Monitors network connection failure.
*
*  @lib Networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkFailureMonitor : public CNWNetworkMonitorBase
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkFailureMonitor();


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
        * Executes the NotifyNetworkConnectionFailure function.
        * 
        */
        void IssueRequest();

    public:

        /**
        * C++ default constructor.
        */
        CNWNetworkFailureMonitor( CNWGsmMessageHandler& aOwner,
                                  RMobilePhone& aPhone,
                                  TNWInfo& aNetworkInfo,
                                  RMmCustomAPI& aCustomAPI );


    private:    // Data

    };

#endif      // CNWNETWORKFAILUREMONITOR_H   
            
// End of File
