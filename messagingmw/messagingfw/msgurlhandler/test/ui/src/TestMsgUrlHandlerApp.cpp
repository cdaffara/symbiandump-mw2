// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <eikstart.h>
#include "TestMsgUrlHandlerApp.H"

const TUid KTestUidMsgUrlHandlerApp = { 0X10008ACE };

//             The function is called by the UI framework to ask for the
//             application's UID.

TUid CTestMsgUrlHandlerApplication::AppDllUid() const
	{
	return KTestUidMsgUrlHandlerApp;
	}

//             This function is called by the UI framework at
//             application start-up. It creates an instance of the
//             document class.

CApaDocument* CTestMsgUrlHandlerApplication::CreateDocumentL()
	{
	return CTestMsgUrlHandlerDocument::NewL(*this);
	}



//             The entry point for the application code. It creates
//             an instance of the CApaApplication derived
//             class, CMsgUrlHandlerApplication.
//
EXPORT_C 
CApaApplication* NewApplication()
	{
	return new CTestMsgUrlHandlerApplication;
	}


GLDEF_C TInt E32Main()
    {
	return EikStart::RunApplication(NewApplication);
	}


