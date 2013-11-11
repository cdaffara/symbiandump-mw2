// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// This file contains the implementation for the class defined in 
// TestMsgUrlHandlerApp.h
// 
//

/**
 @file
 @see	TestMsgUrlHandlerApp.h
*/
  
//#include <TextUtils.h>
#include <uri16.h>
#include <apgcli.h>

#include "TestMsgUrlHandlerApp.H"

CTestMsgUrlHandlerAppUi::CTestMsgUrlHandlerAppUi()
	{
	}

//             The second phase constructor of the application UI class.
// 
void CTestMsgUrlHandlerAppUi::ConstructL()
    {
	           // BaseConstructL() completes the UI framework's
	           // construction of the App UI.
    BaseConstructL();
	iAppView = CTestMsgUrlHandlerAppView::NewL(ClientRect());
	}			

CTestMsgUrlHandlerAppUi::~CTestMsgUrlHandlerAppUi()
	{
	delete iAppView;
	}

void CTestMsgUrlHandlerAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EItem0:
		{
		Exit();
		break;
		}
	case EItem1:
		{
		TThreadId thread;
		_LIT(url, "mailto:shahadat@symbian,david@symbian");
		RApaLsSession apaLsSess;
		TDataRecognitionResult result;
		User::LeaveIfError(apaLsSess.Connect());
		User::LeaveIfError(apaLsSess.RecognizeData(url,KNullDesC8, result));
		User::LeaveIfError(apaLsSess.StartDocument(url,result.iDataType,thread));
		apaLsSess.Close();
		break;
		}
	case EItem2:
		{
		TThreadId thread;
		_LIT(url, "sms:+2516136;via=2836482");
		RApaLsSession apaLsSess;
		TDataRecognitionResult result;
		User::LeaveIfError(apaLsSess.Connect());
		User::LeaveIfError(apaLsSess.RecognizeData(url,KNullDesC8, result));
		User::LeaveIfError(apaLsSess.StartDocument(url,result.iDataType,thread));
		apaLsSess.Close();
		break;
		}

	case EEikCmdExit: 
		Exit();
		break;
		}
	}
