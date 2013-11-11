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

#ifndef TMSCALLSESSION_H
#define TMSCALLSESSION_H

// INCLUDES
#include <e32base.h>
#include <tms.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCallServer;
class TMSCallAdpt;

/*
 * Class Name:  TMSCallSession
 */
class TMSCallSession : public CSession2
    {
public:
    static TMSCallSession* NewL(TMSCallServer& aServer);
    ~TMSCallSession();

private:
    TMSCallSession(TMSCallServer& aServer);
    void ConstructL();

    // Client-server message handler from CSession2
    void ServiceL(const RMessage2& aMessage);

private:
    void HandleMessageL(const RMessage2& aMessage);
    void HandleCreateCallL(const RMessage2& aMessage);
    void HandleCreateStreamL(const RMessage2& aMessage);
    void HandleInitStreamL(const RMessage2& aMessage);
    void HandleStartStreamL(const RMessage2& aMessage);
    void HandlePauseStreamL(const RMessage2& aMessage);
    void HandleStopStreamL(const RMessage2& aMessage);
    void HandleDeinitStreamL(const RMessage2& aMessage);
    void HandleDeleteStreamL(const RMessage2& aMessage);
    void HandleDeleteCallL(const RMessage2& aMessage);
    void HandleDataXferBufferEmptiedCallL(const RMessage2& aMessage);
    void HandleDataXferBufferFilledCallL(const RMessage2& aMessage);
    void HandleDataXferBufferGetHndlCallL(const RMessage2& aMessage);
    void HandleEffectVolumeGetMaxVolL(const RMessage2& aMessage);
    void HandleEffectVolumeSetVolL(const RMessage2& aMessage);
    void HandleEffectVolumeGetVolL(const RMessage2& aMessage);
    void HandleEffectVolumeGetMaxGainL(const RMessage2& aMessage);
    void HandleEffectVolumeSetGainL(const RMessage2& aMessage);
    void HandleEffectVolumeGetGainL(const RMessage2& aMessage);
    void HandleGlobalEffectVolumeGetMaxVolL(const RMessage2& aMessage);
    void HandleGlobalEffectVolumeSetVolL(const RMessage2& aMessage);
    void HandleGlobalEffectVolumeGetVolL(const RMessage2& aMessage);
    void HandleGlobalEffectVolumeGetMaxGainL(const RMessage2& aMessage);
    void HandleGlobalEffectVolumeSetGainL(const RMessage2& aMessage);
    void HandleGlobalEffectVolumeGetGainL(const RMessage2& aMessage);
    void HandleFormatGetCodecModeL(const RMessage2& aMessage);
    void HandleFormatSetCodecModeL(const RMessage2& aMessage);
    void HandleFormatGetSupportedBitRatesCountL(const RMessage2& aMessage);
    void HandleFormatGetSupportedBitRatesL(const RMessage2& aMessage);
    void HandleFormatGetBitRateL(const RMessage2& aMessage);
    void HandleFormatSetBitRateL(const RMessage2& aMessage);
    void HandleFormatGetVADL(const RMessage2& aMessage);
    void HandleFormatSetVADL(const RMessage2& aMessage);
    void HandleFormatGetCNGL(const RMessage2& aMessage);
    void HandleFormatSetCNGL(const RMessage2& aMessage);
    void HandleFormatGetPlcL(const RMessage2& aMessage);
    void HandleFormatSetPlcL(const RMessage2& aMessage);

    void HandleRoutingSetOutputL(const RMessage2& aMessage);
    void HandleRoutingGetOutputL(const RMessage2& aMessage);
    void HandleRoutingGetPreviousOutputL(const RMessage2& aMessage);
    void HandleRoutingGetAvailableOutputsL(const RMessage2& aMessage);
    void HandleGetActiveCallL(const RMessage2& aMessage);

private:
    TMSCallServer& iTMSCallServer;
    TMSCallAdpt* iCallAdpt;
    gint iGlobalVol;
    gint iGlobalGain;
    TMSCallType iActiveCallType;
    };

} //namespace TMS

#endif //TMSCALLSESSION_H

// End of file
