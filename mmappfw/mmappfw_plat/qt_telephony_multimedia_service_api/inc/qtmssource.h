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

#ifndef QTMS_SOURCE_H
#define QTMS_SOURCE_H

#include <qtms.h>

namespace QTMS
{

/**
 * QTMSSource class
 *
 * Base class for QTMS data source type objects.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMSSource
{
public:
    /**
     * Returns type of the concrete data source object implementation.
     *
     * This function can be called at any time.
     *
     * @param  sourcetype
     *      Contains data source type on return.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if source type is returned successfully.
     *
     */
    virtual gint GetType(QTMSSourceType& sourcetype) = 0;
};

} //namespace QTMS

#endif //QTMS_SOURCE_H
