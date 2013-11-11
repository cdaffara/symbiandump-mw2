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
* Description:  Audio stream test component
*
*/


#include "OutputStreamer.h"
#include "AudioDataSettingsParams.h"

const TInt COutputStreamer::KPlayerActionPlay = 1;
const TInt COutputStreamer::KPlayerActionStop = 3;

COutputStreamer::COutputStreamer(CStifLogger *aLogger)
	: CStreamerBase(aLogger), iAlreadyClosed(EFalse),
      iPlaybackStopped(EFalse)
{ }

void COutputStreamer::ConstructL()
{
	iMdaAudioOutputStream = CMdaAudioOutputStream::NewL(*this);
}

COutputStreamer* COutputStreamer::NewL(CStifLogger *aLogger, TInt aId)
{
	COutputStreamer *_self = NewLC(aLogger, aId);
	CleanupStack::Pop(_self);
	return _self;
}

COutputStreamer* COutputStreamer::NewLC(CStifLogger *aLogger, TInt aId)
{
	COutputStreamer *_self = new(ELeave)COutputStreamer(aLogger);
	CleanupStack::PushL(_self);
	_self->ConstructL();
	_self->SetId(aId);
	return _self;
}

void COutputStreamer::ConstructL(TInt aPriority, TMdaPriorityPreference aPref)
{
	iMdaAudioOutputStream = CMdaAudioOutputStream::NewL(*this, aPriority, aPref);
}

COutputStreamer* COutputStreamer::NewL(TInt aPriority, TMdaPriorityPreference aPref,
                                       CStifLogger *aLogger, TInt aId)
{
	COutputStreamer *_self = NewLC(aPriority, aPref, aLogger, aId);
	CleanupStack::Pop(_self);
	return _self;
}

COutputStreamer* COutputStreamer::NewLC(TInt aPriority, TMdaPriorityPreference aPref,
                                        CStifLogger *aLogger, TInt aId)
{
	COutputStreamer *_self = new(ELeave)COutputStreamer(aLogger);
	CleanupStack::PushL(_self);
	_self->ConstructL(aPriority, aPref);
	_self->SetId(aId);
	return _self;
}

COutputStreamer::~COutputStreamer()
{
	iMdaAudioOutputStream->Stop();
	delete iMdaAudioOutputStream;
	iArrBufferErrors.Close();
	TInt Before = iBuffers.Count();
	iBuffers.ResetAndDestroy();
	iLogger->Log(_L("%d) Items in buffer (%d), there were (%d)") , iId, iBuffers.Count() , Before);
	iMdaAudioOutputStream = NULL;
	iWriteBuffers.ResetAndDestroy();
}

void COutputStreamer::SetAudioPropertiesL(TInt aSampleRate, TInt aChannels)
{
	iLogger->Log(_L("%d) COutputStreamer::SetAudioPropertiesL(rate=%d, chan=%d)") , iId, aSampleRate, aChannels);
	iMdaAudioOutputStream->SetAudioPropertiesL(aSampleRate, aChannels);
}

void COutputStreamer::SetVolume(TInt aVolume)
{
	iLogger->Log(_L("%d) COutputStreamer::SetVolume(%d, %d)") , iId, aVolume);
	iMdaAudioOutputStream->SetVolume(aVolume);
}

TInt COutputStreamer::GetVolume() { return iMdaAudioOutputStream->Volume(); }

TInt COutputStreamer::GetMaxVolume() { return iMdaAudioOutputStream->MaxVolume(); }

void COutputStreamer::SetPriority(TInt aPriority, TMdaPriorityPreference aPref)
{
	iLogger->Log(_L("%d) COutputStreamer::SetPriority(%d, %d)") , iId, aPriority, aPref);
	iMdaAudioOutputStream->SetPriority(aPriority, aPref);
}

void COutputStreamer::AdoptBuffers(RPointerArray<TDes8> &aBuffers )
{
	for (TInt x=0; x<aBuffers.Count() ; ++x)
		{
		iBuffers.Append(aBuffers[x]);
		}
	aBuffers.Reset();
}

