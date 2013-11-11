/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The header file of CNWDynamicCapsMonitor
*
*/



#ifndef CNWDYNAMICCAPSMONITOR_H
#define CNWDYNAMICCAPSMONITOR_H

//  INCLUDES
#include "CNWNetworkMonitorBase.h"

// FORWARD DECLARATIONS
class CNWMessageHandler;

// CLASS DECLARATION

/**
*  Monitors dynamic caps change events from MMEtel.
*  Ensures that alpha tag is displayed only when CS registration is successful.
*  If PS registration is successful but CS registration failed, the alpha tag 
*  should not be shown so users do not think they can make voice calls.
*
*  @lib networkhandling.lib
*  @since Series 60_5.0
*/
NONSHARABLE_CLASS( CNWDynamicCapsMonitor ) : public CNWNetworkMonitorBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CNWDynamicCapsMonitor* NewL(
            CNWMessageHandler& aOwner, 
            RMobilePhone& aPhone, 
            TNWInfo& aNetworkInfo, 
            RMmCustomAPI& aCustomAPI );

        /**
        * Destructor.
        */
        virtual ~CNWDynamicCapsMonitor();
        
       
        
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
        
        /**
         * From CActive, RunError.
         * Processes any errors.
         * @param aError The leave code reported.
         * @result return KErrNone if leave is handled.
         */
         TInt RunError( TInt aError );

    private:

        /**
        * Update Dynamic Caps Flags field of TNWInfo
        * @param aDynamicCapsFlags The current dynamic caps flags.
        */
        void UpdateDynamicCapsFlags( 
            const RPacketService::TDynamicCapsFlags aDynamicCapsFlags);

        /**
        * Executes the NotifyDynamicCapsChange() function.
        * 
        */
        void IssueRequest();

        /**
        * C++ default constructor.
        */
        CNWDynamicCapsMonitor( 
            CNWMessageHandler& aOwner, 
            RMobilePhone& aPhone, 
            TNWInfo& aNetworkInfo, 
            RMmCustomAPI& aCustomAPI
            );
            
        /**
        * Second-phase constructor.
        */
        void ConstructL();  
                   
    // Data:
        
        // For RPacketService call
        RPacketService iPacketService;
        // Current dynamic caps flags.
        RPacketService::TDynamicCapsFlags iDynamicCapsFlags;
                
   };

#endif      // CNWDYNAMICCAPSMONITOR_H

//  End of File
