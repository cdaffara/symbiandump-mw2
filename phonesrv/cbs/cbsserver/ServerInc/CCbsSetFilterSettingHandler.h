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
* Description:  This file contains the header file of the CCbsSetFilterSettingHandler class
*
*/


#ifndef     CCBSSETFILTERSETTINGHANDLER_H
#define     CCBSSETFILTERSETTINGHANDLER_H

//  INCLUDES
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
#include <etelmm.h>
#include "CbsServerConstants.h"

//  FORWARD DECLARATIONS
class CCbsRecEtel;
class CCbsRecEtelMonitor;
class MCbsEtelMessaging;

//  CLASS DECLARATION 

/**
*   Handles the SetFilterSetting command.
*
*/
class CCbsSetFilterSettingHandler : public CActive
    {
    public:     // New functions

        /**
        *   Creates an instance of the class.
        *
        *   @param    aEtel             A reference to the receiver
        *   @param    aMessaging        A reference to the messaging object
        *   @return                     A new instance of 
        *                               CCbsSetFilterSettingHandler
        */
        static CCbsSetFilterSettingHandler* NewL( CCbsRecEtel& aEtel, 
            MCbsEtelMessaging& aMessaging );

        /**
        *   Destructor.
        */
        ~CCbsSetFilterSettingHandler();

        /** 
        *   Sets a new filter setting to EPOC Telephony server.
        *
        *   @param      aSetting        The new setting
        */
        void SetFilterSetting( 
            RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter aSetting );

    private:    // From CActive

        /**
        *   Called when EPOC Telephony server has completed the SetFilterSetting operation.      
        */
        void RunL();

        /**
        *   Cancels an outstanding ReadCbsMessage request.
        */
        void DoCancel();
        
        /**
        *   Is called if RunL() leaves.
        *
        *   @param    aError            Error code
        *   @return                     Always KErrNone
        */
        TInt RunError( TInt aError );

    private:

        /**
        *   The constructor.
        *
        *   @param  aEtel               CCbsRecEtel object.
        *   @param  aMessaging          EPOC Telephony server messaging object.
        */
        CCbsSetFilterSettingHandler( CCbsRecEtel& aEtel, 
            MCbsEtelMessaging& aMessaging );

        /**
        *   Finalizes the construction.
        */
        void ConstructL();

    private:    // Data

        // CCbsRecEtel instance to which CB messages are passed.
        CCbsRecEtel& iEtel;
                
        // Messaging subsession.
        MCbsEtelMessaging& iSmsMessaging;

        // Type of broadcast id
        RMobileBroadcastMessaging::TMobileBroadcastIdType iIdType;
    };

#endif      //  CCBSSETFILTERSETTINGHANDLER_H   
            
// End of File


