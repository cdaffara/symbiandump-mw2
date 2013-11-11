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

#include "RecordingBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecordingBase::CRecordingBase(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	console = aConsole;
	logger = aLogger;
	timeToRecord = 0;
	maxFileSize = 0;
	relativeGain = 0;		//for Gain
	callbackErr = KErrNone;	//for Gain
	expectedBalance = -999; // for balance
	expectedBitRate = 0;
	expectedSampleRate = 0;
	fileDuration = 0;
	existingDuration = 0;
	CropTest = false;
	isWav = false;
	isMp4 = false;
	isAmr = false;
}

CRecordingBase::~CRecordingBase()
{  
	TInt err( iFileMan->Delete( recordingFileName ) );
	if (recorder) delete recorder;
	iFs.Close();
	if(iFileMan) delete iFileMan;
	recorder = NULL;
}

CRecordingBase* CRecordingBase::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordingBase::CreateObjectL"));
	return (new (ELeave) CRecordingBase(aConsole, aLogger));
}

CRecordingBase* CRecordingBase::NewL(CTestModuleIf       *aConsole,
                                     CStifLogger         *aLogger,
                                     TFileName           *fileName,
                                     CreateObjectFunction coFunc)
{
	CRecordingBase* self;
	if (coFunc == NULL)
		self = CreateObjectL(aConsole,aLogger);
	else
		self = coFunc(aConsole,aLogger);

    CleanupStack::PushL(self);

	TRAPD(err, self->ConstructL(fileName));
	if (err != KErrNone)
	{
		CleanupStack::PopAndDestroy();
		return NULL;
	}

	CleanupStack::Pop(self);
    return self;
}

TInt CRecordingBase::RunTestL(CTestModuleIf *aConsole,
                              CStifLogger *aLogger,
                              CStifSectionParser *aParser,
                              TInt* clipCounter,
                              CreateObjectFunction coFunc)
{
	TInt error = KErrNone;

	//Create scheduler
	aLogger->Log(_L("Creating scheduler"));
	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	CRecordingBase* recorderObj;
	// create recorder,
	if ((error = CRecordingBase::CreateRecorderL(aConsole, aLogger, aParser, clipCounter, coFunc, recorderObj)) != KErrNone)
	{
		CleanupStack::PopAndDestroy(); //scheduler
		return error;
	}

	CleanupStack::PushL(recorderObj);


	//SimpleRecording will read Error range
	error = CRecordingBase::SimpleRecordL(aConsole, aLogger, recorderObj, true, aParser);


	CleanupStack::PopAndDestroy(2); // recorderObj, scheduler

	return error;
}

