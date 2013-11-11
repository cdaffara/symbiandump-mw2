/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the implementation of asynchronous AO object.
*
*/


// INCLUDES
#include "AsyncAO.h"

using namespace multimedia;

CAsyncAO::CAsyncAO( MAsyncAOObserver& aObserver )
:CActive(CActive::EPriorityStandard),
iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }

CAsyncAO::~CAsyncAO()
    {
    Cancel();
    }

CAsyncAO* CAsyncAO::NewL( MAsyncAOObserver& aObserver )
    {
    CAsyncAO* self = new (ELeave) CAsyncAO(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CAsyncAO::ConstructL()
    {
    // No Implementation
    }

TInt CAsyncAO::SetActive()
    {
    TInt status(KErrInUse);
    if (!CActive::IsActive())
        {
        CActive::SetActive();
        status = KErrNone;
        }
    return status;
    }

void CAsyncAO::RunL()
    {
    iObserver.Event(iStatus.Int());
    }

void CAsyncAO::DoCancel()
    {
    if(iStatus.Int() != KErrNone)
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status,KErrCancel);
        }
    }

TInt CAsyncAO::Error( TInt /* aError */ )
    {
    // RunL left. The client callback function left. Incorrect callback
    // implementation. So ignore the leave and continue
    return KErrNone;
    }

//  End of File
