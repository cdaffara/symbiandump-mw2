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
 * Description:  Declarations for class TMSCenRepObserver
 *
 */

#ifndef TMSCENREPOBSERVER_H
#define TMSCENREPOBSERVER_H

//  INCLUDES
#include <e32std.h>

namespace TMS {

// CLASS DECLARATION

/**
 *  Observer interface for indicating P&S changes
 *
 */
class TMSCenRepObserver
    {
public:
    // New functions

    /**
     * Handler for changed event.
     * @param aUid uid of setting
     * @param aKey the changed key
     * @param aVal value
     */
    virtual void HandleNotifyCenRepL(const TUid aUid, const TUint32 aKey,
            TInt aVal) = 0;
    };

}//namespace TMS

#endif // TMSCENREPOBSERVER_H

// End of File
