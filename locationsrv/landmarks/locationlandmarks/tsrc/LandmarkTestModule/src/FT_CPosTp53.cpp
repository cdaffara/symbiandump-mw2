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
#include "FT_CPosTp53.h"

#include <EPos_CPosLandmarkParser.h>

// CONSTANTS
// const TInt KMaxQueueSize = 5;
const TReal32 KCompactDbLevel = 0.75; // From ...landmarks\inernalServeices\resources\eposlmcompactdblevel.rss

#ifdef __WINS__
    _LIT(KImportFile,           "z:\\system\\test\\testdata\\TP53-CategoriesNotInDb.xml");
    _LIT(KImportFile2,          "z:\\system\\test\\testdata\\TP53-OneLandmark.xml");
    _LIT(KImportFile3,          "z:\\system\\test\\testdata\\TP53-Categories.xml");
    _LIT(KImportFile4,          "z:\\system\\test\\testdata\\TP53-Category.xml");
    _LIT(KImportFile5,          "z:\\system\\test\\testdata\\TP53-GlobalCategory.xml");
    _LIT(KImportFile6,          "z:\\system\\test\\testdata\\TP53-LandmarksOneCategory.xml");
    _LIT(KImportFile7,          "z:\\system\\test\\testdata\\TP53-LandmarkCategories.xml");
    _LIT(KImportFile8,          "z:\\system\\test\\testdata\\TP53-LandmarksCategories.xml");

    _LIT(KCompactImportFile,    "z:\\system\\test\\testdata\\TP53-Compact.xml");
#else                   
    _LIT(KImportFile,           "c:\\system\\test\\testdata\\TP53-CategoriesNotInDb.xml");
    _LIT(KImportFile2,          "c:\\system\\test\\testdata\\TP53-OneLandmark.xml");
    _LIT(KImportFile3,          "c:\\system\\test\\testdata\\TP53-Categories.xml");
    _LIT(KImportFile4,          "c:\\system\\test\\testdata\\TP53-Category.xml");
    _LIT(KImportFile5,          "c:\\system\\test\\testdata\\TP53-GlobalCategory.xml");
    _LIT(KImportFile6,          "c:\\system\\test\\testdata\\TP53-LandmarksOneCategory.xml");
    _LIT(KImportFile7,          "c:\\system\\test\\testdata\\TP53-LandmarkCategories.xml");
    _LIT(KImportFile8,          "c:\\system\\test\\testdata\\TP53-LandmarksCategories.xml");

    _LIT(KCompactImportFile,    "c:\\system\\test\\testdata\\TP53-Compact.xml");
#endif

_LIT(KCategoryName,     "TP53Category1");
_LIT(KCategoryName2,    "TP53Category2");
_LIT(KCategoryName3,    "TP53Category3");

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// CPosTp53::GetName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp53::GetName(TDes& aName) const
    {
    _LIT(KTestName, "Tp53 - Events and compact during import");
    aName = KTestName;
    }

// ---------------------------------------------------------
// CPosTp53::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp53::CloseTest()
    {
	delete iDatabase;
	iDatabase = NULL;
    
    delete iActiveLmEvent;
    iActiveLmEvent = NULL;

    delete iParser;
    iParser = NULL;
    
    RemoveGlobalCategoriesL(); 
    }

// ---------------------------------------------------------
// CPosTp53::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp53::StartL()
    {
    RemoveDefaultDbL();
    RemoveGlobalCategoriesL(); 
    
    iDatabase = UseGlobalCategoriesL();
    
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }
    ExecuteAndDeleteLD(iDatabase->CompactL());

    AddCategoriesL();

    iActiveLmEvent = CActiveLmImportEvent::NewL(iLog);
	
    iLog->Put(_L("Test events during import"));
    ExecuteAndDeleteLD(iDatabase->CompactL());
    LandmarkImportEventTestL();
 
    iLog->Put(_L("Test compact during import"));
    ExecuteAndDeleteLD(iDatabase->CompactL());
    LandmarkImportCompactTestL();
    }

