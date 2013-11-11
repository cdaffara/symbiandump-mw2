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
* Description:  Implementation of the CTestPosLmAreaCriteria class
*
*/


#include <EPos_CPosLmAreaCriteria.h>
#include "testposlmareacriteria.h"


// DESCRIPTION
// This module contains implementation of CTestPosLmAreaCriteria 
// class

//
// Two-phase constructor.
//

CTestPosLmAreaCriteria* CTestPosLmAreaCriteria::NewLC()
	{
	CTestPosLmAreaCriteria* areaCriteria=new(ELeave)CTestPosLmAreaCriteria;
	CleanupStack::PushL(areaCriteria);
	return(areaCriteria);
	}
	
//
// Constructor
//
CTestPosLmAreaCriteria::CTestPosLmAreaCriteria()
	{
		
	}
	
//
// Destructor
//		
CTestPosLmAreaCriteria::~CTestPosLmAreaCriteria()
	{
		
	}
//
// This funtion will test NewLC function of CPosLmAreaCriteria class.
//	
void CTestPosLmAreaCriteria::TestNewLC(TReal64 aSouthLat,TReal64 aNorthLat,TReal64 aWestLong,TReal64 aEastLong)
	{
	CPosLmAreaCriteria* areaCriteria=CPosLmAreaCriteria::NewLC(aSouthLat,aNorthLat,aWestLong,aEastLong);	
	CleanupStack::PopAndDestroy(areaCriteria);
	}

//
// This funtion will test GetSearchArea function of CPosLmAreaCriteria class.
//	
void CTestPosLmAreaCriteria::TestGetSearchAreaL()
	{
	CPosLmAreaCriteria* areaCriteria=CPosLmAreaCriteria::NewLC(45,60,150,120);
	TReal64 southLat,northLat,westLong,eastLong;
	areaCriteria->GetSearchArea(southLat,northLat,westLong,eastLong);
	if((southLat!=45) ||(northLat!=60)||(westLong!=150)||(eastLong!=120))
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(areaCriteria);	
	}

//
// This funtion will test SetSearchArea function of CPosLmAreaCriteria class.
//	
void CTestPosLmAreaCriteria::TestSetSearchAreaL(TReal64 aSouthLat,TReal64 aNorthLat,TReal64 aWestLong,TReal64 aEastLong)
	{
	CPosLmAreaCriteria* areaCriteria=CPosLmAreaCriteria::NewLC(45,60,150,120);
	areaCriteria->SetSearchArea(aSouthLat,aNorthLat,aWestLong,aEastLong);
	TReal64 southLat,northLat,westLong,eastLong;
	areaCriteria->GetSearchArea(southLat,northLat,westLong,eastLong);
	if((southLat!=aSouthLat) ||(northLat!=aNorthLat)||(westLong!=aWestLong)||(eastLong!=aEastLong))
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(areaCriteria);
	}


	