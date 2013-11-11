/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CTestPosLmMultiDbSearch class
*
*/


#include <badesca.h>
#include <EPos_CPosLmMultiDbSearch.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmark.h>
#include <EPos_Landmarks.h>
#include <EPos_CPosLmCategoryManager.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLmItemIterator.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include "testposlmmultidbsearch.h"

// DESCRIPTION
// This module contains implementation of CTestPosLmMultiDbSearch  
// class

//
// Two-phased constructor.
//
CTestPosLmMultiDbSearch * CTestPosLmMultiDbSearch ::NewLC()
	{
	CTestPosLmMultiDbSearch * multiDbSearch=new(ELeave) CTestPosLmMultiDbSearch;
	CleanupStack::PushL(multiDbSearch);
	multiDbSearch->ConstructL();
	return(multiDbSearch);	
	}
	
//
// Constructor
//
CTestPosLmMultiDbSearch ::CTestPosLmMultiDbSearch ()
	{
		
	}

//
// Destructor
//
CTestPosLmMultiDbSearch ::~CTestPosLmMultiDbSearch ()
	{
	delete iLandmarkDatabase;
	delete iLandmark;
	ReleaseLandmarkResources();
	}
	
//	
//Second phase construction is done here.	
//	
void CTestPosLmMultiDbSearch::ConstructL()
	{
	iLandmarkDatabase=CPosLandmarkDatabase::OpenL();
	iLandmark=CPosLandmark::NewL();		
	}
	
//
// Landmark and landmark database used for testing are created here.
//
void CTestPosLmMultiDbSearch::CreateLandmarkL()
	{	
	_LIT(KLandmarkName,"Bangalore");
	TBufC<40> landmarkName(KLandmarkName);
	iLandmark->SetLandmarkNameL(landmarkName);
	iLandmark->AddCategoryL(1);
	ExecuteAndDeleteLD(iLandmarkDatabase->InitializeL());
	iLandmarkId = iLandmarkDatabase->AddLandmarkL( *iLandmark);
	}	

//
//All landmarks in the database are deleted.
//
void CTestPosLmMultiDbSearch::DeleteLandmarksL()
	{
	ExecuteAndDeleteLD(iLandmarkDatabase->InitializeL());
	ExecuteAndDeleteLD(iLandmarkDatabase->RemoveAllLandmarksL());
	}	
	
	
//
// This funtion will test NewL function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestNewL(TInt aValue)
	{
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
	CPosLmMultiDbSearch* multiDbSearch;
	if(!aValue)
		{
		multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
		}
	else
		{
		_LIT(KDatabaseUri,"file://c:landmarks.ldb");
		TBufC<40> databaseUri(KDatabaseUri);
		databaseList->InsertL(0,databaseUri);
	 	multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
		}
		
	CleanupStack::PopAndDestroy(databaseList);
	delete multiDbSearch;
	}
//
// This funtion will test SetDatabasesToSearchL function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestSetDatabasesToSearchL()
	{
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	_LIT(KDatabaseUri,"file://c:landmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseList->InsertL(0,databaseUri);
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	databaseList->Delete(0);
	multiDbSearch->SetDatabasesToSearchL(*databaseList);
	
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::PopAndDestroy(databaseList);
	}
//
// This funtion will test DatabasesToSearchL function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestDatabasesToSearchL()
	{
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	_LIT(KDatabaseUri1,"file://c:landmarks1.ldb");
	TBufC<40> databaseUri1(KDatabaseUri1);
	databaseList->InsertL(0,databaseUri1);
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
		
	_LIT(KDatabaseUri2,"file://c:landmarks2.ldb");
	TBufC<40> databaseUri2(KDatabaseUri2);
	databaseList->InsertL(1,databaseUri2);
	 	
	multiDbSearch->SetDatabasesToSearchL(*databaseList);
		
		
	CDesCArray* databaseList1 = multiDbSearch->DatabasesToSearchL();
	CleanupStack::PushL( databaseList1 );
		
	TInt pos;
	databaseList1->Find(databaseUri1,pos);
	if(pos!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(databaseList1);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::PopAndDestroy(databaseList);
	}
	
//
// This funtion will test SetMaxNumOfMatches function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestSetMaxNumOfMatchesL(TInt aDefault)
	{
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
	_LIT(KDatabaseUri,"file://c:landmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseList->InsertL(0,databaseUri);
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);	
	if(aDefault)
		{
		multiDbSearch->SetMaxNumOfMatches();
		if(multiDbSearch->MaxNumOfMatches()!=KPosLmMaxNumOfMatchesUnlimited)
			{
			User::Leave(KErrGeneral);
			}
		}
	else
		{
		multiDbSearch->SetMaxNumOfMatches(20);
		if(multiDbSearch->MaxNumOfMatches()!=20)
			{
			User::Leave(KErrGeneral);
			}	
		}
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::PopAndDestroy(databaseList);
	}
	
//
// This funtion will test StartLandmarkSearchL function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestStartLandmarkSearchL1L()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	databaseList->InsertL(0,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria));
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}
	
//
// This funtion will test overloaded StartLandmarkSearchL function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestStartLandmarkSearchL2L()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	databaseList->InsertL(0,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	TPosLmSortPref sortPref(CPosLandmark::ELandmarkName);
		
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria,sortPref));
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}
	
//
// This funtion will test overloaded StartLandmarkSearchL function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestStartLandmarkSearchL3L()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	databaseList->InsertL(0,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria,CPosLmCategoryManager::ECategorySortOrderNone));
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}	
	
