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
* Description:  This file contains the header file of the MCbsEtelMessaging class.
*
*/



#ifndef MCBSETELMESSAGING_H
#define MCBSETELMESSAGING_H

//  INCLUDES
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
#include <etelmm.h>        

//  FORWARD DECLARATIONS
class MCbsEtelPhone;

//  CLASS DECLARATION 

/**
*   MCbsEtelMessaging is the interface for EPOC Telephony server connections.
*   
*/
class MCbsEtelMessaging
    {
    public:

        /**
        *   Virtual destructor
        */
        virtual ~MCbsEtelMessaging() { }

        /**
        *   Opens the connection to EPOC Telephony server       
        *
        *   @param  aPhone      A reference to the phone
        *   @return             Error code
        */
        virtual TInt Open( RMobilePhone& aPhone ) = 0;

        /**
        *   Closes the connection to EPOC Telephony server
        */
        virtual void Close() = 0;

        /**
        *   Requests to receive next incoming CB message
        *
        *   @param  aReqStatus      Return: Async request status
        *   @param  aMsgData        Return: CB message data
        *   @param  aMsgAttributes  Return: CB message attributes
        */
        virtual void ReceiveMessage( TRequestStatus& aReqStatus, 
            TDes8& aMsgData, TDes8& aMsgAttributes ) = 0;

        /**
        *   Gets the filter setting from EPOC Telephony server
        *
        *   @param  aSetting        Return: Current filter setting
        */
        virtual TInt GetFilterSetting( 
            RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter& aSetting ) = 0;

        /**
        *   Sets the filter setting to EPOC Telephony server
        *
        *   @param  aReqStatus      Return: Async request status
        *   @param  aSetting        New filter setting
        */
        virtual void SetFilterSetting( TRequestStatus& aReqStatus, 
            RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter aSetting ) const = 0;        

        /**
        *   Cancel ReceiveMessage
        *
        */
        virtual void ReceiveMessageCancel() = 0;

        /**
        *   Cancel SetFilterSetting
        *
        */
        virtual void SetFilterSettingCancel() = 0;
        };

#endif      //  MCBSETELMESSAGING_H
            
// End of File