void COutputStreamer::StartPlayback(TMdaPackage *aSettings)
{
	iLogger->Log(_L("COutputStreamer::StartPlayback()") );
	iCurrentBuffer=-1;

	iMdaAudioOutputStream->Open(aSettings);
}

RArray<TInt> &COutputStreamer::GetBufferErrors(){return iArrBufferErrors;}

TInt COutputStreamer::DesToBuffers(TDes8 &aDes)
{
	TInt NumChunks = aDes.Length() / KBufferSize;
	TInt remainder = aDes.Length() % KBufferSize;

	TInt x;
	for (x=0 ; x<NumChunks ; ++x)
	{
		TDes8 *tmp = new(ELeave)TBuf8<KBufferSize>;
		CleanupStack::PushL(tmp);
		*tmp=aDes.Mid(x*KBufferSize , KBufferSize);
		if (AppendBuffer(tmp)) { iLogger->Log(_L("Error appending buffer in DesToBuffers")); }
		CleanupStack::Pop(tmp);
	}

	if (remainder > 0)
	{
		TDes8 *tmp = new(ELeave)TBuf8<KBufferSize>;
		CleanupStack::PushL(tmp);
		*tmp=aDes.Mid(x*KBufferSize , remainder);
		tmp->SetLength(remainder);

		if (AppendBuffer(tmp)) { iLogger->Log(_L("Error appending buffer in DesToBuffers")); }
		CleanupStack::Pop(tmp);
	}

	iLogger->Log(_L("COutputStreamer::DesToBuffers # Buffers created - %d"), iWriteBuffers.Count());
	return KErrNone;
}

TInt COutputStreamer::GetBalanceL()
{
	TInt retVal = iMdaAudioOutputStream->GetBalanceL();
	return retVal;
}

void COutputStreamer::SetBalanceL(TInt aBalance)
{
	iMdaAudioOutputStream->SetBalanceL(aBalance);
}

void COutputStreamer::Stop()
{
	iLogger->Log(_L("%d) COutputStreamer::Stop()") , iId);
	iMdaAudioOutputStream->Stop();
}

void COutputStreamer::Exit(TInt aExitCode)
{
	iLogger->Log(_L("%d) COutputStreamer::Exit(%d)"), iId, aExitCode);
	iFinalError = aExitCode;
	if (!iAlreadyClosed)
	{
    	iAlreadyClosed=ETrue;
		CActiveScheduler::Stop();
	}
}

TFourCC COutputStreamer::GetDataType()
{
	TFourCC retVal = iMdaAudioOutputStream->DataType();
	iLogger->Log(_L("%d) COutputStreamer::GetDataType() , result=(%d)") , iId, retVal.FourCC());
	return retVal;
}

void COutputStreamer::SetDataTypeL(TFourCC aAudioType)
{
	iLogger->Log(_L("%d) COutputStreamer::SetDataTypeL(dataType=%d)") , iId, aAudioType.FourCC());
	iMdaAudioOutputStream->SetDataTypeL(aAudioType);
}

// inherited from MMdaAudioOutputStreamCallback
void COutputStreamer::MaoscOpenComplete(TInt aError)
{
	iLogger->Log(_L("%d) COutputStreamer::MaoscOpenComplete(%d)") , iId, aError);
	if (aError || iExitAfterOpen)
	{
		Exit(aError);
		return;
	}

	iLogger->Log(_L("%d) Bytes (%d), Position (%d)") , iId, iMdaAudioOutputStream->GetBytes() , iMdaAudioOutputStream->Position());
	++iCurrentBuffer;
	if (0<=iCurrentBuffer && iCurrentBuffer<=iWriteBuffers.Count() )
	{
		iMdaAudioOutputStream->WriteL( *(iWriteBuffers[iCurrentBuffer]) );
		iLogger->Log(_L("%d) COutputStreamer::MaoscOpenComplete(), writting buffer #(%d)") , iId, iCurrentBuffer);
	}
	//for (TInt i=0; i < iWriteBuffers.Count(); i++)
	//{
	//	iMdaAudioOutputStream->WriteL( *(iWriteBuffers[i]) );
	//}
}

