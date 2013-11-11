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
* Description:  This file contains the header file of the RCbsSettings class
*    
*                Client-side subsession which handles requests regarding
*                CbsServer settings, i.e. receive state, topic detection state and
*                selected languages.
*
*
*/



#ifndef     RCBSSETTINGS_H
#define     RCBSSETTINGS_H

// INCLUDES
#include <e32base.h>
#include "CbsCommon.h"


// FORWARD DECLARATIONS
class RCbs;

//  CLASS DECLARATION 

/**
*   This represents the client-side subsession to modify and retrieve settings.
*/
class RCbsSettings 
        : public RSubSessionBase
    {
    public:     // New functions
        
        /**
        *   Constructor.
        */
        RCbsSettings();

        /**
        *   Creates a subsession to the server. 
        *
        *   Note that this method must be called before using any other method.
        *
        *   @param  aServer     It is the session.
        *   @return             Error code.
        */
        TInt Open( RCbs& aServer );
        
        /**
        *   Closes the subsession.
        *
        *   @return             Error code.
        */
        TInt Close();

        /**
        *   Returns the reception status in aStatus, which is ETrue if the 
        *   reception is on. Otherwise it is EFalse.
        *
        *   @param  aStatus     The method returns the reception status in 
        *                       this parameter.
        */
        void GetReceptionStatus( TBool& aStatus );

        /**
        *   Changes the reception status to aStatus.
        *
        *   @param  aStatus     It contains the new reception status.
        *   @return             Result code. KErrNone if no errors.
        */
        TInt SetReceptionStatus( TBool aStatus );

        /**
        *   Returns the topic detection status in aStatus, which is ETrue if 
        *   the detection is on. Otherwise it is EFalse.
        *
        *   @param  aStatus     The method returns the topic detection status 
        *                       in this parameter.
        */
        void GetTopicDetectionStatus( TBool& aStatus );
        
        /**
        *   Changes the topic detection status to aStatus.
        *
        *   @param  aStatus     It contains the new topic detection status.
        *   @return             Result code. KErrNone if no errors.
        */
        TInt SetTopicDetectionStatus( TBool aStatus );

        /**
        *   Returns the preferred languages in aLanguages.
        *
        *   @param aLanguages   The method returns the languages in this 
        *                       parameter.
        */
        void GetLanguages( TCbsSettingsLanguages& aLanguages );

        /**
        *   Changes the preferred languages to aLanguages.
        *
        *   @param aLanguages   It contains the new preferred languages.
        *   @return             Result code. KErrNone if no errors.
        */
        TInt SetLanguages( const TCbsSettingsLanguages& aLanguages );

        /**
        *   Requests the server to notify the client whenever any settings 
        *   will be changed.
        *
        *   Note that for each subsession only one this kind of request can 
        *   be pending. Each client is responsible of assuring this.
        *
        *   @param  aStatus     It is the variable that the server will 
        *                       modify whenever an event occurs.
        *   @param  aEvent      The server will store the type of occurred 
        *                       event to this variable.
        */
        void NotifySettingsChanged( TRequestStatus& aStatus, 
            TCbsSettingsEvent& aEvent );

        /**
        *   Cancels the request to notify the client.
        */
        void NotifySettingsChangedCancel();

    private: // prohibited functions
        // Prohibited copy constructor
        RCbsSettings( const RCbsSettings& );

        // Prohibited assignment operator
        RCbsSettings& operator=( const RCbsSettings& );

    private: // Data

        // Pointer descriptor used to transfer information during a notify 
        // request.
        TPtr8 iNotifyEventPtr;
    };


#endif      //  RCBSSETTINGS_H   
            
// End of File


