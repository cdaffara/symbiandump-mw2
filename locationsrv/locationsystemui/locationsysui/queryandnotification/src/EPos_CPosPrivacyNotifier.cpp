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



#include <lbs/epos_cposcontactrequestor.h>
#include <lbs/epos_cposservicerequestor.h>
#include <lbs/epos_privacynotifier.hrh>
#include <EPos_CPosPrivacyNotifier.h>
#include "EPos_CPosPrivacyNotifierExtension.h"

#if defined(NRH_UNIT_TEST)
// For the NRH unit test, use a custom notifier UID.
// This is so that we can have both the unit test
// Q&N notifier and the integration test Q&N notifier
// (using the real UID) in the same ROM.
const TUid KNotifierUid = { 0x10283744 };
#else
const TUid KNotifierUid = { KPosPrivacyNotifierImplUid };
#endif // NRH_UNIT_TEST


// ---------------------------------------------------------------------------
/**
Default constructor.

This constructor assumes that the notifier only supports
EBasicCapabilities, i.e. the notifier supports queries but only
notifications of accepted requests are supported. If the notifier
is more advanced, the overloaded constructor should be used.
*/
EXPORT_C CPosPrivacyNotifier::CPosPrivacyNotifier()
    {
    }

// ---------------------------------------------------------------------------
/**
Symbian 2nd phase constructor. Must be called first thing during construction.

The notifier must specify channel and priority. These attributes are
defined by the Notifier Framework in eiknotapi.h.

The different priority values are defined in
MEikSrvNotifierBase2::TNotifierPriority.

@param aChannel A channel. See definition in Notifier Framework.
@param aPriority A notifier priority. See definition in Notifier Framework.

@see MEikSrvNotifierBase2::TNotifierPriority
*/
EXPORT_C void CPosPrivacyNotifier::BaseConstructL(
    TUid aChannel,
    TInt aPriority)
    {
    iExtension = CPosPrivacyNotifierExtension::NewL(this);
    iExtension->iNotifierInfo.iChannel = aChannel;
    iExtension->iNotifierInfo.iPriority = aPriority;
    iExtension->iNotifierInfo.iUid = KNotifierUid;
    }

// ---------------------------------------------------------------------------
EXPORT_C CPosPrivacyNotifier::~CPosPrivacyNotifier()
    {
    delete iExtension;
    }

// ---------------------------------------------------------------------------
/**
Retrieves a handle to the notifier base.

This handle is needed to populate the notifier base array in the
notifier factory method.

@return A handle to the notifier base.
*/
EXPORT_C MEikSrvNotifierBase2* CPosPrivacyNotifier::NotifierBase() const
    {
    return iExtension;
    }

// ---------------------------------------------------------------------------
/**
Retrieves the cancel reason.

Cancel reason can only be retrieved when HandleRequestCancelled or 
HandleAllRequestCancelled is running. If the method is called at another 
time, it will return ECancelReasonNotAvailable.

Note that the TCancelReason enum is designed to be extendable,
i.e. new values may be added in the future. This means that any
unrecognized value must be treated like ECancelReasonNotAvailable.

@return The cancel reason.

@see HandleRequestCancelled()
@see HandleAllRequestCancelled()
@see ECancelReasonNotAvailable
@see TCancelReason
*/
EXPORT_C TPosVerifyCancelReason
   CPosPrivacyNotifier::CancelReason() const
    {
    TPosQNRequestId requestId = CurrentRequest();
    TInt index = iExtension->Find(requestId);

    if (index == KErrNotFound)
        {
        return EPosCancelReasonNotAvailable;
        }

    return iExtension->iRequestArray[index].iCancelReason;
    }

// ---------------------------------------------------------------------------
/**
Returns a list of all outstanding privacy query and notification requests.

If there are no outstanding requests, an empty array will be returned.

@param aRequestArray On return, this array will contain all
  outstanding query and notification requests.
*/
EXPORT_C void CPosPrivacyNotifier::GetRequestsL(
    RArray<TPosQNRequestId>& aRequestArray) const
    {
    aRequestArray.Reset();
    for (TInt i = 0; i < iExtension->iRequestArray.Count(); i++)
        {
        User::LeaveIfError(
            aRequestArray.Append(iExtension->iRequestArray[i].iId));
        }
    }

// ---------------------------------------------------------------------------
/**
Checks whether a request is privacy query or notification type.

@param aRequestId The ID of the request to check.
@return @p EQuery if query type and ENotification if notification type.

@leave If the specified request is not an outstanding request, this method
  will leave with error code KErrNotFound.
*/
EXPORT_C CPosPrivacyNotifier::TRequestType CPosPrivacyNotifier::RequestTypeL(
    TPosQNRequestId aRequestId) const
    {
    TInt index = iExtension->Find(aRequestId);
    __ASSERT_ALWAYS(index != KErrNotFound, User::Leave(KErrNotFound));

    if (iExtension->iRequestArray[index].iType ==
        TPosQNInputData::EQuery)
        {
        return CPosPrivacyNotifier::EQuery;
        }

    return CPosPrivacyNotifier::ENotification;
    }

