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

#include <qtmsmodemsink.h>
#include <tmsmodemsink.h>
#include "qtmsmodemsinkimpl.h"
#include "qtmswrapperexport.h"

using namespace QTMS;
using namespace TMS;

QTMSModemSink::QTMSModemSink() :
    iSink(NULL)
{
}

QTMSModemSink::~QTMSModemSink()
{
    delete (static_cast<TMSModemSink*> (iSink));
}

gint QTMSModemSink::GetType(QTMSSinkType& sinktype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSink) {
        //status = iSink->GetType(sinktype);
        status = static_cast<TMSModemSink*> (iSink)->GetType(sinktype);
    }
    return status;
}

// End of file
