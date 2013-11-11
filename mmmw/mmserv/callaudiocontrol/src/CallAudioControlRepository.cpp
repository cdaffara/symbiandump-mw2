/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:  This is the implementation of the CallAudioControlRepository class.
 *
 */

// INCLUDE FILES
#include "CallAudioControlRepository.h"
#include "CallAudioControlImpl.h"
#include "CallAudioControlCommon.h"

// -----------------------------------------------------------------------------
// CCallAudioControlRepository::NewL
//
// -----------------------------------------------------------------------------
//
CCallAudioControlRepository* CCallAudioControlRepository::NewL(
        CCallAudioControlImpl* aImplParent, const TUid aUid,
        const TUint32 aKey, TAction aAction)
    {
    CCallAudioControlRepository* self =
            new (ELeave) CCallAudioControlRepository();
    CleanupStack::PushL(self);
    self->ConstructL(aImplParent, aUid, aKey, aAction);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CCallAudioControlRepository::CCallAudioControlRepository
// C++ constructor.
// -----------------------------------------------------------------------------
//
CCallAudioControlRepository::CCallAudioControlRepository() :
    CActive(EPriorityNormal), iSubscribed(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// CCallAudioControlRepository::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCallAudioControlRepository::ConstructL(
        CCallAudioControlImpl* aImplParent, const TUid aUid,
        const TUint32 aKey, TAction aAction)
    {
    CAC_TRACE1(_L("CCallAudioControlRepository::ConstructL enter"));
    iImplParent = aImplParent;
    iAction = aAction;
    iKey = aKey;
    //Create Repository object:
    iRepository = CRepository::NewL(aUid);
    CActiveScheduler::Add(this);
    CAC_TRACE1(_L("CCallAudioControlRepository::ConstructL exit"));
    }

// -----------------------------------------------------------------------------
// CCallAudioControlRepository::DestructL
// Destructor
// -----------------------------------------------------------------------------
//
CCallAudioControlRepository::~CCallAudioControlRepository()
    {
    CAC_TRACE2(_L("CCallAudioControlRepository[%x]::~CCallAudioControlRepository enter"),this);
    Cancel();
    delete iRepository;
    CAC_TRACE2(_L("CCallAudioControlRepository[%x]::~CCallAudioControlRepository exit"),this);
    }

// ---------------------------------------------------------
// CCallAudioControlRepository::NotifyRequest
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCallAudioControlRepository::NotifyRequest()
    {
    CAC_TRACE2(_L("CCallAudioControlRepository[%x]::NotifyRequest"),this);
    if (!IsActive())
        {
        iRepository->NotifyRequest(iKey, iStatus);
        SetActive();
        }
    iSubscribed = ETrue;
    }

// ---------------------------------------------------------
// CCallAudioControlRepository::NotifyCancel
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCallAudioControlRepository::NotifyCancel()
    {
    CAC_TRACE2(_L("CCallAudioControlRepository[%x]::NotifyCancel"),this);
    iRepository->NotifyCancel(iKey);
    iSubscribed = EFalse;
    }

// -----------------------------------------------------------------------------
// CCallAudioControlRepository::Get
// -----------------------------------------------------------------------------
//
TInt CCallAudioControlRepository::Get(TInt& aValue)
    {
    CAC_TRACE2(_L("CCallAudioControlRepository[%x]::Get"), this);
    TInt error = iRepository->Get(iKey, aValue);
    return error;
    }

// -----------------------------------------------------------------------------
// CCallAudioControlRepository::Set
// -----------------------------------------------------------------------------
//
TInt CCallAudioControlRepository::Set(TInt aValue)
    {
    CAC_TRACE2(_L("CCallAudioControlRepository[%x]::Set"), this);
    TInt error = iRepository->Set(iKey, aValue);
    return error;
    }

//
// Methods required for CActive
//
// -----------------------------------------------------------------------------
// CCallAudioControlRepository::DoCancel
// -----------------------------------------------------------------------------
//
void CCallAudioControlRepository::DoCancel()
    {
    if (iSubscribed)
        {
        iRepository->NotifyCancel(iKey);
        iSubscribed = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// CCallAudioControlRepository::RunL
// -----------------------------------------------------------------------------
//
void CCallAudioControlRepository::RunL()
    {
    CAC_TRACE2(_L("CCallAudioControlRepository[%x]::RunL"), this);
    TInt value;
    TInt err = iRepository->Get(iKey, value);
    if (err != KErrNone)
        {
        CAC_TRACE3(_L("CCallAudioControlRepository[%x]::RunL ERROR From Get: %d"), this,err);
        }
    // Notify CallAudioControl of change:
    if (iImplParent != NULL)
        {
        iImplParent->NotifyL(iAction, err, value);
        }
    // Set self up to get future vol change notifications
    iSubscribed = EFalse;
    NotifyRequest();

    }

// -----------------------------------------------------------------------------
// CCallAudioControlRepository::RunError
// -----------------------------------------------------------------------------
//
TInt CCallAudioControlRepository::RunError()
    {
    return KErrNone;
    }

// End of file
