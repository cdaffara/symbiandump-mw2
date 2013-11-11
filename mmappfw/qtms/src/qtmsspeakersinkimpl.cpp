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

#include <qtms.h>
#include <tmsspeakersink.h>
#include "qtmsspeakersinkimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSSpeakerSinkImpl::QTMSSpeakerSinkImpl()
{
}

QTMSSpeakerSinkImpl::~QTMSSpeakerSinkImpl()
{
}

gint QTMSSpeakerSinkImpl::Create(QTMSSink*& qsink, TMS::TMSSink*& tmssink)
{
    gint ret(QTMS_RESULT_INSUFFICIENT_MEMORY);
    QTMSSpeakerSinkImpl* self = new QTMSSpeakerSinkImpl();
    if (self) {
        ret = self->PostConstruct();
        if (ret != QTMS_RESULT_SUCCESS) {
            delete self;
            self = NULL;
        }
        self->iSink = tmssink;
    }
    qsink = self;
    return ret;
}

gint QTMSSpeakerSinkImpl::PostConstruct()
{
    gint ret(QTMS_RESULT_SUCCESS);
    return ret;
}

gint QTMSSpeakerSinkImpl::GetSink(TMS::TMSSink*& tmssink)
{
    gint ret(QTMS_RESULT_UNINITIALIZED_OBJECT);

    if (iSink) {
        tmssink = iSink;
        ret = QTMS_RESULT_SUCCESS;
    }
    return ret;
}

// End of file
