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
* Description:  This file contains the header file of the server-side settings 
*                subsession class.
*
*/



#ifndef     CCBSSETTINGS_H
#define     CCBSSETTINGS_H

// INCLUDES
#include "CCbsSession.h"
#include "CCbsObject.h"
#include "MCbsDbSettingsObserver.H"

// FORWARD DECLARATIONS
class CCbsDbImpSettings;
class CCbsRecEtel;

//  CLASS DECLARATION 

/**
*   The server-side CbsClient subsession for modifying settings.
*/
class   CCbsSettings
        : public CCbsObject, public MCbsDbSettingsObserver
    {
    public:     // New functions
        /**
        *   Creates a new instance of the class.
        *
        *   @param  aSession        Session object
        *   @param  aSettings       Database settings manager
        *   @param  aReceiver       Receiver object
        *   @return                 Pointer to a new instance of CCbsSettings
        */
        static CCbsSettings* NewL( CCbsSession& aSession, 
                                   CCbsDbImpSettings& aSettings, 
                                   CCbsRecEtel& aReceiver );

        /**
        *   Destructor.
        */
        ~CCbsSettings();

    public:     // From CCbsObject
        /**
        *   Handle the requests for the object.
        *
        *   @param  aMessage        Message to be handled.
        *   @return                 Indicates whether the message was handled.
        */
        TBool HandleRequestsL( const RMessage2& aMessage );

    private:        // From MCbsDbSettingsObserver

        /**
        *   Called when the topic detection is changed.
        */
        void TopicDetectionStatusChangedIndL();

        /**
        *   Called when reception status is changed.
        */
        void ReceptionStatusChangedIndL();

        /**
        *   Called when preferred languages are changed.
        */
        void LanguagesChangedIndL();

        /**
        *   Called when time-limited reception status changed
        */
        void LimitedReceptionStatusChangedIndL();

        /**
        *   Called when clean up interval of read messages changed
        */
        void CleanupTimeChangedIndL();

    private:
        /**
        *   Constructor.
        *
        *   @param  aSession        Session session object
        *   @param  aSettings       Database settings manager
        *   @param  aReceiver       Receiver
        */
        CCbsSettings( CCbsSession& aSession, 
                      CCbsDbImpSettings& aSettings, 
                      CCbsRecEtel& aReceiver );

        /**
        *   Finalizes construction.
        */
        void ConstructL();

        /**
        *   Close the subsession.
        *
        *   Note that the method will delete itself, so the object is no
        *   longer valid after the call.
        */
        void CloseSettings();

        /**
        *   Return the current reception status to the client.
        */
        void GetReceptionStatusL();

        /**
        *   Change the reception status to the requested one.
        */
        void SetReceptionStatusL();

        /**
        *   Return the current topic detection status to the client.
        */
        void GetTopicDetectionStatusL();

        /**
        *   Change the topic detection status to the requested one.
        */
        void SetTopicDetectionStatusL();

        /**
        *   Return the preferred languages to the client.
        */
        void GetLanguagesL();

        /**
        *   Change the preferred languages to the requested.
        */
        void SetLanguagesL();

        /**
        *   Make request to notify when settings are changed.
        */
        void NotifySettingsChanged();

        /**    
        *   Cancel the request to notify when settings are changed.
        */
        void NotifySettingsChangedCancel();
        
        /**
        *   Notifies the client (if there is a pending request).
        *   
        *   @param  aEvent          Identifies the type of request that 
        *                           has happened.
        */
        void NotifyClientL( TCbsSettingsEvent aEvent );

    private:    // Prohibited operators and functions

        // Copy constructor
        CCbsSettings( const CCbsSettings& );

        // Assignment operator
        CCbsSettings& operator=( const CCbsSettings& );

    private:    // Data
        
        // Reference to the database settings manager
        CCbsDbImpSettings& iSettings;

        // Reference to the receiver
        CCbsRecEtel& iReceiver;

        // Indicates whether iMessage is a valid message; that is, there
        // is a pending notify request.        
        TBool iIsMessage;

        // Message of the pending notify request, if available 
        RMessage2 iMessage;
    };

#endif      //  CCBSSETTINGS_H   
            
// End of File


