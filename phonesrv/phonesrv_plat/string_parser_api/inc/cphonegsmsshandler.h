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
* Description:  It is base class for handlers of supplementary service requests.
*
*/


#ifndef CPHONEGSMSSHANDLER_H
#define CPHONEGSMSSHANDLER_H

// INCLUDES
#include    "cphonegsmhandlerbase.h" 

// FORWARD DECLARATIONS
class CPhoneGsmParserResult;

// CLASS DECLARATION

/**
* It is base class for handlers of supplementary service requests.
*
* @since 1.0
* @lib phoneparser.lib
*/
class CPhoneGsmSsHandler 
    : public CPhoneGsmHandlerBase
    {
    public: // New functions

        /**
        * It is enumeration containing all actions.
        */
        enum TAction
            {
            EActivate,
            EDeactivate,
            EInterrogate,
            ERegister,
            EErase
            };

        /**
        * It is enumeration containing all forwarding procedures.
        */
        enum TForwardingProcedure
            {
            EForwardingNotReachable,
            EForwardingNoReply,
            EForwardingBusy,
            EForwardingUnconditional,
            EForwardingAll,
            EForwardingAllConditional
            };

        /**
        * It is enumeration containing all barring procedures.
        */
        enum TBarringProcedure
            {
            EBarringAllOutgoingCalls,
            EBarringAllOutgoingInternationalCalls,
            EBarringAllOutgoingInternationalCallsExceptHome,
            EBarringAllIncomingCalls,
            EBarringAllIncomingCallsWhenRoaming,
            EBarringAllServices,
            EBarringAllOutgoingServices,
            EBarringAllIncomingServices
            };

        /**
        * It is enumeration containing all password procedures.
        */ 
        enum TPasswordProcedure
            {
            EPasswordBarring,
            EPasswordNetwork
            };

        /**
        * Process forwarding procedures.
        *
        * If a parameter is not allowed for a procedure,
        * then it is empty. If a parameter is not explicitly given,
        * then it is empty. If a parameter is present, then it can
        * be assumed to contain appropriate values.
        *
        * @param aAction It is action.
        * @param aForwarding It contains forwarding procedure.
        * @param aNumber It is directory number.
        * @param aBasicCode It is basic service code.
        * @param aTimer It is timer.
        */
        virtual void ProcessForwardingL(
            TAction aAction,
            TForwardingProcedure aForwarding,
            const TDesC& aNumber,
            const TDesC& aBasicCode,
            const TDesC& aTimer ) = 0;

        /**
        * Process barring procedures.
        *
        * See comments for forwarding, they apply also in here.
        *
        * @param aAction It is action.
        * @param aBarring It contains barring procedure.
        * @param aPassword It should be barring password, if required.
        * @param aBasicCode It is basic service code.
        */
        virtual void ProcessBarringL(
            TAction aAction,
            TBarringProcedure aBarring,
            const TDesC& aPassword,
            const TDesC& aBasicCode ) = 0;

        /**
        * Process password change
        *
        * @param aPassword It is password procedure.
        * @param aOldPassword It should be old password.
        * @param aNewPassword1 It is new password.
        * @param aNewPassword2 It is new password.
        */
        virtual void ProcessPasswordChangeL(
            TPasswordProcedure aPassword,
            const TDesC& aOldPassword,
            const TDesC& aNewPassword1,
            const TDesC& aNewPassword2 ) = 0;

        /**
        * Process waiting.
        *
        * @param aAction It is action.
        * @param aBasicCode It is basic service code.
        */
        virtual void ProcessWaitingL(
            TAction aAction,
            const TDesC& aBasicCode ) = 0;

        /**
        * Process calling line identification presentation status.
        */
        virtual void ProcessClipStatusL() = 0;

        /**
        * Process calling line idnetification restriction status.
        */
        virtual void ProcessClirStatusL() = 0;

        /**
        * Process connected line presentation status.
        */
        virtual void ProcessColpStatusL() = 0;

        /**
        * Process connected line restriction status.
        */
        virtual void ProcessColrStatusL() = 0;

        /**
        * Process calling name presentation status.
        */
        virtual void ProcessCnapStatusL() = 0;

    public: // Functions from base classes

        /**
        * From CPhoneGsmHandlerBase, processes result.
        *
        * @param aResult It is to be processed.
        */
        IMPORT_C virtual void ProcessL( CPhoneGsmParserResult& aResult );
        
    private:

        /**
        * Process barring result.
        */
        void DoProcessBarringL(
            TBarringProcedure aBarring,
            CPhoneGsmParserResult& aResult );

        /**
        * Process forwarding result.
        */
        void DoProcessForwardingL(
            TForwardingProcedure aForwarding,
            CPhoneGsmParserResult& aResult );
    };

#endif      // CPHONEGSMSSHANDLER_H
            
// End of File
