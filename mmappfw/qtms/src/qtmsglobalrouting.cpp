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

#include <tmsglobalrouting.h>
#include <qtmsglobalrouting.h>
#include "qtmsglobalroutingimpl.h"
#include "qtmswrapperexport.h"

using namespace QTMS;
using namespace TMS;

QTMSGlobalRouting::QTMSGlobalRouting() :
    iRouting(NULL)
{
}

QTMSGlobalRouting::~QTMSGlobalRouting()
{
    delete iRouting;
}

gint QTMSGlobalRouting::SetOutput(const QTMSAudioOutput output)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRouting) {
        status = iRouting->SetOutput(output);
    }
    return status;
}

gint QTMSGlobalRouting::GetOutput(QTMSAudioOutput& output)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRouting) {
        status = iRouting->GetOutput(output);
    }
    return status;
}

gint QTMSGlobalRouting::GetPreviousOutput(QTMSAudioOutput& output)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRouting) {
        status = iRouting->GetPreviousOutput(output);
    }
    return status;
}

gint QTMSGlobalRouting::GetAvailableOutputs(OutputVector& outputs)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iRouting) {
        status = iRouting->GetAvailableOutputs(outputs);
    }
    return status;
}

// End of file
