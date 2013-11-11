/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Operation for compacting a landmarks database.
*
*
*/


// INCLUDE FILES
#include    <epos_cposlmlocaldbaccess.h>
#include    <epos_cposlmcompactdatabase.h>
#include    "EPos_CPosLmLocalCompactingOp.h"
#include    "epos_cposlmlocaldatabase.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalCompactingOp::CPosLmLocalCompactingOp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalCompactingOp::CPosLmLocalCompactingOp(
    CPosLmLocalDatabase& aDb) :
    CPosLmLocalOperation(aDb),
    iStatusFlag(KPosLmOperationNotComplete)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalCompactingOp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompactingOp::ConstructL()
    {
    BaseConstructL();

    AquireLockL(CPosLmLocalDatabase::EWriteLock);

    iCompactDb = CPosLmCompactDatabase::NewL(*iDb->DatabaseAccess());
    }

// -----------------------------------------------------------------------------
// CPosLmLocalCompactingOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalCompactingOp* CPosLmLocalCompactingOp::NewL(
    CPosLmLocalDatabase& aDb)
    {
    CPosLmLocalCompactingOp* self =
        new(ELeave) CPosLmLocalCompactingOp(aDb);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalCompactingOp::~CPosLmLocalCompactingOp()
    {
    delete iCompactDb;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalCompactingOp::NextStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalCompactingOp::NextStepL(
    TReal32& aProgress)
    {
    if (iCompactDb->NextL(aProgress))
        {
        ReleaseLock();

        iStatusFlag = KErrNone;
        }

    return iStatusFlag;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalCompactingOp::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalCompactingOp::HandleError(
    TInt& aError)
    {
    iStatusFlag = aError;
    delete iCompactDb;
    iCompactDb = NULL;
    ReleaseLock();
    }

//  End of File
