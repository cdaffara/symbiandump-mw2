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
* Description: Implementation of Active object that compacts database.
*
*
*/



// INCLUDE FILES
#include    <epos_cposlmcompactdatabase.h>
#include    "EPos_CPosLmActiveCompacter.h"
#include    "EPos_CPosLmLocalAccessSubsession.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmActiveCompacter::CPosLmActiveCompacter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmActiveCompacter::CPosLmActiveCompacter(
    CPosLmLocalAccessSubsession& aOwner) :
    CActive(EPriorityIdle),
    iOwner(aOwner)
    {
    CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CPosLmActiveCompacter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmActiveCompacter::ConstructL(CPosLmLocalDbAccess& aDb)
    {
    iCompactDb = CPosLmCompactDatabase::NewL(aDb);

    CompleteSelf();
    }

// -----------------------------------------------------------------------------
// CPosLmActiveCompacter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmActiveCompacter* CPosLmActiveCompacter::NewL(
    CPosLmLocalAccessSubsession& aOwner,
    CPosLmLocalDbAccess& aDb)
    {
    CPosLmActiveCompacter* self = new (ELeave) CPosLmActiveCompacter(aOwner);

    CleanupStack::PushL( self );
    self->ConstructL(aDb);
    CleanupStack::Pop();

    return self;
    }


// Destructor
CPosLmActiveCompacter::~CPosLmActiveCompacter()
    {
    Cancel();
    delete iCompactDb;
    }

// -----------------------------------------------------------------------------
// CPosLmActiveCompacter::DoCancel
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmActiveCompacter::DoCancel()
    {
    delete iCompactDb;
    iCompactDb = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmActiveCompacter::RunL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmActiveCompacter::RunL()
    {
    TReal32 progress;
    TBool compactDone = iCompactDb->NextL(progress);

    if (compactDone)
        {
        delete iCompactDb;
        iCompactDb = NULL;

        iOwner.ReleaseCompactLock();
        }
    else
        {
        CompleteSelf();
        }
    }

// -----------------------------------------------------------------------------
// CPosLmActiveCompacter::RunError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmActiveCompacter::RunError(TInt /*aError*/)
    {
    iOwner.ReleaseCompactLock();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPosLmActiveCompacter::CompleteSelf
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmActiveCompacter::CompleteSelf()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
    SetActive();
    }

//  End of File
