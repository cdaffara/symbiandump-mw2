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

#include <tmssource.h>
#include <tmsformat.h>
#include <tmsstream.h>
#include <qtmseffect.h>
#include <qtmsstream.h>
#include "qtmsstreamimpl.h"
#include "qtmssource.h"
#include "qtmsclientsourceimpl.h"
#include "qtmsmodemsourceimpl.h"
#include "qtmsmicsourceimpl.h"
#include "qtmsclientsinkimpl.h"
#include "qtmsmodemsinkimpl.h"
#include "qtmsspeakersinkimpl.h"
#include "qtmspcmimpl.h"
#include "qtmsamrimpl.h"
#include "qtmsg711impl.h"
#include "qtmsg729impl.h"
#include "qtmsilbcimpl.h"
#include "qtmsvolumeeffectimpl.h"
#include "qtmsgaineffectimpl.h"
#include "qtmswrapperexport.h"

using namespace QTMS;
using namespace TMS;

QTMSStream::QTMSStream(void) :
    iStream(NULL)
{
}

QTMSStream::~QTMSStream()
{
    //delete iStream;
    //delete (static_cast<QTMSStreamImpl*> (iStream));
}

gint QTMSStream::AddSource(QTMSSource* qsource)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iStream && qsource) {
        QTMSSourceType qsrctype;
        TMSSource* tmssource(NULL);

        qsource->GetType(qsrctype);

        switch (qsrctype) {
        case QTMS_SOURCE_CLIENT:
            status = static_cast<QTMSClientSourceImpl*> (qsource)->GetSource(tmssource);
            break;
        case QTMS_SOURCE_MODEM:
            status = static_cast<QTMSModemSourceImpl*> (qsource)->GetSource(tmssource);
            break;
        case QTMS_SOURCE_MIC:
            status = static_cast<QTMSMicSourceImpl*> (qsource)->GetSource(tmssource);
            break;
        default:
            break;
        }
        if (status == QTMS_RESULT_SUCCESS && tmssource) {
            status = iStream->AddSource(tmssource);
        }
    }
    return status;
}

gint QTMSStream::RemoveSource(QTMSSource* qsource)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iStream && qsource) {
        QTMSSourceType qsrctype;
        TMSSource* tmssource(NULL);

        qsource->GetType(qsrctype);

        switch (qsrctype) {
        case QTMS_SOURCE_CLIENT:
            status = static_cast<QTMSClientSourceImpl*> (qsource)->GetSource(tmssource);
            break;
        case QTMS_SOURCE_MODEM:
            status = static_cast<QTMSModemSourceImpl*> (qsource)->GetSource(tmssource);
            break;
        case QTMS_SOURCE_MIC:
            status = static_cast<QTMSMicSourceImpl*> (qsource)->GetSource(tmssource);
            break;
        default:
            break;
        }
        if (status == QTMS_RESULT_SUCCESS && tmssource) {
            status = iStream->RemoveSource(tmssource);
        }
    }

    return status;
}

gint QTMSStream::AddSink(QTMSSink* qsink)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iStream && qsink) {
        QTMSSinkType qsinktype;
        TMSSink* tmssink(NULL);

        qsink->GetType(qsinktype);

        switch (qsinktype) {
        case QTMS_SINK_CLIENT:
            status = static_cast<QTMSClientSinkImpl*> (qsink)->GetSink(tmssink);
            break;
        case QTMS_SINK_MODEM:
            status = static_cast<QTMSModemSinkImpl*> (qsink)->GetSink(tmssink);
            break;
        case QTMS_SINK_SPEAKER:
            status = static_cast<QTMSSpeakerSinkImpl*> (qsink)->GetSink(tmssink);
            break;
        default:
            break;
        }
        if (status == QTMS_RESULT_SUCCESS && tmssink) {
            status = iStream->AddSink(tmssink);
        }
    }
    return status;
}

gint QTMSStream::RemoveSink(QTMSSink* qsink)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iStream && qsink) {
        QTMSSinkType qsinktype;
        TMSSink* tmssink(NULL);

        qsink->GetType(qsinktype);

        switch (qsinktype) {
        case QTMS_SINK_CLIENT:
            status = static_cast<QTMSClientSinkImpl*> (qsink)->GetSink(tmssink);
            break;
        case QTMS_SINK_MODEM:
            status = static_cast<QTMSModemSinkImpl*> (qsink)->GetSink(tmssink);
            break;
        case QTMS_SINK_SPEAKER:
            status = static_cast<QTMSSpeakerSinkImpl*> (qsink)->GetSink(tmssink);
            break;
        default:
            break;
        }
        if (status == QTMS_RESULT_SUCCESS && tmssink) {
            status = iStream->RemoveSink(tmssink);
        }
    }

    return status;
}

