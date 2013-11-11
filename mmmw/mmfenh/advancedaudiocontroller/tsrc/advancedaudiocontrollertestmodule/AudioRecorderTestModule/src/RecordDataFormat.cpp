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


#include "RecordDataFormat.h"
#include <mmfFormatImplementationUIDs.hrh>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRecordDataFormat::CRecordDataFormat(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	callbackErr = KErrNone;
}

CRecordDataFormat::~CRecordDataFormat()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CRecordDataFormat::ConstructL(TFileName* fileName)
{
	logger->Log(_L("Creating file: %S"),fileName);
	recorder = CMdaAudioRecorderUtility::NewL(*this);
	if (recorder)
	{
		TRAPD(err,recorder->OpenFileL(*fileName));
		if (err != KErrNone)
		{
			logger->Log(_L("Error %d opening file: %S"),err,fileName);
			User::Leave(KErrRecOpenFile);
		}
	}
}

TInt CRecordDataFormat::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	TInt error = KErrNone;
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	aLogger->Log(_L("Creating CRecordDataFormat"));

	TFileName fileName;

	//Get file name
	TPtrC name;
	CStifItemParser* item;
	item = aParser->GetItemLineL(KFileName, ENoTag);
	if (!item)
	{
		CleanupStack::PopAndDestroy();
		return KErrSyntax;
	}

	CleanupStack::PushL(item);
	if (item->GetString(KEmptyString,name) != KErrNone)
	{
		aLogger->Log(_L("Invalid file name"));
		CleanupStack::PopAndDestroy(2);
		return KErrSyntax;
	}
	CleanupStack::PopAndDestroy(); //item

	GetFileName(name,&fileName);


	TPtrC dataFormat;

	item = aParser->GetNextItemLineL(KDataFormat);
	if (item)
	{
		CleanupStack::PushL(item);
		error = item->GetString(KDataFormat,dataFormat);
		if (error != KErrNone)
		{
			aLogger->Log(_L("The format syntax in the config file is not right") );
			CleanupStack::PopAndDestroy(4);
			return KErrSyntax;
		}

		CleanupStack::PopAndDestroy();   //item
	}
	else
	{
		aLogger->Log(_L("Format parameter is missing in the config file"));
		error = KErrSyntax;
	}


	// create CRecordDataFormat

	CRecordDataFormat* selfObj = CRecordDataFormat::NewL(aConsole, aLogger, &fileName);

	(selfObj->dataFormat).Set(dataFormat);

	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();

	if (selfObj->callbackErr != KErrNone)
	{
		error = selfObj->callbackErr;
	}

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CRecordDataFormat* CRecordDataFormat::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CRecordDataFormat* self = new (ELeave) CRecordDataFormat(aConsole, aLogger);
    CleanupStack::PushL(self);

 //   self->ConstructL(fileName);
	TRAPD(err, self->ConstructL(fileName));
	if (err != KErrNone)
	{
		CleanupStack::PopAndDestroy();
		return NULL;
	}

	CleanupStack::Pop(self);
    return self;
}

void CRecordDataFormat::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CRecordDataFormat::MoscoStateChangeEvent"));
#endif
	TInt err = KErrNone;
	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == 0)
	{
		if (dataFormat == KFormatWav)
		{
			logger->Log(_L("SetDestinationFormatL(KMmfUidFormatWAVWrite)"));
			TUid id;
			id.iUid = KMmfUidFormatWAVWrite;
			TRAP(err,recorder->SetDestinationFormatL(id));
			if (err != KErrNone)
			{
				logger->Log(_L("SetDestinationFormatL(%d) failed with error %d"),KMmfUidFormatWAVWrite, err);
				callbackErr = err;
				CActiveScheduler::Stop();
				return;
			}
			if (recorder->DestinationFormatL() != id)
			{
				logger->Log(_L("Retrieved format is not same as set format") );
				callbackErr = KErrOutOfRange;
			}
		}
		else
		{
			// Only WAV use format object
			logger->Log(_L("Invalid input dataFormat - %S"), &dataFormat);
			callbackErr = KErrInvalidId;
/*			if (dataFormat == KFormatAmr)
			{
				logger->Log(_L("SetDestinationFormatL(AMR)"));
				TUid id;
				id.iUid = KAdvancedUidFormatAMRWrite;
				TRAP(err,recorder->SetDestinationFormatL(id));
				if (err != KErrNone)
				{
					logger->Log(_L("SetDestinationFormatL(AMR) failed with error %d"), err);
					callbackErr =  err;
					CActiveScheduler::Stop();
					return;
				}

				if (recorder->DestinationFormatL() != id)
				{
					logger->Log(_L("Retrieved format is not same as set format") );
					callbackErr = KErrOutOfRange;
				}
			}*/
		}
	}
	if (aErrorCode != KErrNone)
	    {
        callbackErr = aErrorCode;	    
	    }

	CActiveScheduler::Stop();
	return;

}



void CRecordDataFormat::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}


