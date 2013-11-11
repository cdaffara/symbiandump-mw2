/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Class for LBS Privacy Query & Notify (Q&N) privacy notifiers.
*
*/



#ifndef CPOSPRIVACYNOTIFIER_H
#define CPOSPRIVACYNOTIFIER_H

//  INCLUDES
#include <e32base.h>
#include <lbs/epos_privacy.h>
#include <lbs/epos_tposqninputdata.h>

// DATA TYPES


// CONSTANT DECLARATIONS
/**
@publishedPartner
@deprecated
*/
const TPosQNRequestId KPosNullQNRequestId = -1;

// FORWARD DECLARATIONS
class CPosPrivacyNotifierExtension;
class CPosRequestor;
class MEikSrvNotifierBase2;

// CLASS DECLARATION
/**
Base class for LBS Privacy Query & Notify (Q&N) privacy notifiers.

A privacy notifier is a UI plug-in which handles privacy query and
notification requests.

This class contains declarations for event methods that must be implemented
by the notifier in order to receive information about new requests or
cancelled requests.

It also contains methods for accessing information about a request and for
completing a request.

@publishedPartner
@deprecated
*/
class CPosPrivacyNotifier : public CBase
    {
    public:
        /**
        Specifies whether the requested dialog should be a query or a
        notification dialog.
        */
        enum TRequestType {
            /** Query dialog requested. */
            EQuery = 0,
            /** Notification dialog requested. */
            ENotification
            };

    public:
        IMPORT_C virtual ~CPosPrivacyNotifier();

    public:

        /**
        * This method is called when a new privacy query or notification has
        * been requested.
        *
        * @param aRequestId The ID of the new query or notification request.
        */
        virtual void HandleNewRequestL(
        /* IN  */       TPosQNRequestId aRequestId
        ) = 0;

        /**
        * This method is called when a privacy query or notification request is
        * cancelled.
        *
        * It is not necessary to call @ref CompleteRequest for a cancelled
        * request.
        *
        * @param aRequestId The ID of the cancelled query or notification
        *   request.
        */
        virtual void HandleRequestCancelled(
        /* IN  */       TPosQNRequestId aRequestId
        ) = 0;

        /**
        * This method is called to cancel all outstanding requests.
        *
        * It is not necessary to call @ref CompleteRequest for a cancelled
        * request.
        */
        virtual void HandleAllRequestCancelled() = 0;
        
        IMPORT_C MEikSrvNotifierBase2* NotifierBase() const;
        IMPORT_C TPosVerifyCancelReason CancelReason() const;
        IMPORT_C void GetRequestsL(RArray<TPosQNRequestId>& aRequestArray) const;
        IMPORT_C TRequestType RequestTypeL(TPosQNRequestId aRequestId) const;
        IMPORT_C void SetCurrentRequestL(TPosQNRequestId aRequestId);
        IMPORT_C TPosQNRequestId CurrentRequest() const;
        IMPORT_C TPosRequestDecision QueryTimeoutStrategy() const;
        IMPORT_C TPosRequestSource RequestSource() const;
        IMPORT_C TPosRequestDecision LocationRequestDecision() const;
        IMPORT_C TPosNotificationReason NotificationReason() const;
        IMPORT_C TInt RequestorCountL() const;
        IMPORT_C CPosRequestor* RequestorLC(TInt aRequestorIndex) const;
        IMPORT_C void CompleteRequest(TPosQNRequestId aRequestId, TInt aCompletionCode);
        IMPORT_C void CompleteAllRequests(TInt aCompletionCode);
        IMPORT_C TBool CheckClientSecureId(TSecureId aSecureId);

    protected:
        IMPORT_C CPosPrivacyNotifier();
        IMPORT_C void BaseConstructL(TUid aChannel,TInt aPriority);

    private:
        // By default, prohibit copy constructor
        CPosPrivacyNotifier( const CPosPrivacyNotifier& );
        // Prohibit assigment operator
        CPosPrivacyNotifier& operator= ( const CPosPrivacyNotifier& );

    private:
        CPosPrivacyNotifierExtension* iExtension;
    };

#endif      // CPOSPRIVACYNOTIFIER_H