// ---------------------------------------------------------------------------
/**
Sets the request which the notifier wants to read information about.

Current request specifies the request which will be accessed when
RequestorCountL and RequestorLC are called.

@param aRequestId The ID of the current request.

@leave If the specified request is not an outstanding request, this method
  will leave with error code KErrNotFound.
*/
EXPORT_C void CPosPrivacyNotifier::SetCurrentRequestL(
    TPosQNRequestId aRequestId)
    {
    iExtension->PrepareL(aRequestId);
    }

// ---------------------------------------------------------------------------
/**
Returns the ID of the current request.

Current request specifies the request which will be accessed when
RequestorCountL and RequestorLC are called.

@return The ID of the current request. If the current request has
  become invalid, e.g. the request set as current has been cancelled,
  KPosNullQNRequestId will be returned.

@see RequestorCountL()
@see RequestorLC()
*/
EXPORT_C TPosQNRequestId CPosPrivacyNotifier::CurrentRequest() const
    {
    return iExtension->iCurrentRequestId;
    }

// ---------------------------------------------------------------------------
/**
Retrieves the timeout strategy for a query.

Timeout strategy specifies what the decision will be if the
verification query times out. The decision is either
EPosDecisionRejected or EPosDecisionAccepted.

Before calling this method, current request must first be set by
calling SetCurrentRequestL.

Timeout strategy can only be retrieved for a query request. If the
current request is a notification, this method will return
EPosDecisionNotAvailable.

Note that the TPosRequestDecision enum is designed to be
extendable, i.e. new values may be added in the future. This means
that any unrecognized value must be treated like
EPosDecisionNotAvailable.

@return The timeout strategy.

@see SetCurrentRequestL()
*/
EXPORT_C TPosRequestDecision CPosPrivacyNotifier::QueryTimeoutStrategy() const
    {
    TPosQNRequestId requestId = CurrentRequest();
    TInt index = iExtension->Find(requestId);

    if (index == KErrNotFound ||
        iExtension->iRequestArray[index].iType ==
            TPosQNInputData::ENotification)
        {
        return EPosDecisionNotAvailable;
        }

    return iExtension->iRequestArray[index].iTimeoutStrategy;
    }

// ---------------------------------------------------------------------------
/**
Retrieves the source of the location request.

Before calling this method, current request must first be set by
calling SetCurrentRequestL.

Note that the TPosRequestSource enum is designed to be extendable, 
i.e. new values may be added in the future. This means that any 
unrecognized value must be treated like EPosRequestSourceNotAvailable.

@return The request source, e.g. EPosRequestSourceNetwork or
  EPosRequestSourceNotAvailable if the request source is not specified.

@see SetCurrentRequestL()
@see TPosRequestSource
*/
EXPORT_C TPosRequestSource CPosPrivacyNotifier::RequestSource() const
    {
    TPosQNRequestId requestId = CurrentRequest();
    TInt index = iExtension->Find(requestId);

    if (index == KErrNotFound)
        {
        return EPosRequestSourceNotAvailable;
        }

    return iExtension->iRequestArray[index].iRequestSource;
    }

// ---------------------------------------------------------------------------
/**
Retrieves whether the location request was accepted or rejected.

For notification requests, this method will return the request
decision. For queries, this method will return EPosDecisionNotAvailable.

Before calling this method, current request must first be set by
calling SetCurrentRequestL, otherwise this method will return
EPosDecisionNotAvailable.

Note that the TPosRequestDecision enum is designed to be
extendable, i.e. new values may be added in the future. 
This means that any unrecognized value must be treated like
EPosDecisionNotAvailable.

@return Whether the location request was accepted or rejected, or
  EPosDecisionNotAvailable if the outcome has not yet been decided.
  
@see SetCurrentRequestL()
@see TPosRequestDecision
*/
EXPORT_C TPosRequestDecision
    CPosPrivacyNotifier::LocationRequestDecision() const
    {
    TPosQNRequestId requestId = CurrentRequest();
    TInt index = iExtension->Find(requestId);

    if (index == KErrNotFound ||
        iExtension->iRequestArray[index].iType == TPosQNInputData::EQuery)
        {
        return EPosDecisionNotAvailable;
        }

    return iExtension->iRequestArray[index].iRequestDecision;
    }

// ---------------------------------------------------------------------------
/**
Retrieves the reason for a notification.

Note that the TPosNotificationReason enum is designed to be
extendable, i.e. new values may be added in the future. This means
that any unrecognized value must be treated like
EPosNotificationReasonNotAvailable. 
If the notification reason is not known, the notification request 
should be completed with code KErrNotSupported.

@return The notification reason.
  For queries, this method will return EPosNotificationReasonNotAvailable.

  Before calling this method, current request must first be set by
  calling SetCurrentRequestL, otherwise this method will return
  EPosNotificationReasonNotAvailable.

@see EPosNotificationReasonNotAvailable
@see SetCurrentRequestL()
@see TPosNotificationReason
*/
EXPORT_C TPosNotificationReason CPosPrivacyNotifier::NotificationReason() const
    {
    TPosQNRequestId requestId = CurrentRequest();
    TInt index = iExtension->Find(requestId);

    if (index == KErrNotFound ||
        iExtension->iRequestArray[index].iType == TPosQNInputData::EQuery)
        {
        return EPosNotificationReasonNotAvailable;
        }

    return iExtension->iRequestArray[index].iNotificationReason;
    }

