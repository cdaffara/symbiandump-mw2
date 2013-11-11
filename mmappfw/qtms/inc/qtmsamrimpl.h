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

#ifndef QTMS_AMR_FORMAT_IMPL_H
#define QTMS_AMR_FORMAT_IMPL_H

#include <qtms.h>
#include <qtmsamrformat.h>

// FORWARD DECLARATIONS
namespace TMS
{
class TMSFormat;
}

namespace QTMS
{

// Class declaration
class QTMSAMRFormatImpl: public QTMSAMRFormat
{
public:

    static gint Create(QTMSFormat*& qformat, TMS::TMSFormat*& tmsformat);
    virtual ~QTMSAMRFormatImpl();

    gint GetFormat(TMS::TMSFormat*& fmt);

    // From QTMSFormat
    gint GetType(QTMSFormatType& fmttype);

private:
    QTMSAMRFormatImpl();
    gint PostConstruct();
};

} //namespace QTMS

#endif // QTMS_AMR_FORMAT_IMPL_H
// End of file
