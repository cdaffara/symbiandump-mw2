// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// test mms settings proxy dll
// takes a settings list from caller and dumps to file for comparison
// 
//

#include <ecom/implementationproxy.h>
#include "T_MMSSettingsProxy.h"

// constants
_LIT(KSettingsProxyTestLogFileName, "c:\\MsgLogs\\tmmsproxy.log");


CMMSSettingsProxy* CMMSSettingsProxy::NewL()
	{
	CMMSSettingsProxy* self = new(ELeave) CMMSSettingsProxy();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


CMMSSettingsProxy::~CMMSSettingsProxy()
	{
	iFs.Close();
	}


void CMMSSettingsProxy::SetMMSValuesL(TSglQue<TMMSSettingsPair>& aMMSSettingsList)
	{
	// open log file
	RFile file;
	CleanupClosePushL(file);
	User::LeaveIfError(file.Replace(iFs, KSettingsProxyTestLogFileName, EFileWrite | EFileStream));
	TBuf8<256> log;

	// access settings list
	TSglQueIter<TMMSSettingsPair> listIter(aMMSSettingsList);
	listIter.SetToFirst();
	TMMSSettingsPair* settingsPair;
	
	// scan settings writing each in turn to file - replace dynamic data with static text
	while ((settingsPair = listIter++) != NULL)
		{
		// write setting name
		log.Copy(settingsPair->iName);
		log += _L8(",");
		User::LeaveIfError(file.Write(log));
	
		// write setting value (or static text)
		if (settingsPair->iName == KMMSUrl)
			{
			log.Copy(*REINTERPRET_CAST(TPtrC*, settingsPair->iValue));
			User::LeaveIfError(file.Write(log));
			}
		else if (settingsPair->iName == KCommDbRecordId)
			{
			User::LeaveIfError(file.Write(_L8("ID")));
			}
		else if (settingsPair->iName == KMsvEntry)
			{
			User::LeaveIfError(file.Write(_L8("PTR")));
			}
		else  
			{
			User::LeaveIfError(file.Write(_L8("UNKNOWN")));
			}
		User::LeaveIfError(file.Write(_L8("\r\n")));
		}

	CleanupStack::PopAndDestroy(); // file
	}


CMMSSettingsProxy::CMMSSettingsProxy()
	{
	}


void CMMSSettingsProxy::ConstructL()
	{
	// connect to file server
	User::LeaveIfError(iFs.Connect());
	}


const TImplementationProxy ImplementationTable[] =
	{
	// this is used by ECom to select the correct implementation
	IMPLEMENTATION_PROXY_ENTRY(0x101F9421, CMMSSettingsProxy::NewL)
	};


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}


