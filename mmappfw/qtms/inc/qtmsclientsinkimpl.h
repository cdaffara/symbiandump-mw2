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

#ifndef QTMS_CLIENT_SINK_IMPL_H
#define QTMS_CLIENT_SINK_IMPL_H

#include <qtms.h>
#include <qtmsclientsink.h>
#include <tmsclientsinkobsrvr.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSBuffer;
class TMSSink;
}

namespace QTMS
{

// QTMSClientSinkImpl class
class QTMSClientSinkImpl: public QTMSClientSink, public TMS::TMSClientSinkObserver
{
public:
    static gint Create(QTMSSink*& qsink, TMS::TMSSink*& tmsclientsink);
    virtual ~QTMSClientSinkImpl();

    gint AddObserver(TMS::TMSClientSinkObserver& obsrvr, gpointer user_data);
    gint RemoveObserver(TMS::TMSClientSinkObserver& obsrvr);

    gint GetSink(TMS::TMSSink*& tmssink);

    // From TMSClientSinkObserver
    void ProcessBuffer(const TMS::TMSBuffer* buffer);

private:
    QTMSClientSinkImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_CLIENT_SINK_IMPL_H
// End of file
