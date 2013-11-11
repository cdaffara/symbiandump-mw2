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

#ifndef TMS_MODEM_SOURCE_H
#define TMS_MODEM_SOURCE_H

#include <tms.h>
#include <tmssource.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSModemSourceBody;

/**
 * TMSModemSource class
 *
 * This class instantiates TMS mode source object.
 *
 * @lib tmsapi.lib
 *
 */
class TMSModemSource : public TMSSource
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSModemSource();

    /**
     * Return source type.
     *
     * This function can be called at any time.
     *
     * @param  sourcetype
     *      The type of sink object (TMS_SOURCE_MODEM).
     *
     * @return
     *      TMS_RESULT_SUCCESS if the operation was successful otherwise
     *      system error.
     *
     */
    IMPORT_C virtual gint GetType(TMSSourceType& sourcetype);

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSModemSource();

protected:
    TMSModemSourceBody *iBody;
    };

} //namespace TMS

#endif //TMS_MODEM_SOURCE_H

// End of file

