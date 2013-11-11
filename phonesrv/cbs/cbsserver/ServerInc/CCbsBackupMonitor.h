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
* Description:  This file contains the header file of the CCbsBackupMonitor class
*    
*
*/


#ifndef     CCBSBACKUPMONITOR_H
#define     CCBSBACKUPMONITOR_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "CCbsDbImp.H"

//  CLASS DECLARATION 

/**
*  The CCbsBackupMonitor instance listens to 
*  shutdown signal via Publish and Subscribe. On device shutdown,
*  the listener stops server operation by stopping the active scheduler.
*  CbsServer will then be terminated. 
*/
class CCbsBackupMonitor : public CActive
    {
    public:     // New functions

        /**
        *   Creates an instance of the class.
        *
        *   @param  aServer     Server reference
        *   @return             A new instance of CCbsBackupMonitor
        */
        static CCbsBackupMonitor* NewL( CCbsDbImp& aDbImp );

        /**
        *   Destructor.
        */
        ~CCbsBackupMonitor();

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
        CCbsBackupMonitor( CCbsDbImp& aDbImp );

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

#endif      //  CCBSBACKUPMONITOR_H   
            
// End of File


