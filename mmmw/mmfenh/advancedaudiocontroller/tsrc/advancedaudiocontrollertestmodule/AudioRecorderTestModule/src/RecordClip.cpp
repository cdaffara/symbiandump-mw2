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
* Description:  AudioRecorder Test
*
*/

#include "RecordClip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordClip::CRecordClip(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	errRange = TInt64(KDefaultErrorRange);
}

CRecordClip::~CRecordClip()
{

}

CRecordingBase* CRecordClip::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordClip::CreateObjectL"));
	return (new (ELeave) CRecordClip(aConsole, aLogger));
}

TInt CRecordClip::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
	return CRecordingBase::RunTestL(aConsole,aLogger,aParser,clipCounter,CRecordClip::CreateObjectL);
}

TInt CRecordClip::PreProcess(CStifSectionParser *aParser)
{
	TInt err(KErrNone);
	if (recorder)
	{
		//Get error range
		CStifItemParser* item = aParser->GetItemLineL(KErrorRange, ENoTag);
		CleanupStack::PushL(item);
		errRange = CRecordingBase::ParseTime(item);
		CleanupStack::PopAndDestroy();
		//logger->Log(_L("Error range: %d"),errRange.Int64());
		LogTime(_L("Error range:"),errRange.Int64());

		//Get recorded audio format
		TPtrC dataFormat(KPPNone);
		item = aParser->GetItemLineL(KDataFormat);
		if (item)
		{
			CleanupStack::PushL(item);
			err = item->GetString(KDataFormat,dataFormat);
			if (err != KErrNone)
			{
				CleanupStack::PopAndDestroy(); // item
				logger->Log(_L("dataFormat syntax error"));
				return err;
			}
			else
			{
				logger->Log(_L("dataFormat: %S"),&dataFormat);
				CleanupStack::PopAndDestroy(); // item
			}
		}

		if (dataFormat == KFormatMp4)
		{
			isMp4 = true;
		}

	}
	return KErrNone;
}

TInt CRecordClip::PostProcess(CStifSectionParser* /*aParser*/)
{
	TTimeIntervalMicroSeconds duration = Duration();
	//logger->Log(_L("Recorded clip duration: %d"),duration);
	//LogTime(_L("Recorded clip duration:"),duration);
	//LogTime(_L("Time to record:"),timeToRecord);
	logger->Log(_L("PostProcess - Get Duration, %d") , duration.Int64());
	logger->Log(_L("PostProcess - time to record, %d"),timeToRecord.Int64());

	if (Abs((timeToRecord.Int64() + existingDuration) - duration.Int64()) > errRange.Int64())
	{
		logger->Log(_L("Recorded file duration differs from expected duration"));
		return KErrOutOfRange;
	}
	return KErrNone;
}

