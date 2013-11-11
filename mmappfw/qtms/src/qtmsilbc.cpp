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

#include <tmsilbcformat.h>
#include <qtmsilbcformat.h>
#include "tmsutility.h"

using namespace QTMS;
using namespace TMS;

QTMSILBCFormat::QTMSILBCFormat()
{
}

QTMSILBCFormat::~QTMSILBCFormat()
{
}

gint QTMSILBCFormat::SetMode(const QTMSG711CodecMode aMode)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSILBCFormat*> (iFormat)->SetMode(aMode);
    }
    return status;
}

gint QTMSILBCFormat::GetMode(QTMSG711CodecMode& aMode)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSILBCFormat*> (iFormat)->GetMode(aMode);
    }
    return status;
}

gint QTMSILBCFormat::SetCNG(const gboolean aCng)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSILBCFormat*> (iFormat)->SetCNG(aCng);
    }
    return status;
}

gint QTMSILBCFormat::GetCNG(gboolean& aCng)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSILBCFormat*> (iFormat)->GetCNG(aCng);
    }
    return status;
}

gint QTMSILBCFormat::SetVADMode(const gboolean aVad)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSILBCFormat*> (iFormat)->SetVADMode(aVad);
    }
    return status;
}

gint QTMSILBCFormat::GetVADMode(gboolean& aVad)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSILBCFormat*> (iFormat)->GetVADMode(aVad);
    }
    return status;
}

// End of file
