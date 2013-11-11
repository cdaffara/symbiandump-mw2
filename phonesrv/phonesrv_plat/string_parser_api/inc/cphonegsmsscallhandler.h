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
* Description:  It is base handler for supplementary services during calls.
*
*/


#ifndef CPHONEGSMSSCALLHANDLER_H
#define CPHONEGSMSSCALLHANDLER_H

// INCLUDES
#include    "cphonegsmhandlerbase.h" 

// CLASS DECLARATION

/**
* It is base handler for supplementary services during calls.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmSsCallHandler 
    : public CPhoneGsmHandlerBase
    {
    public: // New functions

        /**
        * Reject a call.
        *
        * Command: 0 SEND.
        */
        virtual void ProcessEndOrRejectL() = 0;
        
        /**
        * Ends a call and accepts another.
        *
        * Command: 1 SEND.
        */
        virtual void ProcessEndAndAcceptL() = 0;

        /**
        * End specific active call.
        *
        * Command: 1X SEND.
        *
        * @param aCallNo index of call, X in 1X.
        */
        virtual void ProcessEndCallL( TInt aCallNo ) = 0;

        /**
        * Hold all active calls except specific call.
        *
        * Command: 2X SEND.
        *
        * @param aCallNo index of call, X in 2X.
        */
        virtual void ProcessHoldAllCallsExceptL( TInt aCallNo ) = 0;

        /**
        * Swap
        *
        * Command: 2 SEND.
        */
        virtual void ProcessSwapL() = 0;

        /**
        * Join two calls to conference.
        *
        * Command: 3 SEND.
        */
        virtual void ProcessJoinL() = 0;

        /**
        * Perform explicit call transfer
        *
        * Command: 4 SEND.
        */ 
        virtual void ProcessExplicitCallTransferL() = 0;
               
    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, processes result.
        *
        * @param aResult It is to be processed.
        */
        IMPORT_C virtual void ProcessL( CPhoneGsmParserResult& aResult );
    };

#endif      // CPHONEGSMSSCALLHANDLER_H
            
// End of File
