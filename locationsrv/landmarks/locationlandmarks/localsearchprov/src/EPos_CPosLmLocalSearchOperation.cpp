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
* Description: Operation to search for landmarks or landmark categories from a
*  local database.
*
*
*/


// INCLUDE FILES
#include    <epos_cposlmlocaldbaccess.h>
#include    <epos_cposlmlocaldatabase.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLmLocalSearchOperation.h"
#include    "EPos_CPosLmLocalSearch.h"
#include    "EPos_CPosLmLocalSearchHandler.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalSearchOperation::CPosLmLocalSearchOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalSearchOperation::CPosLmLocalSearchOperation(
    CPosLmLocalDatabase* aDb,
    CPosLmLocalSearch* aLocalSearch,
    CPosLmLocalSearchHandler* aLocalSearchHandler)
:   CPosLmLocalOperation(*aDb),
    iLocalSearch(aLocalSearch),
    iLocalSearchHandler(aLocalSearchHandler)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchOperation::ConstructL()
    {
    BaseConstructL();

    AquireLockL(CPosLmLocalDatabase::EReadLock);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalSearchOperation* CPosLmLocalSearchOperation::NewL(
    CPosLmLocalDatabase* aDb,
    CPosLmLocalSearch* aLocalSearch,
    CPosLmLocalSearchHandler* aLocalSearchHandler)
    {
    CPosLmLocalSearchOperation* self = new (ELeave) CPosLmLocalSearchOperation(
        aDb, aLocalSearch, aLocalSearchHandler);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalSearchOperation::~CPosLmLocalSearchOperation()
    {
    if (iLocalSearch)
        {
        iLocalSearch->SearchCompleted();
        iLocalSearchHandler->CancelSearch();
        }
    else
        {
        delete iLocalSearchHandler;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchOperation::HandleLocalSearchDeleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchOperation::HandleLocalSearchDeleted()
    {
    iLocalSearch = NULL;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchOperation::NextStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalSearchOperation::NextStepL(
    TReal32& aProgress)
    {
    __ASSERT_DEBUG(iLocalSearchHandler,
        Panic(KPosLandmarksClientPanic, EPosInvalidOperationMode));

    return iLocalSearchHandler->NextStepL(aProgress);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchOperation::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchOperation::HandleError(TInt& /*aError*/)
    {
    iLocalSearchHandler->CancelSearch();
    HandleOperationCompleted();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalSearchOperation::HandleOperationCompleted
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalSearchOperation::HandleOperationCompleted()
    {
    if (iLocalSearch)
        {
        // Give ownership of iLocalSearchHandler to iLocalSearch
        iLocalSearch->SearchCompleted();
        iLocalSearch = NULL;
        iLocalSearchHandler = NULL;
        }

    ReleaseLock();
    }

//  End of File
