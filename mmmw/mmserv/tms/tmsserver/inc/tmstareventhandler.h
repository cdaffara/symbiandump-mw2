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

#ifndef TMSTAREVENTHANDLER_H
#define TMSTAREVENTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "tmsserver.h"

namespace TMS {

// CLASS DECLARATION
class TMSTarEventHandler : public CActive
    {
public:
    /**
     * Two-phased constructor.
     */
    static TMSTarEventHandler* NewL(TMSServer* aServer);

    /**
     * Destructor.
     */
    virtual ~TMSTarEventHandler();

private:
    /**
     * From CActive
     * Cancel outstanding request
     */
    void DoCancel();

    /**
     * From CActive
     * Implementation of CActive::RunL.
     * Called when server request has completed.
     **/
    void RunL();

    /**
     * From CActive
     * Called when RunL leaves.
     **/
    TInt RunError(TInt aError);

private:

    /**
     * C++ default constructor.
     */
    TMSTarEventHandler(TMSServer* aServer);

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:
    TMSServer* iTMSSer;
    RProperty iProperty;
    };

} //namespace TMS

#endif //TMSTAREVENTHANDLER_H

