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
 * Description:  Jitter buffer for Audio frames.
 *  Interface   : VoIPJitterBuffer.lib
 *
 */

#ifndef VOIPJITTERBUFFERIMPL_H
#define VOIPJITTERBUFFERIMPL_H

//  INCLUDES
#include <voipjitterbufferintfc.h>

// FORWARD DECLARATIONS
class CVoIPCNGenerator;
class MJitterBufferObserver;

// CLASS DECLARATION

/**
 *  Jitter buffer element including packet received.
 *
 *  @lib VoIPJitterBuffer.lib
 */
class TJitterBufferElement
    {
public:
    // Constructor
    inline TJitterBufferElement() :
        iDataFrame(NULL),
        iSequenceNumber(-1),
        iTimeStamp(-1) {}

public:
    // Methods

    /**
     * Compares two elements based on their sequence number
     *
     * @param aElem1 First element
     * @param aElem2 Second element
     * @return TInt
     */
    static TInt CompareSeqNum(const TJitterBufferElement& aElem1,
            const TJitterBufferElement& aElem2);

    /**
     * Compares two elements based on their stamps
     *
     * @param aElem1 First element
     * @param aElem2 Second element
     * @return TInt
     */
    static TInt CompareStamp(const TJitterBufferElement& aElem1,
            const TJitterBufferElement& aElem2);

public:
    // Data members

    // Data buffer
    CMMFDataBuffer* iDataFrame;

    // Data packet number
    TInt64 iSequenceNumber;

    // Time stamp
    TInt64 iTimeStamp;
    };

/**
 *  Jitter buffer.
 *
 *  @lib VoIPJitterBuffer.lib
 */
class CVoIPJitterBufferImpl : public CBase
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CVoIPJitterBufferImpl* NewL(MJitterBufferObserver* aObserver);

    /**
     * Destructor.
     */
    virtual ~CVoIPJitterBufferImpl();

public:
    // Methods

    /**
     * Setup jitterbuffer
     *
     * @param parameters
     * @return void
     */
    void SetupL(const TFourCC aCodec, const TVoIPJBConfig& aJBConfig);

    /**
     * Reset buffer
     *
     * @param aPlayTone ETrue, if tone is to played
     * @return void
     */
    void ResetBuffer(TBool aPlayTone = ETrue);

    /**
     * Add DataFrame to buffer
     *
     * @param aBuffer Buffer to read data from
     * @return void
     */
    TInt AddDataFrame(CMMFBuffer* aDataBuffer);

    /**
     * Get DataFrame from buffer
     *
     * @param aBuffer Buffer to write data to
     * @return TInt
     */
    TInt GetDataFrame(CMMFBuffer* aBuffer);

    /**
     * Get Current Codec
     *
     * @param None
     * @return TFourCC
     */
    TFourCC CurrentCodec() const;

    /**
     * Delay Up
     *
     * @param None
     * @return void
     */
    void DelayUpL();

    /**
     * Decreases jitter buffer delay
     *
     * @param None
     * @return void
     */
    void DelayDownL();

#ifdef __FEATURE_NOT_SUPPORTED__
    /**
     * ?Description
     *
     * @param None
     * @return TTimeIntervalMicroSeconds32
     */
    TTimeIntervalMicroSeconds32 CalculateDelay() const;
#endif //__FEATURE_NOT_SUPPORTED__

    /**
     * Get the length of jitter buffer, in terms of number of packets.
     *
     * @param none
     * @return buffer lenght.
     */
    TInt BufferLength() const;

#ifdef __FEATURE_NOT_SUPPORTED__
    /**
     * Get number of packets that are currently inside the buffer.
     *
     * @param none
     * @return Number of packet in buffer.
     */
    TInt PacketCount() const;
#endif //__FEATURE_NOT_SUPPORTED__

    /**
     * Fills the given event with statistics of the jitterbuffer
     *
     * @param aEvent Event into which set the statistics
     * @return void
     */
    void GenerateStatistics(/*TJBStats& aStats*/) const;

private:

    /**
     * C++ default constructor.
     */
    CVoIPJitterBufferImpl(MJitterBufferObserver* aObserver);

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:
    // New functions

    /**
     * Is sequence number wrapped around TUint16 65535->0
     *
     * @param TInt aSeqNum1 is oldest packet sequence number in buffer
     * @param TInt aSeqNum2 is new element sequence number to buffer
     * @return detect the wrap-around condition.
     */
    TBool IsSeqNumWrappedAround(TInt64 aSeqNum1, TInt64 aSeqNum2) const;

    /**
     * Find Largest SequenceNumber
     *
     * @param None
     * @return TUint Largest sequence number
     */
    TUint FindLargestSeqNum();

    /**
     * Inserts a databuffer into the jitter buffer
     *
     * @param aBuffer Buffer containing the data
     * @param aLargestSeqNum Largest sequence number returned by
     *                       FindLargestSeqNum()
     * @return void
     */
    void InsertBufferElement(const TDesC8& aBuffer, TInt64 aLargestSeqNum);

    /**
     * Is jitter buffer full?
     *
     * @param none
     * @return True if full.
     */
    TBool IsFull() const;

    /**
     * Is jitter buffer empty?
     *
     * @param none
     * @return True if empty.
     */
    TBool IsEmpty() const;

    /**
     * CheckThresholdBufferLength
     *
     * @param none
     * @return void
     */
    void CheckThresholdBufferLength(TInt& aBufferLength,
            const TInt aTreshhold) const;

private:
    // Data

    // Playout thresholds
    TInt iCurrentPlayThreshold;
    TInt iOriginalPlayThreshold;

    // Buffer length
    TInt iBufferLength;

    // Jitter buffer array (queue)
    RArray<TJitterBufferElement> iBuffer;

    // Instance of comfort noise generator
    CVoIPCNGenerator* iCNGenerator;

    // Frame SequenceNumber
    TInt64 iCurrentSeqNum;

    // Sequence number of previously played frame
    TInt64 iLastPlayedSeqNum;
    TInt64 iLargestSeqNum;

    // Jitter buffer sorting objects
    TLinearOrder<TJitterBufferElement> iBufStampSorter;
    TLinearOrder<TJitterBufferElement> iBufSequenceSorter;

    // JB configuration parameters
    TVoIPJBConfig iJBConfig;

    // Codec frame size
    TInt iFrameSize;

    // Encoded frame sample interval
    TInt iSampleInterval;

    // Increment in frame sequence number
    TUint iSeqNumIncrement;

    TBool iIsWrappedAround;

    MJitterBufferObserver* iObserver;
    CMMFDataBuffer* iDataBuffer;

    // These are mainly for statistical purpose

    // Number of frames totally lost
    TUint iFramesLost;

    // Number of frames received
    TUint iFramesReceived;

    // Number of frames removed by adaptation or because of overflows
    TUint iFramesRemoved;

    // Number of frames played
    TUint iFramesPlayed;

    // Number of frames arrived late
    TUint iNumOfLateFrames;

    // Number of packets in buffer
    TInt iPacketsInBuffer;

    // Buffer overflow counter
    TUint iOverflowCounter;

    // Jitter buffer inctivity time
    TInt iInactivityTime;

    // These are for tone playback
    TInt iPlayToneInterval;
    TTime iTonePlayTime;
    TInt iSampleRate;
    TBool iPlay;
    };

#endif  //VOIPJITTERBUFFERIMPL_H

// End of file
