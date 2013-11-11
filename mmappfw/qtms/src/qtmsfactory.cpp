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

#include <tms.h>
#include <QObject>
#include <qtmsfactory.h>
#include "qtmswrapperexport.h"
#include "qtmsfactoryimpl.h"

using namespace QTMS;

QTMSFactory::QTMSFactory(QTMSVer& ver)
{
    impl = new QTMSFactoryImpl(ver);
}

QTMSFactory::~QTMSFactory()
{
    delete impl;
}

gint QTMSFactory::CreateFactory(QTMSFactory*& tmsfactory, QTMSVer& ver)
{
    QTMSFactory* self = new QTMSFactory(ver);
    tmsfactory = self;
    return QTMS_RESULT_SUCCESS;
}

gint QTMSFactory::CreateCall(QTMSCallType ctype, QTMSCall*& tmscall, guint ctxid)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateCall(ctype, tmscall, ctxid);
    }
    return status;
}

gint QTMSFactory::DeleteCall(QTMSCall*& tmscall)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteCall(tmscall);
    }
    return status;
}

gint QTMSFactory::IsCallTypeSupported(QTMSCallType ctype, gboolean& flag)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->IsCallTypeSupported(ctype, flag);
    }
    return status;
}

gint QTMSFactory::GetSupportedFormats(const QTMSStreamType strmtype, FormatVector& fmtlist)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->GetSupportedFormats(strmtype, fmtlist);
    }
    return status;
}

gint QTMSFactory::CreateFormat(QTMSFormatType qfmttype, QTMSFormat*& qfmt)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateFormat(qfmttype, qfmt);
    }
    return status;

}

gint QTMSFactory::DeleteFormat(QTMSFormat*& qfmt)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteFormat(qfmt);
    }
    return status;
}

gint QTMSFactory::CreateEffect(QTMSEffectType tmseffecttype, QTMSEffect*& tmseffect)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateEffect(tmseffecttype, tmseffect);
    }
    return status;
}

gint QTMSFactory::DeleteEffect(QTMSEffect*& qtmseffect)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        impl->DeleteEffect(qtmseffect);
    }
    return status;
}

gint QTMSFactory::CreateBuffer(QTMSBufferType buffertype, guint size, QTMSBuffer*& qbuffer)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateBuffer(buffertype, size, qbuffer);
    }
    return status;
}

gint QTMSFactory::DeleteBuffer(QTMSBuffer*& qbuffer)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteBuffer(qbuffer);
    }
    return status;
}

gint QTMSFactory::CreateSource(QTMSSourceType srctype, QTMSSource*& qtmssrc)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateSource(srctype, qtmssrc);
    }
    return status;
}

gint QTMSFactory::DeleteSource(QTMSSource*& qtmssrc)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteSource(qtmssrc);
    }
    return status;
}

gint QTMSFactory::CreateSink(QTMSSinkType sinktype, QTMSSink*& qtmssink)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateSink(sinktype, qtmssink);
    }
    return status;
}

gint QTMSFactory::DeleteSink(QTMSSink*& qtmssink)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteSink(qtmssink);
    }
    return status;
}

gint QTMSFactory::CreateGlobalRouting(QTMSGlobalRouting*& globalrouting)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateGlobalRouting(globalrouting);
    }
    return status;
}

gint QTMSFactory::DeleteGlobalRouting(QTMSGlobalRouting*& globrouting)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteGlobalRouting(globrouting);
    }
    return status;
}

gint QTMSFactory::CreateDTMF(QTMSStreamType streamtype, QTMSDTMF*& dtmf)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateDTMF(streamtype, dtmf);
    }
    return status;
}

gint QTMSFactory::DeleteDTMF(QTMSDTMF*& dtmf)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteDTMF(dtmf);
    }
    return status;
}

gint QTMSFactory::CreateRingTonePlayer(QTMSRingTone*& rt)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateRingTonePlayer(rt);
    }
    return status;
}

gint QTMSFactory::DeleteRingTonePlayer(QTMSRingTone*& rt)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteRingTonePlayer(rt);
    }
    return status;
}

gint QTMSFactory::CreateInbandTonePlayer(QTMSInbandTone*& inbandtone)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->CreateInbandTonePlayer(inbandtone);
    }
    return status;
}

gint QTMSFactory::DeleteInbandTonePlayer(QTMSInbandTone*& inbandtone)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (impl) {
        status = impl->DeleteInbandTonePlayer(inbandtone);
    }
    return status;
}
