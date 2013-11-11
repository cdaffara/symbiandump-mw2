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

#include <tmsclientsink.h>
#include <tmsbuffer.h>
#include <qtmsclientsink.h>

using namespace QTMS;
using namespace TMS;

QTMSClientSink::QTMSClientSink() :
    iSink(NULL)
{
}

QTMSClientSink::~QTMSClientSink()
{
    delete iSink;
}

gint QTMSClientSink::BufferProcessed(QTMSBuffer* buffer)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSink) {
        TMSBuffer* buf(reinterpret_cast<TMSBuffer*> (buffer));
        status = static_cast<TMSClientSink*> (iSink)->BufferProcessed(buf);
    }
    return status;
}

gint QTMSClientSink::GetType(QTMSSinkType& sinktype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSink) {
        status = static_cast<TMSClientSink*> (iSink)->GetType(sinktype);
    }
    return status;
}

// End of file
