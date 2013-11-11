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

#ifndef QTMS_BUFFER_IMPL_H
#define QTMS_BUFFER_IMPL_H

#include <qtms.h>

// FORWARD DECLARATION
namespace TMS
{
class TMSBuffer;
}

namespace QTMS
{

// FORWARD DECLARATIONS
class QTMSBuffer;

// TMSBufferImpl class
class QTMSBufferImpl
{
public:
    static gint Create(QTMSBufferType ctype, QTMSBuffer*& buffer, TMS::TMSBuffer*& tmsbuffer);
    static gint Delete(QTMSBuffer*& buffer);
};

} //namespace QTMS

#endif // QTMS_BUFFER_IMPL_H
// End of file
