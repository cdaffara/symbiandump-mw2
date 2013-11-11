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
* Description:  CPhCltComHandRequestMonitor observer getting notifications about 
*                command handling requests from Phone Server.
*
*/

#ifndef CPHCLTCOMHANDREQUESTMONITOR_H
#define CPHCLTCOMHANDREQUESTMONITOR_H

//  INCLUDES
#include <e32base.h>
#include <phclttypes.h> 


//  FORWARD DECLARATIONS
class RPhCltServer;
class CPhCltDialData;
class MPhCltComHandRequestObserver;
class CPhCltExtPhoneDialData;
class RPhCltCallNotify;

// CLASS DECLARATION

/**
*  CCallNotify observer getting notifications about call requests
*  from Phone Server.
*
*  @lib phoneclient.lib
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCltComHandRequestMonitor ) : public CActive
    {
    public: // Constructors and destructors.

        
        static CPhCltComHandRequestMonitor* NewL(RPhCltCallNotify& aCltNotify);
        
        /**
        * Destructor.
        */
        virtual ~CPhCltComHandRequestMonitor();
        
    public: // New functions

        
        
        /**
        * Notifies when a Command handler request arrives.
        *
        * @param aStatus Status to be changed when command handler request
        *                has arrived.
        * @param aCommandArgs Command argument storage place. 
        *                     That is, packaged version of class 
        *                     TPhCltComHandCommandParameters.
        */
        void NotifyComHandCommand( 
            MPhCltComHandRequestObserver* , 
            TDes8& aCommandArgs );

    
        /**
        * Respond to client's command handler request.
        *
        * @param aResultCode Result of the response 
        *        (previous function execution).
        * @return KErrNone if respond succeeded, otherwise Symbian error code.
        */
        TInt RespondComHandClient( const TInt aResultCode );

    
    private:
    
        /*
        * Constructor.
        */
        CPhCltComHandRequestMonitor(RPhCltCallNotify& aCallNotify);
        
        
    private: // Functions from base classes
        
        /**
        * From CActive. 
        * Called when send request is completed.
        */
        void RunL();
        
        
        /**
        * From CActive. 
        * Called when active request is canceled.
        */
        void DoCancel();
        
    private:
        
        //RClass instance
        RPhCltCallNotify& iCallNotify;
               

        // Pointer to dial observer
        MPhCltComHandRequestObserver* iObserver;
    
    };


#endif    // CPHCLTCOMHANDREQUESTMONITOR_H

// End of File
