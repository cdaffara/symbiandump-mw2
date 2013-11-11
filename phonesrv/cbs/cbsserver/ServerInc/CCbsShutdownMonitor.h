/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsShutdownMonitor class
*    
*
*/


#ifndef     CCBSSHUTDOWNMONITOR_H
#define     CCBSSHUTDOWNMONITOR_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CCbsServer;

//  CLASS DECLARATION 

/**
*  The CCbsShutdownMonitor instance listens to 
*  shutdown signal via Publish and Subscribe. On device shutdown,
*  the listener stops server operation by stopping the active scheduler.
*  CbsServer will then be terminated. 
*/
class CCbsShutdownMonitor : public CActive
    {
    public:     // New functions

        /**
        *   Creates an instance of the class.
        *
        *   @param  aServer     Server reference
        *   @return             A new instance of CCbsShutdownMonitor
        */
        static CCbsShutdownMonitor* NewL( CCbsServer& aServer );

        /**
        *   Destructor.
        */
        ~CCbsShutdownMonitor();

        /** 
        *   Reissues the Subscribe request.
        */
        void IssueRequest();

    private:    // From CActive

        /**
        *   Gets called when the system state changes.
        */
        void RunL();

        /**
        *   Cancels an outstanding Subscribe request.
        */
        void DoCancel();

    private:

        /**
        *   The constructor.
        *
        */
        CCbsShutdownMonitor( CCbsServer& aServer );

        /**
        *   Finalizes the construction.
        */
        void ConstructL();
      
    private:    // Data
        
        // PubSub connection
        RProperty iProperty;
        
        // Server reference
        CCbsServer& iServer;
    };

#endif      //  CCBSSHUTDOWNMONITOR_H   
            
// End of File


