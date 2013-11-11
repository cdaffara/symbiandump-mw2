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

#ifndef QTMS_ILBC_FORMAT_IMPL_H
#define QTMS_ILBC_FORMAT_IMPL_H

#include <qtms.h>
#include <qtmsilbcformat.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSFormat;
}

namespace QTMS
{

// QTMSILBCFormatImpl class
class QTMSILBCFormatImpl: public QTMSILBCFormat
{
public:
    static gint Create(QTMSFormat*& qformat, TMS::TMSFormat*& tmsformat);
    virtual ~QTMSILBCFormatImpl();

    gint GetFormat(TMS::TMSFormat*& fmt);

    // From QTMSFormat
    gint GetType(QTMSFormatType& fmttype);

private:
    QTMSILBCFormatImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_ILBC_FORMAT_IMPL_H
// End of file
