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
* Description:  CTestPosLmIdListCriteria  class
*
*/


#include <EPos_CPosLmIdListCriteria.h>
#include <EPos_Landmarks.h>
#include "testposlmidlistcriteria.h"

// DESCRIPTION
// This module contains implementation of CTestPosLmIdListCriteria  
// class

 //
// Two-phased constructor.
//
 CTestPosLmIdListCriteria * CTestPosLmIdListCriteria ::NewLC()
	 {
 	CTestPosLmIdListCriteria * listCriteria=new(ELeave) CTestPosLmIdListCriteria ;
 	CleanupStack::PushL(listCriteria);
 	return(listCriteria);
	 }
//
// Constructor
//
CTestPosLmIdListCriteria ::CTestPosLmIdListCriteria()
	{
		
	}

//
// Destructor
//
CTestPosLmIdListCriteria ::~CTestPosLmIdListCriteria()
	{
		
	}
	
//
// This funtion will test NewLC function of CPosLmIdListCriteria class. 	 
//
void CTestPosLmIdListCriteria::TestNewLC()
	{
	CPosLmIdListCriteria* listCriteria=CPosLmIdListCriteria::NewLC();
	CleanupStack::PopAndDestroy(listCriteria);	
	}
	
//
// This funtion will test SetLandmarkIdsL function of CPosLmIdListCriteria class. 	 
//
void CTestPosLmIdListCriteria::TestSetLandmarkIdsL()
	{
	CPosLmIdListCriteria* listCriteria=CPosLmIdListCriteria::NewLC();
	RArray<TPosLmItemId> idArray1;
	idArray1.Insert(10,0);
	idArray1.Insert(20,1);
	listCriteria->SetLandmarkIdsL(idArray1);
	RArray<TPosLmItemId> idArray2;
	listCriteria->GetLandmarkIdsL(idArray2);
	if((idArray1[0]!=idArray2[0])||(idArray1[1]!=idArray2[1]))
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(listCriteria);	
	}
	
//
// This funtion will test GetLandmarkIdsL function of CPosLmIdListCriteria class. 	 
//
void CTestPosLmIdListCriteria::TestGetLandmarkIdsL()
	{
	CPosLmIdListCriteria* listCriteria=CPosLmIdListCriteria::NewLC();
	RArray<TPosLmItemId> idArray;
	idArray.Insert(10,0);
	listCriteria->GetLandmarkIdsL(idArray);
	if(idArray.Count()!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(listCriteria);	
	}	