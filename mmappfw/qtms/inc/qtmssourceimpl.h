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

#ifndef QTMS_SOURCE_IMPL_H
#define QTMS_SOURCE_IMPL_H

#include <qtms.h>

namespace QTMS {

// FORWARD DECLARATIONS
class QTMSSource;

// TMSSourceImpl class
class QTMSSourceImpl
{
public:
    static gint Create(QTMSSourceType sourcetype, QTMSSource*& tmssource);
    static gint Delete(QTMSSource*& tmssource);
};

} //namespace QTMS

#endif // QTMS_SOURCE_IMPL_H

// End of file
