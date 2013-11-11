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

#include <tmsclientsource.h>
#include <tmsbuffer.h>
#include <qtmsclientsource.h>

using namespace QTMS;
using namespace TMS;

QTMSClientSource::QTMSClientSource() :
    iSource(NULL)
{
}

QTMSClientSource::~QTMSClientSource()
{
    delete iSource;
}

gint QTMSClientSource::SetEnqueueMode(const gboolean enable)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        status = static_cast<TMSClientSource*> (iSource)->SetEnqueueMode(enable);
    }
    return status;
}

gint QTMSClientSource::GetEnqueueMode(gboolean& enable)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        status = static_cast<TMSClientSource*> (iSource)->GetEnqueueMode(enable);
    }
    return status;
}

gint QTMSClientSource::Flush()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        status = static_cast<TMSClientSource*> (iSource)->Flush();
    }
    return status;
}

gint QTMSClientSource::GetType(QTMSSourceType& sourcetype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        status = static_cast<TMSClientSource*> (iSource)->GetType(sourcetype);
    }
    return status;
}

gint QTMSClientSource::BufferFilled(QTMSBuffer& buffer)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        TMSBuffer& buf(reinterpret_cast<TMSBuffer&> (buffer));
        status = static_cast<TMSClientSource*> (iSource)->BufferFilled(buf);
    }
    return status;
}

// Push mode
gint QTMSClientSource::ProcessBuffer(QTMSBuffer* buffer)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        TMSBuffer* buf(reinterpret_cast<TMSBuffer*> (buffer));
        status = static_cast<TMSClientSource*> (iSource)->ProcessBuffer(buf);
    }
    return status;
}

// End of file
