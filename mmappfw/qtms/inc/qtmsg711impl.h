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

#ifndef QTMS_G711_FORMAT_IMPL_H
#define QTMS_G711_FORMAT_IMPL_H

#include <qtms.h>
#include <qtmsg711format.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSFormat;
}

namespace QTMS
{

// QTMSG711FormatImpl class
class QTMSG711FormatImpl: public QTMSG711Format
{
public:
    static gint Create(QTMSFormat*& qformat, TMS::TMSFormat*& tmsformat);
    virtual ~QTMSG711FormatImpl();

    gint GetFormat(TMS::TMSFormat*& fmt);

    // From QTMSFormat
    gint GetType(QTMSFormatType& fmttype);

private:
    QTMSG711FormatImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_G711_FORMAT_IMPL_H
// End of file
