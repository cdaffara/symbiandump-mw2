/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Verifier plug-in which derives from CPosPrivacyNotifier.
*
*/


// INCLUDES

#ifndef CLPDVERIFIERDLG_H
#define CLPDVERIFIERDLG_H

#include "lpdrequestao.h"
#include "lpdresulthandler.h"

#include <epos_cposprivacynotifier.h>
#include <lbs/epos_privacy.h>
#include <lbs/epos_cposrequestor.h>
// SYSTEM INCLUDE
#include <bldvariant.hrh>               // For the 3.2 Flag


// FORWARD DECLARATIONS
class CLpdVerifierQueryLauncher;
class CLpdNotifierQueryLauncher;
class CLpdRequestorProcessor;
class CLocRequestorUtilsResolver;


// CLASS DECLARATION

/**
*  Verifier plug-in which derives from CPosPrivacyNotifier.
*  Notifiers are based on RNotifier framework, and this particular
*  notifier is used to launch a privacy query for accepting/rejecting
*  privacy requests. It also shows informational notes and error
*  messages, which are received as notification.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdVerifierPlugin : public CPosPrivacyNotifier, public MLpdResultHandler
    {
    // To allow calls to HandleNextRequest() from active object
    friend class CLpdRequestAO;
    
    private:

        
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @return created object
        */
        static CLpdVerifierPlugin* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CLpdVerifierPlugin();

	   /**
        * Method for cancelling query or freeing
        * resources after finishing query.
        */
        void FreeQueryResources();

    public: // Functions from base classes

        /**
        * From CPosPrivacyNotifier, called when a new request arrives.
        * @param aRequestId identifier of received request
        */
        void HandleNewRequestL( TPosQNRequestId aRequestId );

        /**
        * From CPosPrivacyNotifier, called when request has been cancelled
        * @param aRequestId identifier of cancelled request
        */
        void HandleRequestCancelled( TPosQNRequestId aRequestId );

        /**
        * From CPosPrivacyNotifier, called when all requests are cancelled.
        */
        void HandleAllRequestCancelled();

        /**
        * From MLpdResultHandler, called when verification result is available.
        * @param aResultCode
        */        
        void HandleVerificationResultL( TInt aResultCode );

        /**
        * From MLpdResultHandler, called when notification has been done.
        * @param aResultCode
        */        
        void HandleNotificationResultL( TInt aResultCode );

        /**
        * From MLpdResultHandler, called if leave occurred in callback method
        * @param aError code
        */        
        void HandleLeave( TInt aError );
        
      


    protected:  // New functions

        /**
        * Non-leaving internal method for handling next pending request.
        */
        void HandleNextRequest();
        
        /**
        * Internal method for handling next pending request.
        */
        void HandleNextRequestL();

        /**
        * Internal helper method used by HandleNextRequestL().
        */
        void HandleNextVerificationL();

        /**
        * Internal helper method used by HandleNextRequestL().
        */
        void HandleNextNotificationL();

        /**
          * Internal helper method used by HandleNextNotificationL().
          */
        void HandleNextNonPeriodicNotificationL();

        
        /**
        * Internal helper method.
        * Completes current request and continues queue handling.
        */
        void CompleteCurrentAndContinue( TInt aResultCode );

        /**
        * Handles assert errors. Panics in debug build but leaves in release
        * build, as this component runs in EikSrv and must never panic.
        */
        void HandleAssertErrorL() const;

        /**
        * Panics in debug build but does nothing in release build.
        */
        void HandleDebugAssertError() const;
     
       
      

    private:

        /**
        * C++ default constructor.
        */
        CLpdVerifierPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
        
      
         /**
          * Checks the type of request and helps to know
          * if a given request is of SUPL periodic type
          * @param aCurrentRequest request id
          * @return CPosRequestor::TRequestType type of request enum
         */
        CPosRequestor::TRequestType CheckRequestTypeL();


    private:    // Data

     

        // Identifier of currently handled request
        TPosQNRequestId iCurrentRequest;

        // Type of the current request, valid if iCurrentRequest valid
        TRequestType iCurrentRequestType;

        // Result code of verifier query, needed if user cancels rule query
        TInt iVerifyResult;

       
        // Location request accept/reject query object
        CLpdVerifierQueryLauncher* iVerifierQuery;  // owns

        // Location request accept/reject query object
        CLpdNotifierQueryLauncher* iNotifier;       // owns
        
        // Active object, used to let requests run to
        // completion instead of recursive calls.
        CLpdRequestAO* iRequestActiveObject;        // owns

        CLpdRequestorProcessor* iRtorProcessor;     // owns
        
        CLocRequestorUtilsResolver* iUtils; // owns

        
        };

#endif      // CLPDVERIFIERDLG_H   
            
// End of File
