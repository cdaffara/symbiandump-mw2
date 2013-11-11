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
* Description:  Processes requestor related data.
*
*/


#ifndef CLPDREQUESTORPROCESSOR_H
#define CLPDREQUESTORPROCESSOR_H

//  INCLUDES
#include "locrequestorutilsresolver.h"
#include <e32base.h>
#include <lbs/epos_rposrequestorstack.h>
#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_privacy.h>
// FORWARD DECLARATIONS
class CPosPrivacyNotifier;


// CLASS DECLARATION

/**
*  This class reads, processes and owns requestor data.
*  @lib locverifierdlg.dll
*  @since 2.1
*/
class CLpdRequestorProcessor : public CBase
    {
    public:  // Constructors and destructor
        
        enum TDialogType
            {
            /** Query dialog requested. */
            EQuery = 0,
            /** Notification dialog requested. */
            ENotification
            };
 
    
    
       /**
        * Two-phased constructor.
        * @return created object
        */
        static CLpdRequestorProcessor* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CLpdRequestorProcessor();

    public: // New functions

        /**
        * Reads requestor information from the privacy notifier.
        * Uses RequestorIdValid() for id validity check.
        * Calls UpdateRuleRequestorsL().
        * @param aPosNotifier reference
        * @return ETrue if all requestor id's were valid, EFalse otherwise.
        */
        TBool ReadRequestorsL( const CPosPrivacyNotifier& aPosNotifier );

        /**
        * Returns requestor array for reading.
        * @return array of requestors
        */
        const RPointerArray<CPosRequestor>& Requestors() const;
        
        /**
         * Resets requestor arrays and destroys requestor objects.
         */
         void ResetAndDestroyRequestors();
       
             		
    protected: // Constructors and destructors

        /**
        * C++ constructor.
        */
        CLpdRequestorProcessor();

    protected: // New methods

        /**
        * Handles assert errors. Panics in debug build but leaves in release
        * build, as this component runs in EikSrv and must never panic.
        */
        void HandleAssertErrorL() const;

    private:
        TBool RequestorIdValid(
            const CPosRequestor& aRequestor );
   
    public:
        RPointerArray<CPosRequestor> iRequestors;
        CPosRequestor::TRequestType  iRequestType;
        TPosRequestDecision          iRequestDecision;
        TDialogType                  iDialogType; 
        TPosNotificationReason       iNotifReason; 
        
    };

#endif      // CLPDREQUESTORPROCESSOR_H   
            
// End of File
