/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CTestPosLmkSearch  class
*
*/

#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmItemIterator.h>
#include "testposlmksearch.h"

// DESCRIPTION
// This module contains implementation of CTestPosLmkSearch 
// class

//
// Two phase constructor.
//
CTestPosLmkSearch* CTestPosLmkSearch::NewLC()
	{
	CTestPosLmkSearch* lmkSearch = new(ELeave) CTestPosLmkSearch;
	CleanupStack::PushL(lmkSearch);
	lmkSearch->ConstructL();
	return lmkSearch;
	}

//
// Constructor.
//
CTestPosLmkSearch::CTestPosLmkSearch()
	{
		
	}


//
// Destructor.
//
CTestPosLmkSearch::~CTestPosLmkSearch()
	{
	delete iLandmarkDatabase;
	delete iLandmark;
	ReleaseLandmarkResources();		
	}

//	
//Second phase construction is done here.	
//	
void CTestPosLmkSearch::ConstructL()
	{
	iLandmarkDatabase=CPosLandmarkDatabase::OpenL();
	iLandmark=CPosLandmark::NewL();		
	}

//
// Landmark and landmark database used for testing are created here.
//
void CTestPosLmkSearch::CreateLandmarkL()
	{	
	_LIT(KLandmarkName,"Bangalore");
	TBufC<40> landmarkName(KLandmarkName);
	iLandmark->SetLandmarkNameL(landmarkName);
	ExecuteAndDeleteLD(iLandmarkDatabase->InitializeL());
	iLandmarkId = iLandmarkDatabase->AddLandmarkL(*iLandmark);
	}

//
//All landmarks in the database are deleted.
//
void CTestPosLmkSearch::DeleteAllLandMarksL()
	{	
	ExecuteAndDeleteLD(iLandmarkDatabase->InitializeL());	
	ExecuteAndDeleteLD(iLandmarkDatabase->RemoveAllLandmarksL());	
	}
	
//
// This funtion will test NewL function of CPosLandmarkSearch class. 	 
//	

void CTestPosLmkSearch::TestNewL()
	{
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);
	delete landmarkSearch;
	
	}

//
// This funtion will test MaxNumOfMatches function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestMaxNumOfMatchesL()
	{
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);	
	TInt numofSearch=landmarkSearch->MaxNumOfMatches();
	if(numofSearch!=-1)
		{
		User::Leave(KErrGeneral);
		}
	delete landmarkSearch;
	}
	
	
//
// This funtion will test SetMaxNumOfMatches function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestSetMaxNumOfMatchesL(TInt aNumOfMatches)
	{
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);	
	landmarkSearch->SetMaxNumOfMatches(aNumOfMatches);
	TInt numofSearch=landmarkSearch->MaxNumOfMatches();
	if(numofSearch!=aNumOfMatches)
		{
		User::Leave(KErrGeneral);
		}
	delete landmarkSearch;	
	}

//
// This funtion will test StartLandmarkSearchL function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestStartLandmarkSearchL(TBool aBool)
	{
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);	
	CleanupStack::PushL(landmarkSearch);
	
	CreateLandmarkL();
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*textCriteria,aBool));
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}
	
//
// This funtion will test StartLandmarkSearchL function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestStartLandmarkSearchL1L()
	{
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);	
	CleanupStack::PushL(landmarkSearch);
		
	CreateLandmarkL();
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();		
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName);
		
	ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*textCriteria,sortPref));
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}	
	
//
// This funtion will test StartLandmarkSearchL function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestStartLandmarkSearchL2L()
	{
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);	
	CleanupStack::PushL(landmarkSearch);
	
	CreateLandmarkL();
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName);
	
	ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*textCriteria,CPosLmCategoryManager::ECategorySortOrderNone));
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}
	
//
// This funtion will test NumOfMatches function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestNumOfMatchesL()
	{
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);	
	CleanupStack::PushL(landmarkSearch);
		
	DeleteAllLandMarksL();
	CreateLandmarkL();
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName);
		
	ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*textCriteria,CPosLmCategoryManager::ECategorySortOrderNone));
	TInt noOfMatches=landmarkSearch->NumOfMatches();
	if(noOfMatches!=1)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}

//
// This funtion will test MatchIteratorL function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestMatchIteratorL()
	{
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);	
	CleanupStack::PushL(landmarkSearch);
	
	DeleteAllLandMarksL();
	CreateLandmarkL();
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName);
	
	ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*textCriteria,CPosLmCategoryManager::ECategorySortOrderNone));
	
	CPosLmItemIterator* itemIterator=landmarkSearch->MatchIteratorL();
	TInt noOfItems=itemIterator->NumOfItemsL();
	if(noOfItems!=1)
		{
		User::Leave(KErrGeneral);
		}
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}


//
// This funtion will test SetDisplayData function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestSetDisplayDataL()
	{
	DeleteAllLandMarksL();
	CreateLandmarkL();
	
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);
	CleanupStack::PushL(landmarkSearch);
	
	CPosLmDisplayData* displayData=CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
	landmarkSearch->SetDisplayData(*displayData);
		
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
	
	ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*textCriteria));
	TInt count=displayData->Count();
	if(count!=1)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}
	
//
// This funtion will test UnsetDisplayData function of CPosLandmarkSearch class. 	 
//	
void CTestPosLmkSearch::TestUnsetDisplayDataL()
	{
	DeleteAllLandMarksL();
	CreateLandmarkL();
	
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);
	CleanupStack::PushL(landmarkSearch);
	
	CPosLmDisplayData* displayData=CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
	landmarkSearch->SetDisplayData(*displayData);
	landmarkSearch->UnsetDisplayData();
		
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
	
	ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*textCriteria));
	TInt count=displayData->Count();
	if(count!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}
	
						