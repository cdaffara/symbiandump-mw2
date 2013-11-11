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
* Description:  CPhCltDialRequestMonitor observer getting notifications about call requests
*                from Phone Server.
*
*/

#ifndef CPHCLTDIALREQUESTMONITOR_H
#define CPHCLTDIALREQUESTMONITOR_H

//  INCLUDES
#include <e32base.h>
#include <phclttypes.h> 

//  FORWARD DECLARATIONS
class RPhCltServer;
class CPhCltDialData;
class MPhCltDialRequestObserver;
class CPhCltExtPhoneDialData;
class MPhCltComHandRequestObserver;
class RPhCltCallNotify;

// CLASS DECLARATION

/**
*  CCallNotify observer getting notifications about call requests
*  from Phone Server.
*
*  @lib phoneclient.lib
*  @since 1.0
*/
NONSHARABLE_CLASS( CPhCltDialRequestMonitor ): public CActive
    {
    public: // Constructors and destructors.

        
        static CPhCltDialRequestMonitor* NewL(RPhCltCallNotify& aCallNotify);
        
        /**
        * Destructor.
        */
        virtual ~CPhCltDialRequestMonitor();
        
    public: // New functions

    
        /**
        * Notify dial requests through given observer.
        * @param aObserver dial request observer
        */
        void NotifyDialRequest( 
            MPhCltDialRequestObserver* aObserver );

        /**
        * Respond to client request to make a call
        *
        * @param aResultCode Result of the response.
        */
        TInt RespondClient( const TInt aResultCode );
        
    
    private:
    
        /*
        * Constructor.
        */
        CPhCltDialRequestMonitor(RPhCltCallNotify& aCallNotify);
        
        
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
               
        //Used to get dial data length from server
        TInt iDialDataLength;
        
        //Package buffer for dial data length 
        TPckg< TInt > iDataLengthPckg;


        // Pointer to dial observer
        MPhCltDialRequestObserver* iObserver;
    
    };


#endif    // CPhCLTDIALREQUESTMONITOR_H

// End of File
