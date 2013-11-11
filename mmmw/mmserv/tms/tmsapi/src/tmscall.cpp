/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#include <tmscall.h>
#include "tmscallbody.h"

using namespace TMS;

EXPORT_C TMSCall::TMSCall() :
    iBody(NULL)
    {
    }

EXPORT_C TMSCall::~TMSCall()
    {
    delete iBody;
    }

EXPORT_C TMSCallType TMSCall::GetCallType()
    {
    TMSCallType calltype(-1);
    if (iBody)
        {
        calltype = iBody->GetCallType();
        }
    return calltype;
    }

EXPORT_C gint TMSCall::GetCallContextId(guint& ctxid)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetCallContextId(ctxid);
        }
    return status;
    }

EXPORT_C gint TMSCall::CreateStream(const TMSStreamType type, TMSStream*& strm)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->CreateStream(type, strm);
        }
    return status;
    }

EXPORT_C gint TMSCall::DeleteStream(TMSStream*& strm)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->DeleteStream(strm);
        }
    return status;
    }

// End of file
