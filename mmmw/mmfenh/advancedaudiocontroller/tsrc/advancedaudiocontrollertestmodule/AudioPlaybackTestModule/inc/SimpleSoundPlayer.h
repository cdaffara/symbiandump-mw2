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
* Description:  mmftsplay test component
*
*/

#ifndef __SOUND_PLAYER_H
#define  __SOUND_PLAYER_H

#include <mdaaudiosampleplayer.h>
#include <StifTestModule.h>
#include "EventTarget.h"
#include "Parameters.h"

#include <e32std.h>
#include <e32base.h>
#include <StifLogger.h>
#include <MdaAudioSamplePlayer.h>
#include "FileNameParameters.h"
#include "Des8Parameters.h"
//#include "FileHandlerParameters.h"
#include "UrlParameters.h"
#include "ObjectCountDown.h"
#include <audiooutput.h>

const TInt KRunCaseOpen = 2;
const TInt KSubCaseNone = 0;
//Error
const TInt KErrIncorrect = -1502;	//-1101

const TInt KRepeatTimes = 3;
const TUid KUidInterfaceMMFController = {KMmfUidPluginInterfaceController};


class CSimpleSoundPlayer: public CBase , public MEventTarget , public MMdaAudioPlayerCallback,
public MAudioLoadingObserver, public MMMFAudioResourceNotificationCallback
	{
public:
	static CSimpleSoundPlayer* NewUrlL(const TDesC &aDescriptor, TInt aIapId, const TDesC8 &aMime, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate=ETrue);
	static CSimpleSoundPlayer* NewUrlLC(const TDesC &aDescriptor, TInt aIapId, const TDesC8 &aMime, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate=ETrue);

	static CSimpleSoundPlayer* NewL(const TDesC8 &aDescriptor, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate=ETrue);
	static CSimpleSoundPlayer* NewLC(const TDesC8 &aDescriptor, CTestModuleIf &aConsole, CStifLogger &aLogger, TBool aImmediate=ETrue);

	static CSimpleSoundPlayer* NewL(const TFileName &aFile, CTestModuleIf &aConsole, CStifLogger &iLogger, TBool aImmediate=ETrue);
	static CSimpleSoundPlayer* NewLC(const TFileName &aFile, CTestModuleIf &aConsole, CStifLogger &iLogger, TBool aImmediate=ETrue);

	static CSimpleSoundPlayer* NewL(const RFile &aFile, CTestModuleIf &aConsole, CStifLogger &iLogger, TBool aImmediate=ETrue);
	static CSimpleSoundPlayer* NewLC(const RFile &aFile, CTestModuleIf &aConsole, CStifLogger &iLogger, TBool aImmediate=ETrue);
	~CSimpleSoundPlayer();


    static CSimpleSoundPlayer* NewL(CTestModuleIf &aConsole, CStifLogger &iLogger, TBool aUseSharedHeap = EFalse);
	static CSimpleSoundPlayer* NewLC(CTestModuleIf &aConsole, CStifLogger &iLogger, TBool aUseSharedHeap);


	// from MMdaAudioPlayerCallback
	virtual void MapcInitComplete(TInt aError, const TTimeIntervalMicroSeconds& aDuration);
	virtual void MapcPlayComplete(TInt aError);
	virtual void DoCancel();

	virtual void MaloLoadingStarted();
	virtual void MaloLoadingComplete();

	virtual void MarncResourceAvailable(TUid aNotificationEventId, const TDesC8 &aNotificationData);

	void SetVolumeRamp(const TTimeIntervalMicroSeconds &aRampDuration);
	void SetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds &aTrailingSilence);
	void SetPosition(const TTimeIntervalMicroSeconds &aPosition);
	TInt SetPlayWindow(const TTimeIntervalMicroSeconds &aStart, const TTimeIntervalMicroSeconds &aEnd);
	TTimeIntervalMicroSeconds GetPlayDuration();
	TTime GetPlayStartTime();
	TTime GetPlayStopTime();
	TInt GetBalance(TInt &aBalance);
	TInt SetBalance(TInt aBalance);
	TInt SetPriority(TInt aPriority, TMdaPriorityPreference aPref);
	TInt SetThread_Priority();
	TTimeIntervalMicroSeconds GetDuration();
	TTimeIntervalMicroSeconds GetDuration_Para();
	TInt ClearPlayWindow();
	void GetConImpInfo();
	void GetLoad_Percentage();
	void GetBit_Rate();
	void GetDRMCmd();
	void CancelNotification();
	void CtrImpInfo();
	void Set_Priority();

	TInt Play();
	TInt OpenDesL(const TDesC8 &aSoundDes , const TBool &aImmediate=ETrue) ;
	TInt GetPosition(TTimeIntervalMicroSeconds &aPosition);

	void SetSupervisor(TObjectCountDown *aCounter) ;
	void SetPlayCompleteTarget(MEventTarget *aTarget, CParameters *aParameters) ;

	void SetVolume(TInt aNewVolume);
	TInt GetVolume(TInt &aVolume);
	TInt MaxVolume();

	TInt GetNumberOfMetaDataEntries(TInt &aNumEntries);
	CMMFMetaDataEntry *GetMetaDataEntryL(TInt aMetaDataIndex);

	TInt OpenFile(const TFileName &aFile);
	TInt OpenWithFileHandler(const RFile &aFile);
    TInt OpenWithFileSource(const TMMSource &aSource);
    TInt OpenWithDesc(const TDesC8 &aDescriptor);

    void RegNotification();
	void RegLoadNotification();

	void SetRunCase(TInt aRunCase);
    void SetSubCase(TInt aSubCase);
    
 
    void AudioOutputCreationL();
    void LoopPlayL();
   


