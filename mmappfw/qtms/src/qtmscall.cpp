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

#include <tmscall.h>
#include <qtmscall.h>
#include <tmsstream.h>
#include "qtmscallimpl.h"
#include "qtmsstreamimpl.h"

using namespace QTMS;
using namespace TMS;

QTMSCall::QTMSCall() :
    iTmsCall(NULL)
{
}

QTMSCall::~QTMSCall()
{
}

QTMSCallType QTMSCall::GetCallType()
{
    QTMSCallType calltype(-1);
    if (iTmsCall) {
        calltype = iTmsCall->GetCallType();
    }
    return calltype;
}

gint QTMSCall::GetCallContextId(guint& ctxid)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iTmsCall) {
        status = iTmsCall->GetCallContextId(ctxid);
    }
    return status;
}

gint QTMSCall::CreateStream(const QTMSStreamType type, QTMSStream*& qstrm)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);

    TMSStream* tmsstrm(NULL);
    if (iTmsCall) {
        status = iTmsCall->CreateStream(type, tmsstrm);
    }

    if (status == TMS_RESULT_SUCCESS) {
        QTMSCallType calltype = iTmsCall->GetCallType();
        switch (type) {
        case TMS_STREAM_UPLINK:
        case TMS_STREAM_DOWNLINK:
            status = QTMSStreamImpl::Create(calltype, type, qstrm, tmsstrm);
            break;
        default:
            status = QTMS_RESULT_STREAM_TYPE_NOT_SUPPORTED;
            break;
        }
    }
    return status;
}

gint QTMSCall::DeleteStream(QTMSStream*& strm)
{
    gint status(QTMS_RESULT_SUCCESS);
    delete strm;
    strm = NULL;
    return status;
}

// End of file
