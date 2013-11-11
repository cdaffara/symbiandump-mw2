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

#ifndef QTMS_SINK_H
#define QTMS_SINK_H

#include <qtms.h>

namespace QTMS
{

/**
 * QTMSSink class
 *
 * Base class for QTMS data sink type objects.
 *
 * @lib QTMSapi.lib
 *
 */
class QTMSSink
{
public:
    /**
     * Returns type of the concrete data sink object implementation.
     *
     * This function can be called at any time.
     *
     * @param  sinktype&
     *      Contains data sink type on return.
     *
     * @return
     *      QTMS_RESULT_SUCCESS if sink type is returned successfully.
     *
     */
    virtual gint GetType(QTMSSinkType& sinktype) = 0;
};

} //namespace QTMS

#endif //QTMS_SINK_H
