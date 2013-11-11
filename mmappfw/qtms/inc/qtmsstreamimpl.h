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

#ifndef QTMS_STREAM_IMPL_H
#define QTMS_STREAM_IMPL_H

#include <qtms.h>
#include <qtmsstream.h>
#include <tmsstreamobsrvr.h>
#include <QObject>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSStream;
}

namespace QTMS
{

// QTMSStreamImpl class
class QTMSStreamImpl: public QTMSStream, public TMS::TMSStreamObserver
{
public:
    static gint Create(QTMSCallType callType, QTMSStreamType stype, QTMSStream*& qstrm,
        TMS::TMSStream*& tmsstrm);

    virtual ~QTMSStreamImpl();

    // From QTMSStreamBody
    gint AddObserver(TMS::TMSStreamObserver& obsrvr, gpointer user_data);
    gint RemoveObserver(TMS::TMSStreamObserver& obsrvr);

    // From TMSStreamObserver
    void TMSStreamEvent(const TMS::TMSStream& stream, TMS::TMSSignalEvent event);

protected:
    QTMSStreamImpl();

    gint PostConstruct(QTMSCallType callType, QTMSStreamType stype, QTMSStream& parent);

    gint CreateQueue(const gint aNumSlots);
    void ReceiveMsgQHandlerEventsL();
};

} //namespace QTMS

#endif // QTMS_STREAM_IMPL_H
// End of file
