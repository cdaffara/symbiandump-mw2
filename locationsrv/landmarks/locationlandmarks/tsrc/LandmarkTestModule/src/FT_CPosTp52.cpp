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
#include "FT_CPosTp52.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLMItemIterator.h>
#include <Xml/XmlParserErrors.h>
#include <LbsPosition.h>

// CONSTANTS

_LIT(KErronousXMLFile, "c:\\system\\test\\TestData\\Tp47ErrounousXMLData.xml");
_LIT(KErronousXMLFileDataMissing1, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataMissingFields1.xml");
_LIT(KErronousXMLFileDataMissing2, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataMissingFields2.xml");
_LIT(KErronousXMLFileDataMissing3, "c:\\system\\test\\TestData\\Tp47ErrounousXMLDataMissingFields3.xml");
_LIT(KUnknownTags, "c:\\system\\test\\TestData\\Tp47UnknownTags.xml");
_LIT(KUnknownTrees, "c:\\system\\test\\TestData\\Tp47UnknownTrees.xml");
_LIT(KNoEndTag1, "c:\\system\\test\\TestData\\Tp47ErrounousXMLNoEndTag1.xml");
_LIT(KNoEndTag2, "c:\\system\\test\\TestData\\Tp47ErrounousXMLNoEndTag2.xml");
_LIT(KXMLFileWithLongCategoryName, "c:\\system\\test\\TestData\\Tp47XMLWithLongCategoryName.xml");
_LIT(KXMLFileWithLongLandmarkName, "c:\\system\\test\\TestData\\Tp47XMLWithLongName.xml");
_LIT(KXMLFileWithLongLatitudeField, "c:\\system\\test\\TestData\\Tp47XMLWithLongLatitudeField.xml");
_LIT(KXMLFileWithEmptyCollection, "c:\\system\\test\\TestData\\Tp47ErronousXMLFileEmptyCollection.xml");
_LIT(KXMLFileWithXMLTagAsDataField1, "c:\\system\\test\\TestData\\Tp47UsingXMLTagAsDataField1.xml");
_LIT(KXMLFileWithXMLTagAsDataField2, "c:\\system\\test\\TestData\\Tp47UsingXMLTagAsDataField2.xml");
_LIT(KXMLFileWithXMLTagAsDataField3, "c:\\system\\test\\TestData\\Tp47UsingXMLTagAsDataField3.xml");
_LIT(KXMLFileWithQuotedXMLTagAsDataField1, "c:\\system\\test\\TestData\\Tp52QuotedXMLTag1.xml");

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CPosTp52::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp52::CloseTest()
    {
    iLog->Log(_L("CloseTest"));

    delete iLandmarkSearch;
    iLandmarkSearch = NULL;


    delete iLandmarkParser;
    iLandmarkParser = NULL;


    delete iDatabase;
    iDatabase=NULL;
    
    
    iLog->Log(_L("CloseTest Done"));
    }

// ---------------------------------------------------------
// CPosTp52::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp52::StartL()
    {
    iLog->Log(_L("1)) *********************"));
    DoTestL(KErronousXMLFile, KLmxMimeType, KErrPosLmUnknownFormat);
    DoTestL(KErronousXMLFile, KGpxMimeType, KErrPosLmUnknownFormat);

    iLog->Log(_L("2)) *********************"));
    DoTestL(KUnknownTags, KLmxMimeType, KErrNone, 1);
    DoTestL(KUnknownTags, KGpxMimeType, KErrNone, 1);

    DoTestL(KUnknownTrees, KLmxMimeType, KErrNone, 1);
    DoTestL(KUnknownTrees, KGpxMimeType, KErrNone, 1);
    
    iLog->Log(_L("3)) *********************"));
    DoTestL(KNoEndTag1, KLmxMimeType, EXmlTagMismatch, 0);
    DoTestL(KNoEndTag1, KGpxMimeType, EXmlTagMismatch, 0);

    iLog->Log(_L("4)) *********************"));
    DoTestL(KNoEndTag2, KLmxMimeType, KErrPosLmUnknownFormat, 0);
    DoTestL(KNoEndTag2, KGpxMimeType, KErrPosLmUnknownFormat, 0);

    // Containing XML tags as data field
    iLog->Log(_L("5)) *********************"));
    DoTestL(KXMLFileWithXMLTagAsDataField1, KLmxMimeType, EXmlTagMismatch, 0);
    DoTestL(KXMLFileWithXMLTagAsDataField1, KGpxMimeType, EXmlTagMismatch, 0);
    
    // Containing XML tags as data field
    iLog->Log(_L("6)) *********************"));
    DoTestL(KXMLFileWithXMLTagAsDataField2, KLmxMimeType, EXmlTagMismatch, 0);
    DoTestL(KXMLFileWithXMLTagAsDataField2, KGpxMimeType, EXmlTagMismatch, 0);

    // Containing XML tags as data field
    iLog->Log(_L("7)) *********************"));
    DoTestL(KXMLFileWithXMLTagAsDataField3, KLmxMimeType, EXmlTagMismatch, 0);
    DoTestL(KXMLFileWithXMLTagAsDataField3, KGpxMimeType, EXmlTagMismatch, 0);

    iLog->Log(_L("8)) *********************"));
    DoTestL(KXMLFileWithEmptyCollection, KLmxMimeType, KErrPosLmUnknownFormat, 0);
    // N/A for GPX

    iLog->Log(_L("9)) *********************"));
    DoTestL(KXMLFileWithLongCategoryName, KLmxMimeType, KErrNone, 1);
    CheckLongCategoryNameL(4);
    DoTestL(KXMLFileWithLongCategoryName, KGpxMimeType, KErrNone, 1);
    CheckLongCategoryNameL(1);

    iLog->Log(_L("10)) *********************"));
    DoTestL(KXMLFileWithLongLandmarkName, KLmxMimeType, KErrNone, 1);
    CheckLongLandmarkNameL();
    DoTestL(KXMLFileWithLongLandmarkName, KGpxMimeType, KErrNone, 1);
    CheckLongLandmarkNameL();

    iLog->Log(_L("11)) *********************"));
    DoTestL(KXMLFileWithLongLatitudeField, KLmxMimeType, KErrNone, 1);
    DoTestL(KXMLFileWithLongLatitudeField, KGpxMimeType, KErrNone, 1);

    iLog->Log(_L("12)) *********************"));
    DoTestL(KErronousXMLFileDataMissing1, KLmxMimeType, KErrPosLmUnknownFormat, 0);
    DoTestL(KErronousXMLFileDataMissing1, KGpxMimeType, KErrPosLmUnknownFormat, 0);

    iLog->Log(_L("13)) *********************"));
    DoTestL(KErronousXMLFileDataMissing2, KLmxMimeType, KErrNone, 1);
    DoTestL(KErronousXMLFileDataMissing2, KGpxMimeType, KErrNone, 1);

    iLog->Log(_L("14)) *********************"));
    DoTestL(KErronousXMLFileDataMissing3, KLmxMimeType, KErrNone, 1);
    DoTestL(KErronousXMLFileDataMissing3, KGpxMimeType, KErrNone, 1);
    
    iLog->Log(_L("15)) *********************"));
    DoTestL(KXMLFileWithQuotedXMLTagAsDataField1, KLmxMimeType, KErrNone, 2);
    DoTestL(KXMLFileWithQuotedXMLTagAsDataField1, KGpxMimeType, KErrNone, 2);

    if (iErrorsFound != KErrNone)
        {
        iLog->Log(_L("Errors were found in Tp52"));
        User::Leave(-1);
        }
	}

// ---------------------------------------------------------
// CPosTp52::DoTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp52::DoTestL(
    const TDesC&  aTestFile, 
    const TDesC8& aMimeType,
    TInt aExpectedErrorCode, 
    TInt aExpNrImportedLandmarks )
    {
    TBuf<150> buf;
    TInt nr=0;
    ResetLandmarksDbL();

    TFileName file( aTestFile );
    if ( aMimeType == KGpxMimeType )
        {
        TInt pos = file.Find( KXmlFileExt );
        file.Replace( pos, KXmlFileExt().Length(), KGpxFileExt );
        }
    
    iLog->Log(_L("FILE (%S)"), &file );
    iLandmarkParser = CPosLandmarkParser::NewL(aMimeType);
    iLandmarkParser->SetInputFileL(file);
    
    TInt errorCode = 0;
    // Test sync
    errorCode = ImportLandmarksL(ETrue, CPosLandmarkDatabase::EIncludeCategories);

    if (errorCode != aExpectedErrorCode)
        {
        buf.Format(_L("ERROR: Errorcode %d , should be %d when importing from "), errorCode, aExpectedErrorCode);
        buf.Append(file);
        iLog->Log(buf);
        iErrorsFound++;
        }
    else
        {
        buf.Format(_L("Correct errorcode %d returned when from "), errorCode);
        buf.Append(file);
        iLog->Log(buf);
        }
    
    delete iLandmarkParser;
    iLandmarkParser = NULL;

    nr = GetNrOfLandmarksL();
    if (nr != aExpNrImportedLandmarks)
        {
        buf.Format(_L("ERROR: Found %d landmarks should found %d"), nr, aExpNrImportedLandmarks);
        iLog->Log(buf);
        iErrorsFound++;
        }
    
    ResetLandmarksDbL();

    iLandmarkParser = CPosLandmarkParser::NewL(aMimeType);
    iLandmarkParser->SetInputFileL(file);

    // Test async
    errorCode = ImportLandmarksL(ETrue, CPosLandmarkDatabase::EIncludeCategories);

    if (errorCode != aExpectedErrorCode)
        {
        buf.Format(_L("ERROR: Errorcode %d , should be %d when importing from "), errorCode, aExpectedErrorCode);
        buf.Append(file);
        iLog->Log(buf);
        iErrorsFound++;
        }
    else
        {
        buf.Format(_L("Correct errorcode %d returned when importing landmarks from file "), errorCode);
        buf.Append(file);
        iLog->Log(buf);
        }
    
    delete iLandmarkParser;
    iLandmarkParser = NULL;

    nr = GetNrOfLandmarksL();
    if (nr != aExpNrImportedLandmarks)
        {
        buf.Format(_L("ERROR: Found %d landmarks should found %d"), nr, aExpNrImportedLandmarks);
        iLog->Log(buf);
        iErrorsFound++;
        }
    }

// ---------------------------------------------------------
// CPosTp52::ImportLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp52::ImportLandmarksL(TBool aSync, TUint aTransferOption)
    {
    iLog->Log(_L("ImportLandmarksL"));

    //TRequestStatus status = KPosLmOperationNotComplete;
    //TReal32 progress = 0;
    TInt err = KErrNone;
    
    CPosLmOperation* op;
    op = iDatabase->ImportLandmarksL(*iLandmarkParser, aTransferOption);

    CleanupStack::PushL(op);
    // Import only a subset of the landmarks in parser
    // iOperation = iDatabase->ImportLandmarksL(*iLandmarkParser, aLandmarkSubSet,  aTransferOption);
    if (aSync)
        {
        // Synchronous
        TRAP(err, op->ExecuteL());
        iOperation = NULL;
        }
    else
        {
        // Asynchronous
        // Progress us checked in test procedure base
        RunAsyncOperationLD(iDatabase->ImportLandmarksL(*iLandmarkParser, aTransferOption));
        iOperation = NULL;
        }
    CleanupStack::PopAndDestroy(op);
    return err;
    }

// ---------------------------------------------------------
// CPosTp52::ResetLandmarksDbL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp52::ResetLandmarksDbL()
    {
    iLog->Log(_L("ResetLandmarksDbL()"));
    
    delete iLandmarkSearch;
    iLandmarkSearch = NULL;

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iDatabase;
    iDatabase=NULL;


    // Use an empty landmarks db in this test
    RemoveDefaultDbL();
    iDatabase = CPosLandmarkDatabase::OpenL();
    if (iDatabase->IsInitializingNeeded())
        {
        ExecuteAndDeleteLD(iDatabase->InitializeL());
        }    
    
    delete iDatabase;
    iDatabase = NULL;
    iDatabase = CPosLandmarkDatabase::OpenL();
    // Do reset so that global categories are included
    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);   
    ExecuteAndDeleteLD(categoryManager->ResetGlobalCategoriesL());
    CleanupStack::PopAndDestroy(categoryManager);
    }

