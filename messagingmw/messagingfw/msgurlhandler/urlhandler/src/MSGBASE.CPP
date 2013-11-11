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
// This file contains the implementation for the class defined in MsgBase.h
// 
//

/**
 @file
 @see	MsgBase.h
*/

// System includes
//
#include <txtrich.h>
#include <mturutils.h>
#include <inetprottextutils.h>

// Local includes
//
#include "MSGBASE.H"

// Constants
//
const TInt KColonChar = ':';

CMsgSchemeHandlerBase::CMsgSchemeHandlerBase()
	{
	}

void CMsgSchemeHandlerBase::ConstructL()
	{
	// Create a rich text
	iPfl = CParaFormatLayer::NewL();
	iCfl = CCharFormatLayer::NewL();
	iRtBody = CRichText::NewL(iPfl, iCfl);
	}

CMsgSchemeHandlerBase::~CMsgSchemeHandlerBase()
	{
	delete iRtBody;
	delete iCfl;
	delete iPfl;
	}

void CMsgSchemeHandlerBase::ParseUrlAndSendL(const TDesC& aUrl)
	{
	// Parse the url
	ParseUrlL(aUrl);

	// Launch the editor
	SendL();
	}

void CMsgSchemeHandlerBase::GetSchemeSpecificPartL(const TDesC& aUrl, TPtrC& aSchemePart)
	{
	// Look for scheme separator 
	TInt schemePos = aUrl.Locate(KColonChar);
	if (schemePos == KErrNotFound)
		{
		User::Leave(KErrNotFound);
		}

	// Set the scheme specific part i.e everything after the colon.  
	aSchemePart.Set(aUrl.Mid(schemePos+1));
	InetProtTextUtils::RemoveWhiteSpace(aSchemePart, InetProtTextUtils::ERemoveBoth);
	}

void CMsgSchemeHandlerBase::LaunchEditorL(const TMsvId& aId)
	{
	MturUtils::LaunchEditorAndWaitL(aId);
	}

