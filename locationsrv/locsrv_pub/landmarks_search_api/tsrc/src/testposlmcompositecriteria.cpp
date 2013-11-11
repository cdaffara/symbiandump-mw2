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
* Description:  CTestPosLmCompositeCriteria.cpp
*
*/


#include <EPos_CPosLmCompositeCriteria.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmSearchCriteria.h>
#include "testposlmcompositecriteria.h"

// DESCRIPTION
// This module contains implementation of  CTestPosLmCompositeCriteria class  
// class

//
// Two-phased constructor.
//
 CTestPosLmCompositeCriteria* CTestPosLmCompositeCriteria::NewLC()
	 {
	 	CTestPosLmCompositeCriteria* compositeCriteria=new(ELeave) CTestPosLmCompositeCriteria;
	 	CleanupStack::PushL(compositeCriteria);
	 	return(compositeCriteria);
	 }
//
// Constructor
//
CTestPosLmCompositeCriteria::CTestPosLmCompositeCriteria()
	{
		
	}

//
// Destructor
//
CTestPosLmCompositeCriteria::~CTestPosLmCompositeCriteria()
	{
		
	}
//
// This funtion will test NewLC function of CPosLmCompositeCriteria class with ECompositionAND composition type.     
//

void CTestPosLmCompositeCriteria::TestNewLC_AND()
    {
    CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
    CleanupStack::PopAndDestroy(compositeCriteria);
    }
    
    
//
// This funtion will test NewLC function of CPosLmCompositeCriteria class with ECompositionOR composition type.      
//

void CTestPosLmCompositeCriteria::TestNewLC_OR()
    {
    CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionOR);
    CleanupStack::PopAndDestroy(compositeCriteria);
    }
    
	
//
// This funtion will test CompositionType function of CPosLmCompositeCriteria class. 	 
//
 
void CTestPosLmCompositeCriteria::TestCompositionTypeL()
	{
	CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	CPosLmCompositeCriteria::TCompositionType compositeType=compositeCriteria->CompositionType();
	if(compositeType!=CPosLmCompositeCriteria::ECompositionAND)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(compositeCriteria);
	}	
	
//
// This funtion will test SetCompositionType function of CPosLmCompositeCriteria class. 	 
//

void CTestPosLmCompositeCriteria::TestSetCompositionTypeL()
	{
	CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	compositeCriteria->SetCompositionType(CPosLmCompositeCriteria::ECompositionAND);
	CPosLmCompositeCriteria::TCompositionType compositeType=compositeCriteria->CompositionType();
	if(compositeType!=CPosLmCompositeCriteria::ECompositionAND)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(compositeCriteria);
	}	
	
//
// This funtion will test NumOfArguments function of CPosLmCompositeCriteria class. 	 
//

void CTestPosLmCompositeCriteria::TestNumOfArgumentsL()
	{
	CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
	TInt error=compositeCriteria->AddArgument(textCriteria);
	TInt noOfArguements=compositeCriteria->NumOfArguments();
	if(noOfArguements!=1)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::Pop(textCriteria);
	CleanupStack::PopAndDestroy(compositeCriteria);
	}
	
//
// This funtion will test Argument function of CPosLmCompositeCriteria class which return constant pointer. 	 
//

void CTestPosLmCompositeCriteria::TestArgument1L()
	{
	CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
	_LIT(KName,"Nokia");
	TBufC<20> name(KName);
	textCriteria->SetTextL(name);
	TInt error=compositeCriteria->AddArgument(textCriteria);
	const CPosLmCompositeCriteria* constCompositeCriteria=static_cast<const CPosLmCompositeCriteria*>(compositeCriteria);
	const CPosLmTextCriteria* searchCriteria=static_cast<const CPosLmTextCriteria*>(&constCompositeCriteria->Argument(0));
	TPtrC searchName=searchCriteria->Text();
	if(searchName.Compare(name)!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::Pop(textCriteria);
	CleanupStack::PopAndDestroy();//compositeCriteria
	}

//
// This funtion will test Argument function of CPosLmCompositeCriteria class which return pointer 
// which is not constant. 	 
//

void CTestPosLmCompositeCriteria::TestArgument2L()
	{
	CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	CPosLmTextCriteria* textCriteria=CPosLmTextCriteria::NewLC();
	_LIT(KName,"Nokia");
	TBufC<20> name(KName);
	textCriteria->SetTextL(name);
	TInt error=compositeCriteria->AddArgument(textCriteria);
	CPosLmTextCriteria* searchCriteria=static_cast<CPosLmTextCriteria*>(&compositeCriteria->Argument(0));
	TPtrC searchName=searchCriteria->Text();
	if(searchName.Compare(name)!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::Pop(textCriteria);
	CleanupStack::PopAndDestroy(compositeCriteria);
	}

//
// This funtion will test AddArgument function of CPosLmCompositeCriteria class. 	 
//

void CTestPosLmCompositeCriteria::TestAddArgumentL()
	{
	CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	CPosLmTextCriteria* textCriteria1=CPosLmTextCriteria::NewLC();
	TInt error1=compositeCriteria->AddArgument(textCriteria1);
	CPosLmTextCriteria* textCriteria2=CPosLmTextCriteria::NewLC();
	TInt error2=compositeCriteria->AddArgument(textCriteria2);
	TInt noOfArguements=compositeCriteria->NumOfArguments();
	if(noOfArguements!=2)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::Pop(textCriteria2);
	CleanupStack::Pop(textCriteria1);
	CleanupStack::PopAndDestroy(compositeCriteria);
	}

//
// This funtion will test RemoveArgument function of CPosLmCompositeCriteria class. 	 
//

void CTestPosLmCompositeCriteria::TestRemoveArgumentL()
	{
	CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	CPosLmTextCriteria* textCriteria1=CPosLmTextCriteria::NewLC();
	TInt error1=compositeCriteria->AddArgument(textCriteria1);
	CPosLmTextCriteria* textCriteria2=CPosLmTextCriteria::NewLC();
	TInt error2=compositeCriteria->AddArgument(textCriteria2);
	CPosLmTextCriteria* searchCriteria=static_cast<CPosLmTextCriteria*>(compositeCriteria->RemoveArgument(0));
	TInt noOfArguements=compositeCriteria->NumOfArguments();
	if(noOfArguements!=1)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::Pop(textCriteria2);
	CleanupStack::Pop(textCriteria1);
	CleanupStack::PopAndDestroy(compositeCriteria);
	}
	
//
// This funtion will test ClearArguments function of CPosLmCompositeCriteria class. 	 
//

void CTestPosLmCompositeCriteria::TestClearArgumentsL()
	{
	CPosLmCompositeCriteria* compositeCriteria=CPosLmCompositeCriteria::NewLC(CPosLmCompositeCriteria::ECompositionAND);
	CPosLmTextCriteria* textCriteria1=CPosLmTextCriteria::NewLC();
	TInt error1=compositeCriteria->AddArgument(textCriteria1);
	CPosLmTextCriteria* textCriteria2=CPosLmTextCriteria::NewLC();
	TInt error2=compositeCriteria->AddArgument(textCriteria2);
	compositeCriteria->ClearArguments();
	TInt noOfArguements=compositeCriteria->NumOfArguments();
	if(noOfArguements!=0)
		{
		User::Leave(KErrGeneral);
		}
	CleanupStack::Pop(textCriteria2);
	CleanupStack::Pop(textCriteria1);
	CleanupStack::PopAndDestroy(compositeCriteria);
	}
		