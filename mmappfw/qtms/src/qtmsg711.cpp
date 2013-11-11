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

#include <tmsg711format.h>
#include <qtmsg711format.h>

using namespace QTMS;
using namespace TMS;

QTMSG711Format::QTMSG711Format()
{
}

QTMSG711Format::~QTMSG711Format()
{
}

gint QTMSG711Format::SetMode(const QTMSG711CodecMode aMode)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG711Format*> (iFormat)->SetMode(aMode);
    }
    return status;
}

gint QTMSG711Format::GetMode(QTMSG711CodecMode& aMode)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG711Format*> (iFormat)->GetMode(aMode);
    }
    return status;
}

gint QTMSG711Format::SetCNG(const gboolean aCng)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG711Format*> (iFormat)->SetCNG(aCng);
    }
    return status;
}

gint QTMSG711Format::GetCNG(gboolean& aCng)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG711Format*> (iFormat)->GetCNG(aCng);
    }
    return status;
}

gint QTMSG711Format::SetPlc(const gboolean aPlc)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG711Format*> (iFormat)->SetPlc(aPlc);
    }
    return status;
}

gint QTMSG711Format::GetPlc(gboolean& aPlc)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG711Format*> (iFormat)->GetPlc(aPlc);
    }
    return status;
}

gint QTMSG711Format::SetVADMode(const gboolean aVad)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG711Format*> (iFormat)->SetVADMode(aVad);
    }
    return status;
}

gint QTMSG711Format::GetVADMode(gboolean& aVad)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG711Format*> (iFormat)->GetVADMode(aVad);
    }
    return status;
}

// End of file
