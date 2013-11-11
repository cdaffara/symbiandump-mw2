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
* Description: 
*   ?description_line
*
*/


//  INCLUDES
#include "FT_CPosTp49.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkParser.h>
#include <Xml/XmlParserErrors.h>

// CONSTANTS
#ifdef __WINS__
_LIT(KFileWithCorrectSyntax, "z:\\system\\test\\testdata\\Tp49ImportInput.xml");
_LIT(KFileWithSyntaxError, "z:\\system\\test\\testdata\\Tp49ImportInputSyntaxError.xml");
#else
_LIT(KFileWithCorrectSyntax, "c:\\system\\test\\testdata\\Tp49ImportInput.xml");
_LIT(KFileWithSyntaxError, "c:\\system\\test\\testdata\\Tp49ImportInputSyntaxError.xml");
#endif
const TInt KNrOfImportedLandmarks = 7;

#ifdef __WINS__
const TInt KNrOfCorrectLandmarks = 3;
#else
const TInt KNrOfCorrectLandmarks = 4;
#endif

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp49::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::GetName(TDes& aName) const
    {
    _LIT(KTestName, "TP49 - Iterate imported landmarks");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp49::InitTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::InitTestL()
    {
    RemoveDefaultDbL();
    RemoveAllLmDatabasesL();

    SetupPreConditionsL();
    }

// ---------------------------------------------------------
// CPosTp49::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::CloseTest()
    {
    // Delete and set base class members to NULL
    delete iOperation;
    iOperation = NULL;
    delete iIterator;
    iIterator = NULL;
    delete iDatabase;
    iDatabase = NULL;

    delete iLandmarkParser;
    iImportedLandmarkIds.Close();
    iRemovedLandmarkIds.Close();
    
    delete iLandmarkNames;
    iLandmarkNames = NULL;

    }

// ---------------------------------------------------------
// CPosTp49::NotifyProgress
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::NotifyProgress(TReal aProgress)
    {
    if (aProgress > iProgressInterruptLevel)
        {
        CActiveScheduler::Stop();
        }
    }

// ---------------------------------------------------------
// CPosTp49::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::StartL()
    {
    // <<<<<<<<<<<<<< 1-2 Import unparsed landmarks >>>>>>>>>>>>>>>>>>>
    RecreateParserL(KFileWithCorrectSyntax);
    ImportAllLandmarksL();
    FetchIteratorL();
    VerifyImportedLandmarksL(KNrOfImportedLandmarks);

    // <<<<<<<<<<<<<< 3-5 Import parsed landmarks >>>>>>>>>>>>>>>>>>>
    RecreateParserL(KFileWithCorrectSyntax);
    ParseLandmarksContentL();
    ImportAllLandmarksL();
    FetchIteratorL();
    VerifyImportedLandmarksL(KNrOfImportedLandmarks);

    // <<<<<<<<<<<<<< 6-8 Remove some imported landmarks >>>>>>>>>>>>>>>>>>>
    RecreateParserL(KFileWithCorrectSyntax);
    ImportAllLandmarksL();
    FetchIteratorL();
    RemoveThreeLandmarksL();
    FetchIteratorL();
    VerifyImportedLandmarksL(KNrOfImportedLandmarks);

    // <<<<<<<<<<<<<< 9-10 Fetch iterator before import is ready >>>>>>>>>>>>>>>>>>>
    iProgressInterruptLevel = 0.5; // Cancel the import operation when half ready.
    TInt nrOfLandmarks = ImportSomeLandmarksL();
    // Cancel import operation, otherwise db will be locked when verifying
    delete iOperation; iOperation = NULL;
    VerifyImportedLandmarksL(nrOfLandmarks);

    // <<<<<<<<<<<<<< 11-12 Fetch iterator before import is started >>>>>>>>>>>>>>>>>>>
    iProgressInterruptLevel = 0; // Cancel the import operation as fast as possible.
    nrOfLandmarks = ImportSomeLandmarksL();
    // Cancel import operation, otherwise db will be locked when verifying
    delete iOperation; iOperation = NULL;
    VerifyImportedLandmarksL(nrOfLandmarks);

    // <<<<<<<<<<<<<< 13-14 Fetch iterator if import fails >>>>>>>>>>>>>>>>>>>
    RecreateParserL(KFileWithSyntaxError);
    StartImportLandmarksL();
    ExecuteLmOpL(EXmlTagMismatch);
    FetchIteratorL();
    CreateNameArrayL(KNrOfCorrectLandmarks);
    // We don't need to cancel operation since import operation failed. Hence 
    // it should release the database lock itself.
    // delete iOperation; iOperation = NULL;
    // Note: It seems that there is a difference between target and emulator in the number of imported 
    // landmarks in each next step, thus it differs in the number of imported lms before the incorrect xml-landmark
    // is found
    VerifyImportedLandmarksL(KNrOfCorrectLandmarks);

    // <<<<<<<<<<<<<< 15 Test leave codes - nothing to do >>>>>>>>>>>>>>>>>>>
    iLog->Put(_L("TP49 Done"));
	}

// ---------------------------------------------------------
// CPosTp49::ImportAllLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::ImportAllLandmarksL()
    {
    // Fetch nr of existing items before import
    CPosLmItemIterator* iterator = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iterator);
    TInt nrOfItems = iterator->NumOfItemsL();
    CleanupStack::PopAndDestroy(iterator);

    StartImportLandmarksL();
    ExecuteLmOpL(KErrNone);
    CreateNameArrayL(KNrOfImportedLandmarks);

    // Check nr of landmarks in db after import
    iterator = iDatabase->LandmarkIteratorL();
    CleanupStack::PushL(iterator);
    _LIT(KErrMsg, "Not all landmarks were imported to database!");
    TInt nrOfImportedItems = iterator->NumOfItemsL() - nrOfItems;
    AssertTrueSecL(
        nrOfImportedItems == KNrOfImportedLandmarks, KErrMsg, KErrGeneral);
    CleanupStack::PopAndDestroy(iterator);
    }

