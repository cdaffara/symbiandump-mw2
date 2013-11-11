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
* Description:  Base class definition of the S60 Audio Player
*
*/

#ifndef __CS60AUDIOPLAYER_H__
#define __CS60AUDIOPLAYER_H__

#include <e32std.h>
#include <e32base.h>
#include <mmf/common/mmfcontroller.h>
#include <mmf/common/mmfaudio.h>
#include <mmf/server/mmffile.h>
#include <mmf/server/mmfdes.h>
#include <mmfcontrollerimplementationuids.hrh>
#include <mmf/common/mmfstandardcustomcommands.h>
#include <mmf/common/mmfdrmcustomcommands.h>
#include <mmf/common/mmfcontrollerframeworkbase.h>

#include "S60AudioUtility.h"
#include "MS60AudioPlayerObserver.h"
#include "S60AudioSource.h"
#include <MCustomInterface.h>


//FORWARD DECLARATION
class CPlayerEventDispatcher;

// CLASS DEFINITIONS

/**
Mixin class to allow notification that the timed silence has finished.
*/
class MRepeatTrailingSilenceTimerObs
	{
public:
	virtual void RepeatTrailingSilenceTimerComplete() = 0;
	};


/**
CTimer-based active object that waits the requested time before notifying its observer.
*/
class CRepeatTrailingSilenceTimer : public CTimer
	{
	public:
		static CRepeatTrailingSilenceTimer* NewL(MRepeatTrailingSilenceTimerObs& aObs);
		void RunL();
	private:
		CRepeatTrailingSilenceTimer(MRepeatTrailingSilenceTimerObs& aObs);
	private:
		MRepeatTrailingSilenceTimerObs& iObs;
	};


