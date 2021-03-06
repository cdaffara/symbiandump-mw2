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

#ifndef QTMS_GLOBAL_GAIN_EFFECT_IMPL_H
#define QTMS_GLOBAL_GAIN_EFFECT_IMPL_H

#include <qtms.h>
#include <qtmsglobalgaineffect.h>
#include <tmseffectobsrvr.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSEffect;
}

namespace QTMS
{

// FORWARD DECLARATIONS
class QTMSEffect;

// QTMSGlobalGainEffectImpl class
class QTMSGlobalGainEffectImpl: public QTMSGlobalGainEffect, public TMS::TMSEffectObserver
{
public:
    static gint Create(QTMSEffect*& qglobalgain, TMS::TMSEffect*& tmsglobalgain);
    virtual ~QTMSGlobalGainEffectImpl();

    gint AddObserver(TMS::TMSEffectObserver& obsrvr, gpointer user_data);
    gint RemoveObserver(TMS::TMSEffectObserver& obsrvr);

    //from TMSEffectObserver
    void EffectsEvent(const TMS::TMSEffect& tmseffect, TMS::TMSSignalEvent event);

private:
    QTMSGlobalGainEffectImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_GLOBAL_GAIN_EFFECT_IMPL_H
// End of file