// ---------------------------------------------------------
// CPosTp53::LandmarkImportEventTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp53::LandmarkImportEventTestL()
    { 
    iActiveLmEvent->Start();
    
    iParser = CPosLandmarkParser::NewL(KMimeType);
   
    /////////////////////////////////////////////
	// Landmark created event #1 
	/////////////////////////////////////////////
    ImportLandmarksL(KImportFile, CPosLandmarkDatabase::EDefaultOptions);
    
    /////////////////////////////////////////////
	// Landmark created event #2
	/////////////////////////////////////////////
    ImportLandmarksL(KImportFile2, CPosLandmarkDatabase::EIncludeCategories);
    
    /////////////////////////////////////////////
	// Landmark created event #3
	/////////////////////////////////////////////
    ImportLandmarksL(KImportFile3, CPosLandmarkDatabase::EIncludeCategories);
    
    /////////////////////////////////////////////
    // Landmark created event #4 and category created event #5
    /////////////////////////////////////////////
    ImportLandmarksL(KImportFile4, CPosLandmarkDatabase::EIncludeCategories);
    
    /////////////////////////////////////////////
    // Landmark created event #6 and category updated event #7
    /////////////////////////////////////////////
    ImportLandmarksL(KImportFile5, CPosLandmarkDatabase::EIncludeCategories | CPosLandmarkDatabase::EIncludeGlobalCategoryNames);

    /////////////////////////////////////////////
    // Unknown landmark changes event #8 and category created event #9
    /////////////////////////////////////////////
    ImportLandmarksL(KImportFile6, CPosLandmarkDatabase::EIncludeCategories);

    /////////////////////////////////////////////
    // Landmark created event #10 and Unknown category changes event #11
    /////////////////////////////////////////////
    ImportLandmarksL(KImportFile7, CPosLandmarkDatabase::EIncludeCategories);
  
    /////////////////////////////////////////////
    // Unknown changes event #12
    /////////////////////////////////////////////
    ImportLandmarksL(KImportFile8, CPosLandmarkDatabase::EIncludeCategories);


    if (iActiveLmEvent->iGlobalErr != 0)
        {
        User::Leave(KErrGeneral);
        }
    }

// ---------------------------------------------------------
// CPosTp53::ImportLandmarksL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp53::ImportLandmarksL(const TDesC& aFileName, CPosLandmarkDatabase::TTransferOptions aTransferOptions)
    {
    iParser->SetInputFileL(aFileName);
    ExecuteAndDeleteLD(iDatabase->ImportLandmarksL(*iParser, aTransferOptions));
    CActiveScheduler::Start();
    }

// ---------------------------------------------------------
// CPosTp53::AddCategoriesL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp53::AddCategoriesL()
    {
    CPosLmCategoryManager* cm = CPosLmCategoryManager::NewL(*iDatabase); 
    CleanupStack::PushL(cm);

    CPosLandmarkCategory* category = CPosLandmarkCategory::NewLC();
    category->SetCategoryNameL(KCategoryName); 

    CPosLandmarkCategory* category2 = CPosLandmarkCategory::NewLC();
    category2->SetCategoryNameL(KCategoryName2);

    CPosLandmarkCategory* category3 = CPosLandmarkCategory::NewLC();
    category3->SetCategoryNameL(KCategoryName3);

    cm->AddCategoryL(*category);
    cm->AddCategoryL(*category2);
    cm->AddCategoryL(*category3);

    CleanupStack::PopAndDestroy(4, cm);
    }

// ---------------------------------------------------------
// CPosTp53::LandmarkImportCompactTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp53::LandmarkImportCompactTestL()
    {
    CPosLandmarkParser* parser = CPosLandmarkParser::NewL(KMimeType);
    CleanupStack::PushL(parser);
    
    User::After(200000);
    TReal32 usage = iDatabase->SizeL().iUsage;
    TInt count=0;
 
    while (usage > KCompactDbLevel)
        {
        TBuf<100> name;
        name.Format(_L("LMTP53%d"), ++count);

        CPosLandmark* landmark = CPosLandmark::NewLC();
        landmark->SetLandmarkNameL(name);
        iDatabase->AddLandmarkL(*landmark);
        User::After(200000);
        
        CleanupStack::PopAndDestroy(landmark);

        usage = iDatabase->SizeL().iUsage;
        }
    
    parser->SetInputFileL(KCompactImportFile);

    iLog->Put(_L("importing..."));
    ExecuteAndDeleteLD(iDatabase->ImportLandmarksL(*parser, CPosLandmarkDatabase::EDefaultOptions));
    iLog->Put(_L("importing...Done"));
    User::After(200000);
    
    usage = iDatabase->SizeL().iUsage;
    
    AssertTrueSecL(usage >= KCompactDbLevel, _L("Import operation haven't done compact on the database"));    

    CleanupStack::PopAndDestroy(parser);
    }

