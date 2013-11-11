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

#ifndef TMS_SINK_H
#define TMS_SINK_H

#include <tms.h>

namespace TMS {

/**
 * TMSSink class
 *
 * Base class for TMS data sink type objects.
 *
 * @lib tmsapi.lib
 *
 */
class TMSSink
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
     *      TMS_RESULT_SUCCESS if sink type is returned successfully.
     *
     */
    virtual gint GetType(TMSSinkType& sinktype) = 0;
    };

} //namespace TMS

#endif //TMS_SINK_H

