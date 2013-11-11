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
 * Description: QT Bindings for TMS
 *
 */

#ifndef QTMSFACTORYIMPL_H_
#define QTMSFACTORYIMPL_H_

#include <e32cmn.h>
#include <qtms.h>
#include <qtmsver.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSFactory;
}

namespace QTMS
{

// FORWARD DECLARATIONS
class QTMSCall;
class QTMSFormat;
class QTMSEffect;
class QTMSBuffer;
class QTMSSource;
class QTMSSink;
class QTMSGlobalRouting;
class QTMSRTPSession;
class QTMSDTMF;
class QTMSRingTone;
class QTMSInbandTone;

// CLASS DECLARATION
class QTMSFactoryImpl
{
public:
    QTMSFactoryImpl(QTMSVer& ver);
    virtual ~QTMSFactoryImpl();

    gint CreateCall(QTMSCallType ctype, QTMSCall*& QTMScall, guint ctxid);
    gint DeleteCall(QTMSCall*& QTMSct);
    gint IsCallTypeSupported(QTMSCallType ctype, gboolean& flag);
    gint GetSupportedFormats(const QTMSStreamType strmtype, FormatVector& fmtlist);
    gint CreateFormat(QTMSFormatType fmttype, QTMSFormat*& QTMSfmt);
    gint DeleteFormat(QTMSFormat*& QTMSfmt);
    gint CreateEffect(QTMSEffectType QTMSeffecttype, QTMSEffect*& QTMSeffect);
    gint DeleteEffect(QTMSEffect*& QTMSeffect);
    gint CreateBuffer(QTMSBufferType buffertype, guint size, QTMSBuffer*& QTMSbuffer);
    gint DeleteBuffer(QTMSBuffer*& QTMSbuffer);
    gint CreateSource(QTMSSourceType srctype, QTMSSource*& QTMSsrc);
    gint DeleteSource(QTMSSource*& QTMSsrc);
    gint CreateSink(QTMSSinkType sinktype, QTMSSink*& QTMSsink);
    gint DeleteSink(QTMSSink*& QTMSsink);
    gint CreateGlobalRouting(QTMSGlobalRouting*& globrouting);
    gint DeleteGlobalRouting(QTMSGlobalRouting*& globrouting);
    gint CreateDTMF(QTMSStreamType streamid, QTMSDTMF*& dtmf);
    gint DeleteDTMF(QTMSDTMF*& dtmf);
    gint CreateRingTonePlayer(QTMSRingTone*& rt);
    gint DeleteRingTonePlayer(QTMSRingTone*& rt);
    gint CreateInbandTonePlayer(QTMSInbandTone*& inbandtone);
    gint DeleteInbandTonePlayer(QTMSInbandTone*& inbandtone);

private:
    TMS::TMSFactory *iFactory;
};

} //namespace QTMS

#endif
