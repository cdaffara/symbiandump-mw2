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

#include <qtmsinbandtone.h>
#include <tmsinbandtone.h>
#include "qtmswrapperexport.h"
#include "qtmsinbandtoneimpl.h"

using namespace QTMS;

QTMSInbandTone::QTMSInbandTone(void) :
    iInbandTone(NULL)
{
}

QTMSInbandTone::~QTMSInbandTone(void)
{
    delete iInbandTone;
}

gint QTMSInbandTone::Start(QTMSInbandToneType inbandtonetype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iInbandTone) {
        status = iInbandTone->Start(inbandtonetype);
    }
    return status;
}

gint QTMSInbandTone::Stop()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iInbandTone) {
        status = iInbandTone->Stop();
    }
    return status;
}

