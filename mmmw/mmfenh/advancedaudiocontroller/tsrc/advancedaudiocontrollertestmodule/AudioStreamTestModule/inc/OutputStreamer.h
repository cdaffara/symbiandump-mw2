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


#ifndef OUTPUT_STREAMER_H
#define OUTPUT_STREAMER_H

//#include <StifTestInterface.h>
#include <MdaAudioOutputStream.h>
#include <StifLogger.h>
#include "mda/common/audio.h"
//#include "EventTarget.h"
#include "StreamerBase.h"
//#include "AudioDataSettingsParams.h"
//#include "OutputPrinter.h"

class COutputStreamer : public CStreamerBase , public MMdaAudioOutputStreamCallback	// , public MEventTarget
	{
private:
	COutputStreamer(CStifLogger *aLogger);
	void Exit(TInt aExitCode);
	void ConstructL();
	void ConstructL(TInt aPriority, TMdaPriorityPreference aPref);

public:
	// Standard two-phase construction
	static COutputStreamer* NewL(CStifLogger *aLogger, TInt aId=0);
	static COutputStreamer* NewLC(CStifLogger *aLogger, TInt aId=0);
	static COutputStreamer* NewL(TInt aPriority, TMdaPriorityPreference aPref,
	                             CStifLogger *aLogger, TInt aId=0);
	static COutputStreamer* NewLC(TInt aPriority, TMdaPriorityPreference aPref,
	                              CStifLogger *aLogger, TInt aId=0);

	~COutputStreamer();

    void SetAudioPropertiesL(TInt aSampleRate, TInt aChannels);
	void SetVolume(TInt Volume);
	TInt GetVolume();
	TInt GetMaxVolume();

	void SetPriority(TInt aPriority, TMdaPriorityPreference aPref);
	TInt GetBalanceL();
	void SetBalanceL(TInt aBalance);
	void Stop();
	TFourCC GetDataType();
	void SetDataTypeL(TFourCC aAudioType);

	void AdoptBuffers(RPointerArray<TDes8> &aBuffers );
	void StartPlayback(TMdaPackage *aSettings);
	RArray<TInt> &COutputStreamer::GetBufferErrors();
	void SetIsClosedLoop(TBool=EFalse);
	TInt DesToBuffers(TDes8 &aDes);

	TInt ExecuteL(CParameters *aParams);

	TInt AppendWriteBuffer(HBufC8 *aBuffer);
	TInt LoadAudioFileL(const TFileName &aFileName);

	// iherited from MMdaAudioOutputStreamCallback
	void MaoscOpenComplete(TInt aError);
	void MaoscBufferCopied(TInt aError, const TDesC8 &aBuffer);
	void MaoscPlayComplete(TInt aError);

public:
	static const TInt KPlayerActionPlay;
	static const TInt KPlayerActionStop;

private:
	CMdaAudioOutputStream *iMdaAudioOutputStream;
	RArray<TInt> iArrBufferErrors;
	TBool iAlreadyClosed;
	TBool iPlaybackStopped;

	RPointerArray<HBufC8> iWriteBuffers;

protected:
	};

#endif      // OUTPUT_STREAMER_H
