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
* Description:  Audio Stream Test Component
*
*/

#include "InputStreamer.h"

const TInt CInputStreamer::KRecorderActionStop = 3;
const TInt CInputStreamer::KSchedulerStop = 4;

CInputStreamer::CInputStreamer(CStifLogger *aLogger)
	:	CStreamerBase(aLogger) ,  iAlreadyClosed(EFalse) , iRecording(EFalse)
{
}

void CInputStreamer::ConstructL()
{
    iMdaAudioInputStream = CMdaAudioInputStream::NewL(*this);
}

CInputStreamer* CInputStreamer::NewL(CStifLogger *aLogger, TInt aId)
{
	CInputStreamer *_self = NewLC(aLogger, aId);
	CleanupStack::Pop(_self);
	return _self;
}

CInputStreamer* CInputStreamer::NewLC(CStifLogger *aLogger, TInt aId)
{
	CInputStreamer *_self = new(ELeave)CInputStreamer(aLogger);
	CleanupStack::PushL(_self);
	_self->ConstructL();
	_self->SetId(aId);
	return _self;
}

void CInputStreamer::ConstructL(TInt aPriority, TMdaPriorityPreference aPref)
{
	iMdaAudioInputStream = CMdaAudioInputStream::NewL(*this, aPriority, aPref);
}

CInputStreamer* CInputStreamer::NewL(TInt aPriority, TMdaPriorityPreference aPref,
                                     CStifLogger *aLogger, TInt aId)
{
	CInputStreamer *_self = NewLC(aPriority, aPref, aLogger, aId);
	CleanupStack::Pop(_self);
	return _self;
}

CInputStreamer* CInputStreamer::NewLC(TInt aPriority, TMdaPriorityPreference aPref,
                                      CStifLogger *aLogger, TInt aId)
{
	CInputStreamer *_self = new(ELeave)CInputStreamer(aLogger);
	CleanupStack::PushL(_self);
	_self->ConstructL(aPriority, aPref);
	_self->SetId(aId);
	return _self;
}


CInputStreamer::~CInputStreamer()
{

	delete iMdaAudioInputStream;
	delete iSettings;
	TInt Before = iBuffers.Count();
	iBuffers.ResetAndDestroy();
	iBuffers.Close();
	iLogger->Log(_L("%d) Items in buffer (%d), there were (%d)") , iId, iBuffers.Count() , Before);
	iMdaAudioInputStream = NULL;
}

void CInputStreamer::SetAudioPropertiesL(TInt aSampleRate, TInt aChannels)
{
	iLogger->Log(_L("%d) CInputStreamer::SetAudioPropertiesL(rate=%d, chan=%d)") , iId, aSampleRate, aChannels);
	iMdaAudioInputStream->SetAudioPropertiesL(aSampleRate, aChannels);
}

void CInputStreamer::SetPriority(TInt aPriority, TMdaPriorityPreference aPref)
{
	iLogger->Log(_L("%d) CInputStreamer::SetPriority(%d, %d)") , iId, aPriority, aPref);
	iMdaAudioInputStream->SetPriority(aPriority, aPref);
}

TInt CInputStreamer::GetBalanceL()
{
	TInt retVal = iMdaAudioInputStream->GetBalanceL();
	iLogger->Log(_L("%d) CInputStreamer::GetBalanceL() , result=(%d)") , iId, retVal);
	return retVal;
}

void CInputStreamer::SetBalanceL(TInt aBalance)
{
	iLogger->Log(_L("%d) CInputStreamer::SetBalanceL(%d)") , iId, aBalance);
	iMdaAudioInputStream->SetBalanceL(aBalance);
}

void CInputStreamer::SetGain(const TInt aNewGain) { iMdaAudioInputStream->SetGain(aNewGain); }
TInt CInputStreamer::GetMaxGain() { return iMdaAudioInputStream->MaxGain(); }
TInt CInputStreamer::GetGain() { return iMdaAudioInputStream->Gain(); }

void CInputStreamer::Stop()
{
	iLogger->Log(_L("%d) CInputStreamer::Stop()") , iId);
	iRecording=EFalse;
	iState=EStopped;
	iMdaAudioInputStream->Stop();
}

TFourCC CInputStreamer::GetDataType()
{
	TFourCC retVal = iMdaAudioInputStream->DataType();
	iLogger->Log(_L("%d) CInputStreamer::GetDataType() , result=(%d)") , iId, retVal.FourCC());
	return retVal;
}

void CInputStreamer::SetDataTypeL(TFourCC aDataType)
{
	iLogger->Log(_L("%d) CInputStreamer::SetDataTypeL(%d)") , iId, aDataType.FourCC());
	iMdaAudioInputStream->SetDataTypeL(aDataType);
}

void CInputStreamer::GetSupportedBitRatesL(RArray<TInt>& aSupportedBitRates)
{
    iMdaAudioInputStream->GetSupportedBitRatesL(aSupportedBitRates);
}

