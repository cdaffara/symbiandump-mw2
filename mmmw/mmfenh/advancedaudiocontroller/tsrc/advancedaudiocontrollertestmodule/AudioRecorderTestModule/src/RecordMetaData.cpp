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

#include "RecordMetaData.h"
//#include "mmfcontrollerframeworkbase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordMetaData::CRecordMetaData(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
}

CRecordMetaData::~CRecordMetaData()
{

}

CRecordingBase* CRecordMetaData::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordMetaData::CreateObjectL"));
	return (new (ELeave) CRecordMetaData(aConsole, aLogger));
}

TInt CRecordMetaData::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter)
{
	return CRecordingBase::RunTestL(aConsole,aLogger,aParser,clipCounter,CRecordMetaData::CreateObjectL);
}

TInt CRecordMetaData::PreProcess(CStifSectionParser* /*aParser*/)
{
	if (recorder)
	{
		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
		recorder->SetGain(recorder->MaxGain());
		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		TRAPD(err,recorder->CropL());
		if (err != KErrNone) logger->Log(_L("Error cropping file: %d"),err);
	}
	return KErrNone;
}

TInt CRecordMetaData::PostProcess(CStifSectionParser* aParser)
{
	//Get MetaData and add it to file
	TPtrC name, value;
	TBool first = true, noMore;

	if (GetMetaData(aParser,name,value,first,noMore) == KErrNone)
	{
		while (!noMore)
		{
			CMMFMetaDataEntry* md = NULL;
			TRAPD(err2,md = CMMFMetaDataEntry::NewL(name,value));
			if (err2 != KErrNone)
			{
				logger->Log(_L("Error %d creating CMMFMetaDataEntry"),err2);
				CleanupStack::PopAndDestroy();
				return KErrAddMetaData;
			}
			CleanupStack::PushL(md);

			logger->Log(_L("Meta Data Name: %S "),&name);
			logger->Log(_L("Meta Data Value: %S"),&value);

			TRAPD(err,recorder->AddMetaDataEntryL(*md));
			if (err != KErrNone)
			{
				logger->Log(_L("Error %d adding Meta Data entry"),err);
				CleanupStack::PopAndDestroy();
				return KErrAddMetaData;
			}

			CleanupStack::PopAndDestroy();
			GetMetaData(aParser,name,value,first,noMore);
		}
	}
	else
		return KErrNoMetaData;

	//Close file
	recorder->Close();

	//Open again
	TRAPD(err,recorder->OpenFileL(recordingFileName));
	if (err != KErrNone)
	{
		logger->Log(_L("Error %d opening file: %S"),err,recordingFileName);
		return KErrRecOpenFile;
	}

	//Read meta data and compare
	logger->Log(_L("Checking Meta Data"));
	first = true;
	noMore = false;
	TInt error = KErrNone;

	if (GetMetaData(aParser,name,value,first,noMore) == KErrNone)
	{
		while (!noMore)
		{
			TBool hasMD;
			TInt err;
			if ((err = IsMetaDataContained(name,value,hasMD)) != KErrNone)
				return err;

			if (!hasMD)
			{
				logger->Log(_L("Meta Data %S, %S not found"),&name,&value);
				error = KErrMetaDataNotFound;
			}
			else
			{
				logger->Log(_L("Meta Data %S, %S found"),&name,&value);
			}

			GetMetaData(aParser,name,value,first,noMore);
		}
	}
	else
		return KErrNoMetaData;

	return error;
}

TInt CRecordMetaData::GetMetaData(CStifSectionParser *aParser, TPtrC& name, TPtrC& value, TBool & first, TBool & noMore)
{
	noMore = false;
	if (first)
	{
		first = false;
		if (aParser->GetLine(KMDName,name,ENoTag) == KErrNone)
		{
			if (aParser->GetLine(KMDValue,value,ENoTag) == KErrNone)
				return KErrNone;
			else
				return KErrSyntax;
		}
		else
			return KErrSyntax;
	}
	else
	{
		if (aParser->GetNextLine(KMDName,name,ENoTag) != KErrNone)
		{
			if (aParser->GetLine(KMDValue,value,ENoTag) == KErrNone)
				return KErrNone;
			else
				noMore = true;
		}
		else
			noMore = true;
	}

	return KErrNone;
}

TInt CRecordMetaData::IsMetaDataContained(TPtrC& name, TPtrC& value, TBool & hasMD)
{
	hasMD = false;
	TInt err, numEntries;
	if ((err = recorder->GetNumberOfMetaDataEntries(numEntries)) == KErrNone)
	{
		TInt counter = 0;
		while (counter < numEntries && !hasMD)
		{
			CMMFMetaDataEntry* md = NULL;
			TRAPD(err2,md = recorder->GetMetaDataEntryL(counter));
			if (err2 != KErrNone)
			{
				logger->Log(_L("Error %d getting Meta Data"),err2);
				return KErrAddMetaData;
			}
			CleanupStack::PushL(md);

			if ((md->Name().Compare(name) == 0) && (md->Value().Compare(value) == 0))
				hasMD = true;

			CleanupStack::PopAndDestroy();
		}
	}
	else
	{
		logger->Log(_L("Error %d getting number of Meta Data entries"),err);
		return err;
	}

	return KErrNone;
}
