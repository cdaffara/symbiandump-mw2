/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMSSERVERSESSION_H
#define TMSSERVERSESSION_H

// INCLUDES
#include <sounddevice.h>
#include <e32msgqueue.h>
#include "tmsserver.h"
#include "tmsshared.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSServerThread;
class TMSAudioInbandTonePlayer;
class TMSRingTonePlayer;

// -----------------------------------------------------------------------------
//  Class Name:  TMSServerSession
//
//  TMS server session class.
// -----------------------------------------------------------------------------
class TMSServerSession : public CSession2
    {
public:
    // Constractor
    static TMSServerSession* NewL(TMSServer &aServer);

    // Destructor
    ~TMSServerSession();

    void HandleGlobalEffectChange(TInt globalevent, TInt level,
            TBool output_change = EFalse,
            TMSAudioOutput output = TMS_AUDIO_OUTPUT_PRIVATE);
    void HandleRoutingChange(TRoutingMsgBufPckg routinginfo);

    // Send notifications to clients
    void NotifyClient(const TInt aCommand, const TInt aStatus = KErrNone);

private:
    // Symbian constructors
    void ConstructL();
    TMSServerSession(TMSServer& aServer);

    // Client-server message handlers
    void ServiceL(const RMessage2& aMessage);
    void DispatchMessageL(const RMessage2& aMessage);
    void SetMessageQueueHandleL(const RMessage2& aMessage);
    void GetTMSCallSessionHandleL(const RMessage2& aMessage);
    void GetCodecsCountL(const RMessage2& aMessage, TMSStreamType strmType);
    void GetSupportedCodecsL(const RMessage2& aMessage, TMSStreamType strmType);
    void SetOutput(const RMessage2& aMessage);
    void SetVolLevel(const RMessage2& aMessage);
    void SetMicGain(const RMessage2& aMessage);

    // for Inband tone player
    void StartInbandTone(const RMessage2& aMessage);
    void StopInbandTone(const RMessage2& aMessage);

private:
    TMSServer& iServer;
    RHandleBase iTMSCallSessionHandle;
    TSharedData iShared;
    CMMFDevSound* iDevSound; // To get supported codecs
    TBool iMsgQueued;
    RMsgQueue<TmsMsgBuf> iMsgQueue;
    TmsMsgBuf iMsgBuffer;
    TMSAudioInbandTonePlayer* iInbandTonePlayer;
    TBool iHasRtPlayer;
    };

} //namespace TMS

#endif //TMSSERVERSESSION_H

// End of file