/**
*  This class defines the common S60 audio player functions.
*
*  @lib S60AudioPlayer.lib
*  @since 3.1
*/
class CS60AudioPlayer: public CBase,
					   public MMMFControllerEventMonitorObserver,
					   public MRepeatTrailingSilenceTimerObs,
					   public MMMFFindAndOpenControllerObserver,
					   public MCustomInterface
	{

	public: // Constructors and destructor

		IMPORT_C static CS60AudioPlayer* NewL( MS60AudioPlayerObserver& aObserver,
											   TInt aPriority = EMdaPriorityNormal,
											   TMdaPriorityPreference aPref = EMdaPriorityPreferenceTimeAndQuality);

		/**
        *	Destructor
        */
		virtual ~CS60AudioPlayer();

    public: // New functions

		/**
		Begins playback of the initialised audio sample at the current volume
		and priority levels.

		When playing of the audio sample is complete, successfully or
		otherwise, the callback function
		MS60AudioPlayerObserver::StateChanged() is
		called.

		@since	3.1
		@lib    "S60Audio.lib"
		*/
		IMPORT_C virtual TInt Play();

		/**
		Stops playback of the audio sample as soon as possible.

		If the audio sample is playing, playback is stopped as soon as
		possible. If playback is already complete, nothing further happens as
		a result of calling this function. The callback function
		MS60AudioPlayerObserver::StateChanged() is called.

		@since	3.1
		@lib	"S60Audio.lib"
		*/
		IMPORT_C virtual TInt Stop();

		/**
		 *
		 * Pauses playback of the audio clip
		 * @returns One of the system-wide error codes
		 * @since	3.1
		 * @lib		"S60Audio.lib"
		 */
		IMPORT_C virtual TInt Pause();

		/**
		Sets the priority for playback. This is used to arbitrate between multiple
		objects trying to access a single sound device.

		@param  aPriority
				The priority level to apply, EMdaPriorityMin client can be interrupted by any
				other client, EMdaPriorityNormal client can only be interrupted by a client
				with a higher priority or EMdaPriorityMax client cannot be interrupted by other
				clients.
		@param  aPref
				The quality/time preferences to apply.

		@return An error code indicating if the function call was successful. KErrNone on success, otherwise
				another of the system-wide error codes.

		@since  3.1

		@capability MultimediaDD
					For applications that do not have the MultimediaDD capability, any supplied priority level
					(aPriority) or priority preference (aPref) will be ignored.
		*/
		IMPORT_C virtual TInt SetPriority(TInt aPriority, TMdaPriorityPreference aPref);

		/**
		 *
		 * Sets the FastForward step for the file
		 *
		 * @param "TTimeIntervalMicroSeconds aStep"
		 *         The duration to move forward in microseconds from the current position in the file.
		 *         If aPosition is greater than the file duration, the position is set to the
		 *         end of the file.
		 *
		 * @since	3.1
		 * @lib		"S60Audio.lib"
		 */
		IMPORT_C virtual TInt FastForward(TTimeIntervalMicroSeconds& aStep);

		/**
		 *
		 * Sets the Rewind step for the file
		 *
		 * @param "TTimeIntervalMicroSeconds aStep"
		 *         The duration to move backward in microseconds from the current position in the file.
		 *         If aPosition is greater than the file duration, the position is set to the
		 *         beginning of the file.
		 *
		 * @since	3.1
		 * @lib		"S60Audio.lib"
		 */
		IMPORT_C virtual TInt Rewind(TTimeIntervalMicroSeconds& aStep);

		/**
		Returns an integer representing the maximum volume.

		This is the maximum value which can be passed to
		CMdaAudioPlayerUtility::SetVolume(). This value is platform
		independent, but is always greater than or equal to one.

		@return The maximum volume setting.

		@since	3.1
		*/
		IMPORT_C virtual TInt GetMaxVolume();

		/**
		Changes the current playback volume to a specified value.

		The volume can be changed before or during playback and is effective
		immediately.

		@param  aVolume
				The volume setting. This can be any value from zero to
				the value returned by a call to
				S60AudioPlayer::GetMaxVolume().
				Setting a zero value mutes the sound. Setting the
				maximum value results in the loudest possible sound.

		@return A System Wide Error code

		@since  3.1
		@lib    "S60Audio.lib"*/
		IMPORT_C virtual TInt SetVolume(TInt aVolume);

		/**
		Returns the current playback volume.

		@param  aVolume
				A value between 0 and the maximum volume settings returned by GetMaxVolume().

		@return An error code indicating if the function call was successful. KErrNone on success, otherwise
				another of the system-wide error codes.

		@since	3.1
		*/
		IMPORT_C virtual TInt GetVolume(TInt& aVolume);


		/**
		Returns the duration of the audio sample in microseconds.

		@return The duration of the sample in microseconds.

		@since	3.1
		*/
		IMPORT_C virtual TTimeIntervalMicroSeconds GetDuration();

		/**
		Returns the bit rate of the audio clip.

		@param  aBitRate
				Bit rate of the audio clip.

		@return One of the global error codes.

		@since  3.1
		@lib	"S60Audio.lib"
		*/
		IMPORT_C virtual TUint GetBitRate();

		/**
		 *
		 * Returns the current playback position in microseconds
		 *
		 * @retval "TTimeIntervalMicroSeconds& aPosition"
		 *          The current time position in microseconds from the start of the file
		 * @returns "TInt" One of the global error codes
		 *
		 * @since	3.1
		 * @lib		"S60Audio.lib"
		 */
		IMPORT_C virtual TTimeIntervalMicroSeconds GetPosition();

		/**
		 *
		 * Set the current playback position in microseconds from the start of the file
		 *
		 * @param "TTimeIntervalMicroSeconds& aPosition"
		 *         The position to move to in microseconds from the start of the file.
		 *         If aPosition is negative, the position is set to the start of the file.
		 *         If aPosition is greater than the file duration, the position is set to the
		 *         end of the file.
		 *
		 * @since	3.1
		 * @lib		"S60Audio.lib"
		 */
		IMPORT_C virtual void SetPosition(TTimeIntervalMicroSeconds& aPosition);

		/**
		 *
		 * Set the current playback window
		 *
		 * @param	"const TTimeIntervalMicroSeconds& aStart"
		 *			Start time of playback window relative to start of file
		 * @param	"const TTimeIntervalMicroSeconds& aEnd"
		 *			End time of playback window relative to start of file
		 *
		 * @returns "TInt" One of the global error codes
		 *
		 * @since	3.1
		 * @lib		"S60Audio.lib"
		 */
		IMPORT_C virtual void SetPlayWindow(TTimeIntervalMicroSeconds& aStart, TTimeIntervalMicroSeconds& aEnd);

		/**
		 *
		 * Gets the current playback window
		 *
		 * @param	"const TTimeIntervalMicroSeconds& aStart"
		 *			Start time of playback window relative to start of file
		 * @param	"const TTimeIntervalMicroSeconds& aEnd"
		 *			End time of playback window relative to start of file
		 *
		 * @returns "TInt" One of the global error codes
		 *
		 * @since	3.1
		 * @lib		"S60Audio.lib"
		 */

		IMPORT_C virtual void GetPlayWindow(TTimeIntervalMicroSeconds& aStart, TTimeIntervalMicroSeconds& aEnd);

		/**
		 *
		 * Clear the current playback window
		 *
		 * @returns "TInt" One of the global error codes
		 *
		 * @since	3.1
		 * @lib		"S60Audio.lib"
 		*/
		IMPORT_C virtual TInt ResetPlayWindow();

		/**
		Sets the number of times the audio sample is to be repeated during the
		playback operation.

		A period of silence can follow each playing of the sample. The audio
		sample can be repeated indefinitely.

		@param   aRepeatNumberOfTimes
				 The number of times the audio sample, together with
				 the trailing silence, is to be repeated. If this is
				 set to KMdaRepeatForever, then the audio
				 sample, together with the trailing silence, is
				 repeated indefinitely or until Stop() is
				 called. If this is set to zero, then the audio sample
				 is not repeated.
		@param   aTrailingSilence
				 The time interval of the trailing silence in microseconds.

		@since	3.1
		*/
		IMPORT_C virtual void SetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds& aTrailingSilence);

		/**
		Returns the current playback balance

		@return A value between KMMFBalanceMaxLeft and KMMFBalanceMaxRight

		@since  3.1
		@lib	"S60Audio.lib"
		*/
		IMPORT_C virtual TInt GetBalance();

		/**
		Sets the current playback balance

		@param  aBalance
				A value between KMMFBalanceMaxLeft and KMMFBalanceMaxRight. The default value is
				KMMFBalanceCenter.

		@return One of the global error codes.

		@since  3.1
		@lib	"S60Audio.lib"
		*/
		IMPORT_C virtual TInt SetBalance(TInt& aBalance);

		/**
		Returns the current Seeking Capability

		@return ETrue/EFalse depending on the Seeking Support

		@since  3.1
		@lib	"S60Audio.lib"
		*/
		IMPORT_C virtual TBool IsSeekingSupported();

		/**
		Returns the current Random Seeking Capability

		@return ETrue/EFalse depending on the Random Seeking Support

		@since  3.1
		@lib	"S60Audio.lib"
		*/
		IMPORT_C virtual TBool IsRandomSeekingSupported();

		/**
        * Add a data source to be used as input to the audio player.
        * @since 3.1
        */
		IMPORT_C virtual void AddDataSource( CS60AudioBaseSource& aDataSource );


		// from MCustomInterface

		IMPORT_C virtual TAny* CustomInterface(TUid aInterfaceId);

	protected:


		// from MMMFControllerEventMonitorObserver
		virtual void HandleEvent(const TMMFEvent& aEvent);

		// from MRepeatTrailingSilenceTimerObs
		virtual void RepeatTrailingSilenceTimerComplete();

		// from MMMFFindAndOpenControllerObserver
		virtual void MfaocComplete(TInt& aError, RMMFController* aController, TUid aControllerUid,
							       TMMFMessageDestination* aSourceHandle, TMMFMessageDestination* aSinkHandle);

	private:

		/**
		* Private C++ constructor for this class.
        * @since 3.1
        * @return -
        */
		CS60AudioPlayer(MS60AudioPlayerObserver& aObserver, TInt aPriority, TMdaPriorityPreference aPref);

		void ConstructL();

		TInt DoPlay();

	protected: // Data

		MS60AudioPlayerObserver& iObserver;

		TPlayerState iState;

		CS60AudioBaseSource* iSource;

		MAudioLoadingObserver* iLoadingObserver;
		MMMFAudioResourceNotificationCallback* iAudioResourceNotificationCallBack;

		CMMFFindAndOpenController* iFindAndOpenController;
		CMMFControllerEventMonitor* iControllerEventMonitor;

		TTimeIntervalMicroSeconds iTrailingSilence;
		CRepeatTrailingSilenceTimer* iRepeatTrailingSilenceTimer;

		CPlayerEventDispatcher* iDispatcher;
		TMMFPrioritySettings iPrioritySettings;
		RMMFController iController;
		RMMFAudioPlayDeviceCustomCommands iAudioPlayDeviceCommands;

		// Playback indicators
		TTimeIntervalMicroSeconds iPosition;
		TUid iControllerUid;
		TMMFMessageDestination iSourceHandle;

	};

#endif // __CS60AUDIOPLAYER_H__

//  End of File
