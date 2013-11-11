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

#include <tmsgaineffect.h>
#include <qtmsgaineffect.h>
#include "qtmsgaineffectimpl.h"
#include "qtmswrapperexport.h"

using namespace QTMS;
using namespace TMS;

QTMSGainEffect::QTMSGainEffect() :
    iEffect(NULL)
{
}

QTMSGainEffect::~QTMSGainEffect()
{
    delete iEffect;
}

gint QTMSGainEffect::GetLevel(guint& level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGainEffect*> (iEffect)->GetLevel(level);
    }
    return status;
}

gint QTMSGainEffect::SetLevel(const guint level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGainEffect*> (iEffect)->SetLevel(level);
    }
    return status;
}

gint QTMSGainEffect::GetMaxLevel(guint& level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGainEffect*> (iEffect)->GetMaxLevel(level);
    }
    return status;
}

gint QTMSGainEffect::GetType(QTMSSourceType& sourcetype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGainEffect*> (iEffect)->GetType(sourcetype);
    }
    return status;
}

// End of file
