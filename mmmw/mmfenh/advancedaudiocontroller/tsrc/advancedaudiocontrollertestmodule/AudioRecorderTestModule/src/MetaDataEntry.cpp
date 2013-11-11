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


#include "MetaDataEntry.h"
#include <mmfFormatImplementationUIDs.hrh>
//#include "MdaAudioSampleEditor.h"
#include <e32cmn.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMetaDataEntry::CMetaDataEntry(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	callbackErr = KErrNone;
}

CMetaDataEntry::~CMetaDataEntry()
{

	if (recorder) delete recorder;
	recorder = NULL;
}


void CMetaDataEntry::ConstructL(TFileName* fileName)
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

TInt CMetaDataEntry::RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* /*clipCounter*/)
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


	// create CMetaDataEntry
	aLogger->Log(_L("Creating CMetaDataEntry"));

	CMetaDataEntry* selfObj = CMetaDataEntry::NewL(aConsole, aLogger, &fileName);


	CleanupStack::PushL(selfObj);

	CActiveScheduler::Start();

	if (selfObj->callbackErr != KErrNone)
	{
		error = selfObj->callbackErr;
	}

	CleanupStack::PopAndDestroy(2); // schedule, selfObj

	return error;
}


CMetaDataEntry* CMetaDataEntry::NewL(CTestModuleIf *aConsole, CStifLogger *aLogger, TFileName* fileName)
{
	CMetaDataEntry* self = new (ELeave) CMetaDataEntry(aConsole, aLogger);
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

void CMetaDataEntry::MoscoStateChangeEvent(CBase* /*aObject*/, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CMetaDataEntry::MoscoStateChangeEvent"));
#endif
	TInt err = KErrNone;
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

		logger->Log(_L("GetNumberOfMetaDataEntries"));

		TInt NumMetaDatas =0;
		err = recorder->GetNumberOfMetaDataEntries(NumMetaDatas);

		if ( err !=KErrNotSupported )
		{	//Error getting meta data info
			logger->Log(_L("Error getting meta data info %d"),err);
			callbackErr = err;
		}


		_LIT(KMetaNameRom,"currentName");
		const TDesC &myName =  KMetaNameRom;
		_LIT(KMetaValueRom,"currentValue");
		const TDesC &myValue =  KMetaValueRom;
		//TBuf<15> myName (_L("currentName"));
		//TBuf<15> myValue (_L("currentValue"));


		logger->Log(_L("Building CMMFMetaDataEntry"));

		CMMFMetaDataEntry *myMetaData = CMMFMetaDataEntry::NewL(myName, myValue);
		TRAPD(err, recorder->AddMetaDataEntryL(*myMetaData));
		if (err != KErrNotSupported)
		{
			logger->Log(_L("Error %d for AddMetaDataEntryL"),err);
			callbackErr = err;

		}
		logger->Log(_L("AddMetaDataEntryL CMMFMetaDataEntry."));

		CMMFMetaDataEntry *currentMetaData;
		TRAPD(err1, (currentMetaData = recorder->GetMetaDataEntryL(0)));

		if (err1 != KErrNotSupported)
			{
				logger->Log(_L("Error %d for GetMetaDataEntryL"),err1);
				callbackErr = err1;

			}
		logger->Log(_L("GetMetaDataEntryL CMMFMetaDataEntry."));


		TInt currMetaDataIndex;
		for ( currMetaDataIndex=0 ; currMetaDataIndex <= NumMetaDatas ; currMetaDataIndex++)
		{
		    logger->Log(_L("getting meta data info %d"),currMetaDataIndex);
		    CMMFMetaDataEntry *currMetaData;
		    TRAPD(err2, (currMetaData = recorder->GetMetaDataEntryL(currMetaDataIndex)));
		    if (err2 != KErrNotSupported)
			{
				logger->Log(_L("Error %d for GetMetaDataEntryL"),err2);
				//delete currMetaData;
				callbackErr = err2;
				break;
			}
		    if( err2 != KErrNone)
		        {
		         break;
		        }
			if (currMetaData != NULL )
			{
				// check for jpeg image
				if (currMetaData->Name() == _L("attachedpicture"))
					logger->Log(_L("MetaData[%d]: Name: [%S], Value: [image]") , currMetaDataIndex , &(currMetaData->Name()) );
				else
			     	logger->Log(_L("MetaData[%d]: Name: [%S], Value: [%S]") , currMetaDataIndex , &(currMetaData->Name()) , &(currMetaData->Value()) );
			  	delete currMetaData;
			}
			logger->Log(_L("getting meta data info %d"),currMetaDataIndex+1);
		}


		TRAPD(err3, recorder->ReplaceMetaDataEntryL(0, *myMetaData););
		if (err3 != KErrNotSupported)
		{
			logger->Log(_L("Error %d for ReplaceMetaDataEntryL"),err3);
			callbackErr = err3;

		}
		logger->Log(_L("ReplaceMetaDataEntryL CMMFMetaDataEntry."));

		TRAPD(err4, recorder->RemoveMetaDataEntry(0));
		if (err4 != KErrNotSupported)
		{
			logger->Log(_L("Error %d for RemoveMetaDataEntry"),err4);
			callbackErr = err4;

		}
		logger->Log(_L("RemoveMetaDataEntry CMMFMetaDataEntry."));

		delete myMetaData;


		recorder->PlayL();


		return;
	}

	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen && aPreviousState == CMdaAudioClipUtility::EPlaying)
	{

		logger->Log(_L("before stop....-"));
		CActiveScheduler::Stop();
	}
	if (aErrorCode != KErrNone)
	    {
        
	    CActiveScheduler::Stop();
	    
	    }


	return;

}



void CMetaDataEntry::GetFileName(TPtrC path, TFileName* fileName)
{

	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());

	fileName->Append(p.Ext());
}