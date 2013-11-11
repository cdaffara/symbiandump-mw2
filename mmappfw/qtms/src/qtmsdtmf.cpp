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

#include <tmsdtmf.h>
#include <qtmsdtmf.h>
#include "qtmsdtmfimpl.h"
//#include "qtmswrapperexport.h"

using namespace QTMS;

QTMSDTMF::QTMSDTMF(void) :
    iDtmf(NULL)
{
}

QTMSDTMF::~QTMSDTMF(void)
{
    delete iDtmf;
}

gint QTMSDTMF::Start()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDtmf) {
        status = iDtmf->Start();
    }
    return status;
}

gint QTMSDTMF::Stop()
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDtmf) {
        status = iDtmf->Stop();
    }
    return status;
}

gint QTMSDTMF::SetTone(GString* string)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDtmf) {
        status = iDtmf->SetTone(string);
    }
    return status;
}

gint QTMSDTMF::ContinueDTMFStringSending(gboolean aContinue)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iDtmf) {
        status = iDtmf->ContinueDTMFStringSending(aContinue);
    }
    return status;
}
