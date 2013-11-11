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

#include <tmsvolumeeffect.h>
#include <qtmsvolumeeffect.h>
#include "qtmsvolumeeffectimpl.h"
#include "qtmswrapperexport.h"

using namespace QTMS;
using namespace TMS;

QTMSVolumeEffect::QTMSVolumeEffect() :
    iEffect(NULL)
{
}

QTMSVolumeEffect::~QTMSVolumeEffect()
{
    delete (static_cast<TMSVolumeEffect*> (iEffect));
}

gint QTMSVolumeEffect::GetLevel(guint& level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSVolumeEffect*> (iEffect)->GetLevel(level);
    }
    return status;
}

gint QTMSVolumeEffect::SetLevel(const guint level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSVolumeEffect*> (iEffect)->SetLevel(level);
    }
    return status;
}

gint QTMSVolumeEffect::GetMaxLevel(guint& level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSVolumeEffect*> (iEffect)->GetMaxLevel(level);
    }
    return status;
}

gint QTMSVolumeEffect::GetType(QTMSSourceType& sourcetype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSVolumeEffect*> (iEffect)->GetType(sourcetype);
    }
    return status;
}

// End of file
