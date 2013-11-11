/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  VOIP Audio Services
 *
 */

#ifndef VOIPDOWNLINKSTREAMIMPL_H
#define VOIPDOWNLINKSTREAMIMPL_H

// INCLUDES
#include <voipdownlinkstream.h>
#include "VoIPAudioSession.h"
#include "VoIPDtmfTonePlayerImpl.h"
#include "VoIPQueueHandler.h"

/**
 *  CVoIPAudioDownlinkStreamImpl class
 *
 *  @lib VoIPAudioIntfc.dll
 */
NONSHARABLE_CLASS(CVoIPAudioDownlinkStreamImpl) : public CBase,
        public CVoIPAudioDownlinkStream,
        public MQueueHandlerObserver
    {
    enum TQueueType
        {
        EMsgBufQueue,
        EMsgComQueue
        };

public:

    /**
     * 2-phased constructor
     *
     *
     * @param none
     * @return none
     */
    static CVoIPAudioDownlinkStreamImpl* NewL(
            const TMMFPrioritySettings aPriority);

    /**
     * Destructor
     *
     * @param none
     * @return none
     */
    virtual ~CVoIPAudioDownlinkStreamImpl();

    /**
     * ?description
     *
     * @param MVoIPDownlinkObserver&
     * @return TInt
     */
    TInt Open(MVoIPDownlinkObserver& aObserver);

    /**
     * ?description
     *
     * @param TVersion&
     * @return TInt
     */
    TInt GetVersion(TVersion& aVersion);

    /**
     * ?description
     *
     * @param TInt&
     * @return TInt
     */
    TInt GetMaxVolume(TInt& aVolume);

    /**
     * ?description
     *
     * @param TInt
     * @return TInt
     */
    TInt SetVolume(TInt aVolume);

    /**
     * ?description
     *
     * @param TInt&
     * @return TInt
     */
    TInt GetVolume(TInt& aVolume);

    /**
     * ?description
     *
     * @param const TMMFPrioritySettings
     * @param RArray<TVoIPCodecFormat>&
     * @return void
     */
    void GetSupportedFormatsL(const TMMFPrioritySettings aPriority,
            RArray<TVoIPCodecFormat>& aFormats);

    /**
     * ?description
     *
     * @param TVoIPCodecFormat
     * @param CVoIPFormatIntfc&
     * @return void
     */
    void SetFormatL(TVoIPCodecFormat aFormat, CVoIPFormatIntfc*& aIntfc);

    /**
     * ?description
     *
     *
     * @param TVoIPOutputDevice
     * @return TInt
     */
    TInt SetAudioDevice(const TVoIPOutputDevice aDevice);

    /**
     * ?description
     *
     * @param TVoIPOutputDevice&
     * @return TInt
     */
    TInt GetAudioDevice(TVoIPOutputDevice& aDevice);

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    TInt Start();

    /**
     * ?description
     *
     * @param none
     * @return TInt
     */
    TInt Stop();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void Close();

    /**
     * ?description
     *
     * @param CVoIPDataBuffer*
     * @return TInt
     */
    TInt BufferFilled(CVoIPDataBuffer* aBuffer);

    /**
     * ?description
     *
     * @param CVoIPJitterBufferIntfc*&
     * @return void
     */
    void CreateJitterBufferIntfcL(CVoIPJitterBufferIntfc*& aJBIntfc);

    /**
     * ?description
     *
     * @param none
     * @return RVoIPAudioSession*
     */
    RVoIPAudioSession* GetSession();

    // From MQueueHandlerObserver

    /**
     * ?description
     *
     * @param CVoIPDataBuffer*
     * @return void
     */
    virtual void FillBuffer(CVoIPDataBuffer* aBuffer);

    /**
     * ?description
     *
     * @param TPtr8
     * @return void
     */
    virtual void EmptyBuffer(CVoIPDataBuffer* /*aBuffer*/) {}

    /**
     * ?description
     *
     * @param TInt
     * @param TInt
     * @return void
     */
    virtual void Event(TInt aEventType, TInt aError);

private:

    /**
     * Constructor
     *
     * @param none
     * @return none
     */
    CVoIPAudioDownlinkStreamImpl();

    /**
     * Symbian constructor
     *
     * @param none
     * @return void
     */
    void ConstructL(const TMMFPrioritySettings aPriority);

    /**
     * ?description
     *
     * @param TDesC&
     * @param TInt
     * @param TQueueType
     * @return TInt
     */
    TInt CreateQueue(const TDesC& aQueueName, const TInt aNumSlots,
            const TQueueType aQueueType);

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void ReceiveMsgQHandlerEventsL();

    /**
     * ?description
     *
     * @param none
     * @return void
     */
    void ReceiveMsgQComHandlerEventsL();

    /**
     * ?description
     *
     * @param TVoIPCodecFormat
     * @return TFourCC
     */
    TUint32 CodecFourCC(TVoIPCodecFormat aFormat);

    /**
     * ?description
     *
     * @param TUint32
     * @return TVoIPCodecFormat
     */
    TVoIPCodecFormat ConvertFourCC(TUint32 aCodec);

    /**
     * ?description
     *
     * @param TVoIPCodecFormat
     * @return TBool
     */
    TBool IsCodecSupportedL(TVoIPCodecFormat aFormat);

    /**
     * ?description
     *
     * @param TVoIPCodecFormat
     * @return TBool
     */
    TBool FindFormat(TVoIPCodecFormat aFormat);

private:
    friend class CDTMFTonePlayerImpl;

private:
    // Client side VoIP server session
    RVoIPAudioSession iVoIPAudioSession;

    MVoIPDownlinkObserver* iObserver;

    // Message queues
    RMsgQueue<TVoIPMsgBuf> iMsgQueue;
    RMsgQueue<TVoIPMsgBuf> iMsgComQueue;

    // Message queue handlers
    CQueueHandler* iMsgQHandler;
    CQueueHandler* iMsgQComHandler;

    TMMFPrioritySettings iPriority;
    TPtr8 iBufferPtr;
    TInt iBufferLen;
    TInt iG711FrameSize;

    TVoIPCodecFormat iFormat;
    RArray<TVoIPCodecFormat> iCodecFormats;
    };

#endif //VOIPDOWNLINKSTREAMIMPL_H

// End of file
