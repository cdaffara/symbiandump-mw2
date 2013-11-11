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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMSSYNCVOL_H
#define TMSSYNCVOL_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include <sounddevice.h>
#include "tmsclientserver.h"

namespace TMS {

// CLASS DECLARATION
class TMSSyncVol
    {
public:
    // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static TMSSyncVol* NewL();

    /**
     * Destructor.
     */
    virtual ~TMSSyncVol();

public:
    // New functions

    void SetSyncVol(TBool syncvol = ETrue);

private:

    /**
     * C++ default constructor.
     */
    TMSSyncVol();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:
    gint iVolume; //Volume
    };

} //namespace TMS

#endif // TMSSYNCVOL_H

// End of File
