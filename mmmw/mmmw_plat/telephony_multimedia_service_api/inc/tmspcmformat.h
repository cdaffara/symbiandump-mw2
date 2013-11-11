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

#ifndef TMS_PCM_FORMAT_H
#define TMS_PCM_FORMAT_H

#include <tms.h>
#include <tmsformat.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSPCMFormatBody;

/**
 * TMSPCMFormat class
 *
 * This class provides access methods to configure and query PCM-16 codec
 * format settings.
 *
 * @lib tmsapi.lib
 *
 */
class TMSPCMFormat : public TMSFormat
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSPCMFormat();

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSPCMFormat();

protected:
    TMSPCMFormatBody *iBody;
    };

} //namespace TMS

#endif //TMS_PCM_FORMAT_H
