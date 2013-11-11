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

#ifndef QTMS_CALL_IMPL_H
#define QTMS_CALL_IMPL_H

#include <glib.h>
#include <qtmscall.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSCall;
}

namespace QTMS
{

// FORWARD DECLARATIONS
class QTMSStream;
class QTMSDTMF;
class QTMSRingTone;

// QTMSCallImpl class
class QTMSCallImpl: public QTMSCall
{
public:
    static gint Create(QTMSCall*& qtmscall, TMS::TMSCall*& tmscall);

    virtual ~QTMSCallImpl();

    // From QTMSCallBody
    // QTMSCallType GetCallType();
    // gint GetCallContextId(guint& ctxid);
    // gint CreateStream(QTMSStreamType type, QTMSStream*& strm);
    // gint DeleteStream(QTMSStream*& strm);

private:
    // TMS::TMSCall* iTmsCall;
    QTMSCallImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_CALL_IMPL_H
// End of file
