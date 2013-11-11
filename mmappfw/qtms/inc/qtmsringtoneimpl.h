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

#ifndef QTMSRINGTONEBODYIMPL_H
#define QTMSRINGTONEBODYIMPL_H

#include <tms.h>
#include <tmsringtoneobsrvr.h>
#include <qtms.h>
#include <qtmsringtone.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSRingTone;
}

namespace QTMS
{

// QTMSRingToneImpl class
class QTMSRingToneImpl: public QTMSRingTone, public TMS::TMSRingToneObserver
{
public:
    static gint Create(QTMSRingTone*& qrt, TMS::TMSRingTone*& tmsrt);
    virtual ~QTMSRingToneImpl();

    gint AddObserver(TMS::TMSRingToneObserver& obsrvr, gpointer user_data);
    gint RemoveObserver(TMS::TMSRingToneObserver& obsrvr);

    //from TMS::TMSRingToneObserver
    void RingtoneEvent(const TMS::TMSRingTone& rt, TMS::TMSSignalEvent event);

private:
    QTMSRingToneImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif //QTMSRINGTONEBODYIMPL_H
// End of file

