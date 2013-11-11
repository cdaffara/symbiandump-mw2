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

#include <tmsg729format.h>
#include <qtmsg729format.h>
#include "tmsutility.h"

using namespace QTMS;
using namespace TMS;

QTMSG729Format::QTMSG729Format()
{
}

QTMSG729Format::~QTMSG729Format()
{
}

gint QTMSG729Format::SetVADMode(const gboolean aVad)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG729Format*> (iFormat)->SetVADMode(aVad);
    }
    return status;
}

gint QTMSG729Format::GetVADMode(gboolean& aVad)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iFormat) {
        status = static_cast<TMSG729Format*> (iFormat)->GetVADMode(aVad);
    }
    return status;
}

// End of file
