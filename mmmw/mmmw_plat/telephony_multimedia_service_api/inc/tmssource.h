/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMS_SOURCE_H
#define TMS_SOURCE_H

#include <tms.h>

namespace TMS {

/**
 * TMSSource class
 *
 * Base class for TMS data source type objects.
 *
 * @lib tmsapi.lib
 *
 */
class TMSSource
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
     *      TMS_RESULT_SUCCESS if source type is returned successfully.
     *
     */
    virtual gint GetType(TMSSourceType& sourcetype) = 0;
    };

} //namespace TMS

#endif //TMS_SOURCE_H
