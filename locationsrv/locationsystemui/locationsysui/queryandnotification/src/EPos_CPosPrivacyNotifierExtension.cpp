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
* Description:   Class for privacy notifier extension helper.
*
*/



// INCLUDE FILES
#include <s32mem.h>
#include <lbs/epos_privacy.h>
#include <lbs/epos_cposrequestor.h>
#include "EPos_CPosPrivacyNotifierExtension.h"
#include "EPos_CPosRequestHandler.h"


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPosPrivacyNotifierExtension::CPosPrivacyNotifierExtension()
    {
    }

// -----------------------------------------------------------------------------
// CPosPrivacyNotifierExtension::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosPrivacyNotifierExtension::ConstructL(
    CPosPrivacyNotifier* aPrivNotifier)
    {
    iRequestHandler = CPosRequestHandler::NewL(aPrivNotifier, this);
    iPrivacyNotifier = aPrivNotifier;
    ResetData();
    }

// Two-phased constructor
CPosPrivacyNotifierExtension* CPosPrivacyNotifierExtension::NewL(
    CPosPrivacyNotifier* aPrivNotifier)
    {
    CPosPrivacyNotifierExtension* self = new (ELeave)
        CPosPrivacyNotifierExtension;
    CleanupStack::PushL(self);
    self->ConstructL(aPrivNotifier);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosPrivacyNotifierExtension::~CPosPrivacyNotifierExtension()
    {
    iRequestorStack.ResetAndDestroy();
    iRequestArray.Close();
    delete iRequestHandler;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::Find
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosPrivacyNotifierExtension::Find(TPosQNRequestId aRequestId) const
    {
    for (TInt index = 0; index < iRequestArray.Count(); index++)
        {
        if (iRequestArray[index].iId == aRequestId)
            {
            return index;
            }
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::ResetData
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosPrivacyNotifierExtension::ResetData()
    {
    iRequestorStack.ResetAndDestroy();
    iCurrentRequestId = KPosNullQNRequestId;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::RemoveRequestFromArray
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosPrivacyNotifierExtension::RemoveRequestFromArray(
    TPosQNRequestId aRequestId)
    {
    TInt index = Find(aRequestId);
    if (index == KErrNotFound)
        {
        return KErrNotFound;
        }

    iRequestArray.Remove(index);
    return index;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::PrepareL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosPrivacyNotifierExtension::PrepareL(
    TPosQNRequestId aRequestId)
    {
    TInt index = Find(aRequestId);
    __ASSERT_ALWAYS(index != KErrNotFound, User::Leave(KErrNotFound));

    iCurrentRequestId = aRequestId;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::CompleteRequest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosPrivacyNotifierExtension::CompleteRequest(
    TPosQNRequestId aRequestId,
    TInt aCompletionCode)
    {
    if (RemoveRequestFromArray(aRequestId) != KErrNotFound)
        {
        iMessage.Complete(aCompletionCode);
        ResetData();
        }
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::CompleteAllRequests
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosPrivacyNotifierExtension::CompleteAllRequests(TInt aCompletionCode)
    {
    CompleteRequest(iCurrentRequestId, aCompletionCode);
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::Release
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosPrivacyNotifierExtension::Release()
    {
    delete iPrivacyNotifier;
    iPrivacyNotifier = NULL;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::RegisterL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosPrivacyNotifierExtension::TNotifierInfo
    CPosPrivacyNotifierExtension::RegisterL()
    {
    return iNotifierInfo;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::Info
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosPrivacyNotifierExtension::TNotifierInfo
    CPosPrivacyNotifierExtension::Info() const
    {
    return iNotifierInfo;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPtrC8 CPosPrivacyNotifierExtension::StartL(const TDesC8& /*aBuffer*/)
    {
    User::Leave(KErrNotSupported);
    return TPtrC8();
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosPrivacyNotifierExtension::StartL(
    const TDesC8& /*aBuffer*/,
    TInt /*aReplySlot*/,
    const RMessagePtr2& aMessage)
    {
    iMessage = aMessage;
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::Cancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosPrivacyNotifierExtension::Cancel()
    {
    if (iRequestArray.Count() > 0)
        {
        TPosQNRequestId reqId = iRequestArray[0].iId;
        iPrivacyNotifier->HandleRequestCancelled(reqId);
        CompleteRequest(reqId, KErrCancel);
        }
    }

// ---------------------------------------------------------
// CPosPrivacyNotifierExtension::UpdateL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPtrC8 CPosPrivacyNotifierExtension::UpdateL(const TDesC8& aBuffer)
    {
    TPtrC8 tposQnData = aBuffer.Left(KPosQNInputDataClassSize);

    TPckgBuf<TPosQNInputData> input;
    input.Copy(tposQnData);
    TPosQNInputData update = input();

    TInt index = Find(update.iId);
    if (update.iCancelReason == EPosCancelReasonNotAvailable || 
        (update.iCancelReason == EPosCancelReasonTimeout && 
        update.iNotificationReason == EPosVerificationTimeout) )
        {
        HBufC8* buf = HBufC8::NewLC(update.iDataSize);
        TPtrC8 bufPtr = aBuffer.Mid(KPosQNInputDataClassSize);

        RDesReadStream stream(bufPtr);
        CleanupClosePushL(stream);
        iRequestorStack.InternalizeL(stream);
        CleanupStack::PopAndDestroy(2, buf); //buf, stream

        User::LeaveIfError(iRequestArray.Append(update));
        iRequestHandler->ScheduleRequest(update);
        }
    else
        {
        if (index == KErrNotFound)
            {
            User::Leave(KErrArgument);
            }

        iRequestArray[index].iCancelReason = update.iCancelReason;
        }

    return TPtrC8();
    }

//  End of File
