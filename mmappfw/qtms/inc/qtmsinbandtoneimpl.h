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

#ifndef QTMS_INBAND_IMPL_H
#define QTMS_INBAND_IMPL_H

#include <tmsinbandtoneobsrvr.h>
#include <qtms.h>
#include <qtmsinbandtone.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSInbandTone;
}

namespace QTMS
{

// QTMSInbandImpl class
class QTMSInbandToneImpl: public QTMSInbandTone, public TMS::TMSInbandToneObserver
{
public:
    static gint Create(QTMSInbandTone*& qinband, TMS::TMSInbandTone*& tmsinband);
    virtual ~QTMSInbandToneImpl();

    gint AddObserver(TMS::TMSInbandToneObserver& obsrvr, gpointer user_data);
    gint RemoveObserver(TMS::TMSInbandToneObserver& obsrvr);

    //from TMS::TMSInbandToneObserver
    void InbandToneEvent(const TMS::TMSInbandTone& inbandtone, TMS::TMSSignalEvent event);

private:
    QTMSInbandToneImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_DTMF_IMPL_H
// End of file
