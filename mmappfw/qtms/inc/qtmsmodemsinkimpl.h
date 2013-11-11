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

#ifndef QTMS_MODEM_SINK_IMPL_H
#define QTMS_MODEM_SINK_IMPL_H

#include <qtms.h>
#include <qtmsmodemsink.h>
#include <qtmssink.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSSink;
}

namespace QTMS
{

// QTMSModemSinkImpl class
class QTMSModemSinkImpl: public QTMSModemSink
{
public:
    static gint Create(QTMSSink*& qsink, TMS::TMSSink*& tmssink);
    virtual ~QTMSModemSinkImpl();

    gint GetSink(TMS::TMSSink*& tmssink);

private:
    QTMSModemSinkImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_MODEM_SINK_IMPL_H
// End of file
