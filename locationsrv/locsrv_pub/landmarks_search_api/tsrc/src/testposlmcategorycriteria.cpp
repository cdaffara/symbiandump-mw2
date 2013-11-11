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
* Description:  CTestPosLmCategoryCriteria class
*
*/


#include <EPos_CPosLmCategoryCriteria.h>
#include "testposlmcategorycriteria.h"


// DESCRIPTION
// This module contains implementation of  CTestPosLmCategoryCriteria class  
// class


 //
// Two-phase constructor.
//
CTestPosLmCategoryCriteria*  CTestPosLmCategoryCriteria::NewLC()
	 {
 	 CTestPosLmCategoryCriteria* testCategoryCriteria=new(ELeave) CTestPosLmCategoryCriteria;
 	 CleanupStack::PushL(testCategoryCriteria);
 	 return testCategoryCriteria;
	 }
	 
//
// Constructor.
//	 
CTestPosLmCategoryCriteria::CTestPosLmCategoryCriteria()
	{
		
	}
	
//
// Destructor.
//	
CTestPosLmCategoryCriteria::~CTestPosLmCategoryCriteria()
	{
		
	}
	
//
// This funtion will test NewLC function of CPosLmCategoryCriteria class.
//	
	
void CTestPosLmCategoryCriteria::TestNewLC()
	{
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	CleanupStack::PopAndDestroy(categoryCriteria);
	}

//
// This funtion will test CategoryItemId function of CPosLmCategoryCriteria class.
//	
	
void CTestPosLmCategoryCriteria::TestCategoryItemIdL()
	{
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	TPosLmItemId itemId=categoryCriteria->CategoryItemId();
	if(itemId!=KPosLmNullItemId)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(categoryCriteria);
	}
	
//
// This funtion will test GlobalCategory function of CPosLmCategoryCriteria class.
//	
void CTestPosLmCategoryCriteria::TestGlobalCategoryL()
	{
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	TPosLmGlobalCategory gobalCategory=categoryCriteria->GlobalCategory();
	if(gobalCategory!=KPosLmNullGlobalCategory)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(categoryCriteria);
	}

//
// This funtion will test CategoryName function of CPosLmCategoryCriteria class.
//	
void CTestPosLmCategoryCriteria::TestCategoryNameL()
	{
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	TPtrC categoryName=categoryCriteria->CategoryName();
	if(categoryName.Length()!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(categoryCriteria);
	}

	
//
// This funtion will test SetCategoryItemId function of CPosLmCategoryCriteria class.
//	
void CTestPosLmCategoryCriteria::TestSetCategoryItemIdL()
	{
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	categoryCriteria->SetCategoryItemId(10);
	TPosLmItemId itemId=categoryCriteria->CategoryItemId();
	if(itemId!=10)
		{
		User::Leave(KErrGeneral);
		}
		CleanupStack::PopAndDestroy(categoryCriteria);
	}	 
	
//
// This funtion will test SetGlobalCategory function of CPosLmCategoryCriteria class.
//	
void CTestPosLmCategoryCriteria::TestSetGlobalCategoryL()
	{
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	categoryCriteria->SetGlobalCategory(10);
	TPosLmGlobalCategory gobalCategory=categoryCriteria->GlobalCategory();
	if(gobalCategory!=10)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(categoryCriteria);
	}
	
//
// This funtion will test SetCategoryNameL function of CPosLmCategoryCriteria class.
//	
void CTestPosLmCategoryCriteria::TestSetCategoryNameL(TPtrC aCategoryName)
	{
	CPosLmCategoryCriteria* categoryCriteria=CPosLmCategoryCriteria::NewLC();
	categoryCriteria->SetCategoryNameL(aCategoryName);
	TPtrC categoryName=categoryCriteria->CategoryName();
	if(categoryName.Compare(aCategoryName)!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(categoryCriteria);
	}