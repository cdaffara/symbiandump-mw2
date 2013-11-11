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
* Description:  Implementation for SUPL Triggered Stop Message
*
*/

#include "epos_comasupltriggeredstop.h"
#include "epos_suplconstants.h"
#include "epos_comasuplend.h"
#include "lbs/epos_eomasuplposerrors.h"


EXPORT_C COMASuplTriggeredStop::COMASuplTriggeredStop()
    {
    iOptionalMask = 0;
    }

EXPORT_C COMASuplTriggeredStop::~COMASuplTriggeredStop()
    {
    
    }

EXPORT_C COMASuplTriggeredStop* COMASuplTriggeredStop::NewL()
    {
    COMASuplTriggeredStop* self = new (ELeave)COMASuplTriggeredStop();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C void COMASuplTriggeredStop::ConstructL()
    {
    SetMessageType(COMASuplAsnMessageBase::ESUPL_TRIG_STOP); 
    }

EXPORT_C TInt COMASuplTriggeredStop::GetSuplStatusCode(COMASuplEnd::TOMASuplStatusCode& aStatusCode) const
    {   
    if(iOptionalMask & 1)
        {
        aStatusCode = iStatusCode;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }

EXPORT_C void COMASuplTriggeredStop::SetSuplStatusCode(const COMASuplEnd::TOMASuplStatusCode aStatusCode) 
    {   
    iOptionalMask |= 1;
    iStatusCode = aStatusCode;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggeredStop::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//              
EXPORT_C void COMASuplTriggeredStop::Clone(const COMASuplAsnMessageBase* aTrigStop,TInt& aErrorCode)
    {
    TRAP(aErrorCode, CloneBaseL(aTrigStop,aErrorCode));
    COMASuplTriggeredStop* trigStop = (COMASuplTriggeredStop*) aTrigStop;
    if(trigStop->iOptionalMask & 1)
        {
        COMASuplEnd::TOMASuplStatusCode statusCode;
        trigStop->GetSuplStatusCode(statusCode); //get status code from the trig start and clone it in 'this'
        SetSuplStatusCode(statusCode);
        }
    }

