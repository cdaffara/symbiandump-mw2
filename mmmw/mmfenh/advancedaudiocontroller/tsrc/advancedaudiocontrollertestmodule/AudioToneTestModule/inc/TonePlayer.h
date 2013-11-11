/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MmfTsTone test module.
*
*/


#ifndef TONE_PLAYER_H
#define TONE_PLAYER_H

#include <Stiftestinterface.h>
#include <MdaAudioTonePlayer.h>
#include <StifLogger.h>
#include "EventTarget.h"
#include "ObjectCountDown.h"
#include "ActiveScheduledTestModuleBase.h"

class CTonePlayer : public CBase , public MEventTarget , public MMdaAudioToneObserver
	{
private:
	//CTonePlayer(CLogger *aLogger);
	CTonePlayer(COutputPrinter *aOutputPrinter);

	void Exit(TInt aExitCode);
	void ConstructL(TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately=ETrue);
	void ConstructL();

	void CancelPrepare();

public:
	// Standard two-phase construction
	//static CTonePlayer* NewL(CLogger *aLogger, TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately=ETrue);
	//static CTonePlayer* NewLC(CLogger *aLogger, TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately=ETrue);

	//static CTonePlayer* NewL(CLogger *aLogger);
	//static CTonePlayer* NewLC(CLogger *aLogger);

	static CTonePlayer* NewL(COutputPrinter *aOutputPrinter, TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately=ETrue);
	static CTonePlayer* NewLC(COutputPrinter *aOutputPrinter, TInt aFrequency, const TTimeIntervalMicroSeconds& aDuration, TBool aPlayInmediately=ETrue);

	static CTonePlayer* NewL(COutputPrinter *aOutputPrinter);
	static CTonePlayer* NewLC(COutputPrinter *aOutputPrinter);

	~CTonePlayer();
	TBool Play();
	void Stop();
	void SetVolumeRamp(TTimeIntervalMicroSeconds aRamp);
	void SetVolume(TInt Volume);
	TInt GetVolume();
	TInt GetMaxVolume();
	void SetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds &aTrailingSilence);
	TInt GetFixedSequenceCount();
	const TDesC &GetFixedSequenceName(TInt aSequenceNumber);
	void PrepareTone(TInt aFreq, const TTimeIntervalMicroSeconds &aToneDuration, TBool aPlayInmediately=ETrue);
	void PrepareFixedSequence(TInt aSequenceNumber, TBool aPlayInmediately=ETrue);
	void PrepareFixedSequenceCustomInterface(TInt aSequenceNumber, TBool aPlayInmediately=ETrue); 
	void PrepareFileSequence(const TDesC &aFileName, TBool aPlayInmediately=ETrue);
	void PrepareRFileSequence(const TDesC &aFileName, TBool aPlayInmediately=ETrue);
    void PrepareDTMFSequence(const TDesC &aSequence, TBool aPlayInmediately=ETrue);
	void PrepareDesSequence(const TDesC8 &aSequence, TBool aPlayInmediately=ETrue);
	void PrepareDualTone(TInt aFreq1, TInt aFreq2, const TTimeIntervalMicroSeconds &aToneDuration, TBool aPlayInmediately=ETrue);
	void SetPriority(TInt aPriority, TMdaPriorityPreference aPref);

	void SetId(TInt aId);
	TInt GetId();
	TInt GetFinalError();
	TInt GetLastPrepareCompleteError();
	TInt GetPrepareCompleteCallbackCount();

	void SetSupervisor(TObjectCountDown *aCounter);
	void SetExitCompleteTarget(MEventTarget *aTarget, CParameters *aParameters) ;
	TTimeIntervalMicroSeconds GetPlayDuration();
	TTime GetPlayStartTime();
	TTime GetPlayStopTime();
	TInt GetBalanceL();
	void SetBalanceL(TInt aBalance);
	void SetDTMFLengths(TTimeIntervalMicroSeconds32 iToneOnLength, TTimeIntervalMicroSeconds32 iToneOffLength, TTimeIntervalMicroSeconds32 iPauseLength);
	// inherited from MMdaAudioToneObserver
	void MatoPrepareComplete(TInt aError);
	void MatoPlayComplete(TInt aError);
	TInt ExecuteL(CParameters *aParams);
	TInt State();

public:	
	static const TInt KPlayerActionPlay;
	static const TInt KPlayerActionReportVolume;
	static const TInt KPlayerActionStop;
	static const TInt KPlayerActionCancelPrepare;

	static const TInt KPlayerActionPrepareFileSequence;

	static const TInt KPlayerActionPrepareFixedSequence;
	static const TInt KPlayerActionPrepareTone;
	static const TInt KPlayerActionPrepareDualTone;
	static const TInt KPlayerActionPrepareDTMFSequence;
	static const TInt KPlayerActionPrepareDesSequence;
	static const TInt KPlayerActionChangeVolumeToMax;
	static const TInt KPlayerActionChangeVolumeToAvg;
	static const TInt KPlayerActionChangeVolumeToZero;

	enum TPrepare {
		EPrepareTone=1,
		EPrepareFixedSequence,
		EPrepareFileSequence,
		
		EPrepareDesSequence,
		EPrepareDTMF,
		EPrepareDualTone
	};
enum TPlay
    {
    EPlayTone=1,
    EPlayFixedSequence,
    EPlayFileSequence,
    EPlayDesSequence,
    EPlayDTMF,
    EPlayDualTone
    };

enum TState
    {
    ETonePlayerNotReady,
    ETonePlayerPrepared,
    ETonePlayerPlaying
    };
private:
	TObjectCountDown *iSupervisor;
	TInt iId;
	TInt iFinalError;
	//const CLogger *iLogger;

	TBool iFreeToPrepare;
	TBool iFreeToPlay;
	TInt iFrequency;
	TBool iPlayInmediately;
	TTimeIntervalMicroSeconds iDuration;
	CMdaAudioToneUtility *iMdaAudioToneUtility;
	TTime iLastPlayTime;
	TTime iLastStopTime;
	TInt iLastPrepareError;
	TInt iPrepareCalledCounter;
	//TTimeIntervalMicroSeconds iPlayInterval;

	MEventTarget *iExitEventTarget;
	CParameters *iExitEventParameters;
	COutputPrinter *iOutputPrinter;
	};

#endif      // TONE_PLAYER_H
