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
* Description:     CTestPosLmCatNameCriteria  class
*
*/





#include <EPos_CPosLmCatNameCriteria.h>
#include "testposlmcatnamecriteria.h"


// DESCRIPTION
// This module contains implementation of  CTestPosLmCatNameCriteria class  
// class

//
// Two-phase constructor.
//
CTestPosLmCatNameCriteria* CTestPosLmCatNameCriteria::NewLC()
	{
	CTestPosLmCatNameCriteria* nameCriteria=new(ELeave) CTestPosLmCatNameCriteria;
	CleanupStack::PushL(nameCriteria);
	return(nameCriteria);	
	}
	
//
// Constructor.
//
CTestPosLmCatNameCriteria::CTestPosLmCatNameCriteria()
	{
		
	}
	
//
// Destructor.
//
CTestPosLmCatNameCriteria::~CTestPosLmCatNameCriteria()
	{
		
	}
	
//
// This funtion will test NewLC function of CPosLmCatNameCriteria class.
//	
void CTestPosLmCatNameCriteria::TestNewLC()	
	{
	CPosLmCatNameCriteria* nameCriteria=CPosLmCatNameCriteria::NewLC();
	CleanupStack::PopAndDestroy(nameCriteria);		
	}
	
//
// This funtion will test SearchPattern function of CPosLmCatNameCriteria class.
//	
void CTestPosLmCatNameCriteria::TestSearchPatternL()	
	{
	CPosLmCatNameCriteria* nameCriteria=CPosLmCatNameCriteria::NewLC();
	_LIT(KSearchPattern,"Nokia");
	TBufC<20> searchPattern(KSearchPattern);
	nameCriteria->SetSearchPatternL(searchPattern);
	TPtrC searchResult=nameCriteria->SearchPattern();
	if((searchResult.Compare(searchPattern))!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(nameCriteria);	
	}
	
//
// This funtion will test SetSearchPatternL function of CPosLmCatNameCriteria class.
//	
void CTestPosLmCatNameCriteria::TestSetSearchPatternL(TPtrC aSearchPattern)	
	{
	CPosLmCatNameCriteria* nameCriteria=CPosLmCatNameCriteria::NewLC();
	nameCriteria->SetSearchPatternL(aSearchPattern);
	TPtrC searchPattern=nameCriteria->SearchPattern();
	if((searchPattern.Compare(aSearchPattern))!=0)
		{
		User::Leave(KErrGeneral);	
		}
	CleanupStack::PopAndDestroy(nameCriteria);	
	}