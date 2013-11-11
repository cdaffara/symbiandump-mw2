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

#ifndef VOIPDTMFTONEPLAYERIMPL_H
#define VOIPDTMFTONEPLAYERIMPL_H

// INCLUDES
#include <voipdtmftoneplayer.h>
#include "VoIPQueueHandler.h"

// CONSTANTS

// FORWARD DECLARATIONS
class CVoIPAudioUplinkStream;
class CVoIPAudioDownlinkStream;
class CVoIPDataBuffer;
class RVoIPAudioSession;

/**
 *  CDTMFTonePlayerImpl class
 *
 *  DTMF Tone Player class
 *
 *  @lib VoIPAudioIntfc.dll
 */
NONSHARABLE_CLASS(CDTMFTonePlayerImpl) : public CBase,
        public CDTMFTonePlayer,
        public MQueueHandlerObserver
    {
    enum TStreamType
        {
        EDownlink,
        EUplink
        };

public:

    /**
     * 2-phase constructor
     *
     *
     * @param none
     * @return CDTMFTonePlayerImpl* -
     */
    static CDTMFTonePlayerImpl* NewL();

    /**
     * Destructor
     *
     *
     * @param none
     * @return none
     */
    virtual ~CDTMFTonePlayerImpl();

    /**
     * ?description
     *
     *
     * @param none
     * @return TInt
     */
    TInt Open(MDTMFToneObserver& aObserver);

    /**
     * ?description
     *
     *
     * @param TPtr -
     * @return TInt -
     */
    TInt Play(const TPtr aTones);

    /**
     * ?description
     *
     *
     * @param none
     * @return TInt
     */
    TInt Stop();

    /**
     * ?description
     *
     *
     * @param none
     * @return void
     */
    void Close();

    /**
     * ?description
     *
     *
     * @param CVoIPAudioUplinkStream& -
     * @return TInt
     */
    TInt AddVoIPAudioUplinkStream(CVoIPAudioUplinkStream& aStream,
            RVoIPAudioSession* aSession);

    /**
     * ?description
     *
     *
     * @param CVoIPAudioUplinkStream&
     * @return TInt
     */
    TInt RemoveVoIPAudioUplinkStream(CVoIPAudioUplinkStream& aStream);

    /**
     * ?description
     *
     *
     * @param CVoIPAudioDownlinkStream& -
     * @return TInt
     */
    TInt AddVoIPAudioDownlinkStream(CVoIPAudioDownlinkStream& aStream,
            RVoIPAudioSession* aSession);

    /**
     * ?description
     *
     *
     * @param CVoIPAudioDownlinkStream& -
     * @return TInt
     */
    TInt RemoveVoIPAudioDownlinkStream(CVoIPAudioDownlinkStream& aStream);

    // From MQueueHandlerObserver

    /**
     * ?description
     *
     *
     * @param TPtr8
     * @return void
     */
    virtual void FillBuffer(CVoIPDataBuffer* /*aBuffer*/) {}

    /**
     * ?description
     *
     *
     * @param TPtr8
     * @return void
     */
    virtual void EmptyBuffer(CVoIPDataBuffer* /*aBuffer*/) {}

    /**
     * ?description
     *
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
     *
     * @param none
     * @return none
     */
    CDTMFTonePlayerImpl();

    /**
     * Symbian constructor
     *
     *
     * @param none
     * @return void
     */
    void ConstructL();

    /**
     *
     * @param none
     * @return void
     */
    void StartMsgQueueL();

    /**
     *
     * @param none
     * @return void
     */
    void ReceiveMsgQComHandlerEventsL();

private:
    // Communication queue
    RMsgQueue<TVoIPMsgBuf> iMsgComQueue;
    CQueueHandler* iMsgQComHandler;

    MDTMFToneObserver* iObserver;
    RVoIPAudioSession* iVoIPAudioSession;

    TStreamType iStreamType;
    };

#endif //VOIPDTMFTONEPLAYERIMPL_H

// End of file