//
// This funtion will test StartCategorySearchL function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestStartCategorySearchL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	databaseList->InsertL(0,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmCatNameCriteria* catNameCriteria=CPosLmCatNameCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	catNameCriteria->SetSearchPatternL(name);
		
	ExecuteAndDeleteLD(multiDbSearch->StartCategorySearchL(*catNameCriteria,CPosLmCategoryManager::ECategorySortOrderNone));
		
	CleanupStack::PopAndDestroy(catNameCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}			
//
// This funtion will test NumOfSearchErrors function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestNumOfSearchErrorsL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	_LIT(KDatabaseUri,"file://c:landmarks5.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseList->InsertL(0,databaseUri);
	databaseList->InsertL(1,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria));
	TInt errors=multiDbSearch->NumOfSearchErrors();
	if(errors!=1)
		{
		User::Leave(KErrGeneral);
		}
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}		

//
// This funtion will test GetSearchError function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestGetSearchErrorL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	_LIT(KDatabaseUri,"file://c:landmarks6.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseList->InsertL(0,databaseUri);
	databaseList->InsertL(1,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria));
	CPosLmMultiDbSearch::TSearchError searchError;
	multiDbSearch->GetSearchError(0,searchError);
	if(searchError.iDatabaseIndex!=0)
		{
		User::Leave(KErrGeneral);
		}
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}		

	
//
// This funtion will test TotalNumOfMatches function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestTotalNumOfMatchesL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	databaseList->InsertL(0,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria));
	TInt errors=multiDbSearch->TotalNumOfMatches();
	if(errors!=1)
		{
		User::Leave(KErrGeneral);
		}
		
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}	
	
//
// This funtion will test NumOfMatches function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestNumOfMatchesL(TInt aValue)
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	_LIT(KDatabaseUri,"file://c:landmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseList->InsertL(0,databaseUri);

	databaseList->InsertL(1,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria));
	TInt matches;
	if(!aValue)
		{
		matches=multiDbSearch->NumOfMatches(1);
		if(matches!=1)
			{
			User::Leave(KErrGeneral);
			}
		}
	else
		{
		matches=multiDbSearch->NumOfMatches(4);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}

//
// This funtion will test NumOfDatabasesToSearch function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestNumOfDatabasesToSearchL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	_LIT(KDatabaseUri,"file://c:landmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseList->InsertL(0,databaseUri);

	databaseList->InsertL(1,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
	
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	TInt databases=multiDbSearch->NumOfDatabasesToSearch();
	if(databases!=2)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}

//
// This funtion will test DatabaseUriPtr function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestDatabaseUriPtrL(TInt aValue)
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	_LIT(KDatabaseUri,"file://c:landmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseList->InsertL(0,databaseUri);

	databaseList->InsertL(1,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	if(!aValue)
		{
		if((multiDbSearch->DatabaseUriPtr(0)).Compare(databaseUri))
			{
			User::Leave(KErrGeneral);	
			}
		}
	else
		{
		multiDbSearch->DatabaseUriPtr(4);	
		}
		
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}

//
// This funtion will test MatchIteratorL function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestMatchIteratorL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
		
	databaseList->InsertL(0,*(dynamic_cast<TDesC*>(iLandmarkDatabase->DatabaseUriLC())));

	
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
				
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
		
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria,CPosLmCategoryManager::ECategorySortOrderNone));
	
	CPosLmItemIterator* itemIterator=multiDbSearch->MatchIteratorL(0);
	TInt noOfItems=itemIterator->NumOfItemsL();	
	
	if(noOfItems!=1)
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::Pop();
	CleanupStack::Pop(databaseList);
	}			


//
// This funtion will test SetDisplayData function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch::TestSetDisplayDataL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	
	CPosLandmarkSearch* multiDbSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);
	CleanupStack::PushL(multiDbSearch);
	
	CPosLmDisplayData* displayData=CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
	multiDbSearch->SetDisplayData(*displayData);
		
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
	
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria));
	TInt count=displayData->Count();
	if(count!=1)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(multiDbSearch);
	}
	
//
// This funtion will test UnsetDisplayData function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch::TestUnsetDisplayDataL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	
	CPosLandmarkSearch* multiDbSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);
	CleanupStack::PushL(multiDbSearch);
	
	CPosLmDisplayData* displayData=CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
	multiDbSearch->SetDisplayData(*displayData);
	multiDbSearch->UnsetDisplayData();
		
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
		
	_LIT(KName,"Bangalore");
	TBufC<40> name(KName);
	textCriteria->SetTextL(name);
	
	ExecuteAndDeleteLD(multiDbSearch->StartLandmarkSearchL(*textCriteria));
	TInt count=displayData->Count();
	if(count!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(multiDbSearch);
	}
	
//
// This funtion will test MaxNumOfMatches function of CPosLmMultiDbSearch class. 	 
//	
void CTestPosLmMultiDbSearch ::TestMaxNumOfMatchesL()
	{
	CDesC16ArrayFlat* databaseList = new(ELeave)CDesC16ArrayFlat(10);
	CleanupStack::PushL(databaseList);
	_LIT(KDatabaseUri,"file://c:landmarks.ldb");
	TBufC<40> databaseUri(KDatabaseUri);
	databaseList->InsertL(0,databaseUri);
		
	CPosLmMultiDbSearch* multiDbSearch=CPosLmMultiDbSearch::NewL(*databaseList);
	CleanupStack::PushL(multiDbSearch);	
	
	if(multiDbSearch->MaxNumOfMatches()!=KPosLmMaxNumOfMatchesUnlimited)
		{
		User::Leave(KErrGeneral);
		}

	CleanupStack::PopAndDestroy(multiDbSearch);
	CleanupStack::PopAndDestroy(databaseList);
	}

					