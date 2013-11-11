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

#ifndef QTMS_SPEAKER_SINK_IMPL_H
#define QTMS_SPEAKER_SINK_IMPL_H

#include <qtms.h>
#include <qtmsspeakersink.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSSink;
}

namespace QTMS
{

// QTMSSpeakerSinkImpl class
class QTMSSpeakerSinkImpl: public QTMSSpeakerSink
{
public:
    static gint Create(QTMSSink*& qsink, TMS::TMSSink*& tmssink);
    virtual ~QTMSSpeakerSinkImpl();

    gint GetSink(TMS::TMSSink*& tmssink);

private:
    QTMSSpeakerSinkImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_SPEAKER_SINK_IMPL_H
// End of file