void CRecordingBase::ConstructL(TFileName* fileName)
{
	logger->Log(_L("Creating file: %S"),fileName);
	User::LeaveIfError( iFs.Connect() );
	iFileMan = CFileMan::NewL( iFs );

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

void CRecordingBase::MoscoStateChangeEvent(CBase* /*aObject*/,
                                           TInt    aPreviousState,
                                           TInt    aCurrentState,
                                           TInt    aErrorCode)
{
    TInt err = KErrNone;

#ifdef _DEBUG
    RDebug::Print (_L ("CRecordingBase::MoscoStateChangeEvent"));
#endif

    

	    logger->Log(_L("MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),
	                   aErrorCode,aPreviousState,aCurrentState);
/*
	TTime time;
	time.UniversalTime();
	logger->Log(_L("MoscoStateChangeEvent time High: %u, Low: %u"),time.Int64().High(),time.Int64().Low());
*/

	callbackErr = aErrorCode; // for Gain
	if (recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen &&
	    aPreviousState == 0)
	{
//		recorder->SetAudioDeviceMode(CMdaAudioRecorderUtility::ELocal);
	//	recorder->SetGain(recorder->MaxGain());

      //  TInt64 duration = recorder->Duration().Int64();
		existingDuration = recorder->Duration().Int64();
		if (existingDuration)
		{
			logger->Log(_L("File Already Exist with duration = %d"), existingDuration);
			logger->Log(_L("Save the existingDuration"));
    		//recorder->SetPosition(TTimeIntervalMicroSeconds(TInt64(0)));
		    //TRAP(err,recorder->CropL());
		}
		if (err != KErrNone)
			logger->Log(_L("Error cropping file: %d"),err);

		if (maxFileSize != 0)
		{
			recorder->SetMaxWriteLength(maxFileSize);
			logger->Log(_L("Set maxWriteLength: %d"), maxFileSize);
		}

		if (!isAmr && isBitRateTest)
		{
			logger->Log(_L("Not AMR format, Stop now."));
			CActiveScheduler::Stop();
		}
		else
		{
			logger->Log(_L("StartRecording"));
			StartRecordingL();
		}
	}

	if (recorder && aPreviousState == CMdaAudioClipUtility::ERecording)
	{
		logger->Log(_L("Recorder stopping"));
		recorder->Stop();
		CActiveScheduler::Stop();
	}
	else
	{
		if (recorder && aErrorCode != KErrNone)
		{
			//logger->Log(_L("Reaching maximum file size. Recorder stopping"));
			logger->Log(_L("unexpected results, error: %d"), aErrorCode);
			recorder->Stop();
			CActiveScheduler::Stop();
		}
	}
}

TInt CRecordingBase::ExecuteL(CParameters* /*aParams*/)
{
	if (recorder)
	{

		logger->Log(_L("RecordClip recorder stopping"));
		recorder->Stop();

		if (CropTest)
		{
			fileDuration = recorder->Duration();

			position = fileDuration.Int64()*relativePos/100.0;
			recorder->SetPosition(TTimeIntervalMicroSeconds(position));

			logger->Log(_L("The audio file duration: %d"), fileDuration);
			logger->Log(_L("Crop position: %d"),position);

			if (fromBeging)
			{
				logger->Log(_L("It starts to call CropL()"));
				TRAPD(error,recorder->CropL());
			}
			else
			{
				logger->Log(_L("It starts to call CropFromBeginningL()"));
				TRAPD(error, recorder->CropFromBeginningL());
			}

			newFileDuration = recorder->Duration();
		}

		CActiveScheduler::Stop();
	}
	return 0;
}

TInt CRecordingBase::StartRecordingL()
{
	TInt err(KErrNone);
	int i;
	if (recorder)
	{
		if (isMp4)
		{

			logger->Log(_L("Record a mp4 audio file, set SampleRate, Channel and BitRate"));

			//Set SampleRate
			RArray<TUint> supportedSampleRates;
			TRAP(err, recorder->GetSupportedSampleRatesL(supportedSampleRates) );
			if (err != KErrNone)
			{
				logger->Log(_L("Error %d for GetSupportedSampleRateL"),err );
				return err;
			}

			for (i=0; i<supportedSampleRates.Count(); i++)
			{
				if (expectedSampleRate == supportedSampleRates[i])
				{
					TRAP(err, recorder->SetDestinationSampleRateL(expectedSampleRate));
					if (err)
					{
						logger->Log(_L("Error %d for SetDestinationSampleRateL %d"),err, expectedSampleRate );
						return err;
					}
					break;
				}
			}

			if (i>= supportedSampleRates.Count())
			{
				logger->Log(_L("expectedSampleRate %d is not supported by system, invalid test case."), expectedSampleRate );
				callbackErr = KErrInvalidId;
				return callbackErr;
			}

			//Set Channel
			RArray<TUint> supportedChannels;
			TRAP(err, recorder->GetSupportedNumberOfChannelsL(supportedChannels) );
			if (err != KErrNone)
			{
				logger->Log(_L("Error %d for GetSupportedNumberOfChannelsL"),err );
				return err;
			}

			for (i=0; i<supportedChannels.Count(); i++)
			{
				if (expectedChannel == supportedChannels[i])
				{
					TRAP(err, recorder->SetDestinationNumberOfChannelsL(expectedChannel));
					if (err)
					{
						logger->Log(_L("Error %d for SetDestinationNumberOfChannelsL %d"),err, expectedChannel );
						return err;
					}
					break;
				}
			}

			if (i>= supportedChannels.Count())
			{
				logger->Log(_L("expectedChannel %d is not supported by system, invalid test case."), expectedChannel );
				callbackErr = KErrInvalidId;
				return callbackErr;
			}



	/*		TRAP(err, recorder->SetDestinationNumberOfChannelsL(expectedChannel));
			if (err)
				{
					logger->Log(_L("Error %d for SetDestinationNumberOfChannelsL %d"),err, expectedChannel );
					return err;
				}
*/
			//Set BitRate
			RArray<TUint> supportedBitRates;
			TRAP(err, recorder->GetSupportedBitRatesL(supportedBitRates) );
			if (err != KErrNone)
			{
				logger->Log(_L("Error %d for GetSupportedBitRatesL"),err );
				return err;
			}

			for (i=0; i<supportedBitRates.Count(); i++)
			{
				//TUint bitRate = bitRates[i];
				if (expectedBitRate == supportedBitRates[i])
				{
					TRAP(err, recorder->SetDestinationBitRateL(expectedBitRate));
					if (err)
					{
						logger->Log(_L("Error %d for SetDestinationBitRateL %d"),err, expectedBitRate );
						return err;
					}
					break;
				}
			}

			if (i>= supportedBitRates.Count())
			{
				logger->Log(_L("expectedSampleRate %d is not supported by system, invalid test case."), expectedSampleRate );
				callbackErr = KErrInvalidId;
				return callbackErr;
			}

		}

		logger->Log(_L("RecordClip recorder recording"));
		TRAP(err,recorder->RecordL());
		return err;

	}
	return KErrNoRecorder;
}

TTimeIntervalMicroSeconds CRecordingBase::Duration()
{
	if (recorder)
    {
		logger->Log(_L("Get Duration, %d") , recorder->Duration().Int64());
		return recorder->Duration();
	}
	else
		return TInt64(0);
}

void CRecordingBase::GetFileName(TPtrC path, TFileName* fileName, TInt clipCounter)
{
	TBuf<5> num;
	num.Format(_L("%d"),clipCounter);
	TParse p;
	p.Set(path,NULL,NULL);
	fileName->Append(p.DriveAndPath());
	fileName->Append(p.Name());
	fileName->Append(num);
	fileName->Append(p.Ext());
}

TInt CRecordingBase::PreProcess(CStifSectionParser* /*aParser*/)
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

TInt CRecordingBase::PostProcess(CStifSectionParser* /*aParser*/)
{
	return KErrNone;
}

TInt CRecordingBase::SimpleRecordL(CTestModuleIf*      aConsole,
                                   CStifLogger*        aLogger,
                                   CRecordingBase*     recObj,
                                   TBool               doProcess,
                                   CStifSectionParser* aParser,
                                   TBool               doTimer)
{
	TInt error = KErrNone;

	//Start scheduler

	if (recObj->maxFileSize != 0)
	{
		doTimer= false;						// since SetMaxWriteLength limits the record time, no timer needed
	}


	if (doTimer)
	{
		aLogger->Log(_L("Creating timer"));
		//Create timer for stopping recorder
		CParameters* params = new CParameters();
		CMediaEvent* timer = CMediaEvent::NewL(*aConsole,*aLogger,recObj->timeToRecord,TTimeIntervalMicroSeconds32(0),recObj,params);
		CleanupStack::PushL(timer);
	}

	if (doProcess)
		if ((error = recObj->PreProcess(aParser)) != KErrNone)
			return error;


	TTime iStartTime;
	iStartTime.HomeTime();
	aLogger->Log(_L("Starting Scheduler"));
	CActiveScheduler::Start();
	TTime iEndTime;
	iEndTime.HomeTime();


	TTimeIntervalMicroSeconds PerceivedDuration = iEndTime.MicroSecondsFrom( iStartTime );
	aLogger->Log(_L("Perceived duration (%d)") , PerceivedDuration.Int64() );


	if (doProcess)
		error = recObj->PostProcess(aParser);

	if (doTimer) CleanupStack::PopAndDestroy(); // timer

	if (error == KErrNone)
	return recObj->callbackErr;

	return error;
}

TInt CRecordingBase::CreateRecorderL(CTestModuleIf *aConsole,
                                     CStifLogger *aLogger,
                                     CStifSectionParser *aParser,
                                     TInt* clipCounter,
                                     CreateObjectFunction coFunc,
                                     CRecordingBase*& recObj,
                                     TBool nameReq,
                                     TBool sizeReq)
{
	TInt error = KErrNone;
	recObj = NULL;
	TFileName fileName;
	TPtrC name;
	TTimeIntervalMicroSeconds recTime = KDefaultRecordTime;
	TInt maxFileLength = 0;

	if (nameReq)
	{
		//Obtain file name from parser
		if (aParser->GetLine(KFileName, name, ENoTag) != KErrNone)
		{
			aLogger->Log(_L("No file name provided"));
			return KErrSyntax;
		}
	}
	CRecordingBase::GetFileName(name,&fileName,*clipCounter);
	*clipCounter = *clipCounter + 1;

	if (sizeReq)
	{
		//Obtain record time from parser
		CStifItemParser* item = aParser->GetItemLineL(KRecordTime, ENoTag);
		if (!item)
		{
			aLogger->Log(_L("No record time provided, read MaxfileLength"));
			item = aParser->GetItemLineL(KMaxLength);
			if (!item)
			{
				aLogger->Log(_L("No MaxfileLength provided either"));
				return KErrSyntax;
			}
			else
			{
				CleanupStack::PushL(item);
				item->GetInt(KMaxLength,maxFileLength);
				CleanupStack::PopAndDestroy();
				//aLogger->Log(_L("Record time: %d"),recTime.Int64());
				aLogger->Log(_L("Maximum recorded file length is: %d"),maxFileLength);
			}
		}
		else
		{
			CleanupStack::PushL(item);
			recTime = CRecordingBase::ParseTime(item);
			CleanupStack::PopAndDestroy();

			//aLogger->Log(_L("Record time: %d"), recTime.Int64());
			LogTime(_L("Record time:"),recTime.Int64(),aLogger);
		}
	}

	//Obtain relative gain(s) from parser and execute test
	TInt relativeGain = 0;
	CStifItemParser* item = aParser->GetNextItemLineL(KRelativeGain);
//	CleanupStack::PushL(item);
	if (item)
	{
		CleanupStack::PushL(item);
		error = item->GetInt(KRelativeGain,relativeGain);
		if (error != KErrNone)
		{
			CleanupStack::PopAndDestroy(); // item
			aLogger->Log(_L("relative Gain syntax error"));
			return error;
		}
		else
		{
			//Calculate expected gain
		//	expectedGain = maxGain*relativeGain/100;
			aLogger->Log(_L("Relative gain: %d"),relativeGain);
		//	aLogger->Log(_L("Relative gain: %d, Value: %d"),relativeGain,expectedGain);
			CleanupStack::PopAndDestroy(); // item
		}
	}


	TInt expectedBalance = -999;
	item = aParser->GetNextItemLineL(KBalance);
	if (item)
	{
		CleanupStack::PushL(item);
		error = item->GetInt(KBalance,expectedBalance);
		if (error != KErrNone)
		{
			CleanupStack::PopAndDestroy(); // item
			aLogger->Log(_L("expectedBalance syntax error"));
			return error;
		}
		else
		{
			aLogger->Log(_L("expectedBalance: %d"),expectedBalance);
			CleanupStack::PopAndDestroy(); // item
		}
	}

	TInt expectedBitRate = 0;
	item = aParser->GetItemLineL(KBitRate);
	if (item)
	{
		CleanupStack::PushL(item);
		error = item->GetInt(KBitRate,expectedBitRate);
		if (error != KErrNone)
		{
			CleanupStack::PopAndDestroy(); // item
			aLogger->Log(_L("expectedRitRate syntax error"));
			return error;
		}
		else
		{
			aLogger->Log(_L("expectedBitRate: %d"),expectedBitRate);
			CleanupStack::PopAndDestroy(); // item
		}
	}

	TInt expectedChannel = 1;
	item = aParser->GetItemLineL(KChannels);
	if (item)
	{
		CleanupStack::PushL(item);
		error = item->GetInt(KChannels,expectedChannel);
		if (error != KErrNone)
		{
			CleanupStack::PopAndDestroy(); // item
			aLogger->Log(_L("expectedChannel syntax error"));
			return error;
		}
		else
		{
			aLogger->Log(_L("expectedChannel: %d"),expectedChannel);
			CleanupStack::PopAndDestroy(); // item
		}
	}

	TInt expectedSampleRate = 1;
	item = aParser->GetItemLineL(KSampleRate);
	if (item)
	{
		CleanupStack::PushL(item);
		error = item->GetInt(KSampleRate,expectedSampleRate);
		if (error != KErrNone)
		{
			CleanupStack::PopAndDestroy(); // item
			aLogger->Log(_L("expectedSampleRate syntax error"));
			return error;
		}
		else
		{
			aLogger->Log(_L("expectedSampleRate: %d"),expectedSampleRate);
			CleanupStack::PopAndDestroy(); // item
		}
	}




	//Create recorder
	aLogger->Log(_L("Creating recorder"));

	recObj = CRecordingBase::NewL(aConsole, aLogger, &fileName, coFunc);
	if (!recObj)
	{
		CleanupStack::PopAndDestroy(); // scheduler
		return KErrRecOpenFile;
	}
	recObj->timeToRecord = recTime;
	recObj->recordingFileName = fileName;
	recObj->maxFileSize = maxFileLength;
	recObj->relativeGain = relativeGain;
	recObj->expectedBalance = expectedBalance;
	recObj->expectedBitRate = expectedBitRate;
	recObj->expectedChannel = expectedChannel;
	recObj->expectedSampleRate = expectedSampleRate;
	//recObj->isWav = isWav;
	//recObj->isAmr = isAmr;
//	recObj->isMp4 = isMp4;
    aLogger->Log(_L("recObj->timeToRecord:%d"),recObj->timeToRecord);
	return error;
}
