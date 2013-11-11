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

#include <qtmsspeakersink.h>
#include <tmsspeakersink.h>

using namespace QTMS;
using namespace TMS;

QTMSSpeakerSink::QTMSSpeakerSink() :
    iSink(NULL)
{
}

QTMSSpeakerSink::~QTMSSpeakerSink()
{
    delete (static_cast<TMSSpeakerSink*> (iSink));
}

gint QTMSSpeakerSink::GetType(QTMSSinkType& sinktype)
{
    gint status(QTMS_RESULT_UNINITIALIZED_OBJECT);
    if (iSink) {
        //status = iSink->GetType(sinktype); //should be ok
        status = static_cast<TMSSpeakerSink*> (iSink)->GetType(sinktype);
    }
    return status;
}

// End of file