// ---------------------------------------------------------
// CPosTp49::ImportSomeLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp49::ImportSomeLandmarksL()
    {
    RecreateParserL(KFileWithCorrectSyntax);
    StartImportLandmarksL();
    ExecuteLmOpL(KPosLmOperationNotComplete, ETrue);
    FetchIteratorL();
    CreateNameArrayL(iIterator->NumOfItemsL());
    return iIterator->NumOfItemsL();
    }

// ---------------------------------------------------------
// CPosTp49::ParseLandmarksContentL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::ParseLandmarksContentL()
    {
    delete iOperation;
    iOperation = NULL;
    iOperation = iLandmarkParser->ParseContentL();
    ExecuteLmOpL(KErrNone);
    }

// ---------------------------------------------------------
// CPosTp49::RemoveThreeLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::RemoveThreeLandmarksL()
    {
    // It is important that the highest index is removed first.
    TInt indexToRemove[] = {6, 5, 2};

    const TInt KNrOfItemsToRemove = 3;
    _LIT(KErrMsg, "Removed landmark exists in database!");

    for (TInt i = 0; i < KNrOfItemsToRemove; i++)
        {
        User::LeaveIfError(
            iRemovedLandmarkIds.Append(iImportedLandmarkIds[indexToRemove[i]]));
        iLandmarkNames->Delete(indexToRemove[i]);
        iDatabase->RemoveLandmarkL(iImportedLandmarkIds[indexToRemove[i]]);

        // Verify landmark is removed
        CPosLandmark* lm = NULL;
        TRAPD(err, lm = iDatabase->ReadLandmarkLC(iImportedLandmarkIds[indexToRemove[i]]));
        delete lm;
        lm = NULL;
        AssertTrueSecL(err == KErrNotFound, KErrMsg, KErrGeneral);
        }
    }

// ---------------------------------------------------------
// CPosTp49::VerifyImportedLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::VerifyImportedLandmarksL(TInt aNrOfExpectedItems)
    {
    iLog->Put(_L("VerifyImportedLandmarksL"));
    // Iterate through all imported landmarks
    TInt i = 0;
    TPosLmItemId id = iIterator->NextL();
    while (id != KPosLmNullItemId)
        {
        CPosLandmark* lm = NULL;
        if (iRemovedLandmarkIds.Find(id) == KErrNotFound)
            {
            // The landmark has not been removed from database.
            // Try to read the imported landmark.
            lm = iDatabase->ReadLandmarkLC(id);

            // Verify that the landmark name exists in the name array.
            _LIT(KErrMsg, "Unexpected landmark name");
            _LIT(KErrMsg1, "GetLandmarkName returned KErrNotFound");
            TPtrC name;
            TInt res = lm->GetLandmarkName(name);
            if (res == KErrNotFound)
                {
                // No name found at all
                LogErrorAndLeave(KErrMsg1, KErrGeneral);
                }
            else
                {
                TInt index;
                iLog->Put(name);
                res = iLandmarkNames->Find(name, index);
                if (res != KErrNone)
                    {
                    // Found name does not match any name in the array.
                    LogErrorAndLeave(KErrMsg, KErrGeneral);
                    }
                }
            CleanupStack::PopAndDestroy(lm);
            }
        else
            {
            // The landmark has been removed from db. 
            // Verify that landmark does not exist in database.
            _LIT(KErrMsg, "Removed landmark exists in database!");
            TRAPD(err, lm = iDatabase->ReadLandmarkLC(id));
            delete lm;
            lm = NULL;
            AssertTrueSecL(err == KErrNotFound, KErrMsg, KErrGeneral);
            }

        i++;
        id = iIterator->NextL();
        }

    // Verify that length of iterator is equal to the nr of imported landmarks.
    _LIT(KErrMsg, "Nr of iterated landmarks did not match the nr of imported.");
    AssertTrueSecL(i == aNrOfExpectedItems, KErrMsg, KErrGeneral);
    }

