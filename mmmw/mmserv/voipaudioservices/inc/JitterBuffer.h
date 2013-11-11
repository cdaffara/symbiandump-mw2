/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Jitter buffer for audio frames.
 * Interface  : VoIPJitterBuffer.lib
 *
 */

#ifndef VOIPJITTERBUFFER_H
#define VOIPJITTERBUFFER_H

// DATA TYPES

// FORWARD DECLARATIONS
class CMccCnGenerator;
class CVoIPJitterBufferImpl;
class TVoIPJBConfig;
class MJitterBufferObserver;

// CLASS DECLARATION

/**
 *  Jitter buffer.
 *
 *  @lib VoIPJitterBuffer.lib
 */
class CVoIPJitterBuffer : public CActive
    {
private:
    /**
     * Jitter buffer states
     */
    enum TJBState
        {
        EJBufStopped,
        EJBufPlaying,
        EJBufPaused
        };

    /**
     * Transition states during JB playback
     */
    enum TJBTransitionState
        {
        EWait,
        //ENeedData,
        EEmptyData
        };

public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    IMPORT_C static CVoIPJitterBuffer* NewL(MJitterBufferObserver* aObserver);

    /**
     * Destructor.
     */
    virtual ~CVoIPJitterBuffer();

public:
    // Methods

    /**
     * SetupL
     * Sets codec information and initializes JB.
     *
     * @param TFourCC - codec's fourcc
     * @param TVoIPJBConfig& - JB configuraion parameters
     * @return void
     */
    IMPORT_C void SetupL(const TFourCC aCodec, const TVoIPJBConfig& aJBConfig);

    /**
     * ResetBuffer
     * Resets JB to its original state with no data.
     *
     * @param TBool - ETrue == tone is to be played
     * @return void
     */
    IMPORT_C void ResetBuffer(TBool aPlayTone = ETrue);

    /**
     * Delay Down
     *
     * @param none
     * @return void
     */
    IMPORT_C void DelayDown();

    /**
     * Delay Up
     *
     * @param none
     * @return void
     */
    IMPORT_C void DelayUp();

    /**
     * Play
     *
     * @param none
     * @return void
     */
    IMPORT_C void Play();

    /**
     * Pause
     *
     * @param none
     * @return void
     */
    IMPORT_C void Pause();

    /**
     * Stop
     *
     * @param none
     * @return void
     */
    IMPORT_C void Stop();

    /**
     * FillBuffer
     * Retrieves next frame from JB queue.
     *
     * @param CMMFBuffer* - buffer to be filled with data from the JB
     * @return TInt
     */
    IMPORT_C TInt FillBuffer(CMMFBuffer* aBuffer);

    /**
     * EmptyBufferL
     * Queues received frame in JB.
     *
     * @param CMMFBuffer* - buffer to be queued in the JB
     * @return TInt
     */
    IMPORT_C TInt EmptyBuffer(CMMFBuffer* aBuffer);

private:

    /**
     * C++ default constructor.
     */
    CVoIPJitterBuffer(MJitterBufferObserver* aObserver);

    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();

    /**
     * from CActive
     */
    void DoCancel();

    /**
     * from CActive
     */
    void RunL();

    /**
     * from CActive
     */
    TInt RunError(TInt aError);

private:
    // Internal helper methods

    /**
     * Play current frame from JB.
     *
     * @param none
     * @return void
     */
    void PlayBuffer();

    /**
     * TransitionState
     *
     * @param TJBTransitionState
     * @param TUint32
     * @return void
     */
    void TransitionState(TJBTransitionState aTransitionState,
            TUint32 aStateChangeDelay = 0);

    /**
     * Calculates the time interval at which data rate should be slowed down
     * when subsequent EmptyBufferL calls are coming too fast.
     *
     * @param none
     * @return Suggested delay in microseconds.
     */
    TUint32 DetermineEmptyBufferDelay();

private:
    // Data

    // Jitter buffer Implementation
    CVoIPJitterBufferImpl* iJitterBufferImpl;

    // Pointer to databuffer
    CMMFBuffer* iPlayBuffer;

    // Jitterbuffer state
    TJBState iState;

    // JB Transition State
    TJBTransitionState iTransitionState;

    MJitterBufferObserver* iObserver;
    TFourCC iCodec;

    RTimer iTimer;
    TTime iTmCurrentEmptyBuffer;
    TTime iTmPreviousEmptyBuffer;

    TUint32 iSampleInterval;
    TUint32 iEmptyBufferDelayThreshold;

    // Current request size of the buffer
    TInt iRequestSize;
    };

#endif //VOIPJITTERBUFFER_H

// End of file
