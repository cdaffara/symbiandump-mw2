/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsRecCollector class.
*    
*                CCbsSession class represents a server-side CbsClient/CbsMcnClient session.
*
*/



#ifndef     CCBSSESSION_H
#define     CCBSSESSION_H

// INCLUDES
#include <e32base.h>
#include "CbsCommon.h"

// FORWARD DECLARATIONS
class CCbsServer;
class CCbsObject;
class CObjectConIx;

//  CLASS DECLARATION 

/**
*   This class represents a server-side session.
*
*   The instances of the class are responsible of handling 
*   the requests of client-side RCbs and RCbsMcnSession objects.
*
*   This class contains handlers for opening subsessions and
*   retrieving some general CBS information.
*
*   Client will be panicked, if a request is invalid.
*/
class   CCbsSession : public CSession2
    {
    public:     // New functions
        /**
        *   Creates a new instance of the class.
        *
        *   @param  aServer         Server
        *   @return                 New CCbsSession instance
        */
        static CSession2* NewL( CCbsServer& aServer );

        /**
        *   Destructor.
        */
        ~CCbsSession();

        /**
        *   Finds the appropriate message service method and calls it to 
        *   handle the request.
        *
        *   @param  aMessage        Handle to the message that 
        *                           contains the request.
        */
        void DispatchMessageL( const RMessage2& aMessage );

        /**
        *   Panics the client.
        *
        *   @param  aPanic          Identifies the reason for panic.
        */
        void PanicClient( TCbsSessionPanic aPanic ) const;

        /**
        *   Returns the number of new topics detected. 
        *
        *   The counter will be reset after a call.
        *
        *   @return                 Number of topics detected.
        */
        TInt TotalTopicsDetected();

        /**
        *   Returns the last message arrived from the client
        *
        *   @return                 Current message
        */
        RMessage2& Message();

        CCbsServer& Server();

    public:     // From CSession2
        
        /**
        *   Handles the received message.
        *
        *   The method simply calls DispatchMessageL (see above) under
        *   trap harness. 
        *
        *   @param  aMessage        Handle to the message.
        */
        void ServiceL( const RMessage2& aMessage );
    
    private:

        /**
        *   Constructor.
        *
        *   @param  aServer         Reference to the server.
        */
        CCbsSession( CCbsServer& aServer );

        /**
        *   Finalizes the construction.
        */
        void ConstructL();

        /**
        *   Handles the session-based requests.
        *
        *   @param  aMessage        A handle to the message.
        *   @return                 ETrue if the request was handled in 
        *                           the main session.
        */
        TBool HandleSessionRequestsL( const RMessage2& aMessage );

        /**
        *   Create a new subsession, settings, for the session.
        */
        void NewSettingsL();

        /**
        *   Create a new subsession, topic list, for the session.
        */
        void NewTopicListL();

        /**
        *   Create a new subsession, topic collection, for the session.
        */
        void NewTopicCollectionL();

        /**
        *   Create a new subsession, topic messages, for the session.
        */
        void NewTopicMessagesL();

        /**
        *   Create a new MCN subsession.
        */
        void NewMcnSubsessionL();

        /**
        *   Return the current cell info message to the client.
        */ 
        void GetCurrentMcnInfoL();        

    private:    // Data
        
        /**
        *   Reference to CCbsServer object. 
        *   iServer is a private member of CSharableSession so
        *   we define a new name for CCbsServer pointer.
        */
        CCbsServer& iCbsServer;


        /** 
        *   The last message arrived from the client
        *   
        */
        RMessage2 iCurrentMessage;

    };

#endif      //  CCBSSESSION_H   
            
// End of File


