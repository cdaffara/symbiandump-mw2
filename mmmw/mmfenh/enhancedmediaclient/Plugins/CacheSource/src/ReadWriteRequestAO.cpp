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
* Description:  Implementation of ReadRequestAO class.
*
*/


#include <mmfdatabuffer.h>
#include "ReadWriteRequestAO.h"
#include "CacheSource.h"

CReadWriteRequestAO::CReadWriteRequestAO(CCacheSource& aSource,TRequestType aType )
: CActive(CActive::EPriorityStandard),
iSource(aSource),
iType(aType),
iError(KErrNone)
    {
    CActiveScheduler::Add(this);
    }

CReadWriteRequestAO::~CReadWriteRequestAO()
    {
    Cancel();
    }

CReadWriteRequestAO* CReadWriteRequestAO::NewL( CCacheSource& aSource,TRequestType aType )
    {
    CReadWriteRequestAO* self = new (ELeave)CReadWriteRequestAO( aSource,aType );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CReadWriteRequestAO::ConstructL()
    {
    }

void CReadWriteRequestAO::SetActive()
    {
    if (!IsActive())
        {
        CActive::SetActive();
        }
    }

TInt CReadWriteRequestAO::Error()
    {
    return iError;
    }

// From CActive
void CReadWriteRequestAO::RunL()
    {
    // Save the error code
    iError = iStatus.Int();
    // Signal the observer that this request is serviced
    iSource.ReadRequestComplete(this,iStatus);
    }

void CReadWriteRequestAO::DoCancel()
    {
    if(iStatus.Int() != KErrNone)
        {
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status,KErrCancel);
        }
    }

TInt CReadWriteRequestAO::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

CMMFDataBuffer* CReadWriteRequestAO::Buffer()
    {
    return static_cast<CMMFDataBuffer*>(iBuffer);        
    }

TInt CReadWriteRequestAO::SetBuffer(CMMFBuffer* aBuffer)
    {
    iBuffer = aBuffer; 
    return KErrNone;   
    }

CReadWriteRequestAO::TRequestType CReadWriteRequestAO::RequestType()
    {
    return iType;       
    }

// End of File

