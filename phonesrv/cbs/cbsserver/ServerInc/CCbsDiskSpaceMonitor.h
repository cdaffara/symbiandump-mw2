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
* Description:  This file contains the header file of the CCbsDiskSpaceMonitor class
*    
*
*/


#ifndef     CCBSDISKSPACEMONITOR_H
#define     CCBSDISKSPACEMONITOR_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CCbsDbImp;

//  CLASS DECLARATION 

/**
*  The CCbsDiskSpaceMonitor instance listens to 
*  shutdown signal via Publish and Subscribe. On device shutdown,
*  the listener stops server operation by stopping the active scheduler.
*  CbsServer will then be terminated. 
*/
class CCbsDiskSpaceMonitor : public CActive
    {
    public:     // New functions

        /**
        *   Creates an instance of the class.
        *
        *   @param  aDbImp      DB reference
        *   @return             A new instance of CCbsDiskSpaceMonitor
        */
        static CCbsDiskSpaceMonitor* NewL( CCbsDbImp& aDbImp );

        /**
        *   Destructor.
        */
        ~CCbsDiskSpaceMonitor();

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
        CCbsDiskSpaceMonitor( CCbsDbImp& aServer );

        /**
        *   Finalizes the construction.
        */
        void ConstructL();
      
    private:    // Data
        
        // PubSub connection
        RProperty iProperty;
        
        // DB reference
        CCbsDbImp& iDbImp;
    };

#endif      //  CCBSSHUTDOWNMONITOR_H   
            
// End of File


