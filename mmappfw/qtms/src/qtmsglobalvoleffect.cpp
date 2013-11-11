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

#include<tmsglobalvoleffect.h>
#include <qtmsglobalvoleffect.h>
#include "qtmsglobalvoleffectimpl.h"
#include "qtmswrapperexport.h"

using namespace QTMS;
using namespace TMS;

QTMSGlobalVolEffect::QTMSGlobalVolEffect() :
    iEffect(NULL)
{
}

QTMSGlobalVolEffect::~QTMSGlobalVolEffect()
{
    delete (static_cast<TMSGlobalVolEffect*> (iEffect));
}

gint QTMSGlobalVolEffect::GetLevel(guint& level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGlobalVolEffect*> (iEffect)->GetLevel(level);
    }
    return status;
}

gint QTMSGlobalVolEffect::SetLevel(const guint level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGlobalVolEffect*> (iEffect)->SetLevel(level);
    }
    return status;
}

gint QTMSGlobalVolEffect::GetMaxLevel(guint& level)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGlobalVolEffect*> (iEffect)->GetMaxLevel(level);
    }
    return status;
}

gint QTMSGlobalVolEffect::GetType(QTMSSourceType& sourcetype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iEffect) {
        status = static_cast<TMSGlobalVolEffect*> (iEffect)->GetType(sourcetype);
    }
    return status;
}

// End of file
