/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base class definition of the S60 Audio Stream Player
*
*/


#include "S60AudioPlayer.h"
#include "S60AudioSource.h"
#include "S60PlayerEventDispatcher.h"

CS60AudioPlayer::~CS60AudioPlayer()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioPlayer::~CS60AudioPlayer"));
#endif

	delete iRepeatTrailingSilenceTimer;
	if (iFindAndOpenController)
		iFindAndOpenController->Close();
	delete iFindAndOpenController;
	delete iControllerEventMonitor;
	delete iDispatcher;
	iController.Close();

	}

CS60AudioPlayer::CS60AudioPlayer(
	MS60AudioPlayerObserver& aObserver,
	TInt aPriority,
	TMdaPriorityPreference aPref ) :
	iObserver(aObserver),
	iSource(NULL),
	iAudioPlayDeviceCommands(iController)
	{
	iPrioritySettings.iPriority = aPriority;
	iPrioritySettings.iPref = aPref;
	}

EXPORT_C CS60AudioPlayer* CS60AudioPlayer::NewL(
	MS60AudioPlayerObserver& aObserver,
	TInt aPriority,
	TMdaPriorityPreference aPref )
	{

#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioPlayer::NewL"));
#endif

	CS60AudioPlayer* self = new(ELeave) CS60AudioPlayer(aObserver, aPriority, aPref);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CS60AudioPlayer::ConstructL()
	{

#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioPlayer::ConstructL"));
#endif
	iDispatcher = CPlayerEventDispatcher::NewL(iObserver);
	iControllerEventMonitor = CMMFControllerEventMonitor::NewL(*this, iController);
    RDebug::Print(_L("CS60AudioPlayer::ConstructL [%x]"),iControllerEventMonitor);
    iFindAndOpenController = CMMFFindAndOpenController::NewL(*this);
    RDebug::Print(_L("CS60AudioPlayer::ConstructL [%x]"),iFindAndOpenController);
	iRepeatTrailingSilenceTimer = CRepeatTrailingSilenceTimer::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::AddDataSource
// Adds the DataSource
// -----------------------------------------------------------------------------
//
void CS60AudioPlayer::AddDataSource(
	CS60AudioBaseSource& aDataSource )
	{

#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioPlayer::AddDataSource"));
#endif

	iSource = &aDataSource;

	iFindAndOpenController->Configure(KUidMediaTypeAudio, iPrioritySettings);
	iFindAndOpenController->ConfigureController(iController, *iControllerEventMonitor,
												CMMFFindAndOpenController::EPlayback);
	iFindAndOpenController->ConfigureSourceSink(CMMFFindAndOpenController::TSourceSink(TUid::Uid(0x10207AF3)),
			CMMFFindAndOpenController::TSourceSink(KUidMmfAudioOutput));

	TBuf8<20> mimeType;
	iSource->GetMimeType(mimeType);
	iFindAndOpenController->OpenByMimeType(mimeType);
	}

/**
Begins playback of the initialised audio sample at the current volume
and priority levels.

When playing of the audio sample is complete, successfully or
otherwise, the callback function
MMdaAudioPlayerCallback::MapcPlayComplete() is
called. This function raises a CMdaAudioPlayerUtility 1 panic if the
audio player utility is not initialised.
*/
EXPORT_C TInt CS60AudioPlayer::Play()
	{
	TInt err = KErrNone;
	if(iState == EPlayerPlaying)
		{

			err = KErrNotReady;

		}
	else

		{

			err = DoPlay();
		}
	return err;
	}

TInt CS60AudioPlayer::DoPlay()
	{

    TInt err = KErrNone;
    if (iState != EPlayerPaused)
        {
		if (err==KErrNone)
			err = iController.SetPosition(iPosition);
        }

	if (err==KErrNone)
		{
		err = iController.Play();

		if(!err)
			{
				iState = EPlayerPlaying;
				iDispatcher->StateChanged(iState, KErrNone);
			}

		}
	return err;
	}

/**
Stops playback of the audio sample as soon as possible.

If the audio sample is playing, playback is stopped as soon as
possible. If playback is already complete, nothing further happens as
a result of calling this function. The callback function
MMdaAudioPlayerCallback::MapcPlayComplete() is not
called. The function raises a CMdaAudioPlayerUtility 1 panic if the
audio player utility is not initialised.

*/
EXPORT_C TInt CS60AudioPlayer::Stop()
	{
TInt err = KErrNone;
    
	// cancel the repeat timer in case the client has called Stop()
	// during the trailing silence period
	//iRepeatTrailingSilenceTimer->Cancel();

	err = iController.Stop();
	
	//tpoon
	iPosition = 0;
	
	TPlayerState iOldState = iState;
	iState = EPlayerStopped;
	
	if (iOldState==EPlayerPlaying || iOldState==EPlayerPaused)
	    iDispatcher->StateChanged(iState, KErrNone);
	
	iController.Prime();
	return err;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::Pause
// Pauses the Audio Playback
// -----------------------------------------------------------------------------
//

EXPORT_C TInt CS60AudioPlayer::Pause()
		{
TInt err = KErrNone;
		if (iState==EPlayerPlaying)
			{
			// cancel the repeat timer in case the client has called Stop()
			// during the trailing silence period
			//iRepeatTrailingSilenceTimer->Cancel();

			err = iController.Pause();
			iState = EPlayerPaused;
			iDispatcher->StateChanged(iState, KErrNone);
			}
		return err;
		}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::SetPriority
// Sets the Priority of the the Audio
// returns One of the Standard Error Codes
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::SetPriority(TInt aPriority, TMdaPriorityPreference aPref)
	{
	iPrioritySettings.iPref = aPref;
	iPrioritySettings.iPriority = aPriority;
	iFindAndOpenController->Configure(KUidMediaTypeAudio, iPrioritySettings);
	return iController.SetPrioritySettings(iPrioritySettings);
	}


// -----------------------------------------------------------------------------
// CS60AudioPlayer::FastForward
// FastForwards the Current the the Audio by aStep
// returns One of the Standard Error Codes
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::FastForward(TTimeIntervalMicroSeconds& /*aStep*/)
	{
		return KErrNotSupported;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::Rewind
// Rewinds the Current the the Audio by aStep
// returns One of the Standard Error Codes
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::Rewind(TTimeIntervalMicroSeconds& /*aStep*/)
	{
		return KErrNotSupported;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::GetMaxVolume
// Gets the maximum volume supported
// returns One of the Standard Error Codes
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::GetMaxVolume()
	{
		TInt maxVolume = 0;
		#ifdef _DEBUG
			TInt error =
		#endif
		iAudioPlayDeviceCommands.GetMaxVolume(maxVolume);
		return maxVolume;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::SetVolume
// Sets the volume with aVolume
// returns One of the Standard Error Codes
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::SetVolume(TInt aVolume)
	{
		TInt err = iAudioPlayDeviceCommands.SetVolume(aVolume);
		return err;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::GetVolume
// Gets the current volume
// returns One of the Standard Error Codes
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::GetVolume(TInt& aVolume)
	{
		TInt error = iAudioPlayDeviceCommands.GetVolume(aVolume);
		return error;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::GetDuration
// Gets the current duration
// returns Duration in Microseconds
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalMicroSeconds CS60AudioPlayer::GetDuration()
	{
		TTimeIntervalMicroSeconds duration = 0;
		return duration;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::GetBitRate
// Gets the Bitrate
// returns bitrate
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CS60AudioPlayer::GetBitRate()
	{
		TUint bitRate;
		RMMFAudioControllerCustomCommands controller(iController);
		TInt err = controller.GetSourceBitRate(bitRate);
		return bitRate;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::GetPosition
// Gets the current Position
// returns position in Microseconds
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalMicroSeconds CS60AudioPlayer::GetPosition()
	{
	//	if (iState==EPlayerPlaying)
			iController.GetPosition(iPosition);
		return iPosition;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::SetPosition
// Sets the current Position
// -----------------------------------------------------------------------------
//
EXPORT_C void CS60AudioPlayer::SetPosition(TTimeIntervalMicroSeconds& aPosition)
	{
			const TTimeIntervalMicroSeconds maxPosition(GetDuration());
			const TTimeIntervalMicroSeconds minPosition(0);

			if (aPosition > maxPosition)
				iPosition = maxPosition;
			else if (aPosition < minPosition)
				iPosition = minPosition;
			else
				iPosition = aPosition;

			if (iState==EPlayerPlaying || iState==EPlayerPaused)
				{
				iController.SetPosition(iPosition);
				}

	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::SetPlayWindow
// Sets the current Playwindow Position
// -----------------------------------------------------------------------------
//
EXPORT_C void CS60AudioPlayer::SetPlayWindow(TTimeIntervalMicroSeconds& /*aStart*/, TTimeIntervalMicroSeconds& /*aEnd*/)
	{
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::GetPlayWindow
// Gets the current Playwindow Position
// -----------------------------------------------------------------------------
//
EXPORT_C void CS60AudioPlayer::GetPlayWindow(TTimeIntervalMicroSeconds& /*aStart*/, TTimeIntervalMicroSeconds& /*aEnd*/)
	{
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::ResetPlayWindow
// Resets the Playwindow
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::ResetPlayWindow()
	{
		return KErrNotSupported;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::SetRepeats
// Sets the number of times the Audio should be played
// -----------------------------------------------------------------------------
//
EXPORT_C void CS60AudioPlayer::SetRepeats(TInt /*aNumber*/, const TTimeIntervalMicroSeconds& /*aTrailingSilence*/)
	{
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::GetBalance
// Gets the current Balance
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::GetBalance()
	{
		TInt balance;
		TInt err = iAudioPlayDeviceCommands.GetBalance(balance);
		return balance;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::SetBalance
// Sets the current Balance
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioPlayer::SetBalance(TInt& aBalance)
	{
		TInt err = iAudioPlayDeviceCommands.SetBalance(aBalance);
		return err;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::IsSeekingSupported
// Gets the current Seeking Support
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CS60AudioPlayer::IsSeekingSupported()
	{
		return EFalse;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::IsRandomSeekingSupported
// Gets the current RandomSeeking Support
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CS60AudioPlayer::IsRandomSeekingSupported()
	{
		return EFalse;
	}

// -----------------------------------------------------------------------------
// CS60AudioPlayer::CustomInterface
// Gets the CustomInterface
// -----------------------------------------------------------------------------

EXPORT_C TAny* CS60AudioPlayer::CustomInterface(TUid /*aInterfaceId*/)
	{
	TAny* result = NULL;
	return result;
	}

/****************************************************************************************************
 MRepeatTrailingSilenceTimerObs implementation
****************************************************************************************************/
void CS60AudioPlayer::RepeatTrailingSilenceTimerComplete()
	{
	}

/****************************************************************************************************
 MMMFFindAndOpenControllerObserver implementation
****************************************************************************************************/

void CS60AudioPlayer::MfaocComplete(
		TInt& aError,
		RMMFController* aController,
		TUid aControllerUid,
		TMMFMessageDestination* aSourceHandle,
		TMMFMessageDestination* /*aSinkHandle*/)
	{

#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioPlayer::MfaocComplete"));
#endif

	if (aError == KErrNone)
		{
		iSource->iSourceHandle = *aSourceHandle;
		iSource->iControllerUid = aControllerUid;
		iSource->iController = *aController;
		}
	iDispatcher->AddSourceComplete(aError);
	iController.Prime();
	}

/****************************************************************************************************
 MMMFControllerEventMonitorObserver implementation
****************************************************************************************************/

void CS60AudioPlayer::HandleEvent(const TMMFEvent& aEvent)
	{

#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioPlayer::HandleEvent"));
#endif

	// handle loading started/complete messages first, as the later code does not explicitly check the event type

	if (aEvent.iEventType == KMMFEventCategoryPlaybackComplete)
		{
				//we've repeated enough times now
				iState = EPlayerStopped;
				iPosition = 0;
				iDispatcher->StateChanged(iState, aEvent.iErrorCode);
				iController.Prime();
		}
	// else we have an unexpected event that cannot be dealt with by the client.
	// We will simply ignore this.
	}


/**************************************************************************************************/

CRepeatTrailingSilenceTimer* CRepeatTrailingSilenceTimer::NewL(MRepeatTrailingSilenceTimerObs& aObs)
	{
	CRepeatTrailingSilenceTimer* s = new(ELeave) CRepeatTrailingSilenceTimer(aObs);
	CleanupStack::PushL(s);
	s->ConstructL();
	CleanupStack::Pop(s);
	return s;
	}

void CRepeatTrailingSilenceTimer::RunL()
	{
	iObs.RepeatTrailingSilenceTimerComplete();
	}

CRepeatTrailingSilenceTimer::CRepeatTrailingSilenceTimer(MRepeatTrailingSilenceTimerObs& aObs) :
	CTimer(EPriorityHigh),
	iObs(aObs)
	{
	CActiveScheduler::Add(this);
	}

