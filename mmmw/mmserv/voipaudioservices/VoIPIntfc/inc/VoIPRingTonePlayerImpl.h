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

#ifndef RINGTONEPLAYERIMPL_H
#define RINGTONEPLAYERIMPL_H

#include <voipringtoneplayer.h>
#include "VoIPAudioSession.h"
#include "VoIPQueueHandler.h"

// FORWARD DECLARATIONS
class RFile;
class RVoIPAudioSession;
class CVoIPDataBuffer;

// CONSTANTS


/**
 *  CRingTonePlayerImpl class
 *
 *  Ring Tone Player class
 *
 *  @lib VoIPAudioIntfc.dll
 */
NONSHARABLE_CLASS(CRingTonePlayerImpl) : public CBase,
        public CRingTonePlayer,
        public MQueueHandlerObserver
    {
public:

    /**
     * 2-phase constructor
     *
     *
     * @param none
     * @return CRingTonePlayerImpl* -
     */
    static CRingTonePlayerImpl* NewL();

    /**
     * Destructor
     *
     *
     * @param none
     * @return none
     */
    virtual ~CRingTonePlayerImpl();

    /**
     * ?description
     *
     *
     * @param MRingToneObserver&
     * @return TInt
     */
    void OpenL(MRingToneObserver& aObserver);

    /**
     * ?description
     *
     *
     * @param MRingToneObserver&
     * @param TPtr
     * @return TInt
     */
    void OpenL(MRingToneObserver& aObserver, TPtr aFileName);

    /**
     * ?description
     *
     *
     * @param MRingToneObserver&
     * @param RFile&
     * @return TInt
     */
    void OpenL(MRingToneObserver& aObserver, RFile& aFileHandle);

    /**
     * ?description
     *
     *
     * @param MRingToneObserver&
     * @param TPtr8
     * @param TPtr8
     * @return TInt
     */
    void OpenL(MRingToneObserver& aObserver, TPtr8 aDesTone, TPtr8 aMimeType);

    /**
     * ?description
     *
     *
     * @param none
     * @return TInt
     */
    TInt Play();

    /**
     * ?description
     *
     *
     * @param none
     * @return TInt
     */
    TInt Pause();

    /**
     * ?description
     *
     *
     * @param none
     * @return TInt
     */
    TInt Resume();

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
     * @return TInt
     */
    void Close();

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
    CRingTonePlayerImpl();

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

    MRingToneObserver* iObserver;
    RVoIPAudioSession iVoIPAudioSession;
    };

#endif //RINGTONEPLAYERIMPL_H

// End of file
