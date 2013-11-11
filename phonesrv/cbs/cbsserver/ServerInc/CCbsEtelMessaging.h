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
* Description:  This file contains the header file of the CCbsEtelMessaging class.
*    
*                CCbsEtelMessaging is a wrapper to RMobileBroadcastMessaging used 
*                in network builds.
*
*/



#ifndef CCBSETELMESSAGING_H
#define CCBSETELMESSAGING_H

// INCLUDES
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
#include <etelmm.h>            

#include <e32base.h>
#include "MCbsEtelMessaging.h"

//  CLASS DECLARATION 

/**
*   CCbsEtelMessaging acts as a wrapper to RMobileBroadcastMessaging instance.
*
*/
class CCbsEtelMessaging : public CBase, public MCbsEtelMessaging
    {
    public:

        /**
        *   Creates and returns a new instance of CCbsEtelMessaging.
        *
        *   @return Instance of CCbsEtelMessaging.
        */
        static CCbsEtelMessaging* NewL();

        /**
        *   Destructor.
        */
        virtual ~CCbsEtelMessaging();

    public:     // MCbsEtelMessaging

        /**
        *   Opens an emulated ETel messaging session.
        *
        *   @param  aPhone RMobilePhone-session
        */
        TInt Open( RMobilePhone& aPhone );

        /**
        *   Closes ETel messaging session.
        */
        void Close();

        /**
        *   Requests forwarding of the next received CB message.
        *
        *   @param  aReqStatus          Client request status
        *   @param  aMsgData            CBS message data descriptor
		*	@param	aMsgAttributes		CBS message attributes
        */
        void ReceiveMessage( TRequestStatus& aReqStatus, TDes8& aMsgData, 
            TDes8& aMsgAttributes );
        
        /**
        *   Cancels an outstanding ReceiveMessage() request.
        */
        void ReceiveMessageCancel();

        /**
        *   Sets CBS receive mode (filter setting).
        *
        *   @param  aReqStatus          Returns: status of async operation
        *	@param	aSetting			The new filter setting to be set to ETel
        */
        void SetFilterSetting( TRequestStatus& aReqStatus, 
            RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter aSetting ) const;

        /**
        *   Cancels an outstanding SetFilterSetting() request.
        *
        */
        void SetFilterSettingCancel();

        /**
        *   Retrieves the current CBS receive mode (filter setting).
        *
        *   @param  aSetting			The setting to beffiled by ETel
        *   @return                     Result code
        */
        TInt GetFilterSetting( 
			RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter& aSetting );        

    private: // Private methods

        /**
        *   Constructor.
        */
        CCbsEtelMessaging();

        /**
        *   Finalizes the construction.
        */
        void ConstructL();

    private: // Data

        // ETel messaging session to which calls to this class are forwarded.
        RMobileBroadcastMessaging iMessaging;
    };

#endif      //  CCBSETELMESSAGING_H
            
// End of File


