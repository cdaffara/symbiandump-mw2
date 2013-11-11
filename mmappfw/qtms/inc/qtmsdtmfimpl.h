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

#ifndef QTMS_DTMF_IMPL_H
#define QTMS_DTMF_IMPL_H

#include <tms.h>
#include <tmsdtmfobsrvr.h>
#include <qtms.h>
#include <qtmsdtmf.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSDTMF;
}

namespace QTMS
{

// QTMSDTMFImpl class
class QTMSDTMFImpl: public QTMSDTMF, public TMS::TMSDTMFObserver
{
public:
    static gint Create(QTMSDTMF*& qdtmf, TMS::TMSDTMF*& tmsdtmf);
    virtual ~QTMSDTMFImpl();

    gint AddObserver(TMS::TMSDTMFObserver& obsrvr, gpointer user_data);
    gint RemoveObserver(TMS::TMSDTMFObserver& obsrvr);

    //from TMS::TMSDTMFObserver
    void DTMFEvent(const TMS::TMSDTMF& dtmf, TMS::TMSSignalEvent event);

private:
    QTMSDTMFImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_DTMF_IMPL_H
// End of file
