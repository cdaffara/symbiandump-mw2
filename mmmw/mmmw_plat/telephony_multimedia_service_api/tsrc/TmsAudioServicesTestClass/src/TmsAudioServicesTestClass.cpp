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
 * Description: Telephony Multimedia Service - STIF TEST
 *
 */

// INCLUDE FILES
#include <Stiftestinterface.h>
#include "TmsAudioServicesTestClass.h"

// -----------------------------------------------------------------------------
// CTmsAudioServicesTestClass::CTmsAudioServicesTestClass
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTmsAudioServicesTestClass::CTmsAudioServicesTestClass(
        CTestModuleIf& aTestModuleIf) :
    CScriptBase(aTestModuleIf),
    iTestModuleIf(aTestModuleIf)
    {
    iDnLinkStatus = UNINITIALIZED;
    iUpLinkStatus = UNINITIALIZED;
    }

// -----------------------------------------------------------------------------
// CTmsAudioServicesTestClass::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTmsAudioServicesTestClass::ConstructL()
    {
    iLog = CStifLogger::NewL(KTmsAudioServicesTestClassLogPath,
            KTmsAudioServicesTestClassLogFile, CStifLogger::ETxt,
            CStifLogger::EFile, EFalse);

    iTimeoutController = CSimpleTimeout::NewL(this, iLog);
    }

// -----------------------------------------------------------------------------
// CTmsAudioServicesTestClass::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTmsAudioServicesTestClass* CTmsAudioServicesTestClass::NewL(
        CTestModuleIf& aTestModuleIf)
    {
    CTmsAudioServicesTestClass* self =
            new (ELeave) CTmsAudioServicesTestClass(aTestModuleIf);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CTmsAudioServicesTestClass::~CTmsAudioServicesTestClass()
    {
    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog;
    delete iTimeoutController;
    }

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Backpointer to STIF Test Framework
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL(CTestModuleIf& aTestModuleIf)
    {
    return (CScriptBase*) CTmsAudioServicesTestClass::NewL(aTestModuleIf);
    }

//  End of File
