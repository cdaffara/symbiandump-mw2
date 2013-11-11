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


#include "RecordDataType.h"
#include <mmfFormatImplementationUIDs.hrh>
//#include "MdaAudioSampleEditor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRecordDataType::CRecordDataType(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	callbackErr = KErrNone;
}

CRecordDataType::~CRecordDataType()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CRecordDataType::ConstructL(TFileName* fileName)
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

TInt CRecordDataType::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
{
	TInt error = KErrNone;
	aLogger->Log(_L("Creating scheduler"));

	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	TFileName fileName;

	//Get file name
	TPtrC name;
	CStifItemParser* item = aParser->GetItemLineL(KFileName, ENoTag);
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
	CleanupStack::PopAndDestroy();

	GetFileName(name,&fileName);






	if (error != KErrNone)
	{
		CleanupStack::PopAndDestroy();  //scheduler
		return error;
	}


	// create CRecordDataType
	aLogger->Log(_L("Creating CRecordDataType"));

	CRecordDataType* selfObj = CRecordDataType::NewL(aConsole, aLogger, &fileName);


	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();
	
	if(selfObj->callbackErr != KErrNone)
	    {
        error = selfObj->callbackErr;
	    }

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CRecordDataType* CRecordDataType::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CRecordDataType* self = new (ELeave) CRecordDataType(aConsole, aLogger);
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

void CRecordDataType::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CRecordDataType::MoscoStateChangeEvent"));
#endif
	
	callbackErr = aErrorCode;
	RArray<TFourCC> dataTypes;

	logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),aErrorCode,aPreviousState,aCurrentState);



	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == 0)
	{
		// Initialize
		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
		logger->Log(_L("Mode set"));
		recorder->SetGain(recorder->MaxGain());
		logger->Log(_L("Gain set"));
		recorder->SetVolume(recorder->MaxVolume());
		logger->Log(_L("Volume set"));
		recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		logger->Log(_L("Position set"));


		logger->Log(_L("SetDestinationDataTypeL set"));


		TRAPD(err, recorder->SetDestinationDataTypeL(KMMFFourCCCodeNULL));
		if (err != KErrNone)
			{
				logger->Log(_L("Error %d for SetDestinationDataTypeL(0x%x)"),err, KMMFFourCCCodeNULL  );
				callbackErr = err;

			}


		TFourCC new_fourCC = recorder->DestinationDataTypeL();
		if (new_fourCC != KMMFFourCCCodeNULL)
			{
				logger->Log(_L("Expected dataType is: 0x%x"), KMMFFourCCCodeNULL);
				logger->Log(_L("New dataType is: 0x%x"), new_fourCC.FourCC());
				logger->Log(_L("Expected  dataType is not right") );
				callbackErr = KErrOutOfRange;

			}
	    logger->Log(_L("DestinationDataTypeL Get"));

		recorder->GetSupportedDestinationDataTypesL(dataTypes);
		logger->Log(_L("GetSupportedDestinationDataTypesL Get"));


		recorder->PlayL();


		return;
	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{
		TFourCC new_fourCC = recorder->DestinationDataTypeL();
		if (new_fourCC != KMMFFourCCCodeNULL)
			{
				logger->Log(_L("Expected dataType is: 0x%x"), KMMFFourCCCodeNULL);
				logger->Log(_L("New dataType is: 0x%x"), new_fourCC.FourCC());
				logger->Log(_L("Expected  dataType is not right") );
				callbackErr = KErrOutOfRange;

			}
	    logger->Log(_L("DestinationDataTypeL after play."));


		CActiveScheduler::Stop();
	}
	if (aErrorCode != KErrNone)
	    {
	    CActiveScheduler::Stop();
	    
	    }


	return;

}



void CRecordDataType::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}