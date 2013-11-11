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

#ifndef TARSETTINGS_H
#define TARSETTINGS_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include <sounddevice.h>
#include "tmsclientserver.h"

namespace TMS {

// CLASS DECLARATION
class TMSTarSettings
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static TMSTarSettings* NewL();

    /**
     * Destructor.
     */
    virtual ~TMSTarSettings();

public:
    // New functions

    void SetTar(TRoutingMsgBufPckg rountingpckg, TBool aPublish = ETrue);

private:

    /**
     * C++ default constructor.
     */
    TMSTarSettings();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:
    gint iVolume; //Volume
    };

} //namespace TMS

#endif      // TarSETTINGS_H

// End of File
