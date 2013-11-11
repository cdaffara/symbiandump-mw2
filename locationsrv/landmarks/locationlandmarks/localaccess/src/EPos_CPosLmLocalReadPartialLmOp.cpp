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
* Description: Operation for reading partial landmarks.
*
*
*/


// INCLUDE FILES
#include    <eposlmasyncops.rsg>
#include    <EPos_LandmarksErrors.h>
#include    <epos_cposlmresourcereader.h>
#include    <EPos_PosLmImplExtension.h>
#include    <epos_poslmlandmarkhandler.h>
#include    <epos_cposlmlocaldbaccess.h>
#include    "EPos_CPosLmLocalReadPartialLmOp.h"
#include    "EPos_LocalLandmarks.h"
#include    "epos_cposlmlocaldatabase.h"

//CONSTANTS
const TInt KGranularity = 5;
const TReal32 KOperationDone = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalReadPartialLmOp::CPosLmLocalReadPartialLmOp
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalReadPartialLmOp::CPosLmLocalReadPartialLmOp(
    CPosLmLocalDatabase& aDb) :
    CPosLmLocalOperation(aDb),
    iStatusFlag(KPosLmOperationNotComplete)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalReadPartialLmOp::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalReadPartialLmOp::ConstructL(
    const RArray<TPosLmItemId>& aLandmarkIdArray,
    CPosLmPartialReadParameters* aPartialSettings)
    {
    BaseConstructL();

    if (aLandmarkIdArray.Count() == 0)
        {
        User::Leave(KErrArgument);
        }

    iPartialSettings = CPosLmPartialReadParameters::NewLC();
    CleanupStack::Pop(iPartialSettings);

    iDb->CopyPartialSettingsL(*aPartialSettings, *iPartialSettings);

    for (TInt i = 0; i < aLandmarkIdArray.Count(); i++)
        {
        User::LeaveIfError(iLandmarkIdArray.Append(aLandmarkIdArray[i]));
        }

    iLmArray = new (ELeave) CArrayPtrFlat<CPosLandmark>(KGranularity);

    ReadInfoFromResourceFileL();
    AquireLockL(CPosLmLocalDatabase::EReadLock);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalReadPartialLmOp::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalReadPartialLmOp* CPosLmLocalReadPartialLmOp::NewL(
    CPosLmLocalDatabase& aDb,
    const RArray<TPosLmItemId>& aLandmarkIdArray,
    CPosLmPartialReadParameters* aPartialSettings)
    {
    CPosLmLocalReadPartialLmOp* self = new (ELeave)
        CPosLmLocalReadPartialLmOp(aDb);

    CleanupStack::PushL(self);
    self->ConstructL(aLandmarkIdArray, aPartialSettings);
    CleanupStack::Pop(self);

    return self;
    }

// Destructor
CPosLmLocalReadPartialLmOp::~CPosLmLocalReadPartialLmOp()
    {
    delete iPartialSettings;
    iLandmarkIdArray.Close();
    if (iLmArray)
        {
        iLmArray->ResetAndDestroy();
        delete iLmArray;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalReadPartialLmOp::NextStepL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalReadPartialLmOp::NextStepL(
    TReal32& aProgress)
    {
    for (TInt i = 0; i < iRequestCounter &&
        iCurrent < iLandmarkIdArray.Count(); i++)
        {
        AddLandmarkToArrayL(iCurrent++);
        }

    if (iCurrent == iLandmarkIdArray.Count())
        {
        iStatusFlag = KErrNone;
        aProgress = KOperationDone;
        ReleaseLock();
        }
    else
        {
        aProgress = TReal32(iCurrent)/iLandmarkIdArray.Count();
        }

    return iStatusFlag;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalReadPartialLmOp::HandleError
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalReadPartialLmOp::HandleError(
    TInt& aError)
    {
    iStatusFlag = aError;

    ReleaseLock();
    }

// -----------------------------------------------------------------------------
// CPosLmLocalReadPartialLmOp::GetResultL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CArrayPtr<CPosLandmark>* CPosLmLocalReadPartialLmOp::GetResultL()
    {
    if (!iLmArray || iStatusFlag == KPosLmOperationNotComplete)
        {
        User::Leave(KErrNotFound);
        }

    for (TInt i = iCurrent; i < iLandmarkIdArray.Count(); i++)
        {
        iLmArray->AppendL(NULL);
        }

    CArrayPtr<CPosLandmark>* result = iLmArray;
    iLmArray = NULL;
    return result;
    }

// -----------------------------------------------------------------------------
// CPosLmLocalReadPartialLmOp::AddLandmarkToArrayL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalReadPartialLmOp::AddLandmarkToArrayL(TInt aIndex)
    {
    CPosLandmark* landmark = CPosLandmark::NewLC();
    PosLmImplExtension::SetLandmarkIdL(*landmark, iLandmarkIdArray[aIndex]);

    TRAPD(err, PosLmLandmarkHandler::ReadPartialLandmarkL(
        *iDb->DatabaseAccess(), *landmark, *iPartialSettings));

    if (err == KErrNone)
        {
        iLmArray->AppendL(landmark);
        CleanupStack::Pop(landmark); // Transfer ownership to array
        }
    else if (err == KErrNotFound)
        {
        iLmArray->AppendL(NULL);
        CleanupStack::PopAndDestroy(landmark);
        }
    else
        {
        User::Leave(err);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmLocalReadPartialLmOp::ReadInfoFromResourceFileL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmLocalReadPartialLmOp::ReadInfoFromResourceFileL()
    {
    CPosLmResourceReader* resources =
        CPosLmResourceReader::NewLC(KPosLocalSuboperationResourceFile);

    iRequestCounter = resources->ReadInt32L(
        R_POS_LM_LOCAL_READ_PARTIAL_LMS_OP);

    if (iRequestCounter <= 0)
        {
        Panic(KPosLandmarksClientPanic,
            EPosInvalidValueSpecifiedInResourceFile);
        }

    CleanupStack::PopAndDestroy(resources);
    }

//  End of File
