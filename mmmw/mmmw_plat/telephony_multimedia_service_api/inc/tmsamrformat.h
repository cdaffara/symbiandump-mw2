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

#ifndef TMS_AMR_FORMAT_H
#define TMS_AMR_FORMAT_H

#include <tmsformat.h>

namespace TMS {

// FORWARD DECLARATIONS
class TMSAMRFormatBody;

/**
 * TMSAMRFormat class
 *
 * This class provides access methods to configure and query AMR-NB codec
 * format settings.
 *
 * This format class can be used for both the encoder and decoder.
 *
 * @lib tmsapi.lib
 *
 */
class TMSAMRFormat : public TMSFormat
    {
public:
    /**
     * Destructor
     */
    IMPORT_C virtual ~TMSAMRFormat();

protected:
    /**
     * Constructor
     */
    IMPORT_C TMSAMRFormat();

protected:
    TMSAMRFormatBody *iBody;
    };

} //namespace TMS

#endif //TMS_AMR_FORMAT_H