void COutputStreamer::MaoscBufferCopied(TInt aError, const TDesC8 &/*aBuffer*/)
{
	iLogger->Log(_L("%d) COutputStreamer::MaoscBufferCopied(%d)") , iId, aError);
	iArrBufferErrors.Append(aError);
	if (aError)
	{
		return;
	}
	iLogger->Log(_L("%d) Bytes (%d), Position (%d)") , iId, iMdaAudioOutputStream->GetBytes() , iMdaAudioOutputStream->Position());
	++iCurrentBuffer;
	if (0<=iCurrentBuffer && iCurrentBuffer<iWriteBuffers.Count() )
	{
		iMdaAudioOutputStream->WriteL( *(iWriteBuffers[iCurrentBuffer]) );
		iLogger->Log(_L("%d) COutputStreamer::MaoscBufferCopied(), writting buffer #(%d)") , iId, iCurrentBuffer);
	}
	else if (iCurrentBuffer == iWriteBuffers.Count())
	//if (iCurrentBuffer == iWriteBuffers.Count())
	{
		Exit(KErrNone);
	}
}

void COutputStreamer::MaoscPlayComplete(TInt aError)
{
	iLogger->Log(_L("%d) Bytes (%d), Position (%d)") , iId, iMdaAudioOutputStream->GetBytes() , iMdaAudioOutputStream->Position());
	iLogger->Log(_L("%d) COutputStreamer::MaoscPlayComplete(%d)") , iId, aError);

	// Underflow occurs at end of playback
	if ( (aError == KErrUnderflow) && (iCurrentBuffer == iBuffers.Count()) )
		aError = KErrNone;

	// Stop causes abort
	if (iPlaybackStopped && (aError == KErrCancel))
		aError = KErrNone;

	Exit(aError);
}

// from MEventTarget
TInt COutputStreamer::ExecuteL(CParameters *aParams)
{
	switch (aParams->iAction)
	{
		case KPlayerActionPlay:
			StartPlayback( (static_cast<CAudioDataSettingsParams*>(aParams))->GetSettings() );
			break;
		case KPlayerActionStop:
			iPlaybackStopped = ETrue;
			Stop();
			break;
	}
	return EFalse;
}

TInt COutputStreamer::AppendWriteBuffer(HBufC8 *aBuffer)
{
	return iWriteBuffers.Append(aBuffer);
}

TInt COutputStreamer::LoadAudioFileL(const TFileName &aFileName)
{
	RFs fs;	// file server instance
	RFile audiofile;

	// open fileserver session and file
	User::LeaveIfError(fs.Connect());
	// push into cleanup stack
	CleanupClosePushL(fs);

	// open file
	TInt err = audiofile.Open(fs, aFileName, EFileRead|EFileStream);
	if (err==KErrNone)
	{
		// file opened ok, proceed reading
		TInt fileSize;
		audiofile.Size(fileSize);

		TInt readPos(0);
		TInt bufLen(KBufferSize);
		HBufC8* tmpBuf(NULL);
		while (readPos < fileSize)
		{
			bufLen = Min(KBufferSize,fileSize-readPos);
			tmpBuf = HBufC8::NewL(KBufferSize);
			CleanupStack::PushL(tmpBuf);
			TPtr8 ptr(tmpBuf->Des());
			ptr.Set(const_cast<TUint8*>(ptr.Ptr()),0,bufLen);
			User::LeaveIfError(audiofile.Read(readPos,ptr,bufLen));
			tmpBuf->Des().SetLength(bufLen);
			AppendWriteBuffer(tmpBuf);
			CleanupStack::Pop(tmpBuf);
			readPos += bufLen;
		}
		iLogger->Log(_L("Loading complete!"));
	}
	else
	{
		// failed to open file
		iLogger->Log(_L("Error loading audio sample!"));
	}
	audiofile.Close();
	// pop and destroy resources pushed into stack
	CleanupStack::PopAndDestroy();

	return err;
}