gint QTMSStream::SetFormat(QTMSFormat* qformat)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iStream && qformat) {
        QTMSFormatType fmttype;
        TMSFormat* tmsformat(NULL);
        qformat->GetType(fmttype);
        switch (fmttype) {
        case QTMS_FORMAT_PCM:
            status = static_cast<QTMSPCMFormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        case QTMS_FORMAT_AMR:
            status = static_cast<QTMSAMRFormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        case QTMS_FORMAT_G711:
            status = static_cast<QTMSG711FormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        case QTMS_FORMAT_G729:
            status = static_cast<QTMSG729FormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        case QTMS_FORMAT_ILBC:
            status = static_cast<QTMSILBCFormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        default:
            break;
        }

        status = iStream->SetFormat(tmsformat);
    }
    return status;
}

gint QTMSStream::ResetFormat(QTMSFormat* qformat)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iStream && qformat) {
        QTMSFormatType fmttype;
        TMSFormat* tmsformat(NULL);
        qformat->GetType(fmttype);
        switch (fmttype) {
        case QTMS_FORMAT_PCM:
            status = static_cast<QTMSPCMFormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        case QTMS_FORMAT_AMR:
            status = static_cast<QTMSAMRFormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        case QTMS_FORMAT_G711:
            status = static_cast<QTMSG711FormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        case QTMS_FORMAT_G729:
            status = static_cast<QTMSG729FormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        case QTMS_FORMAT_ILBC:
            status = static_cast<QTMSILBCFormatImpl*> (qformat)->GetFormat(tmsformat);
            break;
        default:
            break;
        }

        status = iStream->ResetFormat(tmsformat);
    }
    return status;
}

gint QTMSStream::AddEffect(QTMSEffect* qeffect)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream && qeffect) {
        QTMSEffectType effecttype;
        TMSEffect* tmseffect(NULL);
        qeffect->GetType(effecttype);

        switch (effecttype) {
        case QTMS_EFFECT_VOLUME:
            status = static_cast<QTMSVolumeEffectImpl*> (qeffect)->GetEffect(tmseffect);
            break;
        case QTMS_EFFECT_GAIN:
            status = static_cast<QTMSGainEffectImpl*> (qeffect)->GetEffect(tmseffect);
            break;
        default:
            break;
        }
        status = iStream->AddEffect(tmseffect);
    }
    return status;
}

gint QTMSStream::RemoveEffect(QTMSEffect* qeffect)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream && qeffect) {
        QTMSEffectType effecttype;
        TMSEffect* tmseffect(NULL);
        qeffect->GetType(effecttype);

        switch (effecttype) {
        case QTMS_EFFECT_VOLUME:
            status = static_cast<QTMSVolumeEffectImpl*> (qeffect)->GetEffect(tmseffect);
            break;
        case QTMS_EFFECT_GAIN:
            status = static_cast<QTMSGainEffectImpl*> (qeffect)->GetEffect(tmseffect);
            break;
        default:
            break;
        }
        status = iStream->RemoveEffect(tmseffect);
    }
    return status;
}

gint QTMSStream::GetState()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream) {
        status = iStream->GetState();
    }
    return status;
}

gint QTMSStream::GetStreamType()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream) {
        status = iStream->GetStreamType();
    }
    return status;
}

gint QTMSStream::GetStreamId()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream) {
        status = iStream->GetStreamId();
    }
    return status;
}

gint QTMSStream::Init(gint retrytime)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream) {
        status = iStream->Init(retrytime);
    }
    return status;
}

gint QTMSStream::Pause()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream) {
        status = iStream->Pause();
    }
    return status;
}

gint QTMSStream::Start(gint retrytime)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream) {
        status = iStream->Start(retrytime);
    }
    return status;
}

gint QTMSStream::Stop()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iStream) {
        status = iStream->Stop();
    }
    return status;
}

void QTMSStream::Deinit()
{
    if (iStream) {
        iStream->Deinit();
    }
}

