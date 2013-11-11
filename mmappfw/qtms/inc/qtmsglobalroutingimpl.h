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

#ifndef QTMS_GLOBAL_ROUTING_IMPL_H
#define QTMS_GLOBAL_ROUTING_IMPL_H

#include <tms.h>
#include <qtms.h>
#include <qtmsglobalrouting.h>
#include <tmsglobalroutingobsrvr.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSGlobalRouting;
}

namespace QTMS
{

// QTMSGlobalRoutingImpl class
class QTMSGlobalRoutingImpl: public QTMSGlobalRouting, public TMS::TMSGlobalRoutingObserver
{
public:

    static gint Create(QTMSGlobalRouting*& qrouting, TMS::TMSGlobalRouting*& tmsrouting);
    virtual ~QTMSGlobalRoutingImpl();

    gint AddObserver(TMS::TMSGlobalRoutingObserver& obsrvr, gpointer user_data);
    gint RemoveObserver(TMS::TMSGlobalRoutingObserver& obsrvr);

    //from TMSGlobalRoutingObserver
    void GlobalRoutingEvent(const TMS::TMSGlobalRouting& routing, TMS::TMSSignalEvent event,
        TMS::TMSAudioOutput output);

private:
    QTMSGlobalRoutingImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_GLOBAL_ROUTING_IMPL_H
// End of file
