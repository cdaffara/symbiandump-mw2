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

#include "MsgUrlHandlerApp.H"

//             The constructor of the document class just passes the
//             supplied reference to the constructor initialisation list.
//             The document has no real work to do in this application.
//

CMsgUrlHandlerDocument* CMsgUrlHandlerDocument::NewL(CEikApplication& aApp)
    {
    CMsgUrlHandlerDocument* self = new (ELeave) CMsgUrlHandlerDocument(aApp);
    return self;
    }

CMsgUrlHandlerDocument::CMsgUrlHandlerDocument(CEikApplication& aApp)
		: CEikDocument(aApp)
	{
	}

//             This is called by the UI framework as soon as the 
//             document has been created. It creates an instance
//             of the ApplicationUI. The Application UI class is
//             an instance of a CEikAppUi derived class.
//
CEikAppUi* CMsgUrlHandlerDocument::CreateAppUiL()
	{
	return new (ELeave) CMsgUrlHandlerAppUi;
	}
