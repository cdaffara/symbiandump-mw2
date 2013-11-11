/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer for browser termination.
*
*/


#ifndef MSATBROWSERWSOBSERVER_H
#define MSATBROWSERWSOBSERVER_H

#include <w32std.h>

/**
 * Observer for browser termination.
 *
 * @lib EventMonitors.lib
 * @since Series 60 3.1
 */
class MSatBrowserWSObserver
    {

public:

    /**
     * Two-phased constructor.
     * @param aUtils Event monitor utils interface
     */
    MSatBrowserWSObserver() {};

    /**
     * Destructor.
     */
    virtual ~MSatBrowserWSObserver() {};

    /**
     * This function is called when browser's thread has actually died.
     * @param aCause The cause for the browser termination.
     */
    virtual void NotifyBrowserTerminated(
        const RSat::TBrowserTerminationCause aCause ) = 0;

private:

    /**
     * Prohibit copy constructor if not deriving from CBase.
     */
    MSatBrowserWSObserver( const MSatBrowserWSObserver& );

    /**
     * Prohibit assigment operator if not deriving from CBase.
     */
    MSatBrowserWSObserver& operator=( const MSatBrowserWSObserver& );

    };

#endif // MSATBROWSERWSOBSERVER_H
