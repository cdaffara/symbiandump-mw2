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


#ifndef STREAMER_BASE_H
#define STREAMER_BASE_H

//#include <StifTestInterface.h>
#include <MdaAudioOutputStream.h>
#include <StifLogger.h>
#include "mda/common/audio.h"
#include "EventTarget.h"
#include "AudioStreamConstants.h"
//#include "TestModuleConstants.h"

class CStreamerBase : public CBase , public MEventTarget
	{
private:
	void Exit(TInt aExitCode);
protected:
	CStreamerBase(CStifLogger *aLogger);

public:
	~CStreamerBase();

	virtual void SetPriority(TInt aPriority, TMdaPriorityPreference aPref)=0;
	virtual TInt GetBalanceL()=0;
	virtual void SetBalanceL(TInt aBalance)=0;

	void SetId(TInt aId);

	TInt GetId();
	TInt GetFinalError();

	TTimeIntervalMicroSeconds GetExecDuration();
	TTime GetStartTime();
	TTime GetStopTime();

	TInt AppendBuffer(TDes8 *aBuffer);
	TMdaAudioDataSettings::TAudioCaps TranslateRate(TInt aSampleRate, TMdaAudioDataSettings::TAudioCaps aDefaultSampleRate);

	void SetExitAfterOpen(TBool aFlag);

public:
	static const TInt KBitRate8;
	static const TInt KSampleRate8000Hz;
	static const TInt KSampleRate11025Hz;
	static const TInt KSampleRate12000Hz;
	static const TInt KSampleRate16000Hz;
	static const TInt KSampleRate22050Hz;
	static const TInt KSampleRate24000Hz;
	static const TInt KSampleRate32000Hz;
	static const TInt KSampleRate44100Hz;
	static const TInt KSampleRate48000Hz;
	static const TInt KSampleRate64000Hz;
	static const TInt KSampleRate96000Hz;

	static const TInt KChannelsMono;
	static const TInt KChannelsStereo;

protected:
	TInt iFinalError;
	TInt iId;

	//CStifLogger *iLogger;
	CStifLogger *iLogger;

	RPointerArray<TDes8> iBuffers;
	TInt iCurrentBuffer;

	TBool iPlayInmediately;
	TBool iExitAfterOpen;

	TTime iLastStartTime;
	TTime iLastStopTime;

private:
	};

#endif      // STREAMER_BASE_H
