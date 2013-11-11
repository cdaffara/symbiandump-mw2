/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPhCltDialEmergencyRequestMonitor observer getting notifications about call requests
*                from Phone Server.
*
*/

#ifndef CPHCLTDIALEMERGENCYREQUESTMONITOR_H
#define CPHCLTDIALEMERGENCYREQUESTMONITOR_H

//  INCLUDES
#include <e32base.h>
#include <phclttypes.h> 


//  FORWARD DECLARATIONS
class RPhCltServer;
class RPhCltCallNotify;
class MPhCltDialEmergencyRequestObserver;


// CLASS DECLARATION

/**
*  CCallNotify observer getting notifications about call requests
*  from Phone Server.
*
*  @lib phoneclient.lib
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCltDialEmergencyRequestMonitor ): public CActive
    {
    public: // Constructors and destructors.

        
        static CPhCltDialEmergencyRequestMonitor* NewL(RPhCltCallNotify& aCltNotify);
        
        /**
        * Destructor.
        */
        virtual ~CPhCltDialEmergencyRequestMonitor();
        
    public: // New functions

    
        /**
        * Notifies when a emergency call request arrives.
        *
        * @param aStatus Status to be changed when call request have arrived.
        */        
        void NotifyEmergencyCall( 
            MPhCltDialEmergencyRequestObserver* aObserver,
            TDesC& aEmergencyNumber );


        /**
        * Respond to client request to make emergency call
        *
        * @param aResultCode Result of the response.
        */
        TInt RespondEmergencyToClient( const TInt aResultCode );
        
    
    private:
    
        /*
        * Constructor.
        */
        CPhCltDialEmergencyRequestMonitor(RPhCltCallNotify& aCallNotify);
        
        
    private: // Functions from base classes
        
        /**
        * From CActive. 
        * Called when send request is completed.
        */
        void RunL();
        
        /**
        * From CActive.
        * Called when RunL leaves.
        */        
        TInt RunError(TInt aError);
        
        /**
        * From CActive. 
        * Called when active request is canceled.
        */
        void DoCancel();
        
    private:
        
        //RClass instance
        RPhCltCallNotify& iCallNotify;
               
        //Pointer to emergencynumber
        TPtr8 iEmergencyCallNumberPtr;

        // Pointer to dial observer
        MPhCltDialEmergencyRequestObserver* iObserver;
    
    };


#endif    // CPHCLTDIALEMERGENCYREQUESTMONITOR_H

// End of File
