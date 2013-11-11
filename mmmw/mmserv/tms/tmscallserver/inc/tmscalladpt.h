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

#ifndef CALLADPT_H
#define CALLADPT_H

#include <e32base.h>
#include <tms.h>

namespace TMS {

// TMSCallAdpt class
class TMSCallAdpt
    {
protected:
    enum TMSStreamState
        {
        EIdle = 0,
        EInitialized,
        EActivated
        };

public:
    static gint CreateCallL(gint callType, TMSCallAdpt*& callAdpt);
    virtual ~TMSCallAdpt();

    // From TMSStream
    virtual gint CreateStream(const TMSCallType callType,
            const TMSStreamType strmType, gint& outStrmId) = 0;
    virtual gint InitStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId,
            const TMSFormatType frmtType, const gint retrytime,
            const RMessage2& aMessage) = 0;
    virtual gint StartStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId,
            const gint retrytime) = 0;
    virtual gint PauseStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId) = 0;
    virtual gint StopStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId) = 0;
    virtual gint DeinitStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId) = 0;
    virtual gint DeleteStream(const TMSCallType callType,
            const TMSStreamType strmType, const gint strmId) = 0;

    // From TMS routing
    virtual gint SetOutput(const TMSAudioOutput output) = 0;
    virtual gint GetOutput(TMSAudioOutput& output) = 0;
    virtual gint GetPreviousOutput(TMSAudioOutput& output) = 0;
    virtual gint GetAvailableOutputsL(gint& count, CBufFlat*& outputsbuf) = 0;

    void GetCallType(TMSCallType& callType);
    gboolean IsUplActivated();

protected:
    TMSCallAdpt();
    virtual gint PostConstruct() = 0;

protected:
    gint iGlobalVol;
    gint iGlobalGain;
    TMSStreamState iUplState;
    TMSStreamState iDnlState;
    gint iCallType;
    };

} //namespace TMS

#endif // CALLADPT_H