// ---------------------------------------------------------
// CPosTp52::CheckLongLandmarkNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp52::CheckLongLandmarkNameL()
    {
    iLog->Log(_L("CheckLandmarkL"));
    // Should only exist one landmark in db
    CPosLandmark* lm = iDatabase->ReadLandmarkLC(1);

    TPtrC lmname;
    TInt res = lm->GetLandmarkName(lmname);

    if (res != KErrNone) //LogErrorAndLeave(_L("Problem getting Landmark"));
    {
        iLog->Log(_L("Problem getting Landmark"));
        User::Leave(-1);

    }

    _LIT(KLONGNAME, "Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landmark10Landm");
    if (lmname.Compare(KLONGNAME) != 0)
        {
        iLog->Log(_L("Long landmark name does not match"));
        iErrorsFound++;
        }

    CleanupStack::PopAndDestroy(lm);
    }

// ---------------------------------------------------------
// CPosTp52::CheckLongCategoryNameL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp52::CheckLongCategoryNameL(TInt aNumExpectedCategories )
    {
    iLog->Log(_L("CheckLongCategoryNameL"));
    // Should only exist one landmark in db
    CPosLandmark* lm = iDatabase->ReadLandmarkLC(1);

    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);
    lm->GetCategoriesL(array);
    TInt nr = array.Count();
    TPtrC name;

    _LIT(KLONGNAME1, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234");
    _LIT(KLONGNAME2, "Pizzeria10Pizzeria20Pizzeria30Pizzeria40Pizzeria50Pizzeria60Pizzeria70Pizzeria80Pizzeria90Pizzeria10Pizzeria10Pizzeria20Pizz");
    _LIT(KLONGNAME3, "Gizzeria10Gizzeria20Gizzeria30Gizzeria40Gizzeria50Gizzeria60Gizzeria70Gizzeria80Gizzeria90Gizzeria10Gizzeria10Gizzeria20_ABC");
    _LIT(KLONGNAME4, "Dizzeria10Dizzeria20Dizzeria30Dizzeria40Dizzeria50Dizzeria60Dizzeria70Dizzeria80Dizzeria90Dizzeria10Dizzeria10Dizzeria20_ABC");

    const TInt numNames = 4;
    const TPtrC names[] = { KLONGNAME1(), KLONGNAME2(), KLONGNAME3(), KLONGNAME4() };
    
    if (nr != aNumExpectedCategories) //LogErrorAndLeave(_L("Wrong number of categories for landmark"));
    {
        iLog->Log(_L("Wrong number of categories for landmark"));
        User::Leave(-1);
    }

    CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
    CleanupStack::PushL(categoryManager);
    
    for ( TInt i = 0; i < aNumExpectedCategories; i++ )
        {
        CPosLandmarkCategory* cat = categoryManager->ReadCategoryLC(array[i]);

        cat->GetCategoryName(name);
        // Maximum size for category name is 124 characters
        if (name.Length() != 124) 
        {
            iLog->Log(_L("Category name has wrong size, should be 124"));
            User::Leave(-1);

        }
        if (name.Compare(names[i]) != 0) 
            {
            iLog->Log(_L("Long category name does not match"));
            iErrorsFound++;
            }
        CleanupStack::PopAndDestroy(cat);
        }

    CleanupStack::PopAndDestroy(categoryManager);

    CleanupStack::PopAndDestroy(&array);
    CleanupStack::PopAndDestroy(lm);
    }

// ---------------------------------------------------------
// CPosTp52::GetNrOfLandmarksL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CPosTp52::GetNrOfLandmarksL()
    {
    TInt nrOfLandmarks = 0;
    CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
    // Search pattern "*" should match all existing landmarks
    textCriteria->SetTextL(_L("*"));

    CPosLandmarkSearch* landmarkSearch = CPosLandmarkSearch::NewL(*iDatabase);
    CleanupStack::PushL(landmarkSearch);

    TPosLmSortPref sortAsc(CPosLandmark::ELandmarkName, TPosLmSortPref::EAscending);
    ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(
                *textCriteria, 
                sortAsc,
                EFalse));

    CPosLmItemIterator* iter = landmarkSearch->MatchIteratorL();
    CleanupStack::PushL(iter);

    nrOfLandmarks = iter->NumOfItemsL();
    CleanupStack::PopAndDestroy(iter);
    CleanupStack::PopAndDestroy(landmarkSearch);
    CleanupStack::PopAndDestroy(textCriteria);
    return nrOfLandmarks;
    }

//  End of File
