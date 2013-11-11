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

#include <tmsmodemsink.h>
#include "tmsmodemsinkbody.h"

using namespace TMS;

EXPORT_C TMSModemSink::TMSModemSink() :
    iBody(NULL)
    {
    }

EXPORT_C TMSModemSink::~TMSModemSink()
    {
    delete iBody;
    }

EXPORT_C gint TMSModemSink::GetType(TMSSinkType& sinktype)
    {
    gint status(TMS_RESULT_UNINITIALIZED_OBJECT);
    if (iBody)
        {
        status = iBody->GetType(sinktype);
        }
    return status;
    }

// End of file
