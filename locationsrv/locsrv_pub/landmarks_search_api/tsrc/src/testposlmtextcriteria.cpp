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
* Description:  CTestPosLmTextCriteria class
*
*/


#include <EPos_CPosLandmark.h>
#include <EPos_CPosLmTextCriteria.h>
#include <LbsFieldIds.h>
#include "testposlmtextcriteria.h"


// DESCRIPTION
// This module contains implementation of CTestPosLmTextCriteria 
// class
 
 
 //
// Two-phased constructor.
//
 CTestPosLmTextCriteria* CTestPosLmTextCriteria::NewLC()
	 {
 	CTestPosLmTextCriteria* textCriteria=new(ELeave) CTestPosLmTextCriteria;
 	CleanupStack::PushL(textCriteria);
 	return(textCriteria);
	 }
//
// Constructor
//
CTestPosLmTextCriteria::CTestPosLmTextCriteria()
	{
		
	}

//
// Destructor
//
CTestPosLmTextCriteria::~CTestPosLmTextCriteria()
	{
		
	}
	
//
// This funtion will test NewLC function of CPosLmTextCriteria class. 	 
//
void CTestPosLmTextCriteria::TestNewLC()
	{
	CPosLmTextCriteria*	textCriteria=CPosLmTextCriteria::NewLC();
	CleanupStack::PopAndDestroy(textCriteria);
	}


//
// This funtion will test Text function of CPosLmTextCriteria class. 	 
//
void CTestPosLmTextCriteria::TestTextL()
	{
	CPosLmTextCriteria*	textCriteria=CPosLmTextCriteria::NewLC();
	_LIT(KText,"Nokia");
	TBufC<20> text(KText);
	textCriteria->SetTextL(text);
	TPtrC searchText=textCriteria->Text();
	if(searchText.Compare(text.Des())!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	}
	
//
// This funtion will test SetTextL function of CPosLmTextCriteria class. 	 
//
void CTestPosLmTextCriteria::TestSetTextL()
	{
	CPosLmTextCriteria*	textCriteria=CPosLmTextCriteria::NewLC();
	_LIT(KText,"*Nokia");
	TBufC<20> text(KText);
	textCriteria->SetTextL(text);
	TPtrC searchText=textCriteria->Text();
	if(searchText.Compare(text.Des())!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	}	
	
//
// This funtion will test AttributesToSearch function of CPosLmTextCriteria class. 	 
//
void CTestPosLmTextCriteria::TestAttributesToSearchL()
	{
	CPosLmTextCriteria*	textCriteria=CPosLmTextCriteria::NewLC();
	textCriteria->SetAttributesToSearch(CPosLandmark::ENoAttribute );
	if(textCriteria->AttributesToSearch()!=CPosLandmark::ENoAttribute)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	}
	
//
// This funtion will test SetAttributesToSearch function of CPosLmTextCriteria class. 	 
//
void CTestPosLmTextCriteria::TestSetAttributesToSearchL()
	{
	CPosLmTextCriteria*	textCriteria=CPosLmTextCriteria::NewLC();
	textCriteria->SetAttributesToSearch(CPosLandmark::ELandmarkName);
	if(textCriteria->AttributesToSearch()!=CPosLandmark::ELandmarkName)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	}
	
//
// This funtion will test GetPositionFieldsToSearchL function of CPosLmTextCriteria class. 	 
//
void CTestPosLmTextCriteria::TestGetPositionFieldsToSearchL()
	{
	CPosLmTextCriteria*	textCriteria=CPosLmTextCriteria::NewLC();
	RArray<TUint> fieldArray;
	textCriteria->GetPositionFieldsToSearchL(fieldArray);
	if(fieldArray.Count()!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	}
	
//
// This funtion will test SetPositionFieldsToSearchL function of CPosLmTextCriteria class. 	 
//
void CTestPosLmTextCriteria::TestSetPositionFieldsToSearchL()
	{
	CPosLmTextCriteria*	textCriteria=CPosLmTextCriteria::NewLC();
	RArray<TUint> fieldArray1;
	fieldArray1.Insert(EPositionFieldAddressCapabilitiesBegin,0);
	textCriteria->SetPositionFieldsToSearchL(fieldArray1);
	RArray<TUint> fieldArray2;
	textCriteria->GetPositionFieldsToSearchL(fieldArray2);
	if(fieldArray2[0]!=EPositionFieldAddressCapabilitiesBegin)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	}	
	
	
//
// This funtion will test ClearPositionFieldsToSearch function of CPosLmTextCriteria class. 	 
//
void CTestPosLmTextCriteria::TestClearPositionFieldsToSearchL()
	{
	CPosLmTextCriteria*	textCriteria=CPosLmTextCriteria::NewLC();
	RArray<TUint> fieldArray;
	fieldArray.Insert(EPositionFieldAddressCapabilitiesBegin,0);
	fieldArray.Insert(EPositionFieldMagneticHeading,1);
	textCriteria->SetPositionFieldsToSearchL(fieldArray);
	textCriteria->ClearPositionFieldsToSearch();
	textCriteria->GetPositionFieldsToSearchL(fieldArray);
	if(fieldArray.Count()!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(textCriteria);
	}		
	