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

#ifndef TMSFACTORYIMPL_H_
#define TMSFACTORYIMPL_H_

#include <e32cmn.h>
#include <w32std.h>
#include <tms.h>
#include <tmsver.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSCall;
class TMSFormat;
class TMSEffect;
class TMSBuffer;
class TMSSource;
class TMSSink;
class TMSGlobalRouting;
class TMSDTMF;
class TMSInbandTone;

// CLASS DECLARATION
class TMSFactoryImpl
    {
public:
    TMSFactoryImpl(TMSVer& ver);
    virtual ~TMSFactoryImpl();

    gint CreateCall(TMSCallType ctype, TMSCall*& tmscall, guint ctxid);
    gint DeleteCall(TMSCall*& tmsct);
    gint IsCallTypeSupported(TMSCallType ctype, gboolean& flag);
    gint GetSupportedFormats(const TMSStreamType strmtype,
            FormatVector& fmtlist);
    gint CreateFormat(TMSFormatType fmttype, TMSFormat*& tmsfmt);
    gint DeleteFormat(TMSFormat*& tmsfmt);
    gint CreateEffect(TMSEffectType tmseffecttype, TMSEffect*& tmseffect);
    gint DeleteEffect(TMSEffect*& tmseffect);
    gint CreateBuffer(TMSBufferType buffertype, guint size,
            TMSBuffer*& tmsbuffer);
    gint DeleteBuffer(TMSBuffer*& tmsbuffer);
    gint CreateSource(TMSSourceType srctype, TMSSource*& tmssrc);
    gint DeleteSource(TMSSource*& tmssrc);
    gint CreateSink(TMSSinkType sinktype, TMSSink*& tmssink);
    gint DeleteSink(TMSSink*& tmssink);
    gint CreateGlobalRouting(TMSGlobalRouting*& globrouting);
    gint DeleteGlobalRouting(TMSGlobalRouting*& globrouting);
    gint CreateDTMF(TMSStreamType streamid, TMSDTMF*& dtmf);
    gint DeleteDTMF(TMSDTMF*& dtmf);
    gint CreateRingTonePlayer(TMSRingTone*& rt);
    gint CreateRingTonePlayer(TMSRingTone*& rt, RWindow& window, gint scrid);
    gint DeleteRingTonePlayer(TMSRingTone*& rt);
    gint CreateInbandTonePlayer(TMSInbandTone*& inbandtone);
    gint DeleteInbandTonePlayer(TMSInbandTone*& inbandtone);

private:
    gint QueryDevice(const TMSStreamType strmtype);

private:
    RArray<TUint32> iFormats;
    gint iFrameSize;
    };

} //namespace TMS

#endif //TMSFACTORYIMPL_H_

