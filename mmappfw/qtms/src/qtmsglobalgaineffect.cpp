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

#include <tmsglobalgaineffect.h>
#include <qtmsglobalgaineffect.h>
#include "qtmsglobalgaineffectimpl.h"
#include "qtmswrapperexport.h"

using namespace QTMS;
using namespace TMS;

QTMSGlobalGainEffect::QTMSGlobalGainEffect() :
    iEffect(NULL)
{
}

QTMSGlobalGainEffect::~QTMSGlobalGainEffect()
{
    delete (static_cast<TMSGlobalGainEffect*> (iEffect));
}

gint QTMSGlobalGainEffect::GetLevel(guint& level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGlobalGainEffect*> (iEffect)->GetLevel(level);
    }
    return status;
}

gint QTMSGlobalGainEffect::SetLevel(const guint level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGlobalGainEffect*> (iEffect)->SetLevel(level);
    }
    return status;
}

gint QTMSGlobalGainEffect::GetMaxLevel(guint& level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGlobalGainEffect*> (iEffect)->GetMaxLevel(level);
    }
    return status;
}

gint QTMSGlobalGainEffect::GetType(QTMSSourceType& sourcetype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGlobalGainEffect*> (iEffect)->GetType(sourcetype);
    }
    return status;
}

// End of file
