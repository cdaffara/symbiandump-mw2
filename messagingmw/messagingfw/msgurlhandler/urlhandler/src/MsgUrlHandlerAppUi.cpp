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
  
#include <inetprottextutils.h>
#include <uriutils.h>
#include <uri8.h>

#include "MsgUrlHandlerApp.H"
#include "MTSCHEME.H"
#include "SMSSCH.H"


_LIT8(KMailtoScheme, "mailto");
_LIT8(KSmsScheme, "sms");


CMsgUrlHandlerAppUi::CMsgUrlHandlerAppUi()
	{
	}

//             The second phase constructor of the application UI class.
// 
void CMsgUrlHandlerAppUi::ConstructL()
    {
	           // BaseConstructL() completes the UI framework's
	           // construction of the App UI.
    BaseConstructL();

	// Create the idle active object
	iIdleConstructor = CIdle::NewL(CActive::EPriorityHigh);
	iIdleConstructor->Start(TCallBack(IdleConstructCallbackL, this));
	}			

CMsgUrlHandlerAppUi::~CMsgUrlHandlerAppUi()
	{
	delete iCommandLineFileName;
	delete iIdleConstructor;
	}

TInt CMsgUrlHandlerAppUi::IdleConstructCallbackL(TAny* aThis)
	{
	CMsgUrlHandlerAppUi *uiThis = STATIC_CAST(CMsgUrlHandlerAppUi*, aThis);
 
	uiThis->IdleConstructL();

	delete uiThis->iIdleConstructor;
	uiThis->iIdleConstructor = NULL;

	return EFalse;	// Never recall this
	}

void CMsgUrlHandlerAppUi::IdleConstructL()
	{
	LoadStartPageL();
	}

TBool CMsgUrlHandlerAppUi::ProcessCommandParametersL(TApaCommand aCommand,TFileName& aDocumentName,const TDesC8& aTail)
//
// get the document opened from the command line
//
	{
	if (aCommand==EApaCommandOpen && aDocumentName.Length())
		{
		if (iCommandLineFileName == NULL)
			iCommandLineFileName = aDocumentName.AllocL();
		}
	return CEikAppUi::ProcessCommandParametersL(aCommand,aDocumentName,aTail);
	}

void CMsgUrlHandlerAppUi::LoadStartPageL()
	{
	if (iCommandLineFileName)
		{
		LaunchMailEditorL(*iCommandLineFileName);
		delete iCommandLineFileName;
		iCommandLineFileName = NULL;
		}
	}

void CMsgUrlHandlerAppUi::LaunchMailEditorL(const TDesC& aUrl)
	{
	// Get the scheme from the url.
	
	CUri8* uri = UriUtils::CreateUriL(aUrl);
	CleanupStack::PushL(uri);
	TPtrC8 scheme = uri->Uri().Extract(EUriScheme);		
		
	InetProtTextUtils::RemoveWhiteSpace(scheme, InetProtTextUtils::ERemoveBoth);

	// Check if scheme is valid and launch the correct mail editor.
	CMsgSchemeHandlerBase* handler = NULL;
	if (scheme.CompareF(KMailtoScheme) == 0)
		handler = CMailtoSchemeHandler::NewL();
	else if (scheme.CompareF(KSmsScheme) == 0)
		handler = CSmsSchemeHandler::NewL();
	else
		User::Leave(KErrNotSupported);

	CleanupStack::PopAndDestroy(uri);
	CleanupStack::PushL(handler);
	TRAPD(ignore, handler->ParseUrlAndSendL(aUrl));
	CleanupStack::PopAndDestroy(handler);

	PrepareToExit();
	Exit();
	}

void CMsgUrlHandlerAppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EEikCmdExit:
		Exit();
		break;
	default:
		break;
		}
	}