// -----------------------------------------------------------------------------
// CActiveLmImportEvent::NewL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
CActiveLmImportEvent* CActiveLmImportEvent::NewL(MUtfwLog* aLog)
    {
    CActiveLmImportEvent* self = new(ELeave) CActiveLmImportEvent(aLog);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CActiveLmImportEvent::ConstructL
//
//(other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CActiveLmImportEvent::ConstructL()
    {
    iDatabase = CPosLandmarkDatabase::OpenL();
    }

// -----------------------------------------------------------------------------
// CActiveLmImportEvent::CActiveLmImportEvent
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CActiveLmImportEvent::CActiveLmImportEvent(MUtfwLog* aLog) : 
    CActive(EPriorityNormal),
    iLog(aLog)
    {   
    CActiveScheduler::Add(this);
    }

// Destructor
CActiveLmImportEvent::~CActiveLmImportEvent()
    {
    Cancel();
    delete iDatabase;
    iDatabase = NULL;
    }

// ---------------------------------------------------------
// CActiveLmImportEvent::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmImportEvent::Start()                          
    {
    iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
 
    SetActive();
    }   

// ---------------------------------------------------------
// CActiveLmImportEvent::RunL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmImportEvent::RunL()
    {  
    VerifyEvent();
    
    iDatabase->NotifyDatabaseEvent(iEvent, iStatus); 
    SetActive();
    }

// ---------------------------------------------------------
// CActiveLmOperation::DoCancel
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmImportEvent::DoCancel()
    {
    iDatabase->CancelNotifyDatabaseEvent();
    }

// ---------------------------------------------------------
// CActiveLmImportEvent::VerifyEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmImportEvent::VerifyEvent()
    { 
    iEventNumber++;
	switch (iEventNumber)
        {			
		case 1:
        case 2:
        case 3:
            AssertCorrectEvent(EPosLmEventLandmarkCreated, iEvent.iEventType, iEventNumber, iEvent.iLandmarkItemId); 
            CActiveScheduler::Stop();
            break;
        case 4:
            AssertCorrectEvent(EPosLmEventLandmarkCreated, iEvent.iEventType, iEventNumber, iEvent.iLandmarkItemId); 
            break;
        case 5:
            AssertCorrectEvent(EPosLmEventCategoryCreated, iEvent.iEventType, 14, iEvent.iLandmarkItemId); 
            CActiveScheduler::Stop();
            break;
        case 6:
            AssertCorrectEvent(EPosLmEventLandmarkCreated, iEvent.iEventType, 5, iEvent.iLandmarkItemId); 
            break;
        case 7:
            AssertCorrectEvent(EPosLmEventCategoryUpdated, iEvent.iEventType, 2, iEvent.iLandmarkItemId); 
            CActiveScheduler::Stop();
            break;
        case 8:
            AssertCorrectEvent(EPosLmEventCategoryCreated, iEvent.iEventType, 15, iEvent.iLandmarkItemId); 
            break;
        case 9:
            // Two landmarks added
            AssertCorrectEvent(EPosLmEventLandmarkUnknownChanges, iEvent.iEventType, 0, iEvent.iLandmarkItemId); 
            CActiveScheduler::Stop();
            break;
        case 10:
            AssertCorrectEvent(EPosLmEventLandmarkCreated, iEvent.iEventType, 8, iEvent.iLandmarkItemId); 
            break;
        case 11:
            AssertCorrectEvent(EPosLmEventCategoryUnknownChanges, iEvent.iEventType, 0, iEvent.iLandmarkItemId); 
            CActiveScheduler::Stop();
            break;
        case 12:
            AssertCorrectEvent(EPosLmEventUnknownChanges, iEvent.iEventType, 0, iEvent.iLandmarkItemId); 
            CActiveScheduler::Stop();
            break;
		default: 
            iLog->Put(_L("Unexpected event received"));
            ++iGlobalErr;
            CActiveScheduler::Stop();
            break;
        }	
    }
// ---------------------------------------------------------
// CActiveLmImportEvent::AssertCorrectEvent
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CActiveLmImportEvent::AssertCorrectEvent(
	TPosLmEventType  aExpectedEventType,
	TPosLmEventType  aEventType,
	TPosLmItemId aExpectedLandMarkItemId,
	TPosLmItemId aLandMarkItemId)
	{
	if (aExpectedEventType != aEventType)
		{
		_LIT(KError,"Unexpected eventtype. Excpected %d got %d\r\n");
		TBuf<200> error;
		error.Format(KError, aExpectedEventType, aEventType);
		iLog->Put(error);
		iGlobalErr++;
		}
    else
        {
        TBuf<50> debug;
		debug.Format(_L("Received event %d"), aEventType);
		iLog->Put(debug);
        }

	if (aExpectedLandMarkItemId != aLandMarkItemId)
		{
		_LIT(KError,"Unexpected item id. Excpected %d got %d\r\n");
		TBuf<200> error;
		error.Format(KError, aExpectedLandMarkItemId, aLandMarkItemId);
		iLog->Put(error);
		iGlobalErr++;
		}
    else
        {
        TBuf<50> debug;
		debug.Format(_L("Received item id %d"), aLandMarkItemId);
		iLog->Put(debug);
        }
	}

// End of file
