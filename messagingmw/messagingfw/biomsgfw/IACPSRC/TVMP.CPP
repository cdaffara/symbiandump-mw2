// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Telephone Voice Mailbox number settings parser
// 
//

#include <msventry.h>

#include "BSP.H"
#include "IACP.H"
#include "TVMP.H"
#include "IACPDEF.H"
#include "IACPERR.H"


//
// Constructor
//
CTelVoiceMailboxParser::CTelVoiceMailboxParser()
	{
	}
//
// Destruction
//
CTelVoiceMailboxParser::~CTelVoiceMailboxParser()
	{
	delete iTel;
	}

//
// Parse/Set data members
//
void CTelVoiceMailboxParser::ParseL(CParsedFieldCollection& aIacpFields)
	{
	TBuf<64>	aFieldValueBuf;
	if ((aFieldValueBuf= aIacpFields.GetFieldValue(SMS_TEL_VOICE_MAILBOX_NUM)) != TPtrC())
		iTel= aFieldValueBuf.AllocL();
	}
//
// Create entry
//
void CTelVoiceMailboxParser::ProcessL(CMsvEntry& /*aEntry*/)
	{
	//TODO...
	}
