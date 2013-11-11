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
* Description:     CTestPosLmNearestCriteria 
*
*/




#include <e32math.h>
#include <LbsPosition.h>
#include <EPos_CPosLmNearestCriteria.h>
#include "testposlmnearestcriteria.h"


// DESCRIPTION
// This module contains implementation of CTestPosLmNearestCriteria 
// class

 //
// Two-phased constructor.
//
 CTestPosLmNearestCriteria* CTestPosLmNearestCriteria::NewLC()
	 {
 	CTestPosLmNearestCriteria* nearestCriteria=new(ELeave) CTestPosLmNearestCriteria;
 	CleanupStack::PushL(nearestCriteria);
 	return(nearestCriteria);
	 }
//
// Constructor
//
CTestPosLmNearestCriteria::CTestPosLmNearestCriteria()
	{
		
	}

//
// Destructor
//
CTestPosLmNearestCriteria::~CTestPosLmNearestCriteria()
	{
		
	}
	
//
// This funtion will test NewLC function of CPosLmNearestCriteria class. 	 
//
void CTestPosLmNearestCriteria::TestNewLC()
	{
	TCoordinate coordinate(31.6,62.3);
	CPosLmNearestCriteria* nearestCriteria=CPosLmNearestCriteria::NewLC(coordinate);
	CleanupStack::PopAndDestroy(nearestCriteria);
	}
	
//
// This funtion will test GetCoordinate function of CPosLmNearestCriteria class. 	 
//
void CTestPosLmNearestCriteria::TestGetCoordinateL()
	{
	TCoordinate coordinate(31.6,62.3);
	CPosLmNearestCriteria* nearestCriteria=CPosLmNearestCriteria::NewLC(coordinate);
	TCoordinate getCoordinate;
	nearestCriteria->GetCoordinate(getCoordinate);
	if((getCoordinate.Latitude()!=31.6)||(getCoordinate.Longitude()!=62.3))
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(nearestCriteria);
	}

//
// This funtion will test SetCoordinate function of CPosLmNearestCriteria class. 	 
//
void CTestPosLmNearestCriteria::TestSetCoordinateL()
	{
	TCoordinate coordinate(31.6,62.3);
	CPosLmNearestCriteria* nearestCriteria=CPosLmNearestCriteria::NewLC(coordinate);
	TCoordinate setCoordinate(54.3,81.2);
	nearestCriteria->SetCoordinate(setCoordinate);
	TCoordinate getCoordinate;
	nearestCriteria->GetCoordinate(getCoordinate);
	if((getCoordinate.Latitude()!=setCoordinate.Latitude())||(getCoordinate.Longitude()!=setCoordinate.Longitude()))
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(nearestCriteria);
	}

//
// This funtion will test UseCoverageRadius function of CPosLmNearestCriteria class. 	 
//
void CTestPosLmNearestCriteria::TestUseCoverageRadiusL(TInt aUseRadius)
	{
	TCoordinate coordinate(31.6,62.3);
	CPosLmNearestCriteria* nearestCriteria;
	if(!aUseRadius)
		{
		nearestCriteria=CPosLmNearestCriteria::NewLC(coordinate);
		if(nearestCriteria->UseCoverageRadius())
			{
			User::Leave(KErrGeneral);
			}
		}
	else
		{
		nearestCriteria=CPosLmNearestCriteria::NewLC(coordinate,ETrue);
		if(!nearestCriteria->UseCoverageRadius())
			{
			User::Leave(KErrGeneral);
			}
		}
	CleanupStack::PopAndDestroy(nearestCriteria);		

	}

//
// This funtion will test SetUseCoverageRadius function of CPosLmNearestCriteria class. 	 
//	
void CTestPosLmNearestCriteria::TestSetUseCoverageRadiusL(TInt aUseRadius)
	{
	TCoordinate coordinate(31.6,62.3);
	CPosLmNearestCriteria* nearestCriteria=CPosLmNearestCriteria::NewLC(coordinate);
	nearestCriteria->SetUseCoverageRadius(aUseRadius);
	if(!aUseRadius)
		{
		if(nearestCriteria->UseCoverageRadius())
			{
			User::Leave(KErrGeneral);
			}
		}
	else
		{
		if(!nearestCriteria->UseCoverageRadius())
			{
			User::Leave(KErrGeneral);
			}
		}
	CleanupStack::PopAndDestroy(nearestCriteria);
	}

//
// This funtion will test MaxDistance function of CPosLmNearestCriteria class. 	 
//	
void CTestPosLmNearestCriteria::TestMaxDistanceL()
	{
	TCoordinate coordinate(31.6,62.3);
	CPosLmNearestCriteria* nearestCriteria=CPosLmNearestCriteria::NewLC(coordinate);
	if(!Math::IsNaN(nearestCriteria->MaxDistance()))
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(nearestCriteria);
	}
	
//
// This funtion will test SetMaxDistance function of CPosLmNearestCriteria class. 	 
//	
void CTestPosLmNearestCriteria::TestSetMaxDistanceL()
	{
	TCoordinate coordinate(31.6,62.3);
	CPosLmNearestCriteria* nearestCriteria=CPosLmNearestCriteria::NewLC(coordinate);
	nearestCriteria->SetMaxDistance(100);
	if(nearestCriteria->MaxDistance()!=100)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(nearestCriteria);
	}	