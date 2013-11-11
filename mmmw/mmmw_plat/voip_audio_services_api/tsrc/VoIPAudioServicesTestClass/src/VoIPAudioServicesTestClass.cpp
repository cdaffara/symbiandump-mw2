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
 * Description: voip audio service -
 *
 */

// INCLUDE FILES
#include <Stiftestinterface.h>
#include "VoIPAudioServicesTestClass.h"

// -----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::CVoIPAudioServicesTestClass
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVoIPAudioServicesTestClass::CVoIPAudioServicesTestClass(
        CTestModuleIf& aTestModuleIf) :
    CScriptBase(aTestModuleIf),
    iTestModuleIf(aTestModuleIf),
    iPlayBufPtr(0, 0, 0),
    iRecBufPtr(0, 0, 0)
    {
    iDnLinkCodec = ENULL;
    iUpLinkCodec = ENULL;
    iDnLinkStatus = ENotReady;
    iUpLinkStatus = ENotReady;
    iSeqNum = 0;
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CVoIPAudioServicesTestClass::ConstructL()
    {
    iLog = CStifLogger::NewL(KVoIPAudioServicesTestClassLogPath,
            KVoIPAudioServicesTestClassLogFile, CStifLogger::ETxt,
            CStifLogger::EFile, EFalse);

    iTimeoutController = CSimpleTimeout::NewL(this, iLog);
    }

// -----------------------------------------------------------------------------
// CVoIPAudioServicesTestClass::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CVoIPAudioServicesTestClass* CVoIPAudioServicesTestClass::NewL(
        CTestModuleIf& aTestModuleIf)
    {
    CVoIPAudioServicesTestClass* self =
            new (ELeave) CVoIPAudioServicesTestClass(aTestModuleIf);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CVoIPAudioServicesTestClass::~CVoIPAudioServicesTestClass()
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
    return (CScriptBase*) CVoIPAudioServicesTestClass::NewL(aTestModuleIf);
    }

