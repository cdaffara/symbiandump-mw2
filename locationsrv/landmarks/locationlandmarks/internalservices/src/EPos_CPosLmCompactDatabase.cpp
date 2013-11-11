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
* Description: Class compacting a landmark database in steps.
*
*
*/


// INCLUDE FILES
#include    "EPos_CPosLmCompactDatabase.h"
#include    "EPos_CPosLmLocalDbAccess.h"

// CONSTANTS
const TReal32 KOperationDone = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmCompactDatabase::CPosLmCompactDatabase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmCompactDatabase::CPosLmCompactDatabase() :
    CBase()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmCompactDatabase::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmCompactDatabase::ConstructL(
    CPosLmLocalDbAccess& aDbAccess)
    {
    RDbNamedDatabase db;
    aDbAccess.GetDatabase(db);
    User::LeaveIfError(iDbIncremental.Compact(db, iIncrementalStep));
    iIncrementalStartValue = iIncrementalStep;
    }

// -----------------------------------------------------------------------------
// CPosLmCompactDatabase::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmCompactDatabase* CPosLmCompactDatabase::NewL(
    CPosLmLocalDbAccess& aDbAccess)
    {
    CPosLmCompactDatabase* self = new( ELeave ) CPosLmCompactDatabase();
    CleanupStack::PushL(self);
    self->ConstructL(aDbAccess);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmCompactDatabase::~CPosLmCompactDatabase()
    {
    iDbIncremental.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmCompactDatabase::NextL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPosLmCompactDatabase::NextL(
    TReal32& aProgress)
    {
    User::LeaveIfError(iDbIncremental.Next(iIncrementalStep));

    if (iIncrementalStartValue == 0 || iIncrementalStep == 0)
        {
        aProgress = KOperationDone;
        iDbIncremental.Close();
        return ETrue;
        }

    aProgress = TReal32((iIncrementalStartValue - iIncrementalStep)) /
        iIncrementalStartValue;

    return EFalse;
    }

//  End of File
