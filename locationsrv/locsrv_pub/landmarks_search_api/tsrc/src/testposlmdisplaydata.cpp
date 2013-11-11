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
* Description:  CTestPosLmDisplayData class
*
*/


#include <EPos_CPosLmDisplayData.h>
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkSearch.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmPartialReadParameters.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_Landmarks.h>
#include "testposlmdisplaydata.h"

// DESCRIPTION
// This module contains implementation of CTestPosLmDisplayData 
// class

//
// Two-phased constructor.
//
CTestPosLmDisplayData* CTestPosLmDisplayData::NewLC()
	{
	CTestPosLmDisplayData* displayData=new(ELeave) CTestPosLmDisplayData;
	CleanupStack::PushL(displayData);
	displayData->ConstructL();
	return(displayData);	
	}
	
//
// Constructor
//
CTestPosLmDisplayData::CTestPosLmDisplayData()
	{
		
	}

//
// Destructor
//
CTestPosLmDisplayData::~CTestPosLmDisplayData()
	{
	delete iLandmarkDatabase;
	delete iLandmark;
	ReleaseLandmarkResources();
	}	

//	
//Second phase construction is done here.	
//	
void CTestPosLmDisplayData::ConstructL()
	{
	iLandmarkDatabase=CPosLandmarkDatabase::OpenL();
	iLandmark=CPosLandmark::NewL();		
	}

//
//All landmarks in the database are deleted.
//
void CTestPosLmDisplayData::DeleteLandmarksL()
	{
	ExecuteAndDeleteLD(iLandmarkDatabase->InitializeL());
	ExecuteAndDeleteLD(iLandmarkDatabase->RemoveAllLandmarksL());
	}
	
	
//
// Landmark and landmark database used for testing are created here.
//
void CTestPosLmDisplayData::CreateLandmarkL()
	{	
	_LIT(KLandmarkName,"Bangalore");
	TBufC<40> landmarkName(KLandmarkName);
	iLandmark->SetLandmarkNameL(landmarkName);
	iLandmark->AddCategoryL(1);
	ExecuteAndDeleteLD(iLandmarkDatabase->InitializeL());
	iLandmarkId = iLandmarkDatabase->AddLandmarkL( *iLandmark);
	}	

		
//
// This funtion will test NewL function of CPosLmDisplayData class. 	 
//
void CTestPosLmDisplayData::TestNewL()
	{
	CPosLmDisplayData* displayData=CPosLmDisplayData::NewL();
	delete displayData;
	}
	
//
// This funtion will test NewItemIndex function of CPosLmDisplayData class. 	 
//
void CTestPosLmDisplayData::TestNewItemIndexL()
	{
	DeleteLandmarksL();
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

	TInt index=displayData->NewItemIndex();
	if(index!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(landmarkSearch);	
	}

//
// This funtion will test Count function of CPosLmDisplayData class. 	 
//
void CTestPosLmDisplayData::TestCountL()
	{
	DeleteLandmarksL();
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
// This funtion will test Reset function of CPosLmDisplayData class. 	 
//
void CTestPosLmDisplayData::TestResetL()
	{
	DeleteLandmarksL();
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
	displayData->Reset();
	count=displayData->Count();
	if(count!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}
	
//
// This funtion will test DisplayItem function of CPosLmDisplayData class. 	 
//
void CTestPosLmDisplayData::TestDisplayItemL(TInt aValue)
	{
	DeleteLandmarksL();
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

	if(!aValue)
		{
		displayData->DisplayItem(0);	
		}
	else
		{
		displayData->DisplayItem(2);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}
	
//
// This funtion will test SetPartialReadParametersL function of CPosLmDisplayData class. 	 
//
void CTestPosLmDisplayData::TestSetPartialReadParametersL(TInt aValue)
	{
	DeleteLandmarksL();
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
		
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	categoryCriteria->SetCategoryItemId(1);
		
	CPosLmPartialReadParameters* partialParameters=CPosLmPartialReadParameters::NewLC();		
	partialParameters->SetRequestedAttributes(CPosLandmark::EPosition);
	displayData->SetPartialReadParametersL(*partialParameters);
	TInt count;
	if(!aValue)
		{
		ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*textCriteria));
		count=displayData->Count();
		if(count!=1)
			{
			User::Leave(KErrGeneral);
			}
		}
		else
		{
		ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*categoryCriteria));
		count=displayData->Count();
		if(count!=1)
			{
			User::Leave(KErrGeneral);
			}
	
		}		
	CleanupStack::PopAndDestroy(partialParameters);
	CleanupStack::PopAndDestroy(categoryCriteria);
	CleanupStack::PopAndDestroy(textCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}
	
//
// This funtion will test UnsetPartialReadParameters function of CPosLmDisplayData class. 	 
//
void CTestPosLmDisplayData::TestUnsetPartialReadParametersL()
	{
	DeleteLandmarksL();
	CreateLandmarkL();
	CPosLandmarkSearch* landmarkSearch=CPosLandmarkSearch::NewL(*iLandmarkDatabase);
	CleanupStack::PushL(landmarkSearch);
	CPosLmDisplayData* displayData=CPosLmDisplayData::NewL();
	CleanupStack::PushL(displayData);
	landmarkSearch->SetDisplayData(*displayData);
		
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	categoryCriteria->SetCategoryItemId(10);
	CPosLmPartialReadParameters* partialParameters=CPosLmPartialReadParameters::NewLC();		
	partialParameters->SetRequestedAttributes(CPosLandmark::EAllAttributes);
	displayData->SetPartialReadParametersL(*partialParameters);
	displayData->UnsetPartialReadParameters();
	ExecuteAndDeleteLD(landmarkSearch->StartLandmarkSearchL(*categoryCriteria));
	TInt count=displayData->Count();
	if(count!=0)
		{
		User::Leave(KErrGeneral);	
		}
	CleanupStack::PopAndDestroy(partialParameters);
	CleanupStack::PopAndDestroy(categoryCriteria);
	CleanupStack::PopAndDestroy(displayData);
	CleanupStack::PopAndDestroy(landmarkSearch);
	}

	