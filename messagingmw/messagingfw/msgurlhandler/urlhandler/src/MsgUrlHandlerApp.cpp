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
// MsgUrlHandlerApp.h
// 
//

/**
 @file
 @see	MsgUrlHandlerApp.h
*/
#include <eikstart.h>
#include "MsgUrlHandlerApp.H"

const TUid KUidMsgUrlHandlerApp = { 0x1000A7FC }; 

//             The function is called by the UI framework to ask for the
//             application's UID.

TUid CMsgUrlHandlerApplication::AppDllUid() const
	{
	return KUidMsgUrlHandlerApp;
	}

//             This function is called by the UI framework at
//             application start-up. It creates an instance of the
//             document class.

CApaDocument* CMsgUrlHandlerApplication::CreateDocumentL()
	{
	return CMsgUrlHandlerDocument::NewL(*this);
	}

//             The entry point for the application code. It creates
//             an instance of the CApaApplication derived
//             class, CMsgUrlHandlerApplication.
//

LOCAL_C CApaApplication* NewApplication()
	{
	return new CMsgUrlHandlerApplication;
	}

GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication(NewApplication);
	}