// ---------------------------------------------------------
// CPosTp49::SetupPreConditionsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::SetupPreConditionsL()
    {
    // Precondition: A non-empty database exists
    CPosLandmarkDatabase* db = OpenDatabaseLC();
    CleanupStack::Pop(db);
    iDatabase = db;
    
    ExecuteAndDeleteLD(iDatabase->CompactL());

    // Add one item to make it non-empty
    CPosLandmark* dummy = CPosLandmark::NewLC();
    iDatabase->AddLandmarkL(*dummy);
    CleanupStack::PopAndDestroy(dummy);
    }

// ---------------------------------------------------------
// CPosTp49::ExecuteLmOpL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::ExecuteLmOpL(TInt aExpectedResult, TBool aObserveProgress)
    {
    CActiveLmOperation* operationWrapper = 
        new (ELeave) CActiveLmOperation(iLog);
    CleanupStack::PushL(operationWrapper);

    if (aObserveProgress)
        {
        operationWrapper->Start(iOperation, this);
        }
    else 
        {
        operationWrapper->Start(iOperation);
        }

    CActiveScheduler::Start();
    if (aExpectedResult == KErrNone)
        {
        operationWrapper->CheckOperationL();
        }
    else
        {
        if (aExpectedResult != operationWrapper->Status())
            {
            _LIT(KErrMsg, "Expected result %d but got %d");
            TBuf<50> msg;
            msg.Format(KErrMsg, aExpectedResult, operationWrapper->Status());
            LogErrorAndLeave(msg, KErrGeneral);
            }
        }

    CleanupStack::PopAndDestroy(operationWrapper);
    }

// ---------------------------------------------------------
// CPosTp49::FetchIteratorL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::FetchIteratorL()
    {
    delete iIterator;
    iIterator = NULL;
    iIterator = iDatabase->ImportedLandmarksIteratorL(iOperation);
    TBuf<100> buf;
    iImportedLandmarkIds.Close();
    TInt nrOfItems = (TInt) iIterator->NumOfItemsL();
    buf.Format(_L("Retreving %d nr of ids from iterator"), nrOfItems);
    iLog->Put(buf);
    if (nrOfItems > 0)
        {
        iIterator->GetItemIdsL(iImportedLandmarkIds, 0, nrOfItems);
        }
    buf.Zero();    
    for (TInt i=0;i<iImportedLandmarkIds.Count();i++)
        {
        TBuf<15> idde;
        idde.Format(_L(" Id:%d "), iImportedLandmarkIds[i]);
        buf.Append(idde);
        }
    iLog->Put(buf);
    }

// ---------------------------------------------------------
// CPosTp49::RecreateParserL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::RecreateParserL(const TDesC& aFileName)
    {
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    iLandmarkParser = CPosLandmarkParser::NewL(KMimeType);
    iLandmarkParser->SetInputFileL(aFileName);
    }

// ---------------------------------------------------------
// CPosTp49::StartImportLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::StartImportLandmarksL()
    {
    delete iOperation;
    iOperation = NULL;
    iLog->Put(_L("Innan ImportLandmarksL"));
    iOperation = iDatabase->ImportLandmarksL(
        *iLandmarkParser, CPosLandmarkDatabase::EDefaultOptions);
    iLog->Put(_L("EfterImportLandmarksL"));
    }

// ---------------------------------------------------------
// CPosTp49::CreateNameArrayL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp49::CreateNameArrayL(TInt aNrOfItems)
    {
    _LIT(KName0, "Landmark 0");
    _LIT(KName1, "Landmark 1");
    _LIT(KName2, "Landmark 2");
    _LIT(KName3, "Landmark 3");
    _LIT(KName4, "Landmark 4");
    _LIT(KName5, "Landmark 5");
    _LIT(KName6, "Landmark 6");

    const TDesC* names[] = {&KName0, &KName1, &KName2, &KName3, &KName4, 
        &KName5, &KName6};

    // Create array of landmark names. Names match those specified in the 
    // import file.
    delete iLandmarkNames;
    iLandmarkNames = new (ELeave) CDesCArrayFlat(8);
    for (TInt i = 0; i < aNrOfItems; i++)
        {
        iLandmarkNames->AppendL(*names[i]);
        }
    }

//  End of File
