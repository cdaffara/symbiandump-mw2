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
 * Description:  This is the implementation of the CallAudioControlProperty class.
 *
 */

// INCLUDE FILES
#include "CallAudioControlCommon.h"
#include "CallAudioControlProperty.h"
#include "CallAudioControlImpl.h"

// -----------------------------------------------------------------------------
// CCallAudioControlProperty::NewL
//
// -----------------------------------------------------------------------------
//
CCallAudioControlProperty* CCallAudioControlProperty::NewL(
        CCallAudioControlImpl* aImplParent, const TUid aUid,
        const TUint32 aKey, TAction aAction)
    {
    CCallAudioControlProperty* self =
            new (ELeave) CCallAudioControlProperty();
    CleanupStack::PushL(self);
    self->ConstructL(aImplParent, aUid, aKey, aAction);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CCallAudioControlProperty::CCallAudioControlProperty
// C++ constructor.
// -----------------------------------------------------------------------------
//
CCallAudioControlProperty::CCallAudioControlProperty() :
    CActive(EPriorityNormal) // , iSubscribed(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// CCallAudioControlProperty::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCallAudioControlProperty::ConstructL(
        CCallAudioControlImpl* aImplParent, const TUid aUid,
        const TUint32 aKey, TAction aAction)
    {
    CAC_TRACE1(_L("CCallAudioControlProperty::ConstructL enter"));
    iImplParent = aImplParent;
    iAction = aAction;
    //Attach to P&S variable:
    User::LeaveIfError(iProperty.Attach(aUid, aKey));
    CActiveScheduler::Add(this);
    CAC_TRACE1(_L("CCallAudioControlProperty::ConstructL exit"));
    }

// -----------------------------------------------------------------------------
// CCallAudioControlProperty::DestructL
// Destructor
// -----------------------------------------------------------------------------
//
CCallAudioControlProperty::~CCallAudioControlProperty()
    {
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::~CCallAudioControlProperty enter"),this);
    // Cancel possibly outstanding asynchronous requests
    Cancel();
    // Release the property handle
    iProperty.Close();
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::~CCallAudioControlProperty exit"),this);
    }

// ---------------------------------------------------------
// CCallAudioControlProperty::Subscribe
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCallAudioControlProperty::Subscribe()
    {
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::Subscribe"),this);
    if (!IsActive())
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    // iSubscribed = ETrue;
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::Subscribe: SetActive..."),this);
    }

// ---------------------------------------------------------
// CCallAudioControlProperty::Unsubscribe
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCallAudioControlProperty::Unsubscribe()
    {
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::Unsubscribe"),this);
    iProperty.Cancel();
    Cancel();
    // iSubscribed = EFalse;
    }

// -----------------------------------------------------------------------------
// CCallAudioControlProperty::Get
// -----------------------------------------------------------------------------
//
TInt CCallAudioControlProperty::Get(TInt& aValue)
    {
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::Get"), this);
    TInt error = iProperty.Get(aValue);
    return error;
    }

// -----------------------------------------------------------------------------
// CCallAudioControlProperty::Set
// -----------------------------------------------------------------------------
//
TInt CCallAudioControlProperty::Set(TInt aValue)
    {
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::Set"), this);
    TInt error = iProperty.Set(aValue);
    return error;
    }

//
// Methods required for CActive
//
// -----------------------------------------------------------------------------
// CCallAudioControlProperty::DoCancel
// -----------------------------------------------------------------------------
//
void CCallAudioControlProperty::DoCancel()
    {
    iProperty.Cancel();
    }

// -----------------------------------------------------------------------------
// CCallAudioControlProperty::RunL
// -----------------------------------------------------------------------------
//
void CCallAudioControlProperty::RunL()
    {
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::RunL enter"), this);
    TInt value;
    TInt err(KErrNone);
    
    if ( iStatus.Int() != KErrCancel )
        {    
        // Set self up for future notifications:
        // iSubscribed = EFalse;
        Subscribe();

        err = iProperty.Get(value);
    
        if (err != KErrNone)
            {
            CAC_TRACE3(_L("CCallAudioControlProperty[%x]::RunL ERROR From Get: %d"), this, err);
            }
        else
            {
            CAC_TRACE2(_L("RunL: New value = %d..."), value);
            // Notify CallAudioControl of change:
            if (iImplParent != NULL)
                {
                CAC_TRACE1(_L("RunL: Calling NotifyL on CACImpl..."));
                iImplParent->NotifyL(iAction, err, value);
                }
            }
        }
    CAC_TRACE2(_L("CCallAudioControlProperty[%x]::RunL exit"), this);
    }

// -----------------------------------------------------------------------------
// CCallAudioControlProperty::RunError
// -----------------------------------------------------------------------------
//
TInt CCallAudioControlProperty::RunError()
    {
    return KErrNone;
    }

// End of file

