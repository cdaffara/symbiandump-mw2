/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 * This file defines the API for System Tone Service which is
 * implemented in the systemtoneservice.dll.
 */
#ifndef SYSTEMTONESERVICE_H_
#define SYSTEMTONESERVICE_H_
// System includes
#include <e32base.h>
// Forward declarations
NONSHARABLE_CLASS( CStsImplementation);

//Observer declaration
/**
 *  Play Alarm Observer API definition.
 *  This defines the interface that must be implemented by clients using
 *  the PlayAlarm method.  This API allows the clients to observe  when
 *  the Play Alarm completes.  The same observer can be used for different
 *  PlayAlarm calls, even ones that are playing at the same time.
 */
class MStsPlayAlarmObserver
    {
public:
    /**
     * Informs the observer when the play alarm as indicated by the alarm
     * context is complete.  This method is guaranteed to be called unless
     * the alarm context has been stopped with the StopAlarm method, in
     * which case the callback is guaranteed not to be called.
     * 
     * Implementations of this interface cannot make any assumptions about
     * which thread this method is called on, it may or may not be the same
     * thread from which the PlayAlarm was called.  It is up to the clients
     * to provide what ever mutual exclusion protection that is needed for
     * data accessed in this method.
     * 
     * In some cases and especially on SMP systems there is a chance that
     * this method may be called before the PlayAlarm method returns.  It is
     * up to the client to correctly handle this potential race condition.
     * 
     * Clients cannot assume properties about the thread that the callback
     * is running on, for instance the thread may or may not have Active
     * Scheduler.
     * 
     * It is strongly recommended that clients do as little as possible
     * processing in this method and to complete this method as quickly as
     * possible.
     *
     * @param aAlarmContext The context of the alarm that has completed.
     */
    virtual void PlayAlarmComplete(unsigned int aAlarmContext) = 0;
    };

// Class declaration
/**
 * System Tone Service API definition.
 * This is the native C++ API for applications and middleware components
 * to play standard system tones.  This API uses the Private Implementation
 * (PIMPL) pattern to help improve the BC of the API by decoupling the
 * implementation from the interface.
 * 
 * The methods of this class are guaranteed to be thread safe and may be
 * used from any thread within the same process.  An instance of this class
 * may not be shared between processes, even if the processes share memory.
 *
 *  @code
 *   CSystemToneService* sts = CSystemToneService::Create();
 *   TInt calendarAlarmContext;
 *   ...
 *   if (sts)
 *   {
 *       ...
 *       sts->PlayTone(CSystemToneService::EWarningTone);
 *       ...
 *       sts->PlayAlarm(CSystemToneService::ECalendarAlarm, &calendarAlarmContext);
 *       ...
 *       sts->StopAlarm(calendarAlarmContext);
 *       ...
 *   }
 *   ...
 *   CSystemToneService::Delete(sts);
 *  @endcode
 *
 */
