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

#include "tmsmembufferbody.h"
#include "tmsmembuffer.h"

using namespace TMS;

EXPORT_C TMSMemBuffer::TMSMemBuffer() :
    iBody(NULL)
    {
    }

EXPORT_C TMSMemBuffer::~TMSMemBuffer()
    {
    delete iBody;
    }

EXPORT_C gint TMSMemBuffer::GetType(TMSBufferType& buftype)
    {
    return iBody->GetType(buftype);
    }

gint TMSMemBuffer::GetTimeStamp(guint& ts)
    {
    return iBody->GetTimeStamp(ts);
    }

gint TMSMemBuffer::SetTimeStamp(const guint ts)
    {
    return iBody->SetTimeStamp(ts);
    }

gint TMSMemBuffer::GetDataSize(guint& size)
    {
    return iBody->GetDataSize(size);
    }

gint TMSMemBuffer::SetDataSize(const guint size)
    {
    return iBody->SetDataSize(size);
    }

gint TMSMemBuffer::GetDataPtr(guint8*& bufptr)
    {
    return iBody->GetDataPtr(bufptr);
    }

gint TMSMemBuffer::GetLastBufferFlag(gboolean& flag)
    {
    return iBody->GetLastBufferFlag(flag);
    }

gint TMSMemBuffer::SetLastBufferFlag(gboolean flag)
    {
    return iBody->SetLastBufferFlag(flag);
    }

