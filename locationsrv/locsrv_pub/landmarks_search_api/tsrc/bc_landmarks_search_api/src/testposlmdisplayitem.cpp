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
* Description:     CTestPosLmDisplayItem class
*
*/





#include <EPos_CPosLandmark.h>
#include <EPos_CPosLmDisplayItem.h>
#include <EPos_CPosLmNearestCriteria.h>
#include <EPos_CPosLandmarkCategory.h>
#include <LbsPosition.h>
#include "testposlmdisplayitem.h"

// DESCRIPTION
// This module contains implementation of CTestPosLmDisplayItem  
// class

 //
// Two-phased constructor.
//
 CTestPosLmDisplayItem * CTestPosLmDisplayItem ::NewLC()
	 {
	 	CTestPosLmDisplayItem * displayItem=new(ELeave) CTestPosLmDisplayItem ;
	 	CleanupStack::PushL(displayItem);
	 	return(displayItem);
	 }
//
// Constructor
//
CTestPosLmDisplayItem ::CTestPosLmDisplayItem()
	{
		
	}

//
// Destructor
//
CTestPosLmDisplayItem ::~CTestPosLmDisplayItem()
	{
		
	}
	
//
// This funtion will test NewL function of CPosLmDisplayItem class.
// This is tested by passing CPosLandmark pointer to the function. 	 
//	

void CTestPosLmDisplayItem::TestNewL1L(TInt aDatabaseIndex)
	{
	CPosLandmark* landmark=CPosLandmark::NewLC();
	CPosLmDisplayItem* displayItem;
	if(!aDatabaseIndex)
		{
		displayItem=CPosLmDisplayItem::NewL(landmark);	
		}
	else
		{
		displayItem=CPosLmDisplayItem::NewL(landmark,aDatabaseIndex);
		}
	CleanupStack::Pop(landmark);	
	delete displayItem;
	}
	
//
// This funtion will test NewL function of CPosLmDisplayItem class.
// This is tested by passing CPosLandmarkCategory pointer to the function. 	 
//	

void CTestPosLmDisplayItem::TestNewL2L(TInt aDatabaseIndex)
	{
	CPosLandmarkCategory* landmarkCategory=CPosLandmarkCategory::NewLC();
	CPosLmDisplayItem* displayItem;
	if(!aDatabaseIndex)
		{
		displayItem=CPosLmDisplayItem::NewL(landmarkCategory);	
		}
	else
		{
		displayItem=CPosLmDisplayItem::NewL(landmarkCategory,aDatabaseIndex);
		}
	CleanupStack::Pop(landmarkCategory);	
	delete displayItem;
	}	
	
//
// This funtion will test DisplayItemType function of CPosLmDisplayItem class. 	 
//	

void CTestPosLmDisplayItem::TestDisplayItemTypeL(TInt aValue)
	{
	CPosLandmark* landmark=CPosLandmark::NewLC();
	CPosLandmarkCategory* landmarkCategory=CPosLandmarkCategory::NewLC();
	CPosLmDisplayItem* displayItem;
	if(!aValue)
		{
		displayItem=CPosLmDisplayItem::NewL(landmark);	
		if(displayItem->DisplayItemType()!=CPosLmDisplayItem::ELandmarkItem)
			{
			User::Leave(KErrGeneral);
			}
		}
	else
		{
		displayItem=CPosLmDisplayItem::NewL(landmarkCategory);
		if(displayItem->DisplayItemType()!=CPosLmDisplayItem::ECategoryItem)
			{
			User::Leave(KErrGeneral);
			}
		}
	CleanupStack::Pop(landmarkCategory);
	CleanupStack::Pop(landmark);	
	delete displayItem;
	}	
	

//
// This funtion will test DatabaseIndex function of CPosLmDisplayItem class. 	 
//	

void CTestPosLmDisplayItem::TestDatabaseIndexL(TInt aValue)
	{
	CPosLandmark* landmark=CPosLandmark::NewLC();
	CPosLmDisplayItem* displayItem;
	if(!aValue)
		{
		displayItem=CPosLmDisplayItem::NewL(landmark);
		if(displayItem->DatabaseIndex()!=0)
			{
			User::Leave(KErrGeneral);
			}
		}
		else	
		{
		displayItem=CPosLmDisplayItem::NewL(landmark,aValue);
		if(displayItem->DatabaseIndex()!=aValue)
			{
			User::Leave(KErrGeneral);
			}
		}
	CleanupStack::Pop(landmark);	
	delete displayItem;
	}
	
//
// This funtion will test Category function of CPosLmDisplayItem class. 	 
//	

void CTestPosLmDisplayItem::TestCategoryL(TInt aValue)
	{
	CPosLandmarkCategory* landmarkCategory=CPosLandmarkCategory::NewLC();
	CPosLandmark* landmark=CPosLandmark::NewLC();
	CPosLmDisplayItem* displayItem=	CPosLmDisplayItem::NewL(landmarkCategory);
	if(!aValue)
		{
		displayItem->Category();
		}
	else
		{
		displayItem->Landmark();
		}
	CleanupStack::Pop(landmark);
	CleanupStack::Pop(landmarkCategory);
	delete displayItem;
	}
	
	
//
// This funtion will test Landmark function of CPosLmDisplayItem class. 	 
//	

void CTestPosLmDisplayItem::TestLandmarkL(TInt aValue)
	{
	CPosLandmarkCategory* landmarkCategory=CPosLandmarkCategory::NewLC();
	CPosLandmark* landmark=CPosLandmark::NewLC();
	CPosLmDisplayItem* displayItem=	CPosLmDisplayItem::NewL(landmark);
	if(!aValue)
		{
		displayItem->Landmark();
		}
	else
		{
		displayItem->Category();
		}
	CleanupStack::Pop(landmark);
	CleanupStack::Pop(landmarkCategory);
	delete displayItem;
	}
	
//
// This funtion will test GetDistance function of CPosLmDisplayItem class. 	 
//	

void CTestPosLmDisplayItem::TestGetDistanceL()
	{
	CPosLandmarkCategory* landmarkCategory=CPosLandmarkCategory::NewLC();
	CPosLmDisplayItem* displayItem=	CPosLmDisplayItem::NewL(landmarkCategory);
	TReal32 distance;
	TInt error=displayItem->GetDistance(distance);
	if(error!=KErrNotFound)	
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::Pop(landmarkCategory);
	delete displayItem;
	}