NONSHARABLE_CLASS(CSystemToneService) : public CBase
    {
public:
    /**
     * Constructor
     * Creates and initializes an instance of the System Tone Service.
     * 
     * @returns Either a pointer to the new CSystemToneService instance
     *          or NULL if construction fails.
     */
    IMPORT_C static CSystemToneService* Create();

    /** Destructor
     * Deinitializes and deletes the specified System Tone Service instance.  This method
     * should not be called if an existing PlayAlarm has not yet completed or has not been
     * stopped, otherwise whether or not the outstanding alarms actually plays or stops is
     * not guaranteed.
     * 
     * @param[in] aSystemToneService A pointer to the CSystemToneService instance
     *                           to be deleted.  This pointer will be invalid
     *                           after this method completes.
     */
    IMPORT_C static void Delete(CSystemToneService* aSystemToneService);

public:
    // Data types
    /** The type of system tones that are supported by this API. */
    enum TToneType
        {
        // Capture Tones
        EBurstMode = 0x0001,
        ECapture = 0x0002,
        ECallRecording = 0x0003,
        ERecordingStart = 0x0004,
        ERecordingStop = 0x0005,
        ESelfTimer = 0x0006,

        // General Tones
        EConfirmationBeep = 0x0101,
        EDefaultBeep = 0x0102,
        EErrorBeep = 0x0103,
        EInformationBeep = 0x0104,
        EWarningBeep = 0x0105,
        EIntegratedHandsFreeActivated = 0x0106,

        // Key Tones
        ETouchScreen = 0x0201,

        // Location Tones
        ELocationRequest = 0x0301,

        // Messaging Tones
        EChatAlert = 0x0401,
        EEmailAlert = 0x0402,
        EMmsAlert = 0x0403,
        ESmsAlert = 0x0404,
        EDeliveryReport = 0x0405,
        EMessageSendFailure = 0x0406,

        // Power Tones
        EBatteryLow = 0x0501,
        EBatteryRecharged = 0x0502,
        EPowerOn = 0x0503,
        EPowerOff = 0x0504,
        EWakeUp = 0x0505,
        EWrongCharger = 0x0506,

        // Telephony Tones
        EAutomaticRedialComplete = 0x0604,

        // Voice Recognition Tones
        EVoiceStart = 0x0701,
        EVoiceError = 0x0702,
        EVoiceAbort = 0x0703
        };

    /** The type of system alarms that are supported by this API. */
    enum TAlarmType
        {
        // Calendar Alarms
        ECalendarAlarm = 0x8001,
        EClockAlarm = 0x8002,
        EToDoAlarm = 0x8003,

        // Telephony Alarms
        EIncomingCall = 0x8101,
        EIncomingCallLine2 = 0x8102,
        EIncomingDataCall = 0x8103
        };

    /**
     * Plays the specified system tone.  If the tone type is not recognized, a default
     * tone will be played.  Tones are short, fixed duration that always play to
     * completion and cannot be stopped by the client.
     *
     * @param[in] aTone The system tone type to play.
     */
    IMPORT_C void PlayTone(TToneType aTone);

    /**
     * Plays the specified system alarm.  If the alarm type is not recognized, a default
     * alarm will be played.  Alarms are tones that are long or not fixed duration such as a
     * calendar alarm that can be manually stopped by the client.  The client is notified
     * when an alarm playback completes if it is not manually stopped by the client.
     *
     * @param[in]  aAlarm        The system alarm type to play.
     * @param[out] aAlarmContext A guaranteed globally unique context representing this
     *                           specific alarm playback that can be used for stopping the
     *                           alarm or used to indicate to the observer which alarm playback
     *                           has completed.
     * @param[in] aObserver      A reference to the observer that is to be notified if this alarm
     *                           playback completes before StopAlarm is called.  The same observer
     *                           can be used for multiple PlayAlarms, even for ones playing at the
     *                           same time.  It is the responsibility of the client to manage the
     *                           life cycle of the observer.  However the observer must exist until
     *                           all alarms the observer is associated with are either completed
     *                           or stopped.
     */
    IMPORT_C void PlayAlarm(TAlarmType aAlarm, unsigned int& aAlarmContext,
            MStsPlayAlarmObserver& aObserver);

    /**
     * Plays the specified system tone as an alarm.  If the tone type is not recognized, a default
     * default tone will be played.  Using this method allows the clients to stop the tone or get
     * a callback when the tone playback completes.
     *
     * @param[in]  aTone         The system tone type to play.
     * @param[out] aAlarmContext A guaranteed globally unique context representing this
     *                           specific tone playback that can be used for stopping the
     *                           tone or used to indicate to the observer which playback
     *                           has completed.
     * @param[in] aObserver      A reference to the observer that is to be notified if this alarm
     *                           playback completes before StopAlarm is called.  The same observer
     *                           can be used for multiple PlayAlarms, even for ones playing at the
     *                           same time.  It is the responsibility of the client to manage the
     *                           life cycle of the observer.  However the observer must exist until
     *                           all alarms the observer is associated with are either completed
     *                           or stopped.
     */
    IMPORT_C void PlayAlarm(TToneType aTone, unsigned int& aAlarmContext,
            MStsPlayAlarmObserver& aObserver);

    /**
     * Stops the specified system alarm playback.  If the playback has already completed or the
     * context is not valid, this method does nothing.
     *
     * @param[in] aAlarmContext The context to the specific alarm that is to be stopped.
     */
    IMPORT_C void StopAlarm(unsigned int aAlarmContext);

protected:
    // Protected constructors and destructors
    CSystemToneService(CStsImplementation& aImplementation);
    ~CSystemToneService();

protected:
    // Protected data
    /**
     * A reference to the implementation class for this API.
     */
    CStsImplementation& iImplementation;
    };

#endif  // SYSTEMTONESERVICE_H_