private:

    TInt Open_UrlL(const TDesC &aUrl, TInt aIapId, const TDesC8 &aMime) ;

	TInt OpenUrlL(const TDesC &aUrl, TInt aIapId, const TDesC8 &aMime) ;
	TInt Pause();
	TInt PauseResume();
	TInt OpenL(const TFileName &aFileName , const TBool &aImmediate);
	TInt OpenFileHandlerL(const TFileName &aFileName , const TBool &aImmediate);
	TInt Stop(const TBool &aAlsoClose=ETrue);

	CSimpleSoundPlayer(CTestModuleIf &aConsole, CStifLogger &iLogger, TBool aImmediate=ETrue);
	void ConstructUrlL(const TDesC &aDescriptor, TInt aIapId, const TDesC8 &aMime);
	void ConstructL(const TDesC8 &aDescriptor);
	void ConstructL(const TFileName &aFile);
	void ConstructL(const RFile &aFile);
	void ConstructL(TBool aUseSharedHeap);


	void Exit(TInt aExitCode);




private:
	MEventTarget *iEventTarget;
	CParameters *iParameters;

	TObjectCountDown *iSupervisor;
	CMdaAudioPlayerUtility* iMdaPlayer;
	CMdaAudioPlayerUtility* iMdaPlayer_1;
	CMdaAudioPlayerUtility* iMdaPlayer_2;
	CMdaAudioPlayerUtility* iMdaPlayer_3;
	CTestModuleIf &iConsole;
	TBool iImmediate;
	CStifLogger &iLogger;	
	CAudioOutput* iAudioOutput;
		
	TTime iLastPlayTime;
	TTime iLastStopTime;


public:
	static const TInt KPlayerActionPlay;
	static const TInt KPlayerActionPause;
	static const TInt KPlayerActionPauseResume;
	static const TInt KPlayerActionStopPlayFile;
	static const TInt KPlayerActionReportPosition;
	static const TInt KPlayerActionReportDuration;
	static const TInt KPlayerActionReportDuration_Para;
	static const TInt KPlayerActionStopPlayDes;
	static const TInt KPlayerActionReportVolume;
	static const TInt KPlayerActionStopPlayUrl;
	static const TInt KPlayerActionStopPlayFileHandler;

	TInt ExecuteL(CParameters *aParams) ;

	enum TState
	{
		ENotReady,
		EReady,
		EPlaying,
		EPaused
	};
	TInt iPlayerId;
	TState iState;
	TBool iSoundOpen;
	/////TBool iAutoClose;
	TInt iFinalError;
	TTimeIntervalMicroSeconds iPosition;
	TTimeIntervalMicroSeconds startPosition;
	TTimeIntervalMicroSeconds endPosition;
	TInt aBalance;
	TBool playVolume;
	TBool metaInfo;
	TBool playWindow;
	
	TBool iNegativePlayBackWindow;
	TBool clearWindow;
	TBool setBalance;
	TBool iDontPlayAudioClip; // Binh

	TInt iRunCase;
    TInt iSubCase;

private:
    TBool iIsLoopPlayEnabled;

};

#endif //__SOUND_PLAYER_H

