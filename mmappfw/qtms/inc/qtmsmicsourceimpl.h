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

#ifndef QTMS_MIC_SOURCE_IMPL_H
#define QTMS_MIC_SOURCE_IMPL_H

#include <qtms.h>
#include <qtmsmicsource.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSSource;
}

namespace QTMS
{

// QTMSMicSourceImpl class
class QTMSMicSourceImpl: public QTMSMicSource
{
public:
    static gint Create(QTMSSource*& qsource, TMS::TMSSource*& tmssource);
    virtual ~QTMSMicSourceImpl();
    gint GetSource(TMS::TMSSource*& tmssource);

private:
    QTMSMicSourceImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_MIC_SOURCE_IMPL_H
// End of file
