/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "tmstarsettings.h"
#include "tmsclientserver.h"
#include "tmsutility.h"

using namespace TMS;

// CONSTANTS
_LIT_SECURITY_POLICY_PASS(KTMSServerReadPolicy);
_LIT_SECURITY_POLICY_C1(KTMSServerWritePolicy, ECapabilityWriteUserData);

// -----------------------------------------------------------------------------
// TMSTarSettings::TMSTarSettings
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TMSTarSettings::TMSTarSettings()
    {
    }

// -----------------------------------------------------------------------------
// TMSTarSettings::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void TMSTarSettings::ConstructL()
    {
    TRACE_PRN_FN_ENT;
    TInt err = RProperty::Define(ERoutingPs, RProperty::EByteArray,
            KTMSServerReadPolicy, KTMSServerWritePolicy);
    TRACE_PRN_N1(_L("TarSetting::ConstructL err:%d"),err);

    if (err != KErrAlreadyExists)
        {
        User::LeaveIfError(err);
        }
    TRACE_PRN_FN_EXT;
    }

// -----------------------------------------------------------------------------
// TMSTarSettings::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
TMSTarSettings* TMSTarSettings::NewL()
    {
    TMSTarSettings* self = new (ELeave) TMSTarSettings();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
TMSTarSettings::~TMSTarSettings()
    {
    TRACE_PRN_FN_ENT;
    TInt err = RProperty::Delete(KTMSPropertyCategory, ERoutingPs);
    TRACE_PRN_N1(_L("TarSetting::~TMSTarSettings err:%d"),err);
    TRACE_PRN_FN_ENT;
    }

// -----------------------------------------------------------------------------
// TMSTarSettings::SetVolume
// -----------------------------------------------------------------------------
//
void TMSTarSettings::SetTar(TRoutingMsgBufPckg rountingpckg, TBool aPublish)
    {
    TRACE_PRN_FN_ENT;
    //    TInt err(KErrNone);
    if (aPublish)
        {
        /*err =*/RProperty::Set(KTMSPropertyCategory, ERoutingPs, rountingpckg);
        }

    //TRACE_PRN_N1(_L("TarSetting::SetTar err:%d"),err);
    TRACE_PRN_FN_EXT;
    }

//  End of File