// ---------------------------------------------------------------------------
/**
Returns the number of requestors in the current request.

Before calling this method, current request must first be set by
calling SetCurrentRequestL.

@return The number of requestors in the current request.

@leave If the current request has not been set or the current request is no
  longer valid, e.g. because it has been cancelled or completed, this
  method will leave with error code KErrNotFound.

@see SetCurrentRequestL()
*/
EXPORT_C TInt CPosPrivacyNotifier::RequestorCountL() const
    {
    __ASSERT_ALWAYS(CurrentRequest() != KPosNullQNRequestId,
        User::Leave(KErrNotFound));

    return iExtension->iRequestorStack.Count();
    }

// ---------------------------------------------------------------------------
/**
Returns basic information about the requestor at a specified index.

Before calling this method, current request must first be set by
calling SetCurrentRequestL. 

Whether the requestor is a contact or a service can be found by
calling CPosRequestor::RequestorType.

If the requestor is a contact, the requestor object can be cast to
CPosContactRequestor.

If the requestor is a service, the requestor object can be cast to
CPosServiceRequestor.

@param aRequestorIndex The index of the requestor.
@return Information about the requestor.

@leave If the specified requestor index is not valid, i.e. less than 0 or
  larger than or equal to RequestorCountL, this method will
  leave with error code KErrArgument.

  If the current request has not been set or the current request is no 
  longer valid, e.g. because it has been cancelled or completed, this method 
  will leave with error code KErrNotFound.

@see SetCurrentRequestL()
@see RequestorCountL()
@see CPosContactRequestor
@see CPosServiceRequestor
@see CPosRequestor::RequestorType
*/
EXPORT_C CPosRequestor* CPosPrivacyNotifier::RequestorLC(
    TInt aRequestorIndex) const
    {
    __ASSERT_ALWAYS(CurrentRequest() != KPosNullQNRequestId,
        User::Leave(KErrNotFound));

    __ASSERT_ALWAYS(aRequestorIndex >= 0 &&
        aRequestorIndex < iExtension->iRequestorStack.Count(),
        User::Leave(KErrArgument));

    CPosRequestor* requestor = iExtension->iRequestorStack[aRequestorIndex];
    TInt type = requestor->RequestorType();
    CPosRequestor::TRequestorIdFormat format = requestor->RequestorIdFormat();
    TPtrC idString = requestor->RequestorIdString();

    if (type == CPosRequestor::ERequestorService)
        {
        CPosServiceRequestor* serviceRequestor = CPosServiceRequestor::NewLC(format, idString);
        serviceRequestor->SetRequestType(requestor->RequestType());
        serviceRequestor->SetNetworkType(requestor->NetworkType());
        return serviceRequestor;
        }

    CPosContactRequestor* contactRequestor = CPosContactRequestor::NewLC(format, idString);
    contactRequestor->SetRequestType(requestor->RequestType());
    contactRequestor->SetNetworkType(requestor->NetworkType());
    return contactRequestor;
    }

// ---------------------------------------------------------------------------
/**
This method completes a privacy query or notification request.

@param aRequestId The ID of the privacy query or notification request
  to complete.
@param aCompletionCode The request completion code.
  If the request is a privacy query, the completion code should be one of:
  - KErrNone if query is accepted by the phone user.
  - KErrAccessDenied if query is rejected by the phone user.
  - KErrTimedOut if the query times out.

  If the request is a privacy notification, the completion code should be one of:
  - KErrNone if the phone user dismisses the privacy notification
  - KErrTimedOut if the notification times out.
*/
EXPORT_C void CPosPrivacyNotifier::CompleteRequest(
    TPosQNRequestId aRequestId,
    TInt aCompletionCode)
    {
    iExtension->CompleteRequest(aRequestId, aCompletionCode);
    }

// ---------------------------------------------------------------------------
/**
This method completes all outstanding requests.

This method can be used to accept or reject all outstanding
requests by completing with codes @p KErrNone or @p KErrAccessDenied
respectively. All notification requests will also be completed with
this code.

@param aCompletionCode The request completion code.
*/
EXPORT_C void CPosPrivacyNotifier::CompleteAllRequests(TInt aCompletionCode)
    {
    iExtension->CompleteAllRequests(aCompletionCode);
    }

// ---------------------------------------------------------------------------
/**
Checks that the Privacy UI was launched by the expected client.

The caller specifies the secure ID of the expected client and this
method returns whether the Privacy UI was launched by that client or not.

@param aSecureId The secureID of the expected client
@return ETrue if the client that called the notifier is the expected client
*/
#pragma message("NOTE: CPosPrivacyNotifier::CheckClientSecureId has no type/return definition.")
EXPORT_C TBool CPosPrivacyNotifier::CheckClientSecureId(TSecureId aSecureId)
    {
    return iExtension->iMessage.SecureId() == aSecureId;
    }
