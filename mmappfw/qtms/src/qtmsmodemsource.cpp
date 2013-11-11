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

#include <qtmsmodemsource.h>
#include <tmsmodemsource.h>

using namespace QTMS;
using namespace TMS;

QTMSModemSource::QTMSModemSource() :
    iSource(NULL)
{
}

QTMSModemSource::~QTMSModemSource()
{
    delete (static_cast<TMSModemSource*> (iSource));
}

gint QTMSModemSource::GetType(QTMSSourceType& sourcetype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSource) {
        //status = iSource->GetType(sourcetype);
        status = static_cast<TMSModemSource*> (iSource)->GetType(sourcetype);
    }
    return status;
}

// End of file