TInt CInputStreamer::BitRateL()
{
    TInt retVal = iMdaAudioInputStream->BitRateL();
	iLogger->Log(_L("%d) CInputStreamer::BitRateL() , result=(%d)") , iId, retVal);
	return retVal;
}

void CInputStreamer::SetBitRateL(TInt aBitRate)
{
	iLogger->Log(_L("%d) CInputStreamer::SetBitRateL(%d)") , iId, aBitRate);
	iMdaAudioInputStream->SetBitRateL(aBitRate);
}

RPointerArray<TDes8> &CInputStreamer::GetBuffers()
{
	return iBuffers;
}

void CInputStreamer::StartRecording()
{
	iLogger->Log(_L("CInputStreamer::StartRecording()") );
	iMdaAudioInputStream->Open(iSettings);
}

void CInputStreamer::Exit(TInt aExitCode)
{
	iLogger->Log(_L("%d) CInputStreamer::Exit(%d)"), iId, aExitCode);
	iFinalError = aExitCode;
	if (!iAlreadyClosed)
	{
		iAlreadyClosed=ETrue;
		CActiveScheduler::Stop();
	    iLogger->Log(_L("%d) Stop Active Scheduler"), iId);
	}
}

void CInputStreamer::SetSettings(TMdaPackage *aSettings)
{
	iSettings=aSettings;
}

void CInputStreamer::MaiscOpenComplete(TInt aError)
{
	iLogger->Log(_L("%d) CInputStreamer::MaiscOpenComplete(%d)") , iId, aError);
	iLogger->Log(_L("Error=(%d)") , aError);
	if (aError || iExitAfterOpen)
		{
		Exit(aError);
		return;
		}

	iState=ERecording;
	if (iState == EClosed)
		{
		iState = EFirstOpenSuccessful;
		}
	else
		{
		iState = EOpen;
		}
	iLogger->Log(_L("%d) MaiscOpenComplete: Bytes (%d), Position (%d)") , iId, iMdaAudioInputStream->GetBytes() ,
	                                                                      iMdaAudioInputStream->Position() );
	iRecording=ETrue;

	++iCurrentBuffer;
	TDes8 *tmp = new(ELeave)TBuf8<KBufferSize>;
	tmp->SetMax();
	CleanupStack::PushL(tmp);

	if (iBuffers.Append(tmp)) { iLogger->Log(_L("Error appending in openComplete")); }
	CleanupStack::Pop(tmp);
	iLogger->Log(_L("%d) CInputStreamer::MaiscOpenComplete(), adding buffer #(%d)") , iId, iCurrentBuffer);
	iMdaAudioInputStream->ReadL( *iBuffers[iBuffers.Count()-1] );

}

void CInputStreamer::MaiscBufferCopied(TInt aError, const TDesC8 &aBuffer)
{
	iLogger->Log(_L("%d) CInputStreamer::MaiscBufferCopied(%d)") , iId, aError);
	iLogger->Log(_L("  Buffer's length=(%d)") , aBuffer.Length() );
	iLogger->Log(_L("Error=(%d)") , aError);

	if (aError)
	{
		iRecording=EFalse;
		if (iState == EStopped)
		    Exit(KErrNone);
		else
		    Exit(aError);
		//TODO: remove previous line when the MaiscRecordComplete callback gets called correctly
		return;
	}

	iLogger->Log(_L("%d) MaiscBufferCopied: Bytes (%d), Position (%d)") , iId, iMdaAudioInputStream->GetBytes() ,
	                                                                      iMdaAudioInputStream->Position() );
	++iCurrentBuffer;
	TDes8 *tmp = new(ELeave)TBuf8<KBufferSize>;
	tmp->SetMax();
	CleanupStack::PushL(tmp);

	if (iBuffers.Append(tmp)) { iLogger->Log(_L("Error appending in openComplete")); }
	CleanupStack::Pop(tmp);

	iLogger->Log(_L("%d) CInputStreamer::MaiscBufferCopied(), Reading buffer #(%d)") , iId, iCurrentBuffer);
	iMdaAudioInputStream->ReadL( *iBuffers[iBuffers.Count()-1] );
}

void CInputStreamer::MaiscRecordComplete(TInt aError)
{
	iLogger->Log(_L("%d) MaiscRecordComplete: Bytes (%d), Position (%d)") , iId, iMdaAudioInputStream->GetBytes() ,
	                                                                        iMdaAudioInputStream->Position() );
	iLogger->Log(_L("%d) CInputStreamer::MaiscRecordComplete(%d)") , iId, aError);
	iLogger->Log(_L("Error=(%d)") , aError);
	iRecording=EFalse;
	Exit(aError);
	return;
}

// Callback from MEventTarget
TInt CInputStreamer::ExecuteL(CParameters *aParams)
{
	switch (aParams->iAction)
	{
		case KRecorderActionStop:
			Stop();
			break;
	    case KSchedulerStop:
			CActiveScheduler::Stop();
			break;
	}
	return EFalse;
}

