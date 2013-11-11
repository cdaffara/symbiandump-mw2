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

#include <tmsringtone.h>
#include <qtmsringtone.h>
#include "qtmsringtoneimpl.h"

using namespace QTMS;

QTMSRingTone::QTMSRingTone() :
    iRingTone(NULL)
{
}

QTMSRingTone::~QTMSRingTone()
{
    delete iRingTone;
}

gint QTMSRingTone::Init(const QTMSRingToneType type, GString* str, GString* tts)
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRingTone) {
        status = iRingTone->Init(type, str, tts);
    }
    return status;
}

gint QTMSRingTone::Deinit()
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRingTone) {
        status = iRingTone->Deinit();
    }
    return status;
}

gint QTMSRingTone::Play()
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRingTone) {
        status = iRingTone->Play();
    }
    return status;
}

gint QTMSRingTone::Stop()
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRingTone) {
        status = iRingTone->Stop();
    }
    return status;
}

gint QTMSRingTone::Pause()
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRingTone) {
        status = iRingTone->Pause();
    }
    return status;
}

gint QTMSRingTone::Mute()
{
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRingTone) {
        status = iRingTone->Mute();
    }
    return status;
}

// End of file
