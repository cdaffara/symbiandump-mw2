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


#include "RecordCropClip.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordCropClip::CRecordCropClip(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
	errRange = TInt64(KDefaultErrorRange);
	CropTest = true;
}

CRecordCropClip::~CRecordCropClip()
{

}

CRecordingBase* CRecordCropClip::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordCropClip::CreateObjectL"));
	return (new (ELeave) CRecordCropClip(aConsole, aLogger));
}

TInt CRecordCropClip::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
	return CRecordingBase::RunTestL(aConsole,aLogger,aParser,clipCounter,CRecordCropClip::CreateObjectL);
}

TInt CRecordCropClip::PreProcess(CStifSectionParser *aParser)
{
	if (recorder)
	{
		//Get error range
		CStifItemParser* item = aParser->GetItemLineL(KErrorRange, ENoTag);
		CleanupStack::PushL(item);
		errRange = CRecordingBase::ParseTime(item);
		LogTime(_L("Error range:"),errRange.Int64());
		CleanupStack::PopAndDestroy();


		//Obtain Crop from parser and execute test, if this parameter is not configured, using defaut value 1
		fromBeging = 1;
		item = aParser->GetItemLineL(KCropFromBegining);
		CleanupStack::PushL(item);
		if (item)
			item->GetInt(KCropFromBegining,fromBeging );

		CleanupStack::PopAndDestroy(); // item

		//Obtain current position from parser
	//	TInt relativePos;
		item = aParser->GetItemLineL(KRelativePosition);
		CleanupStack::PushL(item);
		if (item)
		{
			item->GetInt(KRelativePosition,relativePos );
			logger->Log(_L("The file crop relativePos is: %d"), relativePos);
		//	position = fileDuration.Int64()*relativePos/100.0;
		}

		CleanupStack::PopAndDestroy();
	}
	return KErrNone;
}

TInt CRecordCropClip::PostProcess(CStifSectionParser* /*aParser*/)
{
//	TTimeIntervalMicroSeconds duration = Duration();
	//logger->Log(_L("Recorded clip duration: %d"),duration);
//	LogTime(_L("Recorded clip duration:"),duration);

	if (fromBeging)
	{
		if (Abs(position.Int64() - newFileDuration.Int64()) > errRange.Int64())
		{
			logger->Log(_L("The file duration after crop differs from expected duration"));
			return KErrOutOfRange;
		}
	}
	else
	{
		if (Abs(fileDuration.Int64() - position.Int64() - newFileDuration.Int64()) > errRange.Int64())
		{
			logger->Log(_L("The file duration after crop differs from expected duration"));
			return KErrOutOfRange;
		}
	}
	return KErrNone;
}