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
// Internet Script Settings Parser
// 
//

#include <txtetext.h>	// CEditableText
#include <msventry.h>

#include "BSP.H"
#include "IACP.H"
#include "ISSP.H"
#include "IACPDEF.H"
#include "IACPERR.H"

//
// Constructor
//
CScriptParser::CScriptParser()
	{
	}

//
// Factory fns
//
CScriptParser* CScriptParser::NewLC()
	{
	CScriptParser* self=new (ELeave) CScriptParser();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CScriptParser* CScriptParser::NewL()
	{
	CScriptParser* self=CScriptParser::NewLC();
	CleanupStack::Pop();
	return self;
	}

//
// 2nd stage of construction
//
void CScriptParser::ConstructL()
	{
	iScriptMode=ENewScript;
	iUseLoginScript=EFalse;
	iDbSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	}

//
// Destruction
//
CScriptParser::~CScriptParser()
	{
	delete iScriptName;
	delete iScriptType;
	delete iLoginScript;
	delete iDbSession;
	}

//
// 
//
void CScriptParser::ParseL(CParsedFieldCollection& aIacpFields)
	{
	TPtrC aFieldValueBuf;

	if (aIacpFields.GetFieldValueAndLength(SMS_SCRIPT_NAME, aFieldValueBuf) != 0)
		{
		delete iScriptName;
		iScriptName = NULL;
		iScriptName= aFieldValueBuf.AllocL();
		}

	if (aIacpFields.GetFieldValueAndLength(SMS_SCRIPT_TYPE, aFieldValueBuf) != 0)
		{
		delete iScriptType;
		iScriptType = NULL;
		iScriptType= aFieldValueBuf.AllocL();
		}

	//login Script
	if (aIacpFields.GetFieldValueAndLength(SMS_SCRIPT_DATA, aFieldValueBuf) != 0)
		iScriptMode= ENewScript;
	else if (aIacpFields.GetFieldValueAndLength(SMS_SCRIPT_ADD, aFieldValueBuf) != 0)
		iScriptMode= EAddToScript;
	else
		User::Leave(KIacpMandatoryDataNotSet);
	
	//if function does'nt leave, it means login script has been assigned to 'aFieldValueBuf'
	delete iLoginScript;
	iLoginScript = NULL;
	iLoginScript= aFieldValueBuf.AllocL();
	iUseLoginScript= ETrue;
	}
//
//
//
void CScriptParser::ProcessL(CMsvEntry& aEntry)
	{
	if (iScriptMode== EAddToScript)
		{
		if(aEntry.Entry().MtmData3() == KBIO_MSG_ENTRY_PROCESSED)
			User::Leave(KIacpErrScriptAlreadyAdd);//we cannot append script more than once for the same Bio Msg
		}

		CCDDialOutISPRecord* dialOutISPRecord = static_cast<CCDDialOutISPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdDialOutISPRecord));
		CleanupStack::PushL(dialOutISPRecord);
		dialOutISPRecord->iRecordName.SetMaxLengthL(iScriptName->Length());
		dialOutISPRecord->iRecordName = iScriptName->Des();
		if (dialOutISPRecord->FindL(*iDbSession))
			{
			if (iScriptMode== EAddToScript)
				{
				TPtrC existingScript = TPtrC(dialOutISPRecord->iLoginScript);
				if (existingScript.Length()==0)
					{
					User::Leave(KIacpErrScriptNotFoundInDB);
					}
				HBufC* tempBuf= HBufC::NewLC(existingScript.Length() + iLoginScript->Length());
				tempBuf->Des().Copy(existingScript);
				tempBuf->Des().Append(iLoginScript->Des());
				//Realloc into iLoginScript then wrtie it to DB
				delete iLoginScript; 
				iLoginScript = NULL;
				iLoginScript= tempBuf->AllocL();
				iLoginScript->Des().Copy(*tempBuf);
				CleanupStack::PopAndDestroy(tempBuf);//tempBuf	
				}
				if(iLoginScript)
					{
					ChangeLoginScriptToNewlines();	
					dialOutISPRecord->iUseLoginScript = iUseLoginScript;
					dialOutISPRecord->iLoginScript.SetMaxLengthL(iLoginScript->Length());
					dialOutISPRecord->iLoginScript = *iLoginScript;
					dialOutISPRecord->ModifyL(*iDbSession);				
					}
				else
					{
					User::Leave(KIacpErrScriptNotDefined);	
					}
			}
		else
			{
			User::Leave(KIacpScriptErrISPNotFound);//there are no records in table
			}
		CleanupStack::PopAndDestroy(dialOutISPRecord);
		
	}
//
//
//
void CScriptParser::ChangeLoginScriptToNewlines()
	{
	TPtr ptr(iLoginScript->Des());
	for (TInt cc=ptr.Length()-1; cc>=0; --cc)
		{
		if (ptr[cc]==CEditableText::EParagraphDelimiter)
			ptr[cc]=0x0a;
		}
	}
