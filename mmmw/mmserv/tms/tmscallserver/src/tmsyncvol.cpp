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
 * Description: Telephony Multimedia Service
 *
 */

// INCLUDE FILES
#include "tmssyncvol.h"
#include "tmsclientserver.h"
#include "tmsutility.h"

using namespace TMS;

// CONSTANTS
_LIT_SECURITY_POLICY_PASS(KTMSServerReadPolicy);
_LIT_SECURITY_POLICY_C1(KTMSServerWritePolicy, ECapabilityWriteUserData);

// -----------------------------------------------------------------------------
// TMSSyncVol::TMSSyncVol
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TMSSyncVol::TMSSyncVol()
    {
    }

// -----------------------------------------------------------------------------
// TMSSyncVol::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void TMSSyncVol::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    TInt err = RProperty::Define(ESyncVolume, RProperty::EInt,
            KTMSServerReadPolicy, KTMSServerWritePolicy);
    TRACE_PRN_N1(_L("TMSSyncVol::ConstructL err:%d"),err);

    if (err != KErrAlreadyExists)
        {
        User::LeaveIfError(err);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSSyncVol::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSSyncVol* TMSSyncVol::NewL()
    {
    TMSSyncVol* self = new (ELeave) TMSSyncVol();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
TMSSyncVol::~TMSSyncVol()
    {
    TRACE_PRN_FN_ENT;
    TInt err = RProperty::Delete(KTMSPropertyCategory, ESyncVolume);
    TRACE_PRN_N1(_L("TMSSyncVol::~TMSSyncVol err:%d"),err);
    TRACE_PRN_FN_ENT;
    }

// -----------------------------------------------------------------------------
// TMSSyncVol::SetSyncVol
// -----------------------------------------------------------------------------
//
void TMSSyncVol::SetSyncVol(TBool syncvol)
    {
    TRACE_PRN_FN_ENT;
    //    TInt err(KErrNone);
    if (syncvol)
        {
        /*err =*/RProperty::Set(KTMSPropertyCategory, ESyncVolume, syncvol);
        }

    //TRACE_PRN_N1(_L("TMSSyncVol::SetSyncVol err:%d"),err);
    TRACE_PRN_FN_EXT;
    }

//  End of File
