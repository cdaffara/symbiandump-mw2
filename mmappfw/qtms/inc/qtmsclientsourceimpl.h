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

#ifndef QTMS_CLIENT_SOURCE_IMPL_H
#define QTMS_CLIENT_SOURCE_IMPL_H

#include <qtms.h>
#include <qtmsclientsource.h>
#include <tmsclientsourceobsrvr.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSBuffer;
class TMSSource;
}

namespace QTMS
{

// QTMSClientSourceImpl class
class QTMSClientSourceImpl: public QTMSClientSource, public TMS::TMSClientSourceObserver
{
public:

    static gint Create(QTMSSource*& qsource, TMS::TMSSource*& tmssource);
    virtual ~QTMSClientSourceImpl();

    gint AddObserver(TMS::TMSClientSourceObserver& obsrvr, gpointer user_data);
    gint RemoveObserver(TMS::TMSClientSourceObserver& obsrvr);

    gint GetSource(TMS::TMSSource*& tmssource);

    //From TMSClientSourceObserver
    void FillBuffer(TMS::TMSBuffer& buffer);
    void BufferProcessed(const TMS::TMSBuffer* buffer, gint reason);

private:
    QTMSClientSourceImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_CLIENT_SOURCE_IMPL_H
// End of file
