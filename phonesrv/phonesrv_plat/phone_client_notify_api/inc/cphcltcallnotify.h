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
* Description:  CPhCltCallNotify observer getting notifications about call requests
*                from Phone Server.
*
*/

#ifndef CPHCLTCALLNOTIFY_H
#define CPHCLTCALLNOTIFY_H

//  INCLUDES
#include <e32base.h>
#include <phclttypes.h> 


//  FORWARD DECLARATIONS
class RPhCltServer;
class CPhCltDialData;
class MPhCltDialRequestObserver;
class MPhCltDialEmergencyRequestObserver;
class CPhCltExtPhoneDialData;
class RPhCltCallNotify;
class MPhCltComHandRequestObserver;
class CPhCltDialRequestMonitor;
class CPhCltDialEmergencyRequestMonitor;
class CPhCltComHandRequestMonitor;


// CLASS DECLARATION

/**
*  CCallNotify observer getting notifications about call requests
*  from Phone Server.
*
*  @lib phoneclient.lib
*  @since 1.0
*/
class CPhCltCallNotify : public CBase
    {
    public: // Constructors and destructors.

        
        IMPORT_C static CPhCltCallNotify* NewL();
        
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CPhCltCallNotify();
        
    public: // New functions

        /**
        * Open subsession to server.
        * 
        * @param aServer Pre-opened server session.
        * @return Symbian OS error code of the success of the opening.
        */
        IMPORT_C TInt Open( RPhCltServer& aServer );

        /**
        * Close subsession.
        */
        IMPORT_C void Close();
    
        /**
        * Notify dial requests through given observer.
        * @param aObserver dial request observer
        */
        IMPORT_C void NotifyDialRequest( 
            MPhCltDialRequestObserver* aObserver );

        /**
        * Cancel pending call attempt notification.
        */
        IMPORT_C void CancelNotifyDialRequest();
        
        /**
        * Respond to client request to make a call
        *
        * @param aResultCode Result of the response.
        */
        IMPORT_C TInt RespondClient( const TInt aResultCode );
        
        /**
        * Notifies when a emergency call request arrives.
        *
        * @param aStatus Status to be changed when call request have arrived.
        */        
        IMPORT_C void NotifyEmergencyCall( 
            MPhCltDialEmergencyRequestObserver* aObserver, 
            TDesC& aEmergencyNumber );

        /**
        * Cancel pending emergency call attempt notification.
        */
        IMPORT_C void CancelNotifyEmergencyCall() const;

        /**
        * Respond to client request to make emergency call
        *
        * @param aResultCode Result of the response.
        */
        IMPORT_C TInt RespondEmergencyToClient( const TInt aResultCode );
        
        /**
        * Notifies when a Command handler request arrives.
        *
        * @param aStatus Status to be changed when command handler request
        *                has arrived.
        * @param aCommandArgs Command argument storage place. 
        *                     That is, packaged version of class 
        *                     TPhCltComHandCommandParameters.
        */
        IMPORT_C void NotifyComHandCommand( 
            MPhCltComHandRequestObserver* aObserver, 
            TDes8& aCommandArgs );
    
        /**
        * Cancel pending command handler request notification.
        */
        IMPORT_C void CancelNotifyComHandCommand() const;
    
        /**
        * Respond to client's command handler request.
        *
        * @param aResultCode Result of the response 
        *        (previous function execution).
        * @return KErrNone if respond succeeded, otherwise Symbian error code.
        */
        IMPORT_C TInt RespondComHandClient( const TInt aResultCode );

    
    private:
    
        /*
        * Constructor.
        */
        CPhCltCallNotify();
        
        /**
        * Memory allocations in ConstructL.
        */
        void ConstructL();
        
    private:
        
        //RClass instance
        RPhCltCallNotify* iCallNotify;
               
        CPhCltDialRequestMonitor* iDialMonitor;
        CPhCltDialEmergencyRequestMonitor* iDialEmergencyMonitor;
        CPhCltComHandRequestMonitor* iComHandMonitor;
    };


#endif    // CPHCLTCALLNOTIFY_H

// End of File
