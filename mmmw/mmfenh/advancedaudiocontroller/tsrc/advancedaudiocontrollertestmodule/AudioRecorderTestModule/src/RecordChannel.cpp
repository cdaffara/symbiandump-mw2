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


#include "RecordChannel.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRecordChannel::CRecordChannel(CTestModuleIf *aConsole, CStifLogger *aLogger):CRecordingBase(aConsole,aLogger)
{
//	iFirstTime = true;
	iCurrChannelNum = 0;
}

CRecordChannel::~CRecordChannel()
{

}

CRecordingBase* CRecordChannel::CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger)
{
	//aLogger->Log(_L("CRecordClip::CreateObjectL"));
	return (new (ELeave) CRecordChannel(aConsole, aLogger));
}

TInt CRecordChannel::RunTestL(CTestModuleIf *aConsole,
                              CStifLogger *aLogger,
                              CStifSectionParser *aParser,
                              TInt* clipCounter)
{
	TInt error = KErrNone;

	//Create scheduler
	aLogger->Log(_L("Creating scheduler"));
	CActiveScheduler*  scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler); // push to clean-up stack
	CActiveScheduler::Install(scheduler); // install as active scheduler

	//Create recorder
	CRecordingBase* recorderObj;
	if ((error = CRecordingBase::CreateRecorderL(aConsole, aLogger, aParser,
	                                             clipCounter, CRecordChannel::CreateObjectL,
	                                             recorderObj)) != KErrNone)
	{
		CleanupStack::PopAndDestroy(); //scheduler
		return error;
	}
	CleanupStack::PushL(recorderObj);

	error = CRecordingBase::SimpleRecordL(aConsole, aLogger, recorderObj, true, aParser);

	CleanupStack::PopAndDestroy(2); // recorder, scheduler
	return error;
}

TInt CRecordChannel::PreProcess(CStifSectionParser* /*aParser*/)
{
//	if (recorder && iFirstTime)
//	{
//		iFirstTime = false;
//	}
	return KErrNone;
}

TInt CRecordChannel::PostProcess(CStifSectionParser* /*aParser*/)
{
	TInt error = KErrNone;

	if ( iCurrChannelNum != expectedChannel)
	{
		logger->Log(_L("ChannelNum differs after recording curr:%d exp:%d"), iCurrChannelNum, expectedChannel);
		error = KErrOutOfRange;
	}

	return error;
//	return KErrNone;
}


void CRecordChannel::MoscoStateChangeEvent(CBase* aObject,
                                           TInt aPreviousState,
                                           TInt aCurrentState,
                                           TInt aErrorCode)
{
#ifdef _DEBUG
    RDebug::Print (_L ("CRecordChannel::MoscoStateChangeEvent"));
#endif
	logger->Log(_L("CRecordChannel::MoscoStateChangeEvent called, error: %d	prev: %d curr : %d"),
	               aErrorCode,aPreviousState,aCurrentState);

	TInt err = KErrNone;
	RArray<TUint> numChannels;

	if(recorder && aErrorCode == KErrNone && aCurrentState == CMdaAudioClipUtility::EOpen)
	{
		TRAP(err, recorder->GetSupportedNumberOfChannelsL(numChannels) );
		if (err != KErrNone)
		{
			logger->Log(_L("Error %d for GetSupportedNumberOfChannelsL"),err );
		//	CleanupStack::PopAndDestroy(3);
			return;
		}


		while (expectedChannel >0)
		{
			if (  expectedChannel <= numChannels.Count() )
			{
				logger->Log(_L("Set expectedChannel: %d"),expectedChannel);
				TRAPD(err, recorder->SetDestinationNumberOfChannelsL(expectedChannel) );
				if (err != KErrNone)
				{
					logger->Log(_L("Error %d for SetDestinationNumberOfChannelsL"),err );
				//	CleanupStack::PopAndDestroy(3);
					return;
				}
				break;
			}
			else
			{
				logger->Log(_L("System cann't support %d channels"),expectedChannel );
				expectedChannel--;
			}
		}


		//	logger->Log(_L("Set expectedChannel: %d"),expectedChannel);
		iCurrChannelNum = recorder->DestinationNumberOfChannelsL();
		logger->Log(_L("Get CurrChannelNum after set BitRate: %d"),iCurrChannelNum);
	}

	CRecordingBase::MoscoStateChangeEvent(aObject, aPreviousState, aCurrentState, err);
}